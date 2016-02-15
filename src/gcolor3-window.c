/* Gcolor3Window
 *
 * Copyright (C) 2015-2016 Jente Hidskes
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

#include "gcolor3-window.h"

/* I know GtkColorSelection is deprecated... */
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

/* Copyright years. */
#define COPYRIGHT "2013-2016"

/* The duration of the revealer's animation. */
#define REVEALER_DURATION 400

/* The border inside the window. */
#define BORDER_WIDTH 6

enum {
	COLOR_PIXBUF,
	COLOR_VALUE,
	COLOR_NAME,
	N_COLUMNS
};

struct _Gcolor3WindowPrivate {
	GtkWidget *button;
	GtkWidget *revealer;
	GtkWidget *entry;
	GtkWidget *stack;
	GtkWidget *picker;
	GtkWidget *tree;

	GKeyFile *colors; /* Do not free: owned by Gcolor3Application! */
	GtkTreeSelection *selection;
	GdkColor current;
};

G_DEFINE_TYPE_WITH_PRIVATE (Gcolor3Window, gcolor3_window, GTK_TYPE_APPLICATION_WINDOW);

static void gcolor3_window_add_color_to_list (Gcolor3Window *window,
					      const gchar *name,
					      const gchar *value,
					      gboolean new);

static gchar *
hex_value (GdkColor *color) {
	return g_strdup_printf ("#%.2X%.2X%.2X",
				color->red / 256,
				color->green / 256,
				color->blue / 256);
}

static void
gcolor3_window_action_save (UNUSED GSimpleAction *action,
			    UNUSED GVariant      *parameter,
			    gpointer              user_data)
{
	Gcolor3WindowPrivate *priv;
	const gchar *key;
	gchar *hex;

	g_return_if_fail (GCOLOR3_IS_WINDOW (user_data));
	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));
	g_return_if_fail (priv->colors != NULL);

	key = gtk_entry_get_text (GTK_ENTRY (priv->entry));
	hex = hex_value (&priv->current);
	if (strlen (key) == 0) {
		/* If using `hex` as key, do not save the first character (e.g., `#`)
		 * because GKeyFile will see these as (and subsequently ignore) comments. */
		key = hex + 1;
	}

	if (!g_key_file_has_key (priv->colors, "Colors", key, NULL)) {
		g_key_file_set_string (priv->colors, "Colors", key, hex);
		gcolor3_window_add_color_to_list (GCOLOR3_WINDOW (user_data), key, hex, TRUE);
	}

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
	GError *error = NULL;
	gchar *key;

	g_return_if_fail (GCOLOR3_IS_WINDOW (user_data));
	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));
	g_return_if_fail (priv->colors != NULL);

	if (!gtk_tree_selection_get_selected (priv->selection, &model, &iter)) {
		return;
	}

	gtk_tree_model_get (model, &iter, COLOR_NAME, &key, -1);
	if (!(g_key_file_remove_key (priv->colors, "Colors", key, &error))) {
		g_warning ("%s%s", _("Error deleting key: "), error->message);
		g_clear_error (&error);
	} else {
		gtk_list_store_remove (GTK_LIST_STORE (model), &iter);
	}

	g_free (key);
}

static void
gcolor3_window_action_change_page (UNUSED GSimpleAction *action,
				   UNUSED GVariant      *parameter,
				   gpointer              user_data)
{
	Gcolor3WindowPrivate *priv;
	const gchar *page;

	g_return_if_fail (GCOLOR3_IS_WINDOW (user_data));
	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));

	page = gtk_stack_get_visible_child_name (GTK_STACK (priv->stack));
	if (g_strcmp0 (page, "saved-colors") == 0) {
		gtk_stack_set_visible_child_name (GTK_STACK (priv->stack), "picker");
	} else {
		gtk_stack_set_visible_child_name (GTK_STACK (priv->stack), "saved-colors");
	}
}

