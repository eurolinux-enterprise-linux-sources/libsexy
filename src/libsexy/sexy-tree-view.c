/*
 * @file libsexy/sexy-tree-view.c A treeview subclass which can display
 *                                tooltips for items in the tree.
 *
 * @Copyright (C) 2006 Christian Hammond.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA  02111-1307, USA.
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>
#include <libsexy/sexy-tree-view.h>
#include <libsexy/sexy-tooltip.h>
#include "sexy-marshal.h"

struct _SexyTreeViewPriv
{
	GtkWidget *current_tooltip;
	guint tooltip_timeout;
	GdkRectangle tip_rect;
	GtkTreePath *tip_path;
	GtkTreeViewColumn *tip_column;
	gint tip_label_column;
	gint mouse_x, mouse_y;

	gint header_size;
	gboolean headers_visible;
};

enum
{
	GET_TOOLTIP,
	LAST_SIGNAL
};

static void sexy_tree_view_class_init(SexyTreeViewClass *klass);
static void sexy_tree_view_init(SexyTreeView *tree_view);
static void sexy_tree_view_finalize(GObject *obj);
static gboolean sexy_tree_view_button_press_event(GtkWidget *widget, GdkEventButton *event);
static gboolean sexy_tree_view_motion_notify_event(GtkWidget *widget, GdkEventMotion *event);
static gboolean sexy_tree_view_leave_notify_event(GtkWidget *widget, GdkEventCrossing *event);
static void sexy_tree_view_columns_changed(GtkTreeView *treeview, gpointer data);
static void sexy_tree_view_header_visible_notify(GObject *object, GParamSpec *pspec, gpointer data);
static void remove_tooltip(SexyTreeView *tree_view);
static gboolean show_tooltip(SexyTreeView *tree_view);
static void recalculate_header(SexyTreeView *tree_view);

static GtkTreeViewClass *parent_class;
static guint signals[LAST_SIGNAL] = {0};

static GtkWidget *
widget_accumulator(GSignalInvocationHint *hint, GValue *return_accu, const GValue *handler_return, gpointer data)
{
	gpointer *obj = g_value_get_object(handler_return);
	if (obj)
		g_value_set_object(return_accu, obj);
	return GTK_WIDGET(obj);
}

G_DEFINE_TYPE(SexyTreeView, sexy_tree_view, GTK_TYPE_TREE_VIEW);

static void
sexy_tree_view_class_init(SexyTreeViewClass *klass)
{
	GObjectClass *gobject_class;
	GtkWidgetClass *widget_class;

	parent_class = g_type_class_peek_parent(klass);

	gobject_class = G_OBJECT_CLASS(klass);
	gobject_class->finalize = sexy_tree_view_finalize;

	widget_class = GTK_WIDGET_CLASS(klass);
	widget_class->button_press_event = sexy_tree_view_button_press_event;
	widget_class->motion_notify_event = sexy_tree_view_motion_notify_event;
	widget_class->leave_notify_event = sexy_tree_view_leave_notify_event;

	/**
	 * SexyTreeView::get-tooltip:
	 * @treeview: The treeview on which the signal is emitted.
	 * @path: The path of the row on which the user is hovering.
	 * @column: The column on which the user is hovering.
	 *
	 * The ::get-tooltip signal is emitted when the user's mouse hovers
	 * over a cell in the tree for longer than 500ms.
	 *
	 * Returns: a #GtkWidget to put in the tooltip window.  If NULL is
	 * returned, no tooltip will be shown.
	 */
	signals[GET_TOOLTIP] = g_signal_new("get-tooltip",
	                                    G_TYPE_FROM_CLASS(gobject_class),
	                                    G_SIGNAL_RUN_LAST,
	                                    G_STRUCT_OFFSET(SexyTreeViewClass, get_tooltip),
	                                    (GSignalAccumulator) widget_accumulator, NULL,
	                                    sexy_marshal_OBJECT__OBJECT_OBJECT,
	                                    GTK_TYPE_WIDGET,
	                                    2, GTK_TYPE_TREE_PATH, GTK_TYPE_TREE_VIEW_COLUMN);
}

