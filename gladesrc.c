


/*  Note: You are free to use whatever license you want.
   Eventually you will be able to edit it within Glade. */

/*  glade
 *  Copyright (C) <YEAR> <AUTHORS>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "gladesig.h"
#include "gladesrc.h"

GtkWidget *get_widget(GtkWidget * widget, gchar * widget_name)
{
    GtkWidget *parent, *found_widget;

    for (;;) {
	if (GTK_IS_MENU(widget))
	    parent = gtk_menu_get_attach_widget(GTK_MENU(widget));
	else
	    parent = widget->parent;
	if (parent == NULL)
	    break;
	widget = parent;
    }

    found_widget = (GtkWidget *) gtk_object_get_data(GTK_OBJECT(widget),
						     widget_name);
    if (!found_widget)
	g_warning("Widget not found: %s", widget_name);
    return found_widget;
}

/* This is an internally used function to set notebook tab widgets. */
/*
void
set_notebook_tab(GtkWidget * notebook, gint page_num, GtkWidget * widget)
{
    GtkNotebookPage *page;
    GtkWidget *notebook_page;

    page = (GtkNotebookPage *)
		g_list_nth(GTK_NOTEBOOK(notebook)->children, page_num)->data;
    notebook_page = page->child;
    gtk_widget_ref(notebook_page);
    gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), page_num);
    gtk_notebook_insert_page(GTK_NOTEBOOK(notebook), notebook_page,
		widget, page_num);
    gtk_widget_unref(notebook_page);
}
*/

static GList *pixmaps_directories = NULL;

/* Use this function to set the directory containing installed pixmaps. */
void add_pixmap_directory(gchar * directory)
{
    pixmaps_directories =
	g_list_prepend(pixmaps_directories, g_strdup(directory));
}

/* This is an internally used function to check if a pixmap file exists. */
#ifndef G_DIR_SEPARATOR_S
#define G_DIR_SEPARATOR_S "/"
#endif
gchar *check_file_exists(gchar * directory, gchar * filename)
{
    gchar *full_filename;
    struct stat s;
    gint status;

    full_filename = g_malloc(strlen(directory) + 1 + strlen(filename) + 1);
    strcpy(full_filename, directory);
    strcat(full_filename, G_DIR_SEPARATOR_S);
    strcat(full_filename, filename);

    status = stat(full_filename, &s);
    if (status == 0 && S_ISREG(s.st_mode))
	return full_filename;
    g_free(full_filename);
    return NULL;
}

/* This is an internally used function to create pixmaps. */
GtkWidget *create_pixmap(GtkWidget * widget, gchar * filename)
{
    gchar *found_filename = NULL;
    GdkColormap *colormap;
    GdkPixmap *gdkpixmap;
    GdkBitmap *mask;
    GtkWidget *pixmap;
    GList *elem;

    /* We first try any pixmaps directories set by the application. */
    elem = pixmaps_directories;
    while (elem) {
	found_filename = check_file_exists((gchar *) elem->data, filename);
	if (found_filename)
	    break;
	elem = elem->next;
    }

    /* If we haven't found the pixmap, try the source directory. */
    if (!found_filename) {
	found_filename = check_file_exists("pixmaps", filename);
    }
    if (!found_filename) {
	g_print("Couldn't find pixmap file: %s", filename);
	return NULL;
    }
    colormap = gtk_widget_get_colormap(widget);
    gdkpixmap = gdk_pixmap_colormap_create_from_xpm(NULL, colormap, &mask,
						    NULL, found_filename);
    g_free(found_filename);
    if (gdkpixmap == NULL)
	return NULL;
    pixmap = gtk_pixmap_new(gdkpixmap, mask);
    gdk_pixmap_unref(gdkpixmap);
    gdk_bitmap_unref(mask);
    return pixmap;
}

