
/*  Note: You are free to use whatever license you want.
   Eventually you will be able to edit it within Glade. */

/*  gentry
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>
#include "gladesrc.h"
#include "gladesig.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char *argv[])
{
    GtkWidget *window;
    int iret, dbset = 0, tblset = 0;
    int c, digit_optind = 0;
    char pass[25], user[25], host[25];
    char flags[255];

    for (c = 0; c < 255; c++)
	flags[c] = 0;

    /* add getopt here */
    /*
       if (argc == 3) {
       sprintf(entry_table, "%s", argv[2]);
       } else {
       fprintf(stderr, "You mush specify the db and the table.\n");
       fprintf(stderr, "Syntax: gentry <db> <table>\n");
       exit(-1);
       }
     */

    while (1) {
	int this_option_optind = optind ? optind : 1;

	c = getopt(argc, argv, "u:p:d:t:vh:");
	if (c == -1)
	    break;
	switch (c) {
	case 'h':
	    strcpy(host, optarg);
	    flags['h'] = 1;
	    printf("option h chosen = host [%s]\n", host);
	    break;
	case 'u':
	    strcpy(user, optarg);
	    flags['u'] = 1;
	    printf("option u chosen = user [%s]\n", user);
	    break;
	case 'p':
	    strcpy(pass, optarg);
	    flags['p'] = 1;
	    printf("option p chosen = password [%s]\n", pass);
	    break;
	case 'd':
	    strcpy(entry_db, optarg);
	    flags['d'] = 1;
	    dbset++;
	    printf("option d chosen = database [%s]\n", entry_db);
	    break;
	case 't':
	    strcpy(entry_table, optarg);
	    flags['t'] = 1;
	    tblset++;
	    printf("option t chosen = table [%s]\n", entry_table);
	    break;
	case 'v':
	    printf("version: %s\n", GENTRY_VERSION);
	    exit(0);
	    break;
	}
    }

    if (optind < argc) {
	printf("non-option ARGV-elements:\n ");
	while (optind < argc) {
	    printf("[%d] %s ", optind, argv[optind]);
	    if (!dbset) {
		printf("Setting db value\n");
		strcpy(entry_db, argv[optind]);
		dbset++;
	    } else if (!tblset) {
		printf("Setting table value\n");
		strcpy(entry_table, argv[optind]);
		tblset++;
	    }
	    optind++;
	}
	printf("\n");
    }
//    if (strlen(user) == 0)
    if (flags['u'] == 0)
	user[0] = NULL;
//    if (strlen(pass) == 0)
    if (flags['p'] == 0)
	pass[0] = NULL;
//    if (strlen(host) == 0)
    if (flags['h'] == 0)
	host[0] = NULL;

    gtk_set_locale();
    gtk_init(&argc, &argv);

    printf("\n");
    printf("Connection data:\n");
    printf("    Host: %s\n", host);
    printf("    User: %s\n", user);
    printf("    Password: %s\n", pass);
    printf("    Database: %s\n", entry_db);
    printf("    Table: %s\n", entry_table);
    printf("\n");

    if (!mysql_connect(&astfhost, "", user, pass)) {
	printf("Error connecting to mySQL: %s\n", mysql_error(&astfhost));
	exit(-1);
    }
    if (mysql_select_db(&astfhost, entry_db)) {
	printf
	    ("The database '%s' is invalid or you do not have permission.\n",
	     entry_db); exit(-1);
    }
    /*
     * The following code was added by Glade to create one of each component
     * (except popup menus), just so that you see something after building
     * the project. Delete any components that you don't want shown initially.
     */
    window = create_window();
    gtk_widget_show(window);
    gtk_main();
    return 0;
}


