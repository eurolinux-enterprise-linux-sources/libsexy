sexyincdir = $(includedir)/libsexy

lib_LTLIBRARIES = libsexy.la

sexy-marshal.h:	marshal.list
	glib-genmarshal --prefix=sexy_marshal ./marshal.list --header > sexy-marshal.h

sexy-marshal.c:	marshal.list
	glib-genmarshal --prefix=sexy_marshal ./marshal.list --body > sexy-marshal.c

sexy_headers = \
	sexy.h \
	sexy-icon-entry.h \
	sexy-spell-entry.h \
	sexy-tooltip.h \
	sexy-tree-view.h \
	sexy-url-label.h

sexyinc_HEADERS = \
	$(sexy_headers) \
	sexy-enum-types.h

noinst_HEADERS = \
	gtkspell-iso-codes.h \
	sexy-marshal.h

BUILT_SOURCES = \
	sexy-enum-types.h \
	sexy-enum-types.c \
	sexy-marshal.h \
	sexy-marshal.c

libsexy_la_SOURCES = \
	gtkspell-iso-codes.c \
	sexy-enum-types.c \
	sexy-icon-entry.c \
	sexy-spell-entry.c \
	sexy-tooltip.c \
	sexy-tree-view.c \
	sexy-url-label.c \
	sexy-marshal.c

libsexy_la_LIBADD = $(PACKAGE_LIBS)
libsexy_la_LDFLAGS = -version-info $(LT_CURRENT):$(LT_REVISION):$(LT_AGE) $(NO_UNDEFINED)

sexy-enum-types.h: s-enum-types-h
	@true
s-enum-types-h: @REBUILD@ $(sexy_headers) Makefile
	( cd $(srcdir) && glib-mkenums \
			--fhead "#ifndef _SEXY_ENUM_TYPES_H_\n#define _SEXY_ENUM_TYPES_H_\n\n#include <glib-object.h>\n\nG_BEGIN_DECLS\n" \
			--fprod "/* enumerations from \"@filename@\" */\n" \
			--vhead "GType @enum_name@_get_type(void);\n#define SEXY_TYPE_@ENUMSHORT@ (@enum_name@_get_type())\n"	\
			--ftail "G_END_DECLS\n\n#endif /* _SEXY_ENUM_TYPES_H_ */" \
		$(sexy_headers) ) > tmp-sexy-enum-types.h \
	&& (cmp -s tmp-sexy-enum-types.h sexy-enum-types.h || cp tmp-sexy-enum-types.h sexy-enum-types.h ) \
	&& rm -f tmp-sexy-enum-types.h	\
	&& echo timestamp > $(@F)

sexy-enum-types.c: s-enum-types-c
	@true
s-enum-types-c: @REBUILD@ $(sexy_headers) Makefile
	( cd $(srcdir) && glib-mkenums \
			--fhead "#include <libsexy/sexy.h>" \
			--fprod "\n/* enumerations from \"@filename@\" */" \
			--vhead "GType\n@enum_name@_get_type (void)\n{\n  static GType etype = 0;\n  if (etype == 0) {\n    static const G@Type@Value values[] = {" 	\
			--vprod "      { @VALUENAME@, \"@VALUENAME@\", \"@valuenick@\" }," \
			--vtail "      { 0, NULL, NULL }\n    };\n    etype = g_@type@_register_static (\"@EnumName@\", values);\n  }\n  return etype;\n}\n" \
		$(sexy_headers) ) > tmp-sexy-enum-types.c \
	&& (cmp -s tmp-sexy-enum-types.c sexy-enum-types.c || cp tmp-sexy-enum-types.c sexy-enum-types.c ) \
	&& rm -f tmp-sexy-enum-types.c \
	&& echo timestamp > $(@F)

INCLUDES = \
	-I$(top_srcdir) \
	$(PACKAGE_CFLAGS)

EXTRA_DIST = \
	sexy-enum-types.h \
	marshal.list

CLEANFILES = \
	sexy-marshal.h \
	sexy-marshal.c \
	sexy-enum-types.h \
	s-enum-types-h \
	sexy-enum-types.c \
	s-enum-types-c
