
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

#include <gtk/gtk.h>

#include <mysql/mysql.h>

/*
 * This function returns a widget in a component created by Glade.
 * Call it with the toplevel widget in the component (i.e. a window/dialog),
 * or alternatively any widget in the component, and the name of the widget
 * you want returned.
 */
GtkWidget *get_widget(GtkWidget * widget, gchar * widget_name);


 /*
  * This is an internally used function for setting notebook tabs. It is only
  * included in this header file so you don't get compilation warnings
  */
void set_notebook_tab(GtkWidget * notebook, gint page_num,
		      GtkWidget * widget);

/* Use this function to set the directory containing installed pixmaps. */
void add_pixmap_directory(gchar * directory);

/* This is an internally used function to create pixmaps. */
GtkWidget *create_pixmap(GtkWidget * widget, gchar * filename);

GtkWidget *create_window(void);

char entry_table[50];
char entry_db[50];

MYSQL astfhost;
MYSQL_RES *astfdata;
MYSQL_ROW astfrow;

MYSQL_RES *maxiddata;
MYSQL_ROW maxidrow;

int iret;

#define GENTRY_VERSION "gentry: 0.1.11"

GtkWidget *entry_data[50];
//gchar table_type[50][3];
int table_type[50];
int table_lens[50];
gchar *table_data[50];
int table_cols;
GtkWidget *status_bar;

#define GENTRY_INT 0
#define GENTRY_DOUBLE 1
#define GENTRY_CHAR 2
#define GENTRY_VARCHAR 3
#define GENTRY_DATE 4
#define GENTRY_ENUM 5
#define GENTRY_AUTO 6
#define GENTRY_TIME 7
#define GENTRY_DATETIME 8
#define GENTRY_TIMESTAMP 9
#define GENTRY_YEAR 10

gchar auto_column[50];
gint auto_max;
