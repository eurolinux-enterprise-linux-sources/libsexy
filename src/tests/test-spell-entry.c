#include <gtk/gtk.h>
#include <libsexy/sexy-spell-entry.h>

int
main(int argc, char **argv)
{
	GtkWidget *window;
	GtkWidget *hbox;
	GtkWidget *label;
	GtkWidget *entry;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_show(window);
	gtk_window_set_title(GTK_WINDOW(window), "Sexy Spell Entry Test");
	gtk_container_set_border_width(GTK_CONTAINER(window), 12);

	g_signal_connect(G_OBJECT(window), "destroy",
					 G_CALLBACK(gtk_main_quit), NULL);

	hbox = gtk_hbox_new(FALSE, 6);
	gtk_widget_show(hbox);
	gtk_container_add(GTK_CONTAINER(window), hbox);

	label = gtk_label_new("Text:");
	gtk_widget_show(label);
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 0);

	entry = sexy_spell_entry_new();
	gtk_widget_show(entry);
	gtk_box_pack_start(GTK_BOX(hbox), entry, TRUE, TRUE, 0);

	gtk_entry_set_text(GTK_ENTRY (entry), "Hello Wrold");

	gtk_main();

	return 0;
}
