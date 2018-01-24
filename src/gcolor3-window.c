/* Gcolor3Window
 *
 * Copyright (C) 2015-2018 Jente Hidskes
 *
 * Author: Jente Hidskes <hjdskes@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <glib/gi18n.h>

#include "gcolor3-color-selection.h"
#include "gcolor3-color-store.h"
#include "gcolor3-window.h"

enum {
	COLOR_PIXBUF,
	COLOR_VALUE,
	COLOR_NAME,
	N_COLUMNS
};

enum {
	PROP_0,
	PROP_STORE,
};

struct _Gcolor3WindowPrivate {
	GtkWidget *headerbar;
	GtkWidget *button;
	GtkWidget *revealer;
	GtkWidget *entry;
	GtkWidget *stack;
	GtkWidget *picker;
	GtkWidget *tree;
	GtkTreeSelection *selection;

	Gcolor3ColorStore *store;

	GdkRGBA current;
};

static void gcolor3_window_color_added (UNUSED Gcolor3ColorStore *store,
					const gchar              *key,
					const gchar              *hex,
					gpointer                  user_data);

G_DEFINE_TYPE_WITH_PRIVATE (Gcolor3Window, gcolor3_window, GTK_TYPE_APPLICATION_WINDOW)

static inline gchar *
hex_value (GdkRGBA *color) {
	return g_strdup_printf ("#%.2X%.2X%.2X",
				(unsigned int) color->red * 256,
				(unsigned int) color->green * 256,
				(unsigned int) color->blue * 256);
}

static void
set_color_in_clipboard (const gchar *color)
{
	GtkClipboard *clipboard;

	clipboard = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);
	gtk_clipboard_set_text (clipboard, color, -1);
}

static GdkPixbuf *
create_pixbuf_from_xpm (const gchar *hex)
{
	gchar colorline[] = ".      c #FFFFFF";
	const gchar *xpm[] = {
		"16 14 1 1",
		".      c #FFFFFF",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................",
		"................"
	};

	g_sprintf (colorline, ".      c %s", hex);
	xpm[1] = colorline;
	return gdk_pixbuf_new_from_xpm_data ((gchar const **) xpm);
}

static void
gcolor3_window_action_copy (UNUSED GSimpleAction *action,
			     UNUSED GVariant      *parameter,
			     gpointer              user_data)
{
	Gcolor3WindowPrivate *priv;
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *hex;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));

	if (!gtk_tree_selection_get_selected (priv->selection, &model, &iter)) {
		return;
	}

	gtk_tree_model_get (model, &iter, COLOR_VALUE, &hex, -1);
	set_color_in_clipboard (hex);
	g_free (hex);
}

static void
gcolor3_window_action_save (UNUSED GSimpleAction *action,
			    UNUSED GVariant      *parameter,
			    gpointer              user_data)
{
	Gcolor3WindowPrivate *priv;
	const gchar *key;
	gchar *hex;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));

	key = gtk_entry_get_text (GTK_ENTRY (priv->entry));
	hex = hex_value (&priv->current);
	if (strlen (key) == 0) {
		/* If using `hex` as key, do not save the first character (e.g., `#`)
		 * because GKeyFile will see these as (and subsequently ignore) comments. */
		key = hex + 1;
	}

	/* This will add the color to the store (if the key isn't already used), which in turn
	 * shall emit the signal which will trigger the color_added callback below. */
	gcolor3_color_store_add_color (priv->store, key, hex);
	g_free (hex);
}

static void
gcolor3_window_action_delete (UNUSED GSimpleAction *action,
			      UNUSED GVariant      *parameter,
			      gpointer              user_data)
{
	Gcolor3WindowPrivate *priv;
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *key;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));

	if (!gtk_tree_selection_get_selected (priv->selection, &model, &iter)) {
		return;
	}

	gtk_tree_model_get (model, &iter, COLOR_NAME, &key, -1);
	/* This will remove the color from the store, which in turn shall emit the signal which
	 * will trigger the color_removed callback below. */
	gcolor3_color_store_remove_color (priv->store, key);
	g_free (key);
}

static void
gcolor3_window_action_change_page (UNUSED GSimpleAction *action,
				   UNUSED GVariant      *parameter,
				   gpointer              user_data)
{
	Gcolor3WindowPrivate *priv;
	const gchar *page;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));

	page = gtk_stack_get_visible_child_name (GTK_STACK (priv->stack));
	if (g_strcmp0 (page, "saved-colors") == 0) {
		gtk_stack_set_visible_child_name (GTK_STACK (priv->stack), "picker");
	} else {
		gtk_stack_set_visible_child_name (GTK_STACK (priv->stack), "saved-colors");
	}
}