void on_ins_button_clicked(GtkButton * button, gpointer user_data)
{
    int x, context_id;
    char q[1000], templen[20];
    char *newq;
    int iret;

    sprintf(q, "insert into %s values (", entry_table);
    for (x = 0; x < table_cols; x++) {
	table_data[x] = gtk_entry_get_text(GTK_ENTRY(entry_data[x]));
	switch (table_type[x]) {
	case GENTRY_DATETIME:
	case GENTRY_TIMESTAMP:
	case GENTRY_YEAR:
	case GENTRY_DATE:
	    table_data[x] = gtk_entry_get_text(GTK_ENTRY(entry_data[x]));
	    if (strlen(table_data[x]) == 0) {
		strcat(q, "null");
	    } else if (!strncmp(table_data[x], "null", 4)) {
		strcat(q, "null");
	    } else {
		strcat(q, "'");
		strcat(q, table_data[x]);
		strcat(q, "'");
	    }
	    break;
	case GENTRY_TIME:
	    table_data[x] = gtk_entry_get_text(GTK_ENTRY(entry_data[x]));
	    if (strlen(table_data[x]) == 0) {
		strcat(q, "null");
	    } else if (!strncmp(table_data[x], "null", 4)) {
		strcat(q, "null");
	    } else {
		strcat(q, "'");
		strcat(q, table_data[x]);
		strcat(q, "'");
	    }
	    break;
	case GENTRY_CHAR:
	    table_data[x] = gtk_entry_get_text(GTK_ENTRY(entry_data[x]));
	    strcat(q, "'");
	    newq =
		malloc(sizeof(char) * ((strlen(table_data[x]) * 2) + 1));
	    iret =
		mysql_escape_string(newq, table_data[x],
				    strlen(table_data[x]));
	    printf("Before: %s\nAfter: %s\n", table_data[x], newq);
	    strcat(q, newq);
	    free(newq);
	    strcat(q, "'");
	    break;
	case GENTRY_VARCHAR:
	    table_data[x] = gtk_entry_get_text(GTK_ENTRY(entry_data[x]));
	    strcat(q, "'");
	    newq =
		malloc(sizeof(char) * ((strlen(table_data[x]) * 2) + 1));
	    iret =
		mysql_escape_string(newq, table_data[x],
				    strlen(table_data[x]));
	    printf("Before: %s\nAfter: %s\n", table_data[x], newq);
	    strcat(q, newq);
	    free(newq);
	    strcat(q, "'");
	    break;
	case GENTRY_ENUM:
	    table_data[x] =
		gtk_entry_get_text(GTK_ENTRY
				   (GTK_COMBO(entry_data[x])->entry));
	    strcat(q, "'");
	    strcat(q, table_data[x]);
	    strcat(q, "'");
	    break;
	case GENTRY_AUTO:
	    table_data[x] = gtk_entry_get_text(GTK_ENTRY(entry_data[x]));
	    strcat(q, "0");
	    break;
	default:
	    table_data[x] = gtk_entry_get_text(GTK_ENTRY(entry_data[x]));
	    if (strlen(table_data[x]) == 0) {
		strcat(q, "null");
	    } else if (!strncmp(table_data[x], "null", 4)) {
		strcat(q, "null");
	    } else {
		strcat(q, "'");
		strcat(q, table_data[x]);
		strcat(q, "'");
	    }
	    break;
	}
	if (x != (table_cols - 1))
	    strcat(q, ", ");
    }

    strcat(q, ")");
    printf("Query: %s\n", q);
    iret = mysql_query(&astfhost, q);

    if (iret == 0) {
	for (x = 0; x < table_cols; x++) {
	    switch (table_type[x]) {
	    case GENTRY_AUTO:
		gtk_entry_set_text(GTK_ENTRY(entry_data[x]), "");
		sprintf(q, "select max(%s) from %s", auto_column,
			entry_table);
		iret = mysql_query(&astfhost, q);
		if (iret) {
		    printf("The table '%s' is not valid.\n", entry_table);
		    exit(-1);
		}
		maxiddata = mysql_store_result(&astfhost);
		maxidrow = mysql_fetch_row(maxiddata);
		auto_max = atoi(maxidrow[0]) + 1;
		sprintf(templen, "%d", auto_max);
		mysql_free_result(maxiddata);
		gtk_entry_set_text(GTK_ENTRY(entry_data[x]), templen);
		gtk_entry_set_editable(GTK_ENTRY(entry_data[x]), FALSE);
		break;
	    default:
		gtk_entry_set_text(GTK_ENTRY(entry_data[x]), "");
		break;
	    }
	}
    }
    context_id =
	gtk_statusbar_get_context_id(GTK_STATUSBAR(status_bar),
				     "mysql query results");
    gtk_statusbar_push(GTK_STATUSBAR(status_bar),
		       GPOINTER_TO_INT(context_id),
		       mysql_error(&astfhost));
    /* test for focus switch */
    gtk_widget_grab_focus(GTK_ENTRY(entry_data[0]));
}


void on_clear_button_clicked(GtkButton * button, gpointer user_data)
{
    int x;
    for (x = 0; x < table_cols; x++) {
	gtk_entry_set_text(GTK_ENTRY(entry_data[x]), "");
    }
    gtk_widget_grab_focus(GTK_ENTRY(entry_data[0]));
}


void on_cancel_button_clicked(GtkButton * button, gpointer user_data)
{

}
