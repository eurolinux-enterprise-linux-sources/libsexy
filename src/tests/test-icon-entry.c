#include <gtk/gtk.h>
#include <libsexy/sexy-icon-entry.h>
#include <stdio.h>

int
main(int argc, char **argv)
{
	GtkWidget *window;
	GtkWidget *table;
	GtkWidget *label;
	GtkWidget *icon_entry;
	GtkWidget *icon;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_show(window);
	gtk_window_set_title(GTK_WINDOW(window), "Sexy Icon Entry Test");
	gtk_container_set_border_width(GTK_CONTAINER(window), 12);

	g_signal_connect(G_OBJECT(window), "destroy",
					 G_CALLBACK(gtk_main_quit), NULL);

	table = gtk_table_new(2, 4, FALSE);
	gtk_widget_show(table);
	gtk_container_add(GTK_CONTAINER(window), table);
	gtk_table_set_row_spacings(GTK_TABLE(table), 6);
	gtk_table_set_col_spacings(GTK_TABLE(table), 6);

	/*
	 * Open File
	 */
	label = gtk_label_new("Open File:");
	gtk_widget_show(label);
	gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1,
					 GTK_FILL, GTK_FILL, 0, 0);
	gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);

	icon_entry = sexy_icon_entry_new();
	gtk_widget_show(icon_entry);
	gtk_table_attach(GTK_TABLE(table), icon_entry, 1, 2, 0, 1,
					 GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);

	icon = gtk_image_new_from_stock(GTK_STOCK_OPEN, GTK_ICON_SIZE_MENU);
	gtk_widget_show(icon);
	sexy_icon_entry_set_icon(SEXY_ICON_ENTRY(icon_entry),
							 SEXY_ICON_ENTRY_PRIMARY,
							 GTK_IMAGE(icon));
	sexy_icon_entry_set_icon_highlight(SEXY_ICON_ENTRY(icon_entry),
									   SEXY_ICON_ENTRY_PRIMARY, TRUE);

	/*
	 * Save File
	 */
	label = gtk_label_new("Save File:");
	gtk_widget_show(label);
	gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2,
					 GTK_FILL, GTK_FILL, 0, 0);
	gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);

	icon_entry = sexy_icon_entry_new();
	gtk_widget_show(icon_entry);
	gtk_table_attach(GTK_TABLE(table), icon_entry, 1, 2, 1, 2,
					 GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);
	gtk_entry_set_text(GTK_ENTRY(icon_entry), "‏Right-to-left");
	gtk_widget_set_direction(icon_entry, GTK_TEXT_DIR_RTL);

	icon = gtk_image_new_from_stock(GTK_STOCK_SAVE, GTK_ICON_SIZE_MENU);
	gtk_widget_show(icon);
	sexy_icon_entry_set_icon(SEXY_ICON_ENTRY(icon_entry),
							 SEXY_ICON_ENTRY_PRIMARY,
							 GTK_IMAGE(icon));
	sexy_icon_entry_set_icon_highlight(SEXY_ICON_ENTRY(icon_entry),
									   SEXY_ICON_ENTRY_PRIMARY, TRUE);

	/*
	 * Search
	 */
	label = gtk_label_new("Search:");
	gtk_widget_show(label);
	gtk_table_attach(GTK_TABLE(table), label, 0, 1, 2, 3,
					 GTK_FILL, GTK_FILL, 0, 0);
	gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);

	icon_entry = sexy_icon_entry_new();
	gtk_widget_show(icon_entry);
	gtk_table_attach(GTK_TABLE(table), icon_entry, 1, 2, 2, 3,
					 GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);
	sexy_icon_entry_add_clear_button(SEXY_ICON_ENTRY(icon_entry));

	icon = gtk_image_new_from_stock(GTK_STOCK_FIND, GTK_ICON_SIZE_MENU);
	gtk_widget_show(icon);
	sexy_icon_entry_set_icon(SEXY_ICON_ENTRY(icon_entry),
							 SEXY_ICON_ENTRY_PRIMARY,
							 GTK_IMAGE(icon));

	/*
	 * Password
	 */
	label = gtk_label_new("Password:");
	gtk_widget_show(label);
	gtk_table_attach(GTK_TABLE(table), label, 0, 1, 3, 4,
					 GTK_FILL, GTK_FILL, 0, 0);
	gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);

	icon_entry = sexy_icon_entry_new();
	gtk_widget_show(icon_entry);
	gtk_table_attach(GTK_TABLE(table), icon_entry, 1, 2, 3, 4,
					 GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);
	gtk_entry_set_visibility(GTK_ENTRY(icon_entry), FALSE);

	icon = gtk_image_new_from_stock(GTK_STOCK_DIALOG_AUTHENTICATION,
									GTK_ICON_SIZE_MENU);
	gtk_widget_show(icon);
	sexy_icon_entry_set_icon(SEXY_ICON_ENTRY(icon_entry),
							 SEXY_ICON_ENTRY_PRIMARY,
							 GTK_IMAGE(icon));

	/* Name */
	label = gtk_label_new("Name:");
	gtk_widget_show(label);
	gtk_table_attach(GTK_TABLE(table), label, 0, 1, 4, 5,
					 GTK_FILL, GTK_FILL, 0, 0);
	gtk_misc_set_alignment(GTK_MISC(label), 0.0, 0.5);

	icon_entry = sexy_icon_entry_new();
	gtk_widget_show(icon_entry);
	gtk_table_attach(GTK_TABLE(table), icon_entry, 1, 2, 4, 5,
					 GTK_FILL | GTK_EXPAND, GTK_FILL, 0, 0);

	gtk_main();

	return 0;
}