static const GActionEntry window_actions[] = {
	{ "copy", gcolor3_window_action_copy, NULL, NULL, NULL },
	{ "save", gcolor3_window_action_save, NULL, NULL, NULL },
	{ "delete", gcolor3_window_action_delete, NULL, NULL, NULL },
	{ "change-page", gcolor3_window_action_change_page, NULL, NULL, NULL },
};

static void
gcolor3_window_add_existing_color (const gchar *key, const gchar *hex, gpointer user_data)
{
	Gcolor3WindowPrivate *priv;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));

	/* Simply wrap this callback to avoid code duplication. */
	gcolor3_window_color_added (priv->store, key, hex, user_data);
}

static void
gcolor3_window_picker_changed (Gcolor3ColorSelection *picker, gpointer user_data)
{
	Gcolor3WindowPrivate *priv;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));

	gcolor3_color_selection_get_current_color (GCOLOR3_COLOR_SELECTION (picker), &priv->current);
}

/* FIXME: delete button is sensitive when switching to saved colors, even though there are none. */
static void
gcolor3_window_stack_changed (GtkStack          *stack,
			      UNUSED GParamSpec *pspec,
			      gpointer           user_data)
{
	Gcolor3WindowPrivate *priv;
	GtkWidget *image;
	const gchar *page;
	GAction *save_action;
	GAction *delete_action;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));

	save_action = g_action_map_lookup_action (G_ACTION_MAP (user_data), "save");
	delete_action = g_action_map_lookup_action (G_ACTION_MAP (user_data), "delete");

	page = gtk_stack_get_visible_child_name (stack);
	if (g_strcmp0 (page, "saved-colors") == 0) {
		image = gtk_image_new_from_icon_name ("edit-delete-symbolic", GTK_ICON_SIZE_MENU);
		if (gtk_tree_selection_get_selected (priv->selection, NULL, NULL)) {
			gtk_widget_set_sensitive (priv->button, TRUE);
		} else {
			gtk_widget_set_sensitive (priv->button, FALSE);
		}
		gtk_actionable_set_action_name (GTK_ACTIONABLE (priv->button), "win.delete");
		g_simple_action_set_enabled (G_SIMPLE_ACTION (save_action), FALSE);
		g_simple_action_set_enabled (G_SIMPLE_ACTION (delete_action), TRUE);
		gtk_revealer_set_reveal_child (GTK_REVEALER (priv->revealer), FALSE);
	} else {
		image = gtk_image_new_from_icon_name ("document-save-symbolic", GTK_ICON_SIZE_MENU);
		gtk_widget_set_sensitive (priv->button, TRUE);
		gtk_actionable_set_action_name (GTK_ACTIONABLE (priv->button), "win.save");
		g_simple_action_set_enabled (G_SIMPLE_ACTION (save_action), TRUE);
		g_simple_action_set_enabled (G_SIMPLE_ACTION (delete_action), FALSE);
		gtk_revealer_set_reveal_child (GTK_REVEALER (priv->revealer), TRUE);
	}

	gtk_button_set_image (GTK_BUTTON (priv->button), image);
}

static void
gcolor3_window_selection_changed (GtkTreeSelection *selection, gpointer user_data)
{
	Gcolor3WindowPrivate *priv;
	GdkRGBA new, current;
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *color;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));

	if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
		gtk_tree_model_get (model, &iter, COLOR_VALUE, &color, -1);
		gdk_rgba_parse (&new, color);
		g_free (color);

		/* Save the old color in the picker. */
		gcolor3_color_selection_get_current_color (
			GCOLOR3_COLOR_SELECTION (priv->picker), &current);
		gcolor3_color_selection_set_previous_color (
			GCOLOR3_COLOR_SELECTION (priv->picker), &current);
		gcolor3_color_selection_set_current_color (
			GCOLOR3_COLOR_SELECTION (priv->picker), &new);

		gtk_widget_set_sensitive (priv->button, TRUE);
	} else {
		gtk_widget_set_sensitive (priv->button, FALSE);
	}
}

static void
gcolor3_window_color_added (UNUSED Gcolor3ColorStore *store,
			    const gchar              *key,
			    const gchar              *hex,
			    gpointer                  user_data)
{
	Gcolor3WindowPrivate *priv;
	GdkPixbuf *pixbuf;
	GtkTreeIter iter;
	GtkTreeModel *model;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));
	model = gtk_tree_view_get_model (GTK_TREE_VIEW (priv->tree));
	pixbuf = create_pixbuf_from_xpm (hex);

	gtk_list_store_prepend (GTK_LIST_STORE (model), &iter);
	gtk_list_store_set (GTK_LIST_STORE (model), &iter,
			    COLOR_PIXBUF, pixbuf,
			    COLOR_VALUE, hex,
			    COLOR_NAME, key,
			    -1);
	gtk_tree_selection_select_iter (priv->selection, &iter);
	g_object_unref (pixbuf);
}