static void
sexy_tree_view_init(SexyTreeView *tree_view)
{
	tree_view->priv = g_new0(SexyTreeViewPriv, 1);

	tree_view->priv->tip_label_column = -1;
	/* GtkTreeView starts out with headers visible */
	tree_view->priv->headers_visible = TRUE;

	g_signal_connect(G_OBJECT(tree_view), "columns-changed", G_CALLBACK(sexy_tree_view_columns_changed), NULL);
	g_signal_connect(G_OBJECT(tree_view), "notify::headers-visible", G_CALLBACK(sexy_tree_view_header_visible_notify), NULL);
}

static void
sexy_tree_view_finalize(GObject *obj)
{
	SexyTreeView *view;

	g_return_if_fail(obj != NULL);
	g_return_if_fail(SEXY_IS_TREE_VIEW(obj));

	view = SEXY_TREE_VIEW(obj);
	g_free(view->priv);

	if (G_OBJECT_CLASS(parent_class)->finalize)
		G_OBJECT_CLASS(parent_class)->finalize(obj);
}

static gboolean
sexy_tree_view_button_press_event(GtkWidget *widget, GdkEventButton *event)
{
	SexyTreeView *view = SEXY_TREE_VIEW(widget);
	remove_tooltip(view);

	if (GTK_WIDGET_CLASS(parent_class)->button_press_event)
		return GTK_WIDGET_CLASS(parent_class)->button_press_event(widget, event);
	return FALSE;
}

static gboolean
sexy_tree_view_motion_notify_event(GtkWidget *widget, GdkEventMotion *event)
{
	SexyTreeView *view;
	GtkTreePath *path;
	GtkTreeViewColumn *column;

	view = SEXY_TREE_VIEW(widget);

	if (view->priv->tooltip_timeout) {
		g_source_remove(view->priv->tooltip_timeout);
		view->priv->tooltip_timeout = 0;
	}
	if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(widget), event->x, event->y, &path, &column, NULL, NULL)) {
		gtk_tree_view_get_background_area(GTK_TREE_VIEW(widget), path, column, &view->priv->tip_rect);
		if (view->priv->tip_path && (gtk_tree_path_compare(path, view->priv->tip_path) != 0)) {
			if (view->priv->current_tooltip != NULL) {
				gtk_widget_destroy(view->priv->current_tooltip);
				view->priv->current_tooltip = NULL;
			}
		}
		if (view->priv->tip_path)
			gtk_tree_path_free(view->priv->tip_path);
		view->priv->tip_path = path;
		view->priv->tip_column = column;
		view->priv->tooltip_timeout = g_timeout_add(500, (GSourceFunc) show_tooltip, view);
		view->priv->mouse_x = event->x;
		view->priv->mouse_y = event->y;
	} else {
		if (view->priv->current_tooltip != NULL) {
			gtk_widget_destroy(view->priv->current_tooltip);
			view->priv->current_tooltip = NULL;
		}
	}

	if (GTK_WIDGET_CLASS(parent_class)->motion_notify_event)
		return GTK_WIDGET_CLASS(parent_class)->motion_notify_event(widget, event);
	return FALSE;
}

static gboolean
sexy_tree_view_leave_notify_event(GtkWidget *widget, GdkEventCrossing *event)
{
	SexyTreeView *view = SEXY_TREE_VIEW(widget);
	remove_tooltip(view);

	if (GTK_WIDGET_CLASS(parent_class)->leave_notify_event)
		return GTK_WIDGET_CLASS(parent_class)->leave_notify_event(widget, event);
	return FALSE;
}

static void
sexy_tree_view_columns_changed(GtkTreeView *treeview, gpointer data)
{
	recalculate_header(SEXY_TREE_VIEW(treeview));
}

static void
sexy_tree_view_header_visible_notify(GObject *object, GParamSpec *pspec, gpointer data)
{
	SexyTreeView *tree_view;

	tree_view = SEXY_TREE_VIEW(object);
	tree_view->priv->headers_visible = gtk_tree_view_get_headers_visible(GTK_TREE_VIEW(object));

	recalculate_header(tree_view);
}

