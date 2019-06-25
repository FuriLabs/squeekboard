/* 
 * Copyright (C) 2010-2011 Daiki Ueno <ueno@unixuser.org>
 * Copyright (C) 2010-2011 Red Hat, Inc.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef CLIENT_H
#define CLIENT_H 1

#include <gio/gio.h>

G_BEGIN_DECLS

#define TYPE_CLIENT (client_get_type())
#define CLIENT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_CLIENT, Client))
#define CLIENT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_CLIENT, ClientClass))
#define IS_CLIENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_CLIENT))
#define IS_CLIENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_CLIENT))
#define CLIENT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_CLIENT, ClientClass))

typedef struct _Client SeatEmitter;

SeatEmitter  *client_new                     (GDBusConnection     *connection);

gboolean client_set_keyboards           (SeatEmitter              *client,
                                         const gchar * const *keyboard);

gboolean client_enable_xkl              (SeatEmitter              *client);
void     client_disable_xkl             (SeatEmitter              *client);

gboolean client_enable_atspi_focus      (SeatEmitter              *client);
void     client_disable_atspi_focus     (SeatEmitter              *client);

gboolean client_enable_atspi_keystroke  (SeatEmitter              *client);
void     client_disable_atspi_keystroke (SeatEmitter              *client);

gboolean client_enable_xtest            (SeatEmitter              *client);
void     client_disable_xtest           (SeatEmitter              *client);

gboolean client_enable_ibus_focus       (SeatEmitter              *client);
void     client_disable_ibus_focus      (SeatEmitter              *client);

G_END_DECLS
#endif  /* CLIENT_H */
