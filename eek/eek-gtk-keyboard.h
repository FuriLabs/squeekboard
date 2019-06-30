/* 
 * Copyright (C) 2010-2011 Daiki Ueno <ueno@unixuser.org>
 * Copyright (C) 2010-2011 Red Hat, Inc.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA
 */

#if !defined(__EEK_H_INSIDE__) && !defined(EEK_COMPILATION)
#error "Only <eek/eek-gtk.h> can be included directly."
#endif

#ifndef EEK_GTK_KEYBOARD_H
#define EEK_GTK_KEYBOARD_H 1

#include <glib.h>
#include <gtk/gtk.h>

#include "eek-keyboard.h"

G_BEGIN_DECLS
#define EEK_TYPE_GTK_KEYBOARD (eek_gtk_keyboard_get_type())
#define EEK_GTK_KEYBOARD(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), EEK_TYPE_GTK_KEYBOARD, EekGtkKeyboard))
#define EEK_GTK_KEYBOARD_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), EEK_TYPE_GTK_KEYBOARD, EekGtkKeyboardClass))
#define EEK_IS_GTK_KEYBOARD(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), EEK_TYPE_GTK_KEYBOARD))
#define EEK_IS_GTK_KEYBOARD_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), EEK_TYPE_GTK_KEYBOARD))
#define EEK_GTK_KEYBOARD_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), EEK_TYPE_GTK_KEYBOARD, EekGtkKeyboardClass))

typedef struct _EekGtkKeyboard EekGtkKeyboard;
typedef struct _EekGtkKeyboardClass EekGtkKeyboardClass;
typedef struct _EekGtkKeyboardPrivate EekGtkKeyboardPrivate;

struct _EekGtkKeyboard
{
    /*< private >*/
    GtkDrawingArea parent;

    GdkEventSequence *sequence; // unowned reference

    EekGtkKeyboardPrivate *priv;
};

struct _EekGtkKeyboardClass
{
    /*< private >*/
    GtkDrawingAreaClass parent_class;

    /*< private >*/
    /* padding */
    gpointer pdummy[24];
};

GType      eek_gtk_keyboard_get_type  (void) G_GNUC_CONST;
GtkWidget *eek_gtk_keyboard_new       (EekKeyboard    *keyboard);
void       eek_gtk_keyboard_set_theme (EekGtkKeyboard *keyboard,
                                       EekTheme       *theme);

G_END_DECLS
#endif  /* EEK_GTK_KEYBOARD_H */