static const GActionEntry window_actions[] = {
	{ "save", gcolor3_window_action_save, NULL, NULL, NULL },
	{ "delete", gcolor3_window_action_delete, NULL, NULL, NULL },
	{ "change-page", gcolor3_window_action_change_page, NULL, NULL, NULL },
};

static void
gcolor3_window_add_color_to_list (Gcolor3Window *window,
				  const gchar *name,
				  const gchar *value,
				  gboolean new)
{
	Gcolor3WindowPrivate *priv;
	GtkTreeModel *model;
	GtkTreeIter iter;
	GdkPixbuf *pixbuf;
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

	g_return_if_fail (GCOLOR3_IS_WINDOW (window));
	priv = gcolor3_window_get_instance_private (window);
	model = gtk_tree_view_get_model (GTK_TREE_VIEW (priv->tree));

	g_sprintf (colorline, ".      c %s", value);
	xpm[1] = colorline;
	pixbuf = gdk_pixbuf_new_from_xpm_data ((gchar const **) xpm);

	if (new) {
		gtk_list_store_prepend (GTK_LIST_STORE (model), &iter);
	} else {
		gtk_list_store_append (GTK_LIST_STORE (model), &iter);
	}

	gtk_list_store_set (GTK_LIST_STORE (model), &iter,
			    COLOR_PIXBUF, pixbuf,
			    COLOR_VALUE, value,
			    COLOR_NAME, name,
			    -1);

	if (new) {
		gtk_tree_selection_select_iter (priv->selection, &iter);
	}
}

static void
gcolor3_window_picker_changed (GtkColorSelection *picker, gpointer user_data)
{
	Gcolor3WindowPrivate *priv;

	g_return_if_fail (GCOLOR3_IS_WINDOW (user_data));
	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));

	gtk_color_selection_get_current_color (GTK_COLOR_SELECTION (picker), &priv->current);
}

/* FIXME: delete button is sensitive when switching to saved colors,
 * even though there are none. */
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

	g_return_if_fail (GCOLOR3_IS_WINDOW (user_data));
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
	GdkColor new, current;
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *color;

	g_return_if_fail (GCOLOR3_IS_WINDOW (user_data));
	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));

	if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
		gtk_tree_model_get (model, &iter, COLOR_VALUE, &color, -1);
		gdk_color_parse (color, &new);
		g_free (color);

		/* Save the old color in the picker. */
		gtk_color_selection_get_current_color (
			GTK_COLOR_SELECTION (priv->picker), &current);
		gtk_color_selection_set_previous_color (
			GTK_COLOR_SELECTION (priv->picker), &current);
		gtk_color_selection_set_current_color (
			GTK_COLOR_SELECTION (priv->picker), &new);

		gtk_widget_set_sensitive (priv->button, TRUE);
	} else {
		gtk_widget_set_sensitive (priv->button, FALSE);
	}
}

