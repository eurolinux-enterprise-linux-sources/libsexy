#include <gtk/gtk.h>
#include <libsexy/sexy-tooltip.h>

int main(int argc, char **argv)
{
	GtkWidget *tooltip;
	GtkWidget *hbox;
	GtkWidget *image;
	GtkWidget *label;

	gtk_init(&argc, &argv);

	tooltip = sexy_tooltip_new();

	hbox = gtk_hbox_new(6, FALSE);
	gtk_widget_show(hbox);
	gtk_container_add(GTK_CONTAINER(tooltip), hbox);

	image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_AUTHENTICATION, GTK_ICON_SIZE_DIALOG);
	gtk_widget_show(image);
	gtk_box_pack_start(GTK_BOX(hbox), image, FALSE, TRUE, 0);

	label = gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(label), "<span size=\"large\" weight=\"bold\">Text</span>");
	gtk_widget_show(label);
	gtk_box_pack_start(GTK_BOX(hbox), label, TRUE, TRUE, 0);

	gtk_widget_show(tooltip);

	gtk_main();

	return 0;
}
