
/* Generated data (by glib-mkenums) */

#include <libsexy/sexy.h>

/* enumerations from "sexy-icon-entry.h" */
GType
sexy_icon_entry_position_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { SEXY_ICON_ENTRY_PRIMARY, "SEXY_ICON_ENTRY_PRIMARY", "primary" },
      { SEXY_ICON_ENTRY_SECONDARY, "SEXY_ICON_ENTRY_SECONDARY", "secondary" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("SexyIconEntryPosition", values);
  }
  return etype;
}

/* enumerations from "sexy-spell-entry.h" */
GType
sexy_spell_error_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { SEXY_SPELL_ERROR_BACKEND, "SEXY_SPELL_ERROR_BACKEND", "backend" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("SexySpellError", values);
  }
  return etype;
}

/* Generated data ends here */

