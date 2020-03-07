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

#include "config.h"

#define _XOPEN_SOURCE 500
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/random.h> // TODO: this is Linux-specific
#include <xkbcommon/xkbcommon.h>


#include "eek-keyboard.h"

void level_keyboard_free(LevelKeyboard *self) {
    xkb_keymap_unref(self->keymap);
    close(self->keymap_fd);
    squeek_layout_free(self->layout);
    g_free(self);
}

LevelKeyboard*
level_keyboard_new (struct squeek_layout *layout)
{
    LevelKeyboard *keyboard = g_new0(LevelKeyboard, 1);

    if (!keyboard) {
        g_error("Failed to create a keyboard");
    }
    keyboard->layout = layout;

    struct xkb_context *context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    if (!context) {
        g_error("No context created");
    }

    const gchar *keymap_str = squeek_layout_get_keymap(keyboard->layout);

    struct xkb_keymap *keymap = xkb_keymap_new_from_string(context, keymap_str,
        XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);

    if (!keymap)
        g_error("Bad keymap:\n%s", keymap_str);

    xkb_context_unref(context);
    keyboard->keymap = keymap;

    keymap_str = xkb_keymap_get_as_string(keymap, XKB_KEYMAP_FORMAT_TEXT_V1);
    keyboard->keymap_len = strlen(keymap_str) + 1;

    g_autofree char *path = strdup("/eek_keymap-XXXXXX");
    char *r = &path[strlen(path) - 6];
    getrandom(r, 6, GRND_NONBLOCK);
    for (unsigned i = 0; i < 6; i++) {
        r[i] = (r[i] & 0b1111111) | 0b1000000; // A-z
        r[i] = r[i] > 'z' ? '?' : r[i]; // The randomizer doesn't need to be good...
    }
    int keymap_fd = shm_open(path, O_RDWR | O_CREAT | O_EXCL, 0600);
    if (keymap_fd < 0) {
        g_error("Failed to set up keymap fd");
    }
    keyboard->keymap_fd = keymap_fd;
    shm_unlink(path);
    if (ftruncate(keymap_fd, (off_t)keyboard->keymap_len)) {
        g_error("Failed to increase keymap fd size");
    }
    char *ptr = mmap(NULL, keyboard->keymap_len, PROT_WRITE, MAP_SHARED,
        keymap_fd, 0);
    if ((void*)ptr == (void*)-1) {
        g_error("Failed to set up mmap");
    }
    strncpy(ptr, keymap_str, keyboard->keymap_len);
    munmap(ptr, keyboard->keymap_len);
    return keyboard;
}
