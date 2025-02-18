/*! Drawing the UI */

use cairo;

use crate::action::{ Action, Modifier };
use crate::keyboard;
use crate::layout::{ Button, ButtonPosition, Label, LatchedState, Layout };
use crate::layout::c::{ Bounds, EekGtkKeyboard, Point };
use crate::submission::c::Submission as CSubmission;

use glib::translate::FromGlibPtrNone;
use gtk::prelude::WidgetExt;

use std::collections::HashSet;
use std::ffi::CStr;
use std::ptr;

mod c {
    use super::*;

    use cairo_sys;
    use std::os::raw::{ c_char, c_void };
    
    // This is constructed only in C, no need for warnings
    #[allow(dead_code)]
    #[repr(transparent)]
    #[derive(Clone, Copy)]
    pub struct EekRenderer(*const c_void);

    // This is constructed only in C, no need for warnings
    /// Just don't clone this for no reason.
    #[allow(dead_code)]
    #[repr(transparent)]
    #[derive(Clone, Copy)]
    pub struct GtkStyleContext(*const c_void);


    extern "C" {
        #[allow(improper_ctypes)]
        pub fn eek_renderer_get_scale_factor(
            renderer: EekRenderer,
        ) -> u32;

        #[allow(improper_ctypes)]
        pub fn eek_render_button_in_context(
            scale_factor: u32,
            cr: *mut cairo_sys::cairo_t,
            x_offset: f64, y_offset: f64,
            x_scale: f64, y_scale: f64,
            ctx: GtkStyleContext,
            bounds: Bounds,
            icon_name: *const c_char,
            label: *const c_char,
        );

        #[allow(improper_ctypes)]
        pub fn eek_get_style_context_for_button(
            renderer: EekRenderer,
            name: *const c_char,
            outline_name: *const c_char,
            locked_class: *const c_char,
            pressed: u64,
        ) -> GtkStyleContext;

        #[allow(improper_ctypes)]
        pub fn eek_put_style_context_for_button(
            ctx: GtkStyleContext,
            outline_name: *const c_char,
            locked_class: *const c_char,
        );
    }

    /// Draws all buttons that are not in the base state
    #[no_mangle]
    pub extern "C"
    fn squeek_layout_draw_all_changed(
        layout: *mut Layout,
        renderer: EekRenderer,
        cr: *mut cairo_sys::cairo_t,
        x_offset: f64, y_offset: f64,
        x_scale: f64, y_scale: f64,
        submission: CSubmission,
    ) {
        let layout = unsafe { &mut *layout };
        let submission = submission.clone_ref();
        let submission = submission.borrow();
        let cr = unsafe { cairo::Context::from_raw_none(cr) };
        let active_modifiers = submission.get_active_modifiers();

        layout.foreach_visible_button(|offset, button, (row, position_in_row)| {
            // TODO: this iterator copies string indices way too much.
            // For efficiency, it would be better to draw pressed buttons from the list first,
            // and then iterate the rest without having to look up their indices.
            let state = layout.state.active_buttons.get(&ButtonPosition {
                view: layout.state.current_view.clone(),
                row,
                position_in_row,
            });

            let locked = LockedStyle::from_action(
                &button.action,
                &active_modifiers,
                layout.get_view_latched(),
                &layout.state.current_view,
            );
            if state.pressed == keyboard::PressType::Pressed
                || locked != LockedStyle::Free
            {
                render_button_at_position(
                    renderer, &cr,
                    x_offset, y_offset,
                    x_scale, y_scale,
                    offset,
                    button,
                    state.pressed, locked,
                );
            }
        })
    }
    
    #[no_mangle]
    pub extern "C"
    fn squeek_draw_layout_base_view(
        layout: *mut Layout,
        renderer: EekRenderer,
        cr: *mut cairo_sys::cairo_t,
        x_offset: f64, y_offset: f64,
        x_scale: f64, y_scale: f64,
    ) {
        let layout = unsafe { &mut *layout };
        let cr = unsafe { cairo::Context::from_raw_none(cr) };
        
        layout.foreach_visible_button(|offset, button, _index| {
            render_button_at_position(
                renderer, &cr,
                x_offset, y_offset,
                x_scale, y_scale,
                offset,
                button,
                keyboard::PressType::Released,
                LockedStyle::Free,
            );
        })
    }
}

#[derive(Clone, Copy, PartialEq, Debug)]
enum LockedStyle {
    Free,
    Latched,
    Locked,
}