static void
gcolor3_window_color_removed (UNUSED Gcolor3ColorStore *store, UNUSED const gchar *key, gpointer user_data)
{
	Gcolor3WindowPrivate *priv;
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *needle;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));
	model = gtk_tree_view_get_model (GTK_TREE_VIEW (priv->tree));

	if (!gtk_tree_model_get_iter_first (model, &iter)) {
		g_warning (_("The model is empty; there are no colors to remove"));
		return;
	}

	/* Naive way to set iter to the correct row, but the data size shouldn't be so large that
	 * this will turn noticably slow. Still a sad waste, considering we do have the correct
	 * iter in action_delete... Note that get_selected cannot be used as it is in action_delete,
	 * since there might be a secondary instance of Gcolor3 running with a different selection.
	 */
	for (;;) {
		gtk_tree_model_get (model, &iter, COLOR_NAME, &needle, -1);
		if (g_strcmp0 (key, needle) == 0) {
			gtk_list_store_remove (GTK_LIST_STORE (model), &iter);
			g_free (needle);
			break;
		}
		g_free (needle);
		if (!gtk_tree_model_iter_next (model, &iter)) {
			break;
		}
	}
}

static void
gcolor3_window_color_renamed (UNUSED GtkCellRendererText *renderer,
			      gchar                      *path,
			      gchar                      *new_text,
			      gpointer                    user_data)
{
	Gcolor3WindowPrivate *priv;
	GtkTreeModel *model;
	GtkTreeIter iter;
	gchar *old_text;
	gboolean success;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));
	model = gtk_tree_view_get_model (GTK_TREE_VIEW (priv->tree));

	success = gtk_tree_model_get_iter_from_string (model, &iter, path);
	if (!success) {
		g_warning (_("Cannot update color: invalid path"));
		return;
	}

	gtk_tree_model_get (model, &iter, COLOR_NAME, &old_text, -1);
	if (gcolor3_color_store_rename_color (priv->store, old_text, new_text)) {
		gtk_list_store_set (GTK_LIST_STORE (model), &iter, COLOR_NAME, new_text, -1);
	}

	g_free (old_text);
}

static void
gcolor3_window_set_property (GObject      *object,
			     guint         prop_id,
			     const GValue *value,
			     GParamSpec   *pspec)
{
	Gcolor3WindowPrivate *priv;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (object));

	switch (prop_id) {
		case PROP_STORE:
			priv->store = GCOLOR3_COLOR_STORE (g_value_dup_object (value));
			g_signal_connect (priv->store, "color-added",
					  G_CALLBACK (gcolor3_window_color_added), object);
			g_signal_connect (priv->store, "color-removed",
					  G_CALLBACK (gcolor3_window_color_removed), object);
			gcolor3_color_store_foreach (priv->store,
						     gcolor3_window_add_existing_color,
						     object);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
gcolor3_window_get_property (GObject    *object,
			     guint       prop_id,
			     GValue     *value,
			     GParamSpec *pspec)
{
	Gcolor3WindowPrivate *priv;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (object));

	switch (prop_id) {
		case PROP_STORE:
			g_value_set_object (value, priv->store);
			break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
			break;
	}
}

static void
gcolor3_window_dispose (GObject *object)
{
	Gcolor3WindowPrivate *priv;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (object));

	g_clear_object (&priv->store);

	G_OBJECT_CLASS (gcolor3_window_parent_class)->dispose (object);
}

static void
gcolor3_window_finalize (GObject *object)
{
	Gcolor3WindowPrivate *priv;
	gchar *hex;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (object));

	hex = hex_value (&priv->current);
	g_printf ("%s\n", hex);
	g_free (hex);

	G_OBJECT_CLASS (gcolor3_window_parent_class)->finalize (object);
}

