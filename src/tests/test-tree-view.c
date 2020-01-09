#include <gtk/gtk.h>
#include <libsexy/sexy-tree-view.h>

GtkWidget *
get_tooltip(SexyTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer data)
{
	GtkTreeIter iter;
	GtkTreeModel *model;
	GtkWidget *box, *image, *label;
	gchar *name, *stock, *text;

	model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
	gtk_tree_model_get_iter(model, &iter, path);
	gtk_tree_model_get(model, &iter, 0, &name, 1, &stock, -1);

	box = gtk_hbox_new(FALSE, 6);
	image = gtk_image_new_from_stock(stock, GTK_ICON_SIZE_DIALOG);
	text = g_strdup_printf("<span size=\"large\" weight=\"bold\">%s</span>", name);
	label = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label), text);
	gtk_box_pack_start(GTK_BOX(box), image, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box), label, FALSE, TRUE, 0);

	gtk_widget_show_all(box);
	return box;
}

int
main(int argc, char **argv)
{
	GtkWidget *window;
	GtkWidget *swin;
	GtkWidget *treeview;
	GtkTreeStore *store;
	GtkTreeViewColumn *col;
	GtkCellRenderer *text;
	GtkTreeIter a, b;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_show(window);
	gtk_window_set_title(GTK_WINDOW(window), "Sexy Tree View Test");
	gtk_container_set_border_width(GTK_CONTAINER(window), 3);
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	swin = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_show(swin);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(swin), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(swin), GTK_SHADOW_ETCHED_IN);
	gtk_container_add(GTK_CONTAINER(window), swin);

	treeview = sexy_tree_view_new();
	gtk_widget_show(treeview);
	gtk_container_add(GTK_CONTAINER(swin), treeview);

	store = gtk_tree_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
	gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));

	g_signal_connect(G_OBJECT(treeview), "get-tooltip", G_CALLBACK(get_tooltip), NULL);

	text = gtk_cell_renderer_text_new();
	col = gtk_tree_view_column_new_with_attributes("Column 1", text, "text", 0, NULL);
	gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), col);

	gtk_tree_store_append(store, &a, NULL);
	gtk_tree_store_set(store, &a, 0, "a", 1, GTK_STOCK_DIALOG_AUTHENTICATION, -1);
	gtk_tree_store_append(store, &b, &a);
	gtk_tree_store_set(store, &b, 0, "one", 1, GTK_STOCK_DIALOG_ERROR, -1);
	gtk_tree_store_append(store, &b, &a);
	gtk_tree_store_set(store, &b, 0, "two", 1, GTK_STOCK_DIALOG_INFO, -1);
	gtk_tree_store_append(store, &b, &a);
	gtk_tree_store_set(store, &b, 0, "three", 1, GTK_STOCK_DIALOG_QUESTION, -1);
	gtk_tree_store_append(store, &a, NULL);
	gtk_tree_store_set(store, &a, 0, "b", 1, GTK_STOCK_DIALOG_WARNING, -1);
	gtk_tree_store_append(store, &b, &a);
	gtk_tree_store_set(store, &b, 0, "one", 1, GTK_STOCK_DIALOG_AUTHENTICATION, -1);
	gtk_tree_store_append(store, &b, &a);
	gtk_tree_store_set(store, &b, 0, "two", 1, GTK_STOCK_DIALOG_ERROR, -1);
	gtk_tree_store_append(store, &b, &a);
	gtk_tree_store_set(store, &b, 0, "three", 1, GTK_STOCK_DIALOG_INFO, -1);

	gtk_main();

	return 0;
};