impl LockedStyle {
    fn from_action(
        action: &Action,
        mods: &HashSet<Modifier>,
        latched_view: &LatchedState,
        current_view: &str,
    ) -> LockedStyle {
        let active_mod = match action {
            Action::ApplyModifier(m) => mods.contains(m),
            _ => false,
        };
        
        let active_view = action.is_active(current_view);
        let latched_button = match latched_view {
            LatchedState::Not => false,
            LatchedState::FromView(view) => !action.has_locked_appearance_from(view),
        };
        match (active_mod, active_view, latched_button) {
            // Modifiers don't latch.
            (true, _, _) => LockedStyle::Locked,
            (false, true, false) => LockedStyle::Locked,
            (false, true, true) => LockedStyle::Latched,
            _ => LockedStyle::Free,
        }
    }
}

/// Renders a button at a position (button's own bounds ignored)
fn render_button_at_position(
    renderer: c::EekRenderer,
    cr: &cairo::Context,
    x_offset: f64, y_offset: f64,
    x_scale: f64, y_scale: f64,
    position: Point,
    button: &Button,
    pressed: keyboard::PressType,
    locked: LockedStyle,
) {
    cr.save().unwrap();
    cr.translate(position.x * x_scale + x_offset, position.y * y_scale + y_offset);
    cr.rectangle(
        0.0, 0.0,
        button.size.width * x_scale,
        button.size.height * y_scale,
    );
    cr.clip();

    let scale_factor = unsafe {
        c::eek_renderer_get_scale_factor(renderer)
    };
    let bounds = button.get_bounds();
    let scaled_and_offset_bounds = Bounds {
        x: bounds.x * x_scale + x_offset,
        y: bounds.y * y_scale + y_offset,
        width: bounds.width * x_scale,
        height: bounds.height * y_scale,
    };

    let (label_c, icon_name_c) = match &button.label {
        Label::Text(text) => (text.as_ptr(), ptr::null()),
        Label::IconName(name) => {
            let l = unsafe {
                // CStr doesn't allocate anything, so it only points to
                // the 'static str, avoiding a memory leak
                CStr::from_bytes_with_nul_unchecked(b"icon\0")
            };
            (l.as_ptr(), name.as_ptr())
        },
    };

    with_button_context(
        renderer,
        button,
        pressed,
        locked,
        |ctx| unsafe {
            // TODO: split into separate procedures:
            // draw outline, draw label, draw icon.
            c::eek_render_button_in_context(
                scale_factor,
                cairo::Context::to_raw_none(&cr),
                x_offset, y_offset,
                x_scale, y_scale,
                *ctx,
                scaled_and_offset_bounds,
                icon_name_c,
                label_c,
            )
        }
    );

    cr.restore().unwrap();
}

fn with_button_context<R, F: FnOnce(&c::GtkStyleContext) -> R>(
    renderer: c::EekRenderer,
    button: &Button,
    pressed: keyboard::PressType,
    locked: LockedStyle,
    operation: F,
) -> R {
    let outline_name_c = button.outline_name.as_ptr();
    let locked_class_c = match locked {
        LockedStyle::Free => ptr::null(),
        LockedStyle::Locked => unsafe {
            CStr::from_bytes_with_nul_unchecked(b"locked\0").as_ptr()
        },
        LockedStyle::Latched => unsafe {
            CStr::from_bytes_with_nul_unchecked(b"latched\0").as_ptr()
        },
    };
    
    let ctx = unsafe {
        c::eek_get_style_context_for_button(
            renderer,
            button.name.as_ptr(),
            outline_name_c,
            locked_class_c,
            pressed as u64,
        )
    };
    
    let r = operation(&ctx);

    unsafe {
        c::eek_put_style_context_for_button(
            ctx,
            outline_name_c,
            locked_class_c,
        )
    };

    r
}

pub fn queue_redraw(keyboard: EekGtkKeyboard) {
    let widget = unsafe { gtk::Widget::from_glib_none(keyboard.0) };
    widget.queue_draw();
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_exit_only() {
        assert_eq!(
            LockedStyle::from_action(
                &Action::LockView {
                    lock: "ab".into(), 
                    unlock: "a".into(),
                    latches: true,
                    looks_locked_from: vec!["b".into()],
                },
                &HashSet::new(),
                &LatchedState::FromView("b".into()),
                "ab",
            ),
            LockedStyle::Locked,
        );
    }
}
