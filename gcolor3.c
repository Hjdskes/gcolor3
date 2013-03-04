/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * gcolor3
 * Copyright (C) 2013 Jente (jthidskes at outlook dot com)
 *
 * gcolor3 is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gcolor3 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gi18n.h>
#include <glib/gstdio.h>
#include <gdk/gdk.h>

GtkWidget *window, *color_chooser, *save_dialog, *tree, *button_save, *save_label, *button_delete, *save_dialog_ok_button, *save_entry;
GtkTreeIter selection_iter, iter;
GtkListStore *liststore;
GdkColor colorvalue;
gchar *colorname, *user_filename;

enum
{
	COLOR,
	COLOR_VALUE,
	COLOR_NAME,
	N_COLUMNS
};

void on_save_entry_changed (void);
void on_list_selection_changed (GtkTreeSelection *selection);
void on_colorselection_color_changed (void);
void on_save_button_clicked (void);
void on_delete_button_clicked (void);

gboolean save_selected_color (void);
gboolean delete_color (gchar* color_name, gchar* color_value);
void add_rgb_file (void);

void on_colorselection_color_changed (void);
void on_save_entry_changed (void);
void on_list_selection_changed (GtkTreeSelection *selection);
void on_save_button_clicked (void);
void on_delete_button_clicked (void);

gchar* hex_value (GdkColor colorvalue);
void add_color_to_treeview ();
void add_list_color (gchar *spec, gchar *colorname, gboolean is_new_color);

/*#####################################################################################################################################
  #####################################################################################################################################
  #####################################################################################################################################*/

void
show_file_error (gchar* type)
{
	GtkDialog *error_dialog;

	error_dialog = GTK_DIALOG (gtk_message_dialog_new (GTK_WINDOW (window), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR,
													   GTK_BUTTONS_OK, _("An error occurred trying to open file \"%s\" for %s access!\n\n"
																		 "Please check the file permissions and try again."), user_filename, type));
	gtk_dialog_run (error_dialog);
	gtk_widget_destroy (GTK_WIDGET (error_dialog));
}

static void
about_dialog_close (GtkWidget *about_dialog)
{
	gtk_widget_destroy (GTK_WIDGET (about_dialog));
}

static void
about_dialog_open (void)
{
	GtkWidget *about_dialog;
	gchar *license_trans;

	const gchar *authors[] = {"Unia (Jente)", NULL};
	const gchar *license[] = {
		N_("Gcolor3 is free software: you can redistribute it and/or modify "
		   "it under the terms of the GNU General Public License as published by "
		   "the Free Software Foundation, either version 3 of the License, or "
		   "(at your option) any later version."),
		N_("Gcolor3 is distributed in the hope that it will be useful "
		   "but WITHOUT ANY WARRANTY; without even the implied warranty of "
		   "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
		   "GNU General Public License for more details."),
		N_("You should have received a copy of the GNU General Public License "
		   "along with this program. If not, see <http://www.gnu.org/licenses/>.")
	};
	license_trans = g_strjoin ("\n\n", _(license[0]), _(license[1]), _(license[2]), NULL);

	about_dialog = gtk_about_dialog_new ();
	gtk_about_dialog_set_program_name (GTK_ABOUT_DIALOG (about_dialog), "Gcolor3");
	gtk_about_dialog_set_comments (GTK_ABOUT_DIALOG (about_dialog), "A simple color selection dialog in GTK3"),
	gtk_about_dialog_set_copyright (GTK_ABOUT_DIALOG (about_dialog), "Copyright \xc2\xa9 2013 Unia (Jente)");
	gtk_about_dialog_set_license (GTK_ABOUT_DIALOG (about_dialog), license_trans);
	gtk_about_dialog_set_wrap_license (GTK_ABOUT_DIALOG (about_dialog), TRUE);
	gtk_about_dialog_set_authors (GTK_ABOUT_DIALOG (about_dialog), authors);
	gtk_about_dialog_set_website_label (GTK_ABOUT_DIALOG (about_dialog), "GitHub repository");
	gtk_about_dialog_set_website (GTK_ABOUT_DIALOG (about_dialog), "https://github.com/Unia/gcolor3");
	gtk_about_dialog_set_logo_icon_name (GTK_ABOUT_DIALOG (about_dialog), "gcolor2");

	g_signal_connect (GTK_DIALOG (about_dialog), "response", G_CALLBACK (about_dialog_close), NULL);

	gtk_widget_show (about_dialog);
}

static GtkWidget*
save_dialog_open (void)
{
	GtkWidget *content_area, *save_dialog_cancel_button;

	save_dialog = gtk_dialog_new ();
	gtk_container_set_border_width (GTK_CONTAINER (save_dialog), 5);
	gtk_window_set_resizable (GTK_WINDOW (save_dialog), FALSE);
	gtk_window_set_title (GTK_WINDOW (save_dialog), _("Save a Color"));
	gtk_window_set_position (GTK_WINDOW (save_dialog), GTK_WIN_POS_CENTER_ON_PARENT);
	gtk_window_set_modal (GTK_WINDOW (save_dialog), TRUE);
	gtk_window_set_destroy_with_parent (GTK_WINDOW (save_dialog), TRUE);
	gtk_window_set_type_hint (GTK_WINDOW (save_dialog), GDK_WINDOW_TYPE_HINT_DIALOG);

	content_area = gtk_dialog_get_content_area (GTK_DIALOG (save_dialog));

	save_label = gtk_label_new ("");
	gtk_widget_show (save_label);
	gtk_label_set_justify (GTK_LABEL (save_label), GTK_JUSTIFY_CENTER);
	gtk_label_set_line_wrap (GTK_LABEL (save_label), TRUE);

	save_entry = gtk_entry_new ();
	gtk_widget_show (save_entry);
	gtk_entry_set_activates_default (GTK_ENTRY (save_entry), TRUE);

	save_dialog_ok_button = gtk_button_new_from_stock ("gtk-save");
	gtk_widget_show (save_dialog_ok_button);
	gtk_dialog_add_action_widget (GTK_DIALOG (save_dialog), save_dialog_ok_button, GTK_RESPONSE_OK);
	gtk_widget_set_sensitive (save_dialog_ok_button, FALSE);
	gtk_widget_set_can_default(save_dialog_ok_button, TRUE);

	save_dialog_cancel_button = gtk_button_new_from_stock ("gtk-cancel");
	gtk_widget_show (save_dialog_cancel_button);
	gtk_dialog_add_action_widget (GTK_DIALOG (save_dialog), save_dialog_cancel_button, GTK_RESPONSE_CANCEL);
	gtk_widget_set_sensitive (save_dialog_cancel_button, TRUE);
	gtk_widget_set_can_default(save_dialog_cancel_button, TRUE);

	g_signal_connect ((gpointer) save_entry, "changed", G_CALLBACK (on_save_entry_changed), NULL);

	gtk_container_add (GTK_CONTAINER (content_area), save_label);
	gtk_container_add (GTK_CONTAINER (content_area), save_entry);

	gtk_widget_grab_focus (save_entry);

	return save_dialog;
}

static GtkWidget*
create_window (void)
{
	GtkWidget *window, *box_all, *expander, *expander_box_all, *expander_box_buttons, *separator, *box_buttons, *button_quit, *button_about;
	GtkTreeViewColumn *column;
	GtkTreeSelection *selection;
	GtkCellRenderer *renderer;

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "Gcolor3");
	gtk_window_set_default_icon_name ("gcolor2");
	gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	box_all = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	gtk_container_set_border_width (GTK_CONTAINER (box_all), 5);
	box_buttons = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_container_set_border_width (GTK_CONTAINER (box_buttons), 5);

	color_chooser = gtk_color_selection_new();
	expander = gtk_expander_new("Show saved colors");
	expander_box_all = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	expander_box_buttons = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 5);
	button_save = gtk_button_new_from_stock ("gtk-save");
	button_delete = gtk_button_new_from_stock ("gtk-delete");
	gtk_widget_set_sensitive (button_delete, FALSE);

	/* setup the tree view widget */
	liststore = gtk_list_store_new (N_COLUMNS, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
	tree = gtk_tree_view_new();
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (tree), TRUE);
	gtk_tree_view_set_model (GTK_TREE_VIEW (tree), GTK_TREE_MODEL (liststore));

	column = gtk_tree_view_column_new ();
	gtk_tree_view_column_set_title (column, "Color");

	renderer = gtk_cell_renderer_pixbuf_new ();
	gtk_tree_view_column_pack_start (column, renderer, FALSE);
	gtk_tree_view_column_set_attributes (column, renderer, "pixbuf", COLOR, NULL);

	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column, renderer, TRUE);
	gtk_tree_view_column_set_attributes (column, renderer, "text", COLOR_VALUE, NULL);

	gtk_tree_view_column_set_sort_column_id (column, COLOR_VALUE);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);

	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes ("Name", renderer, "text", COLOR_NAME, NULL);
	gtk_tree_view_column_set_sort_column_id (column, COLOR_NAME);
	gtk_tree_view_append_column (GTK_TREE_VIEW (tree), column);

	/* enable searching on Name column */
	gtk_tree_view_set_search_column (GTK_TREE_VIEW (tree), COLOR_NAME);

	/* setup the selection handler */
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (tree));
	gtk_tree_selection_set_mode (selection, GTK_SELECTION_SINGLE);

	separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
	button_quit = gtk_button_new_from_stock ("gtk-quit");
	button_about = gtk_button_new_from_stock ("gtk-about");

	gtk_box_pack_start (GTK_BOX (box_all), color_chooser, TRUE, TRUE, 0);
	gtk_box_pack_end (GTK_BOX (expander_box_buttons), button_save, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (expander_box_buttons), button_delete, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (expander_box_all), expander_box_buttons, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (expander_box_all), tree, TRUE, TRUE, 0);
	gtk_container_add (GTK_CONTAINER (expander), expander_box_all);
	gtk_box_pack_start (GTK_BOX (box_all), expander, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (box_all), separator, TRUE, TRUE, 0);
	gtk_box_pack_end (GTK_BOX (box_buttons), button_quit, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (box_buttons), button_about, FALSE, FALSE, 0);

	g_signal_connect (G_OBJECT (selection), "changed", G_CALLBACK (on_list_selection_changed), NULL);
	g_signal_connect ((gpointer) color_chooser, "color_changed", G_CALLBACK (on_colorselection_color_changed), NULL);
	g_signal_connect (button_save, "clicked", G_CALLBACK (on_save_button_clicked), NULL);
	g_signal_connect (button_delete, "clicked", G_CALLBACK (on_delete_button_clicked), NULL);
	g_signal_connect (button_quit, "clicked", G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect (button_about, "clicked", G_CALLBACK (about_dialog_open), NULL);

	gtk_container_add (GTK_CONTAINER (box_all), box_buttons);
	gtk_container_add (GTK_CONTAINER (window), box_all);

	return window;
}

/*#####################################################################################################################################
  #####################################################################################################################################
  #####################################################################################################################################*/

gboolean
save_selected_color (void)
{
	FILE     *fp;
	gchar     old[512] = "";

	/* get entry text */
	colorname = g_strdup (gtk_entry_get_text (GTK_ENTRY (save_entry)));
	if (!strcmp (colorname, ""))
		return FALSE;

	/* if file exists already, get its contents, otherwise just write to it */
	if (g_file_test (user_filename, G_FILE_TEST_EXISTS)) {
		fp = fopen (user_filename, "r");
		if (fp) {
			fread (old, sizeof (old), 1, fp);
			fclose (fp);
		} else {
			show_file_error ("read");
			return FALSE;
		}
	}

	fp = fopen (user_filename, "w");
	if (!fp) {
		show_file_error ("write");
		return FALSE;
	}

	fprintf (fp, "%3d %3d %3d\t\t%s\n%s", colorvalue.red/256, colorvalue.green/256, colorvalue.blue/256, colorname, old);
	fclose (fp);
	return TRUE;
}

gboolean
delete_color (gchar *color_name, gchar *color_value)
{
	FILE     *fp;
	gchar    *p;
	gchar    *file_color_name;
	gchar     file_color_value[8];
	gint      r, g, b;
	gchar     buffer[512] = "";
	gchar     newstuff[512] = "";
	gboolean  found = FALSE;

	/* remove from file */
	fp = fopen (user_filename, "r");
	if (!fp) {
		show_file_error ("read");
		return FALSE;
	}

	while ((p = fgets (buffer, sizeof buffer, fp)) != NULL) {
		if (buffer[0] == '!')
			continue;
		r = g_ascii_strtoull (p, &p, 10);
		g = g_ascii_strtoull (p, &p, 10);
		b = g_ascii_strtoull (p, &p, 10);
		p += strspn (p, " \t");
		g_sprintf (file_color_value, "#%.2X%.2X%.2X", r, g, b);
		file_color_name = g_strchomp (g_strdup (p));

		/* make sure to only remove the first matching color; both value and name must match */
		if (found || strcmp (file_color_name, color_name) != 0 || strcmp (g_ascii_strup (file_color_value, -1), color_value) != 0) {
			g_sprintf (newstuff+strlen(newstuff), "%3d %3d %3d\t\t%s\n", r, g, b, file_color_name);
		} else {
			found = TRUE;
		}
	}
	fclose (fp);

	/* only rewrite the file if we found a match */
	if (found) {
		fp = fopen (user_filename, "w");
		if (!fp) {
			show_file_error ("write");
			return FALSE;
		}

		fprintf (fp, "%s", newstuff);
		fclose (fp);
		return TRUE;
	}
	return FALSE;
}

void
add_rgb_file (void)
{
	FILE  *fp;
	gchar *p;
	gchar  buffer[512];
	gchar  spec[8];
	gint   r, g, b;

	fp = fopen (user_filename, "r");
	if (!fp)
		return; /* silently fail */

	while ((p = fgets (buffer, sizeof buffer, fp)) != NULL) {
		if (buffer[0] == '!')
			continue;
		r = g_ascii_strtoull (p, &p, 10);
		g = g_ascii_strtoull (p, &p, 10);
		b = g_ascii_strtoull (p, &p, 10);
		p += strspn (p, " \t");
		g_sprintf (spec, "#%.2X%.2X%.2X", r, g, b);
		colorname = g_strchomp (g_strdup (p));
		add_list_color (spec, colorname, FALSE);
	}
	fclose (fp);
}

/*#####################################################################################################################################
  #####################################################################################################################################
  #####################################################################################################################################*/

void
on_colorselection_color_changed (void)
{
	gtk_color_selection_get_current_color (GTK_COLOR_SELECTION (color_chooser), &colorvalue);
}

void
on_save_entry_changed (void)
{
	gchar *text;

	text = g_strdup (gtk_entry_get_text (GTK_ENTRY (save_entry)));
	if (strlen (text) > 0) {
		gtk_widget_set_sensitive (save_dialog_ok_button, TRUE);
	} else {
		gtk_widget_set_sensitive (save_dialog_ok_button, FALSE);
	}
}

void
on_list_selection_changed (GtkTreeSelection *selection)
{
	GdkColor     new_color, curr_color;
	GtkTreeModel *model;
	gchar        *color;

	if (gtk_tree_selection_get_selected (selection, &model, &selection_iter)) {
		gtk_tree_model_get (model, &selection_iter, COLOR_VALUE, &color, -1);
		gdk_color_parse (color, &new_color);

		/* save the old color in color wheel */
		gtk_color_selection_get_current_color (GTK_COLOR_SELECTION (color_chooser), &curr_color);
		gtk_color_selection_set_previous_color (GTK_COLOR_SELECTION (color_chooser), &curr_color);
		gtk_color_selection_set_current_color (GTK_COLOR_SELECTION (color_chooser), &new_color);
		g_free (color);

		gtk_widget_set_sensitive (button_delete, TRUE);
	} else {
		gtk_widget_set_sensitive (button_delete, FALSE);
	}
}