static void
gcolor3_window_construct_ui (Gcolor3Window *window)
{
	Gcolor3WindowPrivate *priv;
	GtkListStore *liststore;
	GtkTreeViewColumn *column;
	GtkCellRenderer *renderer;
	GtkWidget *headerbar;
	GtkWidget *switcher;
	GtkWidget *scroll;

	g_return_if_fail (GCOLOR3_IS_WINDOW (window));
	priv = gcolor3_window_get_instance_private (window);

	headerbar = gtk_header_bar_new ();
	gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (headerbar), TRUE);
	gtk_header_bar_set_title (GTK_HEADER_BAR (headerbar), g_get_application_name ());
	gtk_window_set_titlebar (GTK_WINDOW (window), headerbar);
	gtk_widget_show (headerbar);

	priv->button = gtk_button_new ();
	gtk_header_bar_pack_end (GTK_HEADER_BAR (headerbar), priv->button);
	gtk_actionable_set_action_name (GTK_ACTIONABLE (priv->button), "win.save");
	/* Make default, so that the GtkEntry later on can use it. */
	gtk_widget_set_can_default (priv->button, TRUE);
	gtk_widget_grab_default (priv->button);
	gtk_widget_show (priv->button);

	priv->revealer = gtk_revealer_new ();
	gtk_revealer_set_transition_duration (GTK_REVEALER (priv->revealer), REVEALER_DURATION);
	gtk_revealer_set_transition_type (GTK_REVEALER (priv->revealer),
					  GTK_REVEALER_TRANSITION_TYPE_SLIDE_LEFT);
	gtk_header_bar_pack_end (GTK_HEADER_BAR (headerbar), priv->revealer);
	gtk_widget_show (priv->revealer);

	priv->entry = gtk_entry_new ();
	/* Activate default widget on enter, set above. */
	gtk_entry_set_activates_default (GTK_ENTRY (priv->entry), TRUE);
	gtk_entry_set_placeholder_text (GTK_ENTRY (priv->entry), _("Color name..."));
	gtk_container_add (GTK_CONTAINER (priv->revealer), priv->entry);
	gtk_widget_show (priv->entry);

	priv->stack = gtk_stack_new ();
	gtk_stack_set_transition_type (GTK_STACK (priv->stack),
				       GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
	g_signal_connect (priv->stack, "notify::visible-child",
			  G_CALLBACK (gcolor3_window_stack_changed), window);
	gtk_container_add (GTK_CONTAINER (window), priv->stack);
	gtk_widget_show (priv->stack);

	switcher = gtk_stack_switcher_new ();
	gtk_stack_switcher_set_stack (GTK_STACK_SWITCHER (switcher), GTK_STACK (priv->stack));
	gtk_header_bar_pack_start (GTK_HEADER_BAR (headerbar), switcher);
	gtk_widget_show (switcher);

	priv->picker = gtk_color_selection_new ();
	g_signal_connect (priv->picker, "color-changed",
			  G_CALLBACK (gcolor3_window_picker_changed), window);
	/* Call the above callback to initialise the GtkEntry and to prevent
	 * saving #000000 when saving the white color right away. */
	gcolor3_window_picker_changed (GTK_COLOR_SELECTION (priv->picker), window);
	gtk_stack_add_titled (GTK_STACK (priv->stack), priv->picker, "picker", _("Picker"));
	gtk_widget_show (priv->picker);

	scroll = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_set_vexpand (scroll, TRUE);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll),
					GTK_POLICY_AUTOMATIC,
					GTK_POLICY_AUTOMATIC);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (scroll),
					     GTK_SHADOW_IN);
	gtk_stack_add_titled (GTK_STACK (priv->stack), scroll, "saved-colors", _("Saved colors"));
	gtk_widget_show (scroll);

	liststore = gtk_list_store_new (N_COLUMNS,
					GDK_TYPE_PIXBUF,
					G_TYPE_STRING,
					G_TYPE_STRING,
					G_TYPE_STRING);
	priv->tree = gtk_tree_view_new_with_model (GTK_TREE_MODEL (liststore));
	g_object_unref (liststore);
	gtk_tree_view_set_search_column (GTK_TREE_VIEW (priv->tree), COLOR_NAME);
	gtk_container_add (GTK_CONTAINER (scroll), priv->tree);
	gtk_widget_show (priv->tree);

	renderer = gtk_cell_renderer_pixbuf_new ();
	column = gtk_tree_view_column_new_with_attributes (_("Color"),
							   renderer,
							   "pixbuf",
							   COLOR_PIXBUF,
							   NULL);
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column, renderer, TRUE);
	gtk_tree_view_column_set_attributes (column,
					     renderer,
					     "text",
					     COLOR_VALUE,
					     NULL);
	gtk_tree_view_column_set_sort_column_id (column, COLOR_VALUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (priv->tree), column);

	column = gtk_tree_view_column_new_with_attributes (_("Name"),
							   renderer,
							   "text",
							   COLOR_NAME,
							   NULL);
	gtk_tree_view_column_set_sort_column_id (column, COLOR_NAME);
	gtk_tree_view_append_column (GTK_TREE_VIEW (priv->tree), column);

	priv->selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (priv->tree));
	gtk_tree_selection_set_mode (priv->selection, GTK_SELECTION_SINGLE);
	g_signal_connect (priv->selection, "changed",
			  G_CALLBACK (gcolor3_window_selection_changed), window);
}

