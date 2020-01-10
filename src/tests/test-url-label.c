#include <gtk/gtk.h>
#include <libsexy/sexy-url-label.h>
#include <stdio.h>

#define LIBSEXY_URL "http://osiris.chipx86.com/svn/osiris-misc/trunk/libsexy/"
static void
url_activated_cb(GtkWidget *url_label, const gchar *url)
{
	char *escaped_url;
	char *cmd;

	escaped_url = g_shell_quote(url);
	cmd = g_strdup_printf("gnome-open %s", escaped_url);
	g_free(escaped_url);

	printf("Executing %s\n", cmd);
	g_spawn_command_line_async(cmd, NULL);

	g_free(cmd);
}

int
main(int argc, char **argv)
{
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *url_label;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_show(window);
	gtk_window_set_title(GTK_WINDOW(window), "Sexy URL Label Test");
	gtk_container_set_border_width(GTK_CONTAINER(window), 12);

	g_signal_connect(G_OBJECT(window), "destroy",
					 G_CALLBACK(gtk_main_quit), NULL);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	url_label = sexy_url_label_new();
	gtk_widget_show(url_label);
	gtk_box_pack_start(GTK_BOX(vbox), url_label, TRUE, TRUE, 0);
	gtk_label_set_line_wrap(GTK_LABEL(url_label), TRUE);
	gtk_label_set_selectable(GTK_LABEL(url_label), TRUE);
	gtk_misc_set_alignment(GTK_MISC(url_label), 0.0, 0.0);
	sexy_url_label_set_markup(SEXY_URL_LABEL(url_label),
		"This is a sample SexyUrlLabel. For the latest version, please "
		"the <a href=\"" LIBSEXY_URL "\">SVN repository</a>. For a great "
		"page about mornings and what you can do with them, see "
		"<a href=\"http://www.destroymornings.com/\">DestroyMornings.com</a>.");

	g_signal_connect(G_OBJECT(url_label), "url_activated",
					 G_CALLBACK(url_activated_cb), NULL);

	gtk_main();

	return 0;
}
