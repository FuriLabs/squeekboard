#ifndef EEK_XML_H
#define EEK_XML_H 1

#include <glib-object.h>
#include "eek-keyboard.h"
#include "eek-xml-layout.h"

G_BEGIN_DECLS

#define EEK_XML_SCHEMA_VERSION "0.90"

void eek_keyboard_output (EekKeyboard *keyboard, GString *output, gint indent);

G_END_DECLS
#endif  /* EEK_XML_H */