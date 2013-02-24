/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * gcolor3
 * Copyright (C) 2013 Jente <jthidskes@outlook.com>
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
#include <glib/gi18n.h>

static void
dialog_close (GtkWidget *about_dialog)
{
	gtk_widget_destroy (GTK_WIDGET (about_dialog));
}

static void
dialog_open ()
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

	g_signal_connect (GTK_DIALOG (about_dialog), "response", G_CALLBACK (dialog_close), NULL);

	gtk_widget_show (about_dialog);
}

static GtkWidget*
create_window (void)
{
	GtkWidget *window, *box_all, *color_chooser, *separator, *box_buttons, *button_quit, *button_about;

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title (GTK_WINDOW (window), "Gcolor3");
	gtk_window_set_default_icon_name ("gcolor2");
	g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

	box_all = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
	gtk_container_set_border_width (GTK_CONTAINER (box_all), 5);
	box_buttons = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 5);
	gtk_container_set_border_width (GTK_CONTAINER (box_buttons), 5);

	color_chooser = gtk_color_selection_new();
	separator = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
	button_quit = gtk_button_new_from_stock ("gtk-quit");
	button_about = gtk_button_new_from_stock ("gtk-about");

	gtk_box_pack_start (GTK_BOX (box_all), color_chooser, TRUE, TRUE, 0);
	gtk_box_pack_start (GTK_BOX (box_all), separator, TRUE, TRUE, 0);
	gtk_box_pack_end (GTK_BOX (box_buttons), button_quit, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (box_buttons), button_about, FALSE, FALSE, 0);

	g_signal_connect (button_quit, "clicked", G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect (button_about, "clicked", G_CALLBACK (dialog_open), NULL);

	gtk_container_add (GTK_CONTAINER (box_all), box_buttons);
	gtk_container_add (GTK_CONTAINER (window), box_all);

	return window;
}

int
main (int argc, char *argv[])
{
 	GtkWidget *window;

	gtk_init (&argc, &argv);
	window = create_window ();
	gtk_widget_show_all (window);
	gtk_main ();
	return 0;
}