static GObject *
gcolor3_window_constructor (GType type,
			    guint n_construct_properties,
			    GObjectConstructParam *construct_params)
{
	GObject *object;

	object = G_OBJECT_CLASS (gcolor3_window_parent_class)->constructor
		(type, n_construct_properties, construct_params);

	gcolor3_window_construct_ui (GCOLOR3_WINDOW (object));

	return object;
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

	object_class->constructor = gcolor3_window_constructor;
	object_class->finalize = gcolor3_window_finalize;
}

static void
gcolor3_window_init (Gcolor3Window *window)
{
	gtk_container_set_border_width (GTK_CONTAINER (window), BORDER_WIDTH);
	gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
	gtk_window_set_position (GTK_WINDOW (window), GTK_WIN_POS_CENTER);

	g_action_map_add_action_entries (G_ACTION_MAP (window),
					 window_actions,
					 G_N_ELEMENTS (window_actions),
					 window);
}

Gcolor3Window *
gcolor3_window_new (Gcolor3Application *application)
{
	g_return_val_if_fail (GCOLOR3_IS_APPLICATION (application), NULL);
	return g_object_new (GCOLOR3_TYPE_WINDOW,
			     "application",
			     application,
			     NULL);
}

void
gcolor3_window_show_about_dialog (Gcolor3Window *window)
{
	g_return_if_fail (GCOLOR3_IS_WINDOW (window));

	static const char *authors[] = {
		"Jente Hidskes <hjdskes@gmail.com>",
		NULL,
	};

	static const char *artists[] = {
		"hhh",
		NULL,
	};

	gtk_show_about_dialog (GTK_WINDOW (window),
			       "program-name", g_get_application_name (),
			       "version", PACKAGE_VERSION,
			       "copyright", "Copyright \xc2\xa9 "COPYRIGHT" Jente Hidskes",
			       "comments", _("Choose colors from the picker or the screen"),
			       "authors", authors,
			       "artists", artists,
			       /* Translators: translate this to give yourself credits. */
			       "translator-credits", _("translator-credits"),
			       "website-label", _("Website"),
			       "website", PACKAGE_URL,
			       "logo-icon-name", gtk_window_get_default_icon_name (),
			       "wrap-license", TRUE,
			       "license-type", GTK_LICENSE_GPL_2_0,
			       NULL);
}

void
gcolor3_window_close (Gcolor3Window *window)
{
	g_return_if_fail (GCOLOR3_IS_WINDOW (window));
	gtk_widget_destroy (GTK_WIDGET (window));
}

void
gcolor3_window_add_colors (Gcolor3Window *window)
{
	GtkApplication *application;
	Gcolor3WindowPrivate *priv;
	GError *error = NULL;
	gchar **keys = NULL;
	gsize length;

	g_return_if_fail (GCOLOR3_IS_WINDOW (window));

	priv = gcolor3_window_get_instance_private (window);
	application = gtk_window_get_application (GTK_WINDOW (window));

	if (!(priv->colors = gcolor3_application_get_colors (GCOLOR3_APPLICATION (application)))) {
		return;
	}

	if (!(keys = g_key_file_get_keys (priv->colors, "Colors", &length, &error))) {
		g_warning ("%s%s", _("Error reading keys: "), error->message);
		g_clear_error (&error);
		return;
	}

	for (guint i = 0; i < length; i++) {
		gchar *key = *(keys + i);
		gchar *value;

		if (!(value = g_key_file_get_value (priv->colors, "Colors", key, NULL))) {
			continue;
		}

		gcolor3_window_add_color_to_list (window, key, value, FALSE);
	}

	g_strfreev (keys);
}