static void
recalculate_header(SexyTreeView *tree_view)
{
	GList *columns, *i;
	gint header = 0;

	if (tree_view->priv->headers_visible == FALSE) {
		tree_view->priv->header_size = 0;
		return;
	}

	columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(tree_view));
	for (i = columns; i; i = g_list_next(i)) {
		GtkTreeViewColumn *column;
		GtkRequisition requisition;

		column = GTK_TREE_VIEW_COLUMN(i->data);
		gtk_widget_size_request (column->button, &requisition);
		header = MAX(header, requisition.height);
	}
	g_list_free(columns);

	tree_view->priv->header_size = header;
}

static void
remove_tooltip(SexyTreeView *tree_view)
{
	if (tree_view->priv->current_tooltip != NULL) {
		gtk_widget_hide_all(tree_view->priv->current_tooltip);
		gtk_widget_destroy(tree_view->priv->current_tooltip);
		tree_view->priv->current_tooltip = NULL;
	}
	if (tree_view->priv->tooltip_timeout != 0) {
		g_source_remove(tree_view->priv->tooltip_timeout);
		tree_view->priv->tooltip_timeout = 0;
	}
	tree_view->priv->tip_path = NULL;
	tree_view->priv->tip_column = NULL;
}

static gboolean
show_tooltip(SexyTreeView *tree_view)
{
	tree_view->priv->tooltip_timeout = 0;

	if (tree_view->priv->tip_path == NULL)
		return FALSE;

	if ((tree_view->priv->tip_label_column) != -1 && (tree_view->priv->current_tooltip == NULL)) {
		GtkTreeIter iter;
		GtkTreeModel *model;
		gchar *text;

		model = gtk_tree_view_get_model(GTK_TREE_VIEW(tree_view));
		gtk_tree_model_get_iter(model, &iter, tree_view->priv->tip_path);

		gtk_tree_model_get(model, &iter, tree_view->priv->tip_label_column, &text, -1);
		tree_view->priv->current_tooltip = sexy_tooltip_new_with_label(text);
		g_free(text);
	} else if (tree_view->priv->current_tooltip == NULL) {
		GtkWidget *tip_widget = NULL;

		g_signal_emit(G_OBJECT(tree_view), signals[GET_TOOLTIP], 0, tree_view->priv->tip_path, tree_view->priv->tip_column, &tip_widget);

		if (tip_widget) {
			tree_view->priv->current_tooltip = sexy_tooltip_new();
			gtk_container_add(GTK_CONTAINER(tree_view->priv->current_tooltip), tip_widget);
		}
	}

	if (tree_view->priv->current_tooltip != NULL) {
		gint ox, oy;
		gint tx, ty;
		GdkRectangle rect;
		GdkScreen *screen;

		gdk_window_get_origin(GTK_WIDGET(tree_view)->window, &ox, &oy);
		screen = gtk_widget_get_screen(GTK_WIDGET(tree_view));

		tx = tree_view->priv->tip_rect.x;
		ty = tree_view->priv->tip_rect.y;

		rect.x = tx + ox;
		rect.y = ty + oy + tree_view->priv->header_size;
		rect.width = tree_view->priv->tip_rect.width;
		rect.height = tree_view->priv->tip_rect.height;

		sexy_tooltip_position_to_rect(SEXY_TOOLTIP(tree_view->priv->current_tooltip), &rect, screen);
		gtk_widget_show(tree_view->priv->current_tooltip);
	}

	return FALSE;
}

/**
 * sexy_tree_view_new
 *
 * Creates a new SexyTreeView widget
 *
 * Returns: a new #SexyTreeView
 */
GtkWidget *
sexy_tree_view_new(void)
{
	return GTK_WIDGET(g_object_new(SEXY_TYPE_TREE_VIEW, NULL));
}

/**
 * sexy_tree_view_set_tooltip_label_column:
 * @treeview: A #SexyTreeView
 * @column: The column of the tree model to use for tooltip text.
 *
 * Sets a column of the view's model to be used for the text in tooltips.
 */
void
sexy_tree_view_set_tooltip_label_column(SexyTreeView *treeview, guint column)
{
	treeview->priv->tip_label_column = column;
}