GtkWidget *create_window()
{
    GtkWidget *window;
    GtkWidget *vbox1;
    GtkWidget *label;
    GtkWidget *label1;
    GtkWidget *label2;
    GtkWidget *hbox1;
    GtkWidget *hbox2;
    GtkWidget *centry;
    GtkWidget *ins_button;
    GtkWidget *clear_button;
    GtkWidget *cancel_button;
    char q[200], templen[5];
    char *tempstr, **temptok;
    int x, y, z, t, maxlen, num_rows;
    GList *toklist = NULL;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_object_set_data(GTK_OBJECT(window), "window", window);
    gtk_signal_connect(GTK_OBJECT(window), "destroy",
		       GTK_SIGNAL_FUNC(gtk_main_quit), NULL);
    gtk_window_set_title(GTK_WINDOW(window), "Gentry");
    gtk_window_set_policy(GTK_WINDOW(window), TRUE, TRUE, FALSE);

    vbox1 = gtk_vbox_new(FALSE, 0);
    gtk_object_set_data(GTK_OBJECT(window), "vbox1", vbox1);
    gtk_widget_show(vbox1);
    gtk_container_add(GTK_CONTAINER(window), vbox1);

    label1 = gtk_label_new("Gentry v0.1");
    gtk_object_set_data(GTK_OBJECT(window), "label1", label1);
    gtk_widget_show(label1);
    gtk_box_pack_start(GTK_BOX(vbox1), label1, TRUE, TRUE, 0);

    label2 = gtk_label_new(entry_table);
    gtk_object_set_data(GTK_OBJECT(window), "label2", label2);
    gtk_widget_show(label2);
    gtk_box_pack_start(GTK_BOX(vbox1), label2, TRUE, TRUE, 0);

    sprintf(q, "show columns from %s", entry_table);
    iret = mysql_query(&astfhost, q);
    if (iret) {
	printf("The table '%s' is not valid.\n", entry_table);
	exit(-1);
    }
    astfdata = mysql_store_result(&astfhost);

//    astfdata = mysql_list_fields(&astfhost, entry_table, NULL);
    y = mysql_num_rows(astfdata);
    table_cols = y;
    x = 0;
    for (x = 0; x < y; x++) {
//      printf("X = %d\n", x);

	for (z = 0; z < 5; z++)
	    templen[z] = '\0';

	astfrow = mysql_fetch_row(astfdata);

	if (!strncmp(astfrow[1], "int", 3))
	    table_type[x] = GENTRY_INT;
	if (!strncmp(astfrow[5], "auto_increment", 14)) {
	    table_type[x] = GENTRY_AUTO;
	    sprintf(auto_column, "%s", astfrow[0]);
	}
	if (!strncmp(astfrow[1], "double", 6))
	    table_type[x] = GENTRY_DOUBLE;
	if (!strncmp(astfrow[1], "float", 5))
	    table_type[x] = GENTRY_DOUBLE;
	if (!strncmp(astfrow[1], "decimal", 7))
	    table_type[x] = GENTRY_DOUBLE;
	if (!strncmp(astfrow[1], "char", 4)) {
	    table_type[x] = GENTRY_CHAR;
	    strncpy(templen, astfrow[1] + 5, (strlen(astfrow[1]) - 6));
	    table_lens[x] = atoi(templen);
//              g_print ("char: %d\n", table_lens[x]);
	}
	if (!strncmp(astfrow[1], "varchar", 7)) {
	    table_type[x] = GENTRY_VARCHAR;
	    strncpy(templen, astfrow[1] + 8, (strlen(astfrow[1]) - 9));
	    table_lens[x] = atoi(templen);
//              g_print ("varchar: %d\n", table_lens[x]);
	}
	if (!strncmp(astfrow[1], "date", 4)) {
	    table_type[x] = GENTRY_DATE;
	    table_lens[x] = 10;
//              g_print ("date: %d\n", table_lens[x]);
	}
	if (!strncmp(astfrow[1], "time", 4)) {
	    table_type[x] = GENTRY_TIME;
	    table_lens[x] = 15;
//              g_print ("time: %d\n", table_lens[x]);
	}
	if (!strncmp(astfrow[1], "timestamp", 8)) {
	    table_type[x] = GENTRY_TIMESTAMP;
	    table_lens[x] = 19;
//              g_print ("datetime: %d\n", table_lens[x]);
	}
	if (!strncmp(astfrow[1], "datetime", 8)) {
	    table_type[x] = GENTRY_DATETIME;
	    table_lens[x] = 19;
//              g_print ("datetime: %d\n", table_lens[x]);
	}
	if (!strncmp(astfrow[1], "year", 4)) {
	    table_type[x] = GENTRY_YEAR;
	    table_lens[x] = 4;
//              g_print ("time: %d\n", table_lens[x]);
	}
	if (!strncmp(astfrow[1], "enum", 4)) {
	    table_type[x] = GENTRY_ENUM;
	    /* fix this length here and insert code to "understand"
	     * the enum type. */
/*          g_print("row data: %s\n%d, %d\n", astfrow[1],
   strlen(astfrow[1]), strlen(astfrow[1]) - 6);
 */
	    tempstr = g_strndup(astfrow[1] + 5, (strlen(astfrow[1]) - 6));
//          g_print("enum: %s\n", tempstr);
	    temptok = g_strsplit(tempstr, ",", -1);
//          g_print("after strsplit call\n");
	    t = 0;
	    maxlen = strlen(temptok[t]) - 2;
	    toklist = NULL;
	    while (temptok[t]) {
		if (strlen(temptok[t]) > maxlen)
		    maxlen = strlen(temptok[t]) - 2;
		tempstr =
		    g_strndup(temptok[t] + 1, (strlen(temptok[t]) - 2));
//              g_print("pre token: %s\n", temptok[t++]);
		toklist = g_list_append(toklist, tempstr);
//              g_print("token: %s\n", tempstr);
		t++;
	    }
	    table_lens[x] = maxlen;
//              g_print ("enum: %d\n", table_lens[x]);
	}
	hbox2 = gtk_hbox_new(FALSE, 0);
	gtk_object_set_data(GTK_OBJECT(window), "hbox1", hbox2);
	gtk_widget_show(hbox2);
	gtk_box_pack_start(GTK_BOX(vbox1), hbox2, TRUE, TRUE, 0);
	label = gtk_label_new(astfrow[0]);
	gtk_box_pack_start(GTK_BOX(hbox2), label, TRUE, TRUE, 5);
	gtk_widget_show(label);

	switch (table_type[x]) {
	case GENTRY_CHAR:
	case GENTRY_VARCHAR:
	case GENTRY_TIME:
	case GENTRY_DATE:
	case GENTRY_DATETIME:
	case GENTRY_TIMESTAMP:
	case GENTRY_YEAR:
	    entry_data[x] = gtk_entry_new();
	    gtk_entry_set_max_length(GTK_ENTRY(entry_data[x]),
				     table_lens[x]);
	    gtk_entry_set_text(GTK_ENTRY(entry_data[x]), "");
	    break;
	case GENTRY_AUTO:
	    entry_data[x] = gtk_entry_new();

	    /*
	     * Check for number of rows in table
	     * This is to get around a segfault when getting the
	     * max value from an empty table.
	     */
	    sprintf(q, "select count(*) from %s", entry_table);
	    iret = mysql_query(&astfhost, q);
	    if (iret) {
		printf("The table '%s' is not valid.\n", entry_table);
		exit(-1);
	    }
	    maxiddata = mysql_store_result(&astfhost);
	    if (maxiddata == NULL) {
		printf("MySQL error: %s\n", mysql_error(&astfhost));
	    	exit(-1);
	    }
	    maxidrow = mysql_fetch_row(maxiddata);
	    num_rows = atoi(maxidrow[0]);
		printf ("Number of rows in %s = %d\n", entry_table, num_rows);
	    
	    if (num_rows == 0) {
		    auto_max = 1;
	    } else {

		    sprintf(q, "select max(%s) from %s", auto_column, entry_table);
		    iret = mysql_query(&astfhost, q);
		    if (iret) {
			printf("The table '%s' is not valid.\n", entry_table);
			exit(-1);
		    }
		    maxiddata = mysql_store_result(&astfhost);
		    if (maxiddata == NULL) {
			printf("MySQL error: %s\n", mysql_error(&astfhost));
			exit(-1);
		    }
			printf ("Number of rows: %lld\n", mysql_num_rows(maxiddata));
			printf ("Number of fields: %d\n", mysql_field_count(&astfhost));
		    maxidrow = mysql_fetch_row(maxiddata);
		    if (maxidrow == NULL) {
			    printf("MySQL error: %s\n", mysql_error(&astfhost));
			    exit(-1);
		    }
		    auto_max = atoi(maxidrow[0]) + 1;
	    }

        g_print("Auto column = %s, next value = %d\n", auto_column, auto_max);
	    sprintf(templen, "%d", auto_max);
        g_print("Auto column data = %s\n", templen);
	    mysql_free_result(maxiddata);
	    gtk_entry_set_text(GTK_ENTRY(entry_data[x]), templen);
	    gtk_entry_set_editable(GTK_ENTRY(entry_data[x]), FALSE);
	    break;
	case GENTRY_ENUM:
	    entry_data[x] = gtk_combo_new();
	    gtk_combo_set_popdown_strings(GTK_COMBO(entry_data[x]),
					  toklist);
	    centry = GTK_ENTRY(GTK_COMBO(entry_data[x])->entry);
	    gtk_entry_set_editable(centry, FALSE);
	    g_list_free(toklist);
	    break;
	default:
	    entry_data[x] = gtk_entry_new();
	    gtk_entry_set_text(GTK_ENTRY(entry_data[x]), "");
	    break;
	}
	gtk_box_pack_start(GTK_BOX(hbox2), entry_data[x], FALSE, TRUE, 0);
//  gtk_signal_connect(GTK_OBJECT(entry_data[x]), "activate",
	//                        GTK_SIGNAL_FUNC(entry_callback), entry_data[x]);
	gtk_widget_show(entry_data[x]);
    }
    mysql_free_result(astfdata);

    hbox1 = gtk_hbox_new(FALSE, 0);
    gtk_object_set_data(GTK_OBJECT(window), "hbox1", hbox1);
    gtk_widget_show(hbox1);
    gtk_box_pack_start(GTK_BOX(vbox1), hbox1, TRUE, TRUE, 0);

    ins_button = gtk_button_new_with_label("Insert");
    gtk_object_set_data(GTK_OBJECT(window), "ins_button", ins_button);
    gtk_widget_show(ins_button);
    gtk_box_pack_start(GTK_BOX(hbox1), ins_button, TRUE, TRUE, 0);
    gtk_signal_connect(GTK_OBJECT(ins_button), "clicked",
		       GTK_SIGNAL_FUNC(on_ins_button_clicked), NULL);

    clear_button = gtk_button_new_with_label("Clear");
    gtk_object_set_data(GTK_OBJECT(window), "clear_button", clear_button);
    gtk_widget_show(clear_button);
    gtk_box_pack_start(GTK_BOX(hbox1), clear_button, TRUE, TRUE, 0);
    gtk_signal_connect(GTK_OBJECT(clear_button), "clicked",
		       GTK_SIGNAL_FUNC(on_clear_button_clicked), NULL);

    cancel_button = gtk_button_new_with_label("Cancel");
    gtk_object_set_data(GTK_OBJECT(window), "cancel_button",
			cancel_button);
    gtk_widget_show(cancel_button);
    gtk_box_pack_start(GTK_BOX(hbox1), cancel_button, TRUE, TRUE, 0);
    gtk_signal_connect_object(GTK_OBJECT(cancel_button), "clicked",
			      GTK_SIGNAL_FUNC(gtk_widget_destroy),
			      GTK_OBJECT(window));

    status_bar = gtk_statusbar_new();
    gtk_box_pack_start(GTK_BOX(vbox1), status_bar, TRUE, TRUE, 0);
    gtk_widget_show(status_bar);

    /* test for focus switch */
    gtk_widget_grab_focus(GTK_ENTRY(entry_data[0]));

    return window;
}
