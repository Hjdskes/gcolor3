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

#include "config.h"

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <glib/gi18n.h>

#include "gcolor3-color-item.h"
#include "gcolor3-color-row.h"
#include "gcolor3-color-selection.h"
#include "gcolor3-color-store.h"
#include "gcolor3-window.h"
#include "utils.h"

enum {
	PROP_0,
	PROP_STORE,
};

struct _Gcolor3WindowPrivate {
	GtkWidget *button_save;
	GtkWidget *entry;
	GtkWidget *page_stack;
	GtkWidget *picker;
	GtkWidget *list_stack;
	GtkWidget *scroll;
	GtkWidget *listbox;
	GtkWidget *empty_placeholder;

	Gcolor3ColorStore *store;

	GdkRGBA current;
};

G_DEFINE_TYPE_WITH_PRIVATE (Gcolor3Window, gcolor3_window, GTK_TYPE_APPLICATION_WINDOW)

static void
save_color (Gcolor3WindowPrivate *priv)
{
	const gchar *key;
	gchar *hex;

	key = gtk_entry_get_text (GTK_ENTRY (priv->entry));
	hex = hex_value (&priv->current);
	if (strlen (key) == 0) {
		/* If using `hex` as key, do not save the first character (e.g., `#`)
		 * because GKeyFile will see these as (and subsequently ignore) comments. */
		key = hex + 1;
	}

	/* This will add the color to the store (if the key isn't
	 * already used), which in turn will update the list box due
	 * to the binding between the store's list model and the list
	 * box. */
	gcolor3_color_store_add_color (priv->store, key, hex);
	g_free (hex);

	if (!gcolor3_color_store_empty (priv->store)) {
		gtk_stack_set_visible_child (GTK_STACK (priv->list_stack),
					     priv->scroll);
	}
}

static gboolean
gcolor3_window_picker_page_key_handler (GtkWidget   *widget,
					GdkEventKey *event,
					gpointer     user_data)
{
	Gcolor3WindowPrivate *priv;

	if (event->type != GDK_KEY_PRESS) {
		return GDK_EVENT_PROPAGATE;
	}

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));

	switch (event->keyval) {
	case GDK_KEY_s:
		if ((event->state & GDK_CONTROL_MASK) != GDK_CONTROL_MASK) {
			return GDK_EVENT_PROPAGATE;
		}
		/* Emulate a button click, to give the user visual feedback of
		   the save action. */
		g_signal_emit_by_name (priv->button_save, "activate");
		return GDK_EVENT_STOP;
	default:
		return GDK_EVENT_PROPAGATE;
	}
}

static void
gcolor3_window_entry_activated (UNUSED GtkEntry *entry, gpointer user_data)
{
	Gcolor3WindowPrivate *priv;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));
	/* Emulate a button click, to give the user visual feedback of
	   the save action. */
	g_signal_emit_by_name (priv->button_save, "activate");
}

static void
gcolor3_window_save_button_clicked (UNUSED GtkButton *button, gpointer user_data)
{
	Gcolor3WindowPrivate *priv;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));
	save_color (priv);
}

static void
gcolor3_window_color_row_deleted (Gcolor3ColorRow *row, gpointer user_data)
{
	Gcolor3WindowPrivate *priv;
	gchar *key;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));

	g_object_get (row, "key", &key, NULL);
	/* Removing a color from the store will automatically remove
	   it from the list box, due to the binding between the two. */
	gcolor3_color_store_remove_color (priv->store, key);
	g_free (key);

	if (gcolor3_color_store_empty (priv->store)) {
		gtk_stack_set_visible_child (GTK_STACK (priv->list_stack),
					     priv->empty_placeholder);
	}
}

static void
gcolor3_window_color_row_renamed (UNUSED Gcolor3ColorRow *row,
				  const gchar *old_name,
				  const gchar *new_name,
				  gpointer user_data)
{
	Gcolor3WindowPrivate *priv;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));
	gcolor3_color_store_rename_color (priv->store, old_name, new_name);
}

static void
gcolor3_window_action_change_page (UNUSED GSimpleAction *action,
				   UNUSED GVariant      *parameter,
				   gpointer              user_data)
{
	Gcolor3WindowPrivate *priv;
	const gchar *page;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));

	page = gtk_stack_get_visible_child_name (GTK_STACK (priv->page_stack));
	if (g_strcmp0 (page, "saved-colors") == 0) {
		gtk_stack_set_visible_child_name (GTK_STACK (priv->page_stack), "picker");
	} else {
		gtk_stack_set_visible_child_name (GTK_STACK (priv->page_stack), "saved-colors");
	}
}

static const GActionEntry window_actions[] = {
	{ "change-page", gcolor3_window_action_change_page, NULL, NULL, NULL },
};

static void
gcolor3_window_picker_changed (Gcolor3ColorSelection *picker, gpointer user_data)
{
	Gcolor3WindowPrivate *priv;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));

	gcolor3_color_selection_get_current_color (GCOLOR3_COLOR_SELECTION (picker), &priv->current);
}

