/*
 * @file libsexy/sexy-tree-view.h A treeview subclass which can display
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
#ifndef _SEXY_TREE_VIEW_H
#define _SEXY_TREE_VIEW_H

typedef struct _SexyTreeView      SexyTreeView;
typedef struct _SexyTreeViewClass SexyTreeViewClass;
typedef struct _SexyTreeViewPriv  SexyTreeViewPriv;

#include <gtk/gtktreeview.h>

#define SEXY_TYPE_TREE_VIEW            (sexy_tree_view_get_type())
#define SEXY_TREE_VIEW(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), SEXY_TYPE_TREE_VIEW, SexyTreeView))
#define SEXY_TREE_VIEW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass), SEXY_TYPE_TREE_VIEW, SexyTreeViewClass))
#define SEXY_IS_TREE_VIEW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), SEXY_TYPE_TREE_VIEW))
#define SEXY_IS_TREE_VIEW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass), SEXY_TYPE_TREE_VIEW))
#define SEXY_TREE_VIEW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), SEXY_TYPE_TREE_VIEW, SexyTreeViewClass))

struct _SexyTreeView
{
	GtkTreeView parent;

	SexyTreeViewPriv *priv;
};

struct _SexyTreeViewClass
{
	GtkTreeViewClass parent_class;

	/* Signals */
	GtkWidget *(*get_tooltip)(SexyTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column);
};

G_BEGIN_DECLS

GType      sexy_tree_view_get_type(void);
GtkWidget *sexy_tree_view_new(void);
void       sexy_tree_view_set_tooltip_label_column(SexyTreeView *treeview, guint column);

G_END_DECLS

#endif /* _SEXY_TREE_VIEW_H_ */