void
on_save_button_clicked (void)
{
	gchar *labeltext;
	gint result;

	save_dialog = save_dialog_open ();
	gtk_window_set_transient_for (GTK_WINDOW (save_dialog), GTK_WINDOW (window));

	labeltext = g_strdup_printf (_("Enter a color name for %s:"), hex_value (colorvalue));
	gtk_label_set_markup (GTK_LABEL (save_label), labeltext);
	result = gtk_dialog_run (GTK_DIALOG (save_dialog));
	switch (result) {
		case GTK_RESPONSE_OK:
			if (save_selected_color ())
				add_color_to_treeview();
			break;
		case GTK_RESPONSE_CANCEL:
		default:
			colorname = "";
			break;
	}
	gtk_widget_destroy (save_dialog);
}

void
on_delete_button_clicked (void)
{
	gchar *color_value;

	gtk_tree_model_get (gtk_tree_view_get_model (GTK_TREE_VIEW (tree)), &selection_iter, COLOR_NAME, &colorname, COLOR_VALUE, &color_value, -1);
	if (delete_color (colorname, color_value))
		gtk_list_store_remove (GTK_LIST_STORE (gtk_tree_view_get_model (GTK_TREE_VIEW (tree))), &selection_iter);
}

/*#####################################################################################################################################
  #####################################################################################################################################
  #####################################################################################################################################*/

gchar*
hex_value (GdkColor colorvalue)
{
	return g_strdup_printf ("#%.2X%.2X%.2X", colorvalue.red/256, colorvalue.green/256, colorvalue.blue/256);
}

void
add_color_to_treeview ()
{
	GtkTreeSelection *selection;
	GtkTreePath *path;

	/* add color to tree view */
	add_list_color (hex_value (colorvalue), colorname, TRUE);

	/* scroll tree view so user sees new color */
	selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (tree));
	if (selection) {
		if (gtk_tree_selection_get_selected (selection, NULL, &iter) ) {
			path = gtk_tree_model_get_path (gtk_tree_view_get_model (GTK_TREE_VIEW(tree)), &iter);
			gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW(tree), path, NULL, FALSE, 0, 0);
			gtk_tree_path_free (path);
		}
	}
}

void
add_list_color (gchar *spec, gchar *colorname, gboolean is_new_color)
{
	GdkPixbuf   *buf;

	gchar * swatchxpm[] =
	{
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
	gchar colorline[] = ".      c #FFFFFF";

	g_sprintf (colorline, ".      c %s", spec);
	swatchxpm[1] = colorline;
	buf = gdk_pixbuf_new_from_xpm_data ((gchar const **)swatchxpm);

	/* setting where to insert will only take effect if user didn't sort either of the columns yet */
	if (is_new_color)
		gtk_list_store_prepend (liststore, &iter);
	else
		gtk_list_store_append (liststore, &iter);
	gtk_list_store_set (liststore, &iter, COLOR, buf, COLOR_VALUE, spec, COLOR_NAME, colorname, -1);
	if (is_new_color) {
		/* when adding a new color, select it in the treeview */
		gtk_tree_selection_select_iter (gtk_tree_view_get_selection (GTK_TREE_VIEW (tree)), &iter);
	}
}

int
main (int argc, char *argv[])
{
	gtk_init (&argc, &argv);
	colorvalue.red = colorvalue.green = colorvalue.blue = 65535;
	window = create_window ();
	gtk_widget_show_all (window);
	user_filename = g_build_filename (g_get_home_dir(), ".rgb.txt", NULL);
	add_rgb_file ();
	gtk_main ();
	return 0;
}