static void
gcolor3_window_class_init (Gcolor3WindowClass *gcolor3_window_class)
{
	GObjectClass *object_class = G_OBJECT_CLASS (gcolor3_window_class);
	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (gcolor3_window_class);

	object_class->set_property = gcolor3_window_set_property;
	object_class->get_property = gcolor3_window_get_property;
	object_class->dispose = gcolor3_window_dispose;
	object_class->finalize = gcolor3_window_finalize;

	g_object_class_install_property (object_class, PROP_STORE,
					 g_param_spec_object ("color-store",
							      "ColorStore",
							      "The managed colors",
							      GCOLOR3_TYPE_COLOR_STORE,
							      G_PARAM_READWRITE |
							      G_PARAM_PRIVATE |
							      G_PARAM_CONSTRUCT_ONLY |
							      G_PARAM_STATIC_STRINGS));

	gtk_widget_class_set_template_from_resource (widget_class, "/org/hjdskes/gcolor3/window.ui");

	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, headerbar);
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, button);
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, revealer);
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, entry);
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, stack);
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, tree);
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, selection);

	gtk_widget_class_bind_template_callback (widget_class, gcolor3_window_stack_changed);
	gtk_widget_class_bind_template_callback (widget_class, gcolor3_window_picker_changed);
	gtk_widget_class_bind_template_callback (widget_class, gcolor3_window_selection_changed);
}

static void
gcolor3_window_setup_tree_view (Gcolor3Window *window)
{
	Gcolor3WindowPrivate *priv;
	GtkCellRenderer *pixbuf_renderer;
	GtkCellRenderer *hex_renderer;
	GtkCellRenderer *name_renderer;
	GtkTreeViewColumn *column;

	priv = gcolor3_window_get_instance_private (window);

	pixbuf_renderer = gtk_cell_renderer_pixbuf_new ();
	column = gtk_tree_view_column_new_with_attributes (_("Color"),
							   pixbuf_renderer,
							   "pixbuf",
							   COLOR_PIXBUF,
							   NULL);

	hex_renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column, hex_renderer, TRUE);
	gtk_tree_view_column_set_attributes (column,
					     hex_renderer,
					     "text",
					     COLOR_VALUE,
					     NULL);
	gtk_tree_view_column_set_sort_column_id (column, COLOR_VALUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (priv->tree), column);

	name_renderer = gtk_cell_renderer_text_new ();
	g_object_set (name_renderer, "editable", TRUE, NULL);
	g_signal_connect (name_renderer, "edited",
			  G_CALLBACK (gcolor3_window_color_renamed), window);
	column = gtk_tree_view_column_new_with_attributes (_("Name"),
							   name_renderer,
							   "text",
							   COLOR_NAME,
							   NULL);
	gtk_tree_view_column_set_sort_column_id (column, COLOR_NAME);
	gtk_tree_view_append_column (GTK_TREE_VIEW (priv->tree), column);
}

static void
gcolor3_window_init (Gcolor3Window *window)
{
	Gcolor3WindowPrivate *priv;

	priv = gcolor3_window_get_instance_private (window);

	gtk_widget_init_template (GTK_WIDGET (window));
	gtk_header_bar_set_title (GTK_HEADER_BAR (priv->headerbar), g_get_application_name ());

	/* Add the custom color selection widget. */
	priv->picker = gcolor3_color_selection_new ();
	gtk_widget_set_valign (priv->picker, GTK_ALIGN_CENTER);
	gtk_widget_set_halign (priv->picker, GTK_ALIGN_CENTER);
	g_signal_connect (priv->picker, "color-changed",
			  G_CALLBACK (gcolor3_window_picker_changed), window);
	gtk_stack_add_titled (GTK_STACK (priv->stack), priv->picker, "picker", _("Picker"));
	gtk_container_child_set (GTK_CONTAINER (priv->stack), priv->picker, "position", 0, NULL);
	gtk_widget_set_visible (priv->picker, TRUE);

	/* Call the callback to initialise the GtkEntry and to prevent
	 * saving #000000 when saving the white color right away. */
	gcolor3_window_picker_changed (GCOLOR3_COLOR_SELECTION (priv->picker), window);

	g_action_map_add_action_entries (G_ACTION_MAP (window),
					 window_actions,
					 G_N_ELEMENTS (window_actions),
					 window);

	gcolor3_window_setup_tree_view (window);

	/* Finally, make the color picker the visible stack page. */
	gtk_stack_set_visible_child_name (GTK_STACK (priv->stack), "picker");
}

Gcolor3Window *
gcolor3_window_new (Gcolor3Application *application, Gcolor3ColorStore *store)
{
	g_return_val_if_fail (GCOLOR3_IS_APPLICATION (application), NULL);
	g_return_val_if_fail (GCOLOR3_IS_COLOR_STORE (store), NULL);

	return g_object_new (GCOLOR3_TYPE_WINDOW,
			     "application", application,
			     "color-store", store,
			     NULL);
}