static void
gcolor3_window_stack_changed (GtkStack          *stack,
			      UNUSED GParamSpec *pspec,
			      gpointer           user_data)
{
	Gcolor3WindowPrivate *priv;
	const gchar *page;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));

	page = gtk_stack_get_visible_child_name (stack);
	if (g_strcmp0 (page, "saved-colors") == 0) {
		gtk_widget_hide (priv->button_save);
		gtk_widget_hide (priv->entry);
	} else {
		gtk_widget_show (priv->button_save);
		gtk_widget_show (priv->entry);
	}
}

static void
gcolor3_window_selection_changed (GtkListBox *listbox, gpointer user_data)
{
	Gcolor3WindowPrivate *priv;
	GtkListBoxRow *row;
	GdkRGBA new, current;
	gchar *color;

	priv = gcolor3_window_get_instance_private (GCOLOR3_WINDOW (user_data));
	row = gtk_list_box_get_selected_row (listbox);

	if (!row) {
		/* This callback also triggers when a row is deleted,
		   in which the selected row returns NULL.  In this
		   case, we don't want to select another color,
		   anyway. */
		return;
	}

	g_object_get (row, "hex", &color, NULL);
	gdk_rgba_parse (&new, color);
	g_free (color);

	/* Save the old color in the picker. */
	gcolor3_color_selection_get_current_color (GCOLOR3_COLOR_SELECTION (priv->picker), &current);
	gcolor3_color_selection_set_previous_color (GCOLOR3_COLOR_SELECTION (priv->picker), &current);
	gcolor3_color_selection_set_current_color (GCOLOR3_COLOR_SELECTION (priv->picker), &new);
}

static GtkWidget *
create_widget_func (gpointer item, gpointer user_data)
{
	Gcolor3ColorRow *row;
	gchar *key, *hex;

	g_object_get ((Gcolor3ColorItem *) item, "key", &key, "hex", &hex, NULL);
	row = gcolor3_color_row_new (key, hex);
	g_signal_connect (row, "color-removed",
			  G_CALLBACK (gcolor3_window_color_row_deleted), user_data);
	g_signal_connect (row, "color-renamed",
			  G_CALLBACK (gcolor3_window_color_row_renamed), user_data);
	g_free (key);
	g_free (hex);
	return GTK_WIDGET (row);
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
			gtk_list_box_bind_model (GTK_LIST_BOX (priv->listbox),
						 G_LIST_MODEL (priv->store),
						 create_widget_func,
						 object,
						 NULL);
			if (gcolor3_color_store_empty (priv->store)) {
				gtk_stack_set_visible_child (GTK_STACK (priv->list_stack),
							     priv->empty_placeholder);
			} else {
				gtk_stack_set_visible_child (GTK_STACK (priv->list_stack),
							     priv->scroll);
			}
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

	gtk_widget_class_set_template_from_resource (widget_class, "/nl/hjdskes/gcolor3/window.ui");

	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, button_save);
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, entry);
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, page_stack);
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, list_stack);
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, scroll);
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, listbox);
	gtk_widget_class_bind_template_child_private (widget_class, Gcolor3Window, empty_placeholder);

	gtk_widget_class_bind_template_callback (widget_class, gcolor3_window_stack_changed);
	gtk_widget_class_bind_template_callback (widget_class, gcolor3_window_picker_changed);
	gtk_widget_class_bind_template_callback (widget_class, gcolor3_window_selection_changed);
	gtk_widget_class_bind_template_callback (widget_class, gcolor3_window_picker_page_key_handler);
	gtk_widget_class_bind_template_callback (widget_class, gcolor3_window_entry_activated);
	gtk_widget_class_bind_template_callback (widget_class, gcolor3_window_save_button_clicked);
}

static void
gcolor3_window_init (Gcolor3Window *window)
{
	Gcolor3WindowPrivate *priv;

	priv = gcolor3_window_get_instance_private (window);

	gtk_widget_init_template (GTK_WIDGET (window));

	/* Add the custom color selection widget. */
	priv->picker = gcolor3_color_selection_new ();
	gtk_widget_set_valign (priv->picker, GTK_ALIGN_CENTER);
	gtk_widget_set_halign (priv->picker, GTK_ALIGN_CENTER);
	g_signal_connect (priv->picker, "color-changed",
			  G_CALLBACK (gcolor3_window_picker_changed), window);
	g_signal_connect (priv->picker, "key-press-event",
			  G_CALLBACK (gcolor3_window_picker_page_key_handler), window);
	gtk_stack_add_titled (GTK_STACK (priv->page_stack), priv->picker, "picker", _("Picker"));
	gtk_container_child_set (GTK_CONTAINER (priv->page_stack), priv->picker, "position", 0, NULL);
	gtk_widget_set_visible (priv->picker, TRUE);

	/* Call the callback to initialise the GtkEntry and to prevent
	 * saving #000000 when saving the white color right away. */
	gcolor3_window_picker_changed (GCOLOR3_COLOR_SELECTION (priv->picker), window);

	g_action_map_add_action_entries (G_ACTION_MAP (window),
					 window_actions,
					 G_N_ELEMENTS (window_actions),
					 window);

	/* Finally, make the color picker the visible stack page. */
	gtk_stack_set_visible_child_name (GTK_STACK (priv->page_stack), "picker");
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

void
gcolor3_window_destroy (Gcolor3Window *window, UNUSED gpointer user_data)
{
	g_return_if_fail (window != NULL);
	gtk_widget_destroy (GTK_WIDGET (window));
}
