noinst_PROGRAMS = \
	test-icon-entry \
	test-spell-entry \
	test-tooltip \
	test-tree-view \
	test-url-label

common_ldflags = \
	$(top_builddir)/libsexy/libsexy.la \
	$(PACKAGE_LIBS)

test_url_label_SOURCES = test-url-label.c
test_url_label_LDADD   = $(common_ldflags)

test_spell_entry_SOURCES = test-spell-entry.c
test_spell_entry_LDADD   = $(common_ldflags)

test_icon_entry_SOURCES = test-icon-entry.c
test_icon_entry_LDADD   = $(common_ldflags)

test_tooltip_SOURCES = test-tooltip.c
test_tooltip_LDADD   = $(common_ldflags)

test_tree_view_SOURCES = test-tree-view.c
test_tree_view_LDADD   = $(common_ldflags)

INCLUDES = \
	-I$(top_srcdir) \
	$(PACKAGE_CFLAGS)
