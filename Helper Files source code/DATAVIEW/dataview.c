//*** dataview.c ***

/*
*	MIT License
*
*	Copyright (c) 2023 maihoaomv
*
*	Permission is hereby granted, free of charge, to any person obtaining a copy
*	of this software and associated documentation files (the "Software"), to deal
*	in the Software without restriction, including without limitation the rights
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*	copies of the Software, and to permit persons to whom the Software is
*	furnished to do so, subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included in all
*	copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*	SOFTWARE.
*/
// dataview.c

#include "dataview.h"
#include "dataview_version.h"

// SAVE!!  SAVE!!  SAVE!!  SAVE!!  JUST IN CASE I SHOULD WANT TO REVISIT
//~ gboolean on_dataview_Pane_query_tooltip(GtkStatusIcon *status_icon,  int    x,  int    y,  gboolean  keyboard_mode,  GtkTooltip  *tooltip,  app_variables *dataviewVars)
//~ {
//~ // SAVE!!   SAVE!!   SAVE!!   SAVE!!   SAVE!!
//~ // SEE The "query-tooltip" IN devhelp FOR THIS SIGNAL
//~ g_autofree gchar *pos = g_strdup_printf("%d", gtk_paned_get_position (GTK_PANED (dataviewVars->w_dataview_Pane)));
//~ gtk_tooltip_set_text (tooltip, pos);
//~ gtk_tooltip_set_tip_area (tooltip, NULL);
//~ return TRUE;
//~ }
// SAVE!!  SAVE!!  SAVE!!  SAVE!!  JUST IN CASE I SHOULD WANT TO REVISIT

int main(int argc, gchar **argv)
{   //linenum;
    gtk_init(&argc, &argv);

    // IS THE HELP OR VERSION OPTION ON THE COMMAND LINE?
    // IF VERSION OPTION -v OR --version THEN PRINT VERSION STRING AND EXIT
    int loop = 1;

    if(argc > 1)
    {
        while(loop < argc)
        {
            // IF REQUESTING VERBOSE THEN SET THE FLAG
            if((g_strcmp0(argv[loop], "--verbose") == 0) || (g_strcmp0(argv[loop], "-V") == 0) )
            {
                globalVerbose++;
                printf("Line %4d -- VERBOSE ON\n", __LINE__);
            }

            // IF THE VERSION OPTION ON COMMAND LINE THAN PRINT STRING TO STDOUT AND EXIT
            if((g_strcmp0(argv[loop], "--version") == 0) || (g_strcmp0(argv[loop], "-v") == 0) )
            {
                fprintf(stdout, "%s\n", versionString());
                exit(0);
            }

            // IF HELP OPTION -h OR --help ON COMMAND LINE THEN PRINT HELP STRING AND EXIT
            if((g_strcmp0(argv[loop], "--help") == 0) || (g_strcmp0(argv[loop], "-h") == 0))
            {   //  printf("Line %4d -- Print help to stdout\n", __LINE__);

                GString *helpStr = NULL;
                helpStr= g_string_new("");

                g_string_append_printf(helpStr, "\n Usage: \t%s  [ --host ]  [ --port ] ", g_get_prgname ());
                g_string_append_printf(helpStr, " [ --timeout ]  [ --width ]  [ --height ]  [ --font ]  [ --title ]\n");

                g_string_append_printf(helpStr, " Example: \t%s --host=\"localhost\" --port=6600 --timeout=30000 ", g_get_prgname ());

                g_string_append_printf(helpStr, "--width=1200 --height=800 --font=\"Bold 20px Sans\" ");
                g_string_append_printf(helpStr, "--title=\"Display MPD Database\"\n\n");
                g_string_append_printf(helpStr, "\n This app will display the MPD database files and is intended to be used as a helper, but it can be used standalone.\n Default is localhost unless specified othewise with the --host= option.\n");

                g_string_append_printf(helpStr, "\n %s  [ -h ]  -Print help to command line then exit\n", g_get_prgname ());
                g_string_append_printf(helpStr, " %s  [ -v ]  -Print version to command line then exit\n", g_get_prgname ());
                g_string_append_printf(helpStr, "\n %s  [ -V ]  -Print verbose useful stats to stdout command line\n\n", g_get_prgname ());

				g_string_append_printf(helpStr, "\n Press F1 for Help, Press ESC to exit %s app\n", g_get_prgname ());

				g_string_append_printf(helpStr, "\n NOTE: Optionally an external config file may be used with %s. If the config file  %s.conf  exist in the\n       current working directory, then the app will automatically read configuation from the external configuration file\n       instead of the internal resouce configuration file.\n\n", g_get_prgname (), g_get_prgname ());

                g_fprintf(stderr,"%s", helpStr->str);
                g_string_free(helpStr, TRUE);
                exit(0);
            }
            loop++;
            if(loop > argc) break;
        }
    }

    //****** INITILIZE DATAVIEW VARS STRUCT ******
    app_variables *dataviewVars = g_slice_new0(app_variables);

	// SET TO NULL TO ACCOMMODATE COMMAND ARGS
    dataviewVars->mpdHostname = NULL;
    dataviewVars->windowTitle = NULL;
    dataviewVars->dialogFont = NULL;

	// SET TO -1 TO ACCOMMODATE COMMAND ARGS
    dataviewVars->mpdPortnumber = -1;
    dataviewVars->mpdTimeout = -1;
    dataviewVars->winWidth = -1;
    dataviewVars->winHeight = -1;

	//SET TO NULL TO BE ON SAFE SIDE
    dataviewVars->cwdPath = NULL;
    dataviewVars->gladeFile = NULL;
    dataviewVars->cssFile = NULL;
    dataviewVars->cfgFile = NULL;

	// DEFAULT DIALOG TIMEOUT IS 3 SECONDS
	dataviewVars->notifyDialogTimeout = 3;

	// SELECT TRACK TREEVIEW ICONS
    dataviewVars->dir_Icon_pixBuf = NULL;			// TOP DIRECOTY ICON IN SELECT TRACK TREEVIEW
    dataviewVars->sub_dir_Icon_pixBuf = NULL;	// SUB DIRECOTY ICON IN SELECT TRACK TREEVIEW
    dataviewVars->trk_Icon_pixBuf = NULL;			// TRACK ICON IN SELECT TRACK TREEVIEW

    // IS THIS EXECUTABLE A LINKED FILENAME, A RELATIVE FILENAME OR AN ABSOULTE PATH/FILENAME?
    gchar *isLink = g_file_read_link (argv[0], NULL);
    gchar *isDir = g_path_get_dirname (argv[0]);

    if(isLink == NULL)
    {
        if(0 == g_strcmp0(isDir, "."))
            dataviewVars->cwdPath = g_get_current_dir ();
        else dataviewVars->cwdPath = g_strdup(isDir);
    }
    else {
        dataviewVars->cwdPath = g_path_get_dirname(isLink);
    }
    g_free(isDir);
    g_free(isLink);

    // GET AND PROCESS THE COMMAND LINE ARGUMENTS
    getTheOptions(argc, argv, dataviewVars);

    // GET THE CONFIG KEY VARIABLES
    getConfigKeys(dataviewVars);

    // SETUP SIGNAL FOR CTRL+C
    signal(SIGINT, (void *)gtk_main_quit);	/* 2 Ctrl-C */

    // THE OTHER SIGNALS ARE SENT TO THE CALLBACK FUNCTION
    signal(SIGTERM, signal_callback_sigFault);	/* 15 "regular" kill */
    signal(SIGHUP, signal_callback_sigFault);	/* 1 kill -HUP */
    signal(SIGPIPE, signal_callback_sigFault);   /* 13 write to closed socket */
    signal(SIGSEGV, signal_callback_sigFault);   // 11 -- SEGMENTATION FAULT IS USUALLY THE MOST COMMON FAULT
    signal(SIGILL, signal_callback_sigFault);	/* 4 Illegal Instruction (ANSI) */
    signal(SIGFPE, signal_callback_sigFault);	/* 8 Floating-Point arithmetic Exception (ANSI)*/
    signal(SIGABRT, signal_callback_sigFault);	/* 6 Abort (ANSI) */

    // INITALIZE THE GLADE GUI VARIABLES
    initilizeGladeWidgets(dataviewVars);

    // LOAD THE CSS STYLE FILE
    loadCCSProvider(dataviewVars);

    // SET THE SIGNALS
    setSignals(dataviewVars);

    // **** TEST MPD CONNECTION
    struct mpd_connection *conn  = NULL;
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);
    if(conn == NULL) {
        mpd_connection_free(conn);
        gtk_main_quit();
    }
    mpd_connection_free(conn);

    // TELL THE TREEVIEW TO INITILIZE THE TREEVIEW FOR THE FIRST TIME AND SHOW ON MAIN WINDOW AND BOX_0
    dataviewVars->startUpFirstPass = 1;

    // START A MILLISECOND TIMER TO GET STATUS AND TRACK DATA
    g_timeout_add_full (G_PRIORITY_HIGH, 250, (GSourceFunc) showViewTimer, dataviewVars, NULL);

    // SET KEY MASK
    gtk_accelerator_set_default_mod_mask (GDK_CONTROL_MASK | GDK_SHIFT_MASK | GDK_MOD1_MASK );

    // DRAG-N-DROP FROM DATABASE TO SELECT TRACKS
    dragANDdrop(dataviewVars);

    // PUT THE APP WINDOW ON TOP
    gtk_window_set_keep_above (GTK_WINDOW(dataviewVars->w_window_main), TRUE);

    //SHOW THE WINDOW
    gtk_main();

	// FREE STUFF WHEN WE EXIT THE MAIN LOOP
	g_free(dataviewVars->mpdHostname);
	g_free(dataviewVars->cwdPath);
	g_free(dataviewVars->gladeFile);
	g_free(dataviewVars->cssFile);
	g_free(dataviewVars->cfgFile);

	g_free(dataviewVars->windowTitle);
	g_free(dataviewVars->dialogFont);

    g_object_unref (dataviewVars->dir_Icon_pixBuf);
    g_object_unref (dataviewVars->sub_dir_Icon_pixBuf);
    g_object_unref (dataviewVars->trk_Icon_pixBuf);


    // FREE THE STRUCT
    g_slice_free(app_variables, dataviewVars);
    //    printf("\nEnd of -- %s -- Line %d --\n", __func__, __LINE__);
    exit(0);
} // *** End of main ***

// ****** START OF DATAVIEW.C ******  //
gboolean dataView(app_variables *dataviewVars)
{   // linenum;

    // CHECK STAT VARIABLES TO SEE IF ANYTHING, THAT MIGHT AFFECT THE DATABASE, HAS CHANGED
    // THESE ARE THE STAT VARIABLES
    struct mpd_connection *conn;
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);
    struct mpd_stats *stats = mpd_run_stats(conn);

    dataviewVars->db_number_of_artists 	=	mpd_stats_get_number_of_artists (stats);
    dataviewVars->db_number_of_albums =	mpd_stats_get_number_of_albums(stats);
    dataviewVars->db_number_of_songs 	=	mpd_stats_get_number_of_songs(stats);
    dataviewVars->db_update_time			=	mpd_stats_get_db_update_time(stats);
    dataviewVars->db_play_time				=	mpd_stats_get_db_play_time(stats);

    mpd_stats_free(stats);
    mpd_connection_free(conn);

    static unsigned 			saveDbArtist = 0;
    static unsigned 			saveDbAlbums = 0;
    static unsigned 			saveDbSongs = 0;
    static unsigned long  saveDbUpTime  = 0;
    static unsigned long  saveDbPlayTime  = 0;

    // HAS ANY OF THESE CHANGED? IF SO, THEN IT MIGHT BE AN INDICATION THAT WE NEED TO UPDATE THE TREE VIEW
    int x = 0;
    // THERE IS PROBABLY A BETTER WAY OF DOING THIS
    // HAS SONG QUEUE POSITION CHANGED?
    if(dataviewVars->db_number_of_artists  != saveDbArtist ) {
        saveDbArtist = dataviewVars->db_number_of_artists;
        x++;
    }
    // HAS SONG QUEUE LENGTH CHANGED?
    if(dataviewVars->db_number_of_albums  != saveDbAlbums ) {
        saveDbAlbums = dataviewVars->db_number_of_albums;
        x++;
    }
    // HAS QUEUE VERSION CHANGED?
    if(dataviewVars->db_number_of_songs  != saveDbSongs ) {
        saveDbSongs = dataviewVars->db_number_of_songs;
        x++;
    }
    // HAS SONG QUEUE ID CHANGED?
    if(dataviewVars->db_update_time  != saveDbUpTime ) {
        saveDbUpTime = dataviewVars->db_update_time;
        x++;
    }
    // HAS DB PLAY TIME CHANGED?
    if(dataviewVars->db_play_time  != saveDbPlayTime ) {
        saveDbPlayTime = dataviewVars->db_play_time;
        x++;
    }

    // IF THIS IS THE FIRST PASS OF THE TRACKVIEW MODE,
    // THEN WE NEED TO PASS ALL THE WAY THRU TO POPULATE / UPDATE THE TREEVIEW
    if(dataviewVars->dataviewFirstPass)   {
        // SET PANED 0-100 PERCENT OF WINDOW WIDTH
        guint windowWidth = gtk_widget_get_allocated_width (GTK_WIDGET(dataviewVars->w_window_main));
        // 75 PERCENT (0.75) WOULD BE -LEFT PANE 75% OF WINDOW WIDTH  -RIGHT PANE 25% OF WINDOW WIDTH
        gfloat panedPos = (gfloat)dataviewVars->panedPosition / (gfloat)100;
        gtk_paned_set_position (dataviewVars->w_dataview_Pane, (gint)(windowWidth * panedPos));
    }
    // OTHERWISE IF NOTHING HAS CHANGED, WE JUST RETURN.
    // THIS PREVENT CONSISTENT HIGH CPU USAGE
    else 	if(x == 0) {
        return TRUE;
    }

    x = 0;
    // OK -- SO HERE WE GO
    GtkTreeIter    toplevel, child_0;
    gchar *directory = NULL;

    // GET MPD CONNECTION
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);

    // RECEIVE THE ENTITY FROM THE MPD SERVER
    struct mpd_entity * entity;
    mpd_send_list_all(conn, NULL);

    // CLEAR THE TREE STORE
    gtk_tree_store_clear (dataviewVars->w_dataviewTreeStore);

    // BECAUSE OF SPEED ISSUES, DETACH TREESTORE FROM TREEVIEW
    GtkTreeModel *model;
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(dataviewVars->w_dataviewTreeView));

    /* MAKE SURE THE MODEL STAYS WITH US AFTER THE TREE VIEW UNREFS IT */
    g_object_ref(model);

    /* DETACH MODEL FROM VIEW */
    gtk_tree_view_set_model(GTK_TREE_VIEW(dataviewVars->w_dataviewTreeView), NULL);

    // ***** ROOT TREE -- SAVE! SAVE! ***** THIS WILL PUT THE ROOT OF THE MPD DATABASE TREE IN THE TREESTORE AT THE VERY TOP (COL 0)
    gtk_tree_store_append(dataviewVars->w_dataviewTreeStore, &toplevel, NULL);
    gtk_tree_store_set(dataviewVars->w_dataviewTreeStore, &toplevel, COL_DBDIR, "root", COL_DBPATH, "/",  -1);

    gchar *dirPath = NULL;
    while ((entity = mpd_recv_entity(conn)) != NULL) {

        const struct mpd_directory *dir;

        if (mpd_entity_get_type(entity) != MPD_ENTITY_TYPE_DIRECTORY) {
            mpd_entity_free(entity);
            continue;
        }

        dir = mpd_entity_get_directory(entity);
        directory = g_strdup (mpd_directory_get_path(dir));  //THIS IS THE PATH

        if(!(g_strstr_len (directory, -1, "/"))) { // IF NULL, THEN START OF NEW DIRECTORY
            gtk_tree_store_append(dataviewVars->w_dataviewTreeStore, &toplevel, NULL);
            gtk_tree_store_set(dataviewVars->w_dataviewTreeStore, &toplevel,
                               COL_DBDIR, directory,
                               COL_DBPATH, directory,  -1);
        }

        else  {
            dirPath = g_path_get_basename (mpd_directory_get_path(dir));
            gtk_tree_store_append(dataviewVars->w_dataviewTreeStore, &child_0, &toplevel);
            gtk_tree_store_set(dataviewVars->w_dataviewTreeStore, &child_0,
                               COL_DBDIR, dirPath,
                               COL_DBPATH, directory,  -1);
            g_free(dirPath);
        }

        if(directory) {
            g_free(directory);
            directory = NULL;
        }

        mpd_entity_free(entity);
    }

    // REATTACH MODEL TO THE TREEVIEW
    gtk_tree_view_set_model(GTK_TREE_VIEW(dataviewVars->w_dataviewTreeView), model);
    g_object_unref(model);

    if (!mpd_response_finish(conn)) {
        mpd_connection_clear_error(conn);
        gtk_main_quit();
    }

    // FREE THE MPD CONNECTION
    mpd_connection_free(conn);
    dataviewVars->dataviewFirstPass = 0;

    // GRAB FOCUS OF THE VIEW AT ROW 0 COL 0
    gtk_widget_grab_focus(GTK_WIDGET(dataviewVars->w_dataviewTreeView));

    //	printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
    return FALSE;
} // *** END OF dataView // *** END OF dataView

// THIS FUNCTION GET TRACKS FROM THE SELECTED DIRECTORY
// DOUBLE CLICK ON DIRECTORY TO OPEN IN SELECT TRACK VIEW
gboolean dataview_onRowActivated (GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn  *col, app_variables *dataviewVars)
{   // linenum;

    // ****** THIS WILL ONLY WORK IN SINGLE OR BROWSE SELECTION MODE! ******//
    GtkTreeSelection *selection;
    GtkTreeModel     *model;
    GtkTreeIter       iter;
    g_autofree gchar *value = NULL;

    // GET MPD CONNECTION
    struct mpd_connection *conn;
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);
    mpd_run_stop(conn);
    mpd_run_clear(conn);

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    gtk_tree_selection_get_selected(selection, &model, &iter);
    gtk_tree_model_get (model, &iter, COL_DBPATH, &value, -1);

    mpd_run_add(conn, value);
    mpd_run_play(conn);
    mpd_connection_free(conn);

    g_autofree gchar *textStr = g_strdup_printf("Replaced MPD Queue -- %s", value);
	showNotify (textStr,  dataviewVars);
    //    printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
    return FALSE;
} // *** End of dataview_onRowActivated

gboolean selTrack_popupMenu(GdkEventButton *event, app_variables *dataviewVars)
{   // linenum;

    GtkWidget *selTrkPopupMenu;

    selTrkPopupMenu = gtk_menu_new();
//////////////////////////////////////////////////////////////////////////////////////////////////
	GtkWidget *titleMi = gtk_menu_item_new_with_label("Select Tracks Menu");
	gtk_menu_shell_append(GTK_MENU_SHELL(selTrkPopupMenu), titleMi);
	gtk_widget_set_name (titleMi, "CSS_selTrksTitleMi");

//////////////////////////////////////////////////////////////////////////////////////////////////
    // *** SET THE CALLBACKS  FOR THE SELECT TRACKS POPUP MENU ***

    // REMOVE SELECTED ROWS
    GtkWidget *selRowsMi = gtk_menu_item_new_with_label("Remove Selected Rows");
    g_signal_connect(selRowsMi, "activate", (GCallback) removeSelectedRows_signal,  dataviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(selTrkPopupMenu), selRowsMi);

    // REMOVE ALL ROWS
    GtkWidget *allRowsMi = gtk_menu_item_new_with_label("Remove All Rows");
    g_signal_connect(allRowsMi, "activate", (GCallback) clearAllRows_signal, dataviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(selTrkPopupMenu), allRowsMi);

    // ADD TRACKS TO MPD QUEUE
    GtkWidget *addTracksMI = gtk_menu_item_new_with_label("Add Tracks to MPD Queue");
    g_signal_connect(addTracksMI, "activate", (GCallback) selTrk_addTracks, dataviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(selTrkPopupMenu), addTracksMI);

    // REPLACE MPD QUEUE - START PLAYER
    GtkWidget *replaceTracksMI = gtk_menu_item_new_with_label("Replace MPD Queue - Start Player");
    g_signal_connect(replaceTracksMI, "activate", (GCallback) replaceTracksPlay, dataviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(selTrkPopupMenu), replaceTracksMI);

    // ADD SEPERATOR
    GtkWidget *separator1Mi = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(selTrkPopupMenu), separator1Mi);

    // EXPAND ALL ROWS
    GtkWidget *expandAllRowsMi = gtk_menu_item_new_with_label("Expand All Rows");
    g_signal_connect(expandAllRowsMi, "activate", (GCallback) expandAllRows, dataviewVars->w_selTrkTreeView);
    gtk_menu_shell_append(GTK_MENU_SHELL(selTrkPopupMenu), expandAllRowsMi);

    // COLLAPSE ALL ROWS
    GtkWidget *collapseAllRowsMi = gtk_menu_item_new_with_label("Collapse All Rows");
    g_signal_connect(collapseAllRowsMi, "activate", (GCallback) collapseAllRows, dataviewVars->w_selTrkTreeView);
    gtk_menu_shell_append(GTK_MENU_SHELL(selTrkPopupMenu), collapseAllRowsMi);

    // EXPAND SELECTED ROW
    GtkWidget *expandSelectedRowMi = gtk_menu_item_new_with_label("Expand Selected Row");
    g_signal_connect(expandSelectedRowMi, "activate", (GCallback) expandSelectedRow, dataviewVars->w_selTrkTreeView);
    gtk_menu_shell_append(GTK_MENU_SHELL(selTrkPopupMenu), expandSelectedRowMi);

    // COLLAPSE SELECTED ROW
    GtkWidget *collapseSelectedRowMi = gtk_menu_item_new_with_label("Collapse Selected Row");
    g_signal_connect(collapseSelectedRowMi, "activate", (GCallback) collapseSelectedRow, dataviewVars->w_selTrkTreeView);
    gtk_menu_shell_append(GTK_MENU_SHELL(selTrkPopupMenu), collapseSelectedRowMi);

    // ADD SEPERATOR
    GtkWidget *separator2Mi = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(selTrkPopupMenu), separator2Mi);

    // CLEAR QUEUE PLAYLIST
    GtkWidget *clearQueMi = gtk_menu_item_new_with_label("Clear Queue Playlist");
    g_signal_connect(clearQueMi,  "activate",     (GCallback)   clearPlaylist_signal,  dataviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(selTrkPopupMenu), clearQueMi);

    // IF PLAY QUEUE IS EMPTY (GRAY BACKGROUND), GREY OUT THE MENU ITEM
    if(dataviewVars->queueLengthStatus == 0)
        gtk_widget_set_sensitive (clearQueMi, FALSE);

    gtk_widget_set_name (selTrkPopupMenu, "CSS_selTrkPopupMenu");

    gtk_widget_show_all(selTrkPopupMenu);

    gtk_menu_popup_at_widget (GTK_MENU(selTrkPopupMenu), GTK_WIDGET(dataviewVars->w_window_main),
                              GDK_GRAVITY_CENTER,
                              GDK_GRAVITY_CENTER,
                              NULL);

    // printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
    return FALSE;
}

// WORKS WITH Add Tracks and Play, Replace Tracks and Stop, Replace Tracks and Play MENU ITEMS
void dataview_view_selected_foreach_func(GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer conn)
{   //  linenum;

    g_autofree gchar *value = NULL;
    gtk_tree_model_get(model, iter, COL_TRKPATH, &value, -1);

    mpd_run_add(conn, value);

    // printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
}

void selTrk_addTracks(GtkMenuItem *menuItem, app_variables *dataviewVars)
{   //  linenum;

    // SETUP MPD CONNECTION
    struct mpd_connection *conn;
    // GET MPD CONNECTION
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);

    GtkTreeSelection *selection;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(dataviewVars->w_selTrkTreeView));

    gtk_tree_selection_selected_foreach(selection, dataview_view_selected_foreach_func, conn);

    mpd_connection_free(conn);

    g_autofree gchar *textStr = g_strdup("Add Tracks to MPD Queue");

	showNotify (textStr,  dataviewVars);
    // printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
}

void addTracksPlay(GtkMenuItem *menuItem, app_variables *dataviewVars)
{   //  linenum;

    // SETUP MPD CONNECTION
    struct mpd_connection *conn;
    // GET MPD CONNECTION
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);

    GtkTreeSelection *selection;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(dataviewVars->w_selTrkTreeView));

    gtk_tree_selection_selected_foreach(selection, dataview_view_selected_foreach_func, conn);
    mpd_run_play(conn);

    mpd_connection_free(conn);
    // printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
}

void addTracksStop(GtkMenuItem *menuItem, app_variables *dataviewVars)
{   //   linenum;

    // SETUP MPD CONNECTION
    struct mpd_connection *conn;
    // GET MPD CONNECTION
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);

    GtkTreeSelection *selection;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(dataviewVars->w_selTrkTreeView));

    gtk_tree_selection_selected_foreach(selection, dataview_view_selected_foreach_func, conn);
    mpd_run_stop(conn);

    mpd_connection_free(conn);
    // printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
}

void replaceTracksStop(GtkMenuItem *menuItem, app_variables *dataviewVars)
{   //   linenum;

    // SETUP MPD CONNECTION
    struct mpd_connection *conn;
    // GET MPD CONNECTION
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);
    if(conn == NULL) {
        gtk_main_quit();
    }
    // CLEAR THE QUEUE OF ALL TRACKS
    mpd_run_clear(conn);

    GtkTreeSelection *selection;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(dataviewVars->w_selTrkTreeView));

    gtk_tree_selection_selected_foreach(selection, dataview_view_selected_foreach_func, conn);

    mpd_connection_free(conn);
    // printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
}

void directory_replaceTracksPlay(GtkMenuItem *menuItem, app_variables *dataviewVars)
{   //  linenum;
    // ****** THIS WILL ONLY WORK IN SINGLE OR BROWSE SELECTION MODE! ******//
    GtkTreeSelection *selection;
    GtkTreeModel     *model;
    GtkTreeIter       iter;
    g_autofree gchar *value = NULL;

    // GET MPD CONNECTION
    struct mpd_connection *conn;
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);
    mpd_run_stop(conn);
    mpd_run_clear(conn);

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(dataviewVars->w_dataviewTreeView));
    gtk_tree_selection_get_selected(selection, &model, &iter);
    gtk_tree_model_get (model, &iter, COL_DBPATH, &value, -1);

    mpd_run_add(conn, value);

    mpd_run_play(conn);
    mpd_connection_free(conn);

    g_autofree gchar *textStr = g_strdup("Replace MPD Queue - Start Player");

	showNotify (textStr,  dataviewVars);
    //    printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
}


void addDirToMpdQue(GtkMenuItem *menuItem, app_variables *dataviewVars)
{   //      linenum;
    // ****** THIS WILL ONLY WORK IN SINGLE OR BROWSE SELECTION MODE! ******//
    GtkTreeSelection *selection;
    GtkTreeModel     *model;
    GtkTreeIter       iter;
    g_autofree gchar *value = NULL;

    // GET MPD CONNECTION
    struct mpd_connection *conn;
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(dataviewVars->w_dataviewTreeView));
    gtk_tree_selection_get_selected(selection, &model, &iter);
    gtk_tree_model_get (model, &iter, COL_DBPATH, &value, -1);

    mpd_run_add(conn, value);

    mpd_connection_free(conn);

    g_autofree gchar *textStr = g_strdup("Add Directory to MPD Queue");

	showNotify (textStr,  dataviewVars);
    //    printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
}

void replaceTracksPlay(GtkMenuItem *menuItem, app_variables *dataviewVars)
{   //   linenum;

    // GET MPD CONNECTION
    struct mpd_connection *conn;
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);
    // CLEAR THE QUEUE OF ALL TRACKS
    mpd_run_clear(conn);

    GtkTreeSelection *selection;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(dataviewVars->w_selTrkTreeView));

    gtk_tree_selection_selected_foreach(selection, dataview_view_selected_foreach_func, conn);
    mpd_run_play(conn);
    mpd_connection_free(conn);

    g_autofree gchar *textStr = g_strdup("Replace MPD Queue - Start Player");

	showNotify (textStr,  dataviewVars);
    //printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
}

void dataview_clearQueue(GtkMenuItem *menuItem, app_variables *dataviewVars)
{   //  linenum;

    // SETUP MPD CONNECTION
    struct mpd_connection *conn;
    // GET MPD CONNECTION
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);

    mpd_send_clear(conn);

    mpd_connection_free(conn);
    //printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
}

void startPlayer(GtkMenuItem *menuItem, app_variables *dataviewVars)
{   // linenum;

    struct mpd_connection *conn;
    struct mpd_status * status;

    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);

    mpd_send_status(conn);
    status = mpd_recv_status(conn);

    if (mpd_status_get_state(status) != MPD_STATE_PLAY)   {
        mpd_run_play(conn);

    }
    else if (mpd_status_get_state(status) != MPD_STATE_PAUSE) {
        mpd_run_pause(conn, true);

    }

    mpd_status_free(status);
    mpd_connection_free(conn);
    //printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
}

void pausePlayer(GtkMenuItem *menuItem, app_variables *dataviewVars)
{   // linenum;

    startPlayer(menuItem, dataviewVars);
    //~ linenum;
    return;
}

void stopPlayer(GtkMenuItem *menuItem, app_variables *dataviewVars)
{   // linenum;
    //~ g_autofree gchar *textStr = NULL;
    struct mpd_connection *conn;
    struct mpd_status * status;
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);
    mpd_send_status(conn);
    status = mpd_recv_status(conn);

    if (mpd_status_get_state(status) == MPD_STATE_STOP)
        mpd_run_play(conn);
    else
        mpd_run_stop(conn);

    mpd_status_free(status);
    mpd_connection_free(conn);
    //printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
}

gboolean dataview_selTrk_onButtonPressed (GtkWidget *widget, GdkEventButton *event, app_variables *dataviewVars)
{   // linenum;

    // ***** THIS CODE PREVENTS SELECTED TRACKS FROM BEING UN-SELECTED WITH RIGHT CLICK MENU BUTTON IS CLICK ON TREEVIEW
    // ***** NOTE: TAKEN FROM gmpc-mpddata-treeview.gob LINE 718 IN GMPC SOURCE CODE ******
    GtkTreePath *path = NULL;
    if(event->button == 3 && event->window != NULL && gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(widget), event->x, event->y,&path,NULL,NULL,NULL))
    {
        GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
        if(gtk_tree_selection_path_is_selected(sel, path))
        {
            gtk_tree_path_free(path);
            selTrack_popupMenu(event, dataviewVars);
            return TRUE;
        }
    }
    if(path) {
        gtk_tree_path_free(path);
    }
    // printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
    return FALSE;
    // ***** NOTE: TAKEN FROM gmpc-mpddata-treeview.gob LINE 718 IN GMPC SOURCE CODE ******
    // ***** End of THIS CODE PREVENTS SELECTED TRACKS FROM BEING UN-SELECTED WITH RIGHT CLICK MENU BUTTON IS CLICK ON TREEVIEW
}

// IF A TRACK OR DIRECTORY IS DOUBLE CLICKED IN the selTrack TREEVIEW,
// THEN REPLACE THE TRACKS IN PLAY QUEUE AND START PLAYER
gboolean selTrack_onRowActivated (GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn  *col, app_variables *dataviewVars)
{   // linenum;
    replaceTracksPlay(NULL, dataviewVars);
    // printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
    return FALSE;
}

gboolean directory_popupMenu(GdkEventButton *event, app_variables *dataviewVars)
{   //  linenum;

    GtkWidget *directoryPopupMenu;

    directoryPopupMenu = gtk_menu_new();

	GtkWidget *titleMi = gtk_menu_item_new_with_label("Database Directory Menu");
	gtk_menu_shell_append(GTK_MENU_SHELL(directoryPopupMenu), titleMi);
	gtk_widget_set_name (titleMi, "CSS_databaseTitleMi");

    // *** set the CALLBACKS  FOR THE POPUP MENU ***

    // ADD DIRECTORY TO SELECT TRACKS
    GtkWidget *adtstMi = gtk_menu_item_new_with_label("Add Directory to Select Tracks");
    g_signal_connect(adtstMi, "activate", (GCallback) addDirToSelTrk, dataviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(directoryPopupMenu), adtstMi);

    // ADD DIRECTORY TO MPD QUEUE
    GtkWidget *adtmqMi = gtk_menu_item_new_with_label("Add Directory to MPD Queue");
    g_signal_connect(adtmqMi, "activate", (GCallback) addDirToMpdQue, dataviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(directoryPopupMenu), adtmqMi);

    // REPLACE MPD QUEUE - START PLAYER
    GtkWidget *rmqspMi = gtk_menu_item_new_with_label("Replace MPD Queue - Start Player");
    g_signal_connect(rmqspMi, "activate", (GCallback) directory_replaceTracksPlay, dataviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(directoryPopupMenu), rmqspMi);

    // UPDATE DATABASE
    GtkWidget *uddbMi = gtk_menu_item_new_with_label("Update Database");
    g_signal_connect(uddbMi, "activate", (GCallback) upDateDatabase, dataviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(directoryPopupMenu), uddbMi);

    // ADD SEPERATOR
    GtkWidget *separator1Mi = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(directoryPopupMenu), separator1Mi);

    // EXPAND ALL DATABASE ROWS
    GtkWidget *expandAllRowsMi = gtk_menu_item_new_with_label("Expand All Rows");
    g_signal_connect(expandAllRowsMi, "activate", (GCallback) expandAllRows, dataviewVars->w_dataviewTreeView);
    gtk_menu_shell_append(GTK_MENU_SHELL(directoryPopupMenu), expandAllRowsMi);

    // COLLAPSE ALL ROWS
    GtkWidget *collapseAllRowsMi = gtk_menu_item_new_with_label("Collapse All Rows");
    g_signal_connect(collapseAllRowsMi, "activate", (GCallback) collapseAllRows, dataviewVars->w_dataviewTreeView);
    gtk_menu_shell_append(GTK_MENU_SHELL(directoryPopupMenu), collapseAllRowsMi);

    // EXPAND SELECTED DATABASE ROW
    GtkWidget *expandSelectedRowMi = gtk_menu_item_new_with_label("Expand Selected Row");
    g_signal_connect(expandSelectedRowMi, "activate", (GCallback) expandSelectedRow, dataviewVars->w_dataviewTreeView);
    gtk_menu_shell_append(GTK_MENU_SHELL(directoryPopupMenu), expandSelectedRowMi);

    // COLLAPSE SELECTED ROW
    GtkWidget *collapseSelectedRowMi = gtk_menu_item_new_with_label("Collapse Selected Row");
    g_signal_connect(collapseSelectedRowMi, "activate", (GCallback) collapseSelectedRow, dataviewVars->w_dataviewTreeView);
    gtk_menu_shell_append(GTK_MENU_SHELL(directoryPopupMenu), collapseSelectedRowMi);

    // ADD SEPERATOR
    GtkWidget *separator2Mi = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(directoryPopupMenu), separator2Mi);

    // CLEAR THE EXISTING PLAYLIST QUEUE MENU ITEM
    GtkWidget *clearPlaylistMi = gtk_menu_item_new_with_label("Clear Queue Playlist");
    g_signal_connect(clearPlaylistMi,  "activate",     (GCallback)   clearPlaylist_signal,  dataviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(directoryPopupMenu), clearPlaylistMi);

    // IF PLAY QUEUE IS EMPTY (GRAY BACKGROUND), GREY OUT THE MENU ITEM
    if(dataviewVars->queueLengthStatus == 0)
        gtk_widget_set_sensitive (clearPlaylistMi, FALSE);

    gtk_widget_set_name (directoryPopupMenu, "CSS_directoryPopupMenu");

    gtk_widget_show_all(directoryPopupMenu);

    gtk_menu_popup_at_widget (GTK_MENU(directoryPopupMenu), GTK_WIDGET(dataviewVars->w_window_main),
                              GDK_GRAVITY_CENTER,
                              GDK_GRAVITY_CENTER,
                              NULL);

    // printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
    return FALSE;
}

void addDirToSelTrk(GtkMenuItem *menuItem, app_variables *dataviewVars)
{   // linenum;

    GtkTreeSelection *selection;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(dataviewVars->w_dataviewTreeView));

    GtkTreeIter iter;
    GtkTreeModel *model;
    g_autofree gchar *value = NULL;

    //    https://wiki.gnome.org/action/show/Community/GettingInTouch/Bugzilla/GettingTraces?action=show&redirect=GettingTraces
    // *** IF THE ROW IS CLICKED THEN GET THE NEW SELECTION
    gtk_tree_selection_get_selected(GTK_TREE_SELECTION( selection), &model, &iter); // WON'T WORK WITHOUT THIS

    gtk_tree_model_get(model, &iter, COL_DBPATH, &value,  -1);

    // GET MPD CONNECTION
    struct mpd_connection *conn;
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);

    GtkTreeIter toplevel, child_0;

    if (!mpd_send_list_all_meta(conn, value))  {
        gtk_main_quit();
    }

    gchar *directory = NULL;

    struct mpd_entity *entity;

    // FOR SPEED ISSUES, DETACH TREESTORE FROM TREEVIEW
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(dataviewVars->w_selTrkTreeView));
    g_object_ref(model); /* Make sure the model stays with us after the tree view unrefs it */
    gtk_tree_view_set_model(GTK_TREE_VIEW(dataviewVars->w_selTrkTreeView), NULL); /* Detach model from view */

    dataviewVars->selTrkFirstPass = 1;
    gchar *dirPath = NULL;
    gint topDir = 1;
    while ((entity = mpd_recv_entity(conn)) != NULL) {
        const struct mpd_directory *dir;
        const struct mpd_song *song;

        switch (mpd_entity_get_type(entity)) {

        case MPD_ENTITY_TYPE_PLAYLIST:
        case MPD_ENTITY_TYPE_UNKNOWN:
            break;

        case MPD_ENTITY_TYPE_DIRECTORY:
            dir = mpd_entity_get_directory(entity);
            dirPath = g_path_get_basename (mpd_directory_get_path(dir));

 //~ linenum;	printf("DIR %s\n", mpd_directory_get_path(dir));
 //~ linenum;	printf("DIRPATH %s\n", dirPath);
			if(!g_strrstr (mpd_directory_get_path(dir), "/")) topDir = 0;

            gtk_tree_store_append(dataviewVars->w_selTrkTreeStore, &toplevel, NULL);

			gtk_tree_store_set(dataviewVars->w_selTrkTreeStore, &toplevel,
								COL_TRKPIXBUF, (topDir) ? dataviewVars->sub_dir_Icon_pixBuf : dataviewVars->dir_Icon_pixBuf,
								COL_FOLDER, dirPath,	COL_TRKPATH, mpd_directory_get_path(dir), -1);


            g_free(dirPath);

            if(directory) {
                g_free(directory);
                directory = NULL;
            }
            topDir = 1;
            break;

        case MPD_ENTITY_TYPE_SONG:
            song = mpd_entity_get_song(entity);

            gtk_tree_store_append(dataviewVars->w_selTrkTreeStore, &child_0, &toplevel);

            gtk_tree_store_set(dataviewVars->w_selTrkTreeStore, &child_0,
                               COL_TRKPIXBUF, dataviewVars->trk_Icon_pixBuf,
                               COL_TRKPATH,    mpd_song_get_uri(song),

                               COL_TRKTRACK,   mpd_song_get_tag(song, MPD_TAG_TRACK, 0),
                               COL_TRKDISC,     mpd_song_get_tag(song, MPD_TAG_DISC, 0),
                               COL_TRKTITLE,    mpd_song_get_tag(song, MPD_TAG_TITLE, 0),
                               COL_TRKARTIST,  mpd_song_get_tag(song, MPD_TAG_ARTIST, 0),
                               COL_TRKALBUM,  mpd_song_get_tag(song, MPD_TAG_ALBUM, 0),
                               -1);
            break;
        }

        mpd_entity_free(entity);

    }

    dataviewVars->selTrkFirstPass = 0;

    // FOR SPEED ISSUES, REATTACH TREESTORE TO TREEVIEW
    gtk_tree_view_set_model(GTK_TREE_VIEW(dataviewVars->w_selTrkTreeView), model); /* Re-attach model to view */
    g_object_unref(model);

    if (!mpd_response_finish(conn)) {
        mpd_connection_clear_error(conn);
        gtk_main_quit();
    }

    mpd_connection_free(conn);	// FREE MPD CONNECTION

    GtkTreePath *path = gtk_tree_path_new_from_string ("0");
    gtk_tree_view_set_cursor(GTK_TREE_VIEW(dataviewVars->w_selTrkTreeView), path, COL_TRKPIXBUF, FALSE);
    gtk_widget_grab_focus(GTK_WIDGET(dataviewVars->w_selTrkTreeView));
    gtk_tree_path_free(path);

}

void upDateDatabase(GtkMenuItem *menuItem, app_variables *dataviewVars)
{   //linenum;

    // SETUP MPD CONNECTION
    struct mpd_connection *conn;
    // GET MPD CONNECTION
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);

    struct mpd_status * status;
    mpd_send_status(conn);
    status = mpd_recv_status(conn);

    gint update =  mpd_run_rescan(conn, NULL);
    mpd_status_free(status);

    struct mpd_stats *stats = mpd_run_stats(conn);
    time_t t = mpd_stats_get_db_update_time(stats);
    mpd_stats_free(stats);

    g_autofree gchar *textStr = NULL;
    textStr = g_strdup_printf("Database Last Updated: %s\nUpdating Database (#%u) ...", ctime(&t), update);

    mpd_connection_free(conn);
	showNotify (textStr,  dataviewVars);
    //	 printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
}

gboolean dataview_directory_onButtonPressed (GtkWidget *widget, GdkEventButton *event, app_variables *dataviewVars)
{   // linenum;
    //* SET BG AND FG COLORS WHEN TREEVIEW IS NOT THE FOCUSED / GRABBED WIDGET  *//
    gtk_widget_set_name (GTK_WIDGET(dataviewVars->w_selTrkTreeView), "CSS_selectTrackBG");
    gtk_widget_set_name (GTK_WIDGET(dataviewVars->w_dataviewTreeView), "CSS_dataviewTreeView");

    // ***** THIS CODE PREVENTS SELECTED TRACKS FROM BEING UN-SELECTED WITH RIGHT CLICK MENU BUTTON IS CLICK ON TREEVIEW
    // ***** NOTE: TAKEN FROM gmpc-mpddata-treeview.gob LINE 718 IN GMPC SOURCE CODE ******
    GtkTreePath *path = NULL;
    if(event->button == 3 && event->window != NULL && gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(widget), event->x, event->y,&path,NULL,NULL,NULL))
    {
        GtkTreeSelection *sel = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));
        if(gtk_tree_selection_path_is_selected(sel, path))
        {
            gtk_tree_path_free(path);
            directory_popupMenu(event, dataviewVars);
            return TRUE;
        }
    }
    if(path) {
        gtk_tree_path_free(path);
    }
    return FALSE;
    // ***** NOTE: TAKEN FROM gmpc-mpddata-treeview.gob LINE 718 IN GMPC SOURCE CODE ******
    // ***** End of THIS CODE PREVENTS SELECTED TRACKS FROM BEING UN-SELECTED WITH RIGHT CLICK MENU BUTTON IS CLICK ON TREEVIEW
}

void clearAllRows_signal(GtkMenuItem *menuItem, app_variables *dataviewVars)
{   //  linenum;
    gtk_tree_store_clear (dataviewVars->w_selTrkTreeStore);
}

void removeSelectedRows_signal(GtkMenuItem *menuItem, app_variables *dataviewVars)
{   //linenum;

    GtkTreeModel *model   = gtk_tree_view_get_model (dataviewVars->w_selTrkTreeView);

    GtkTreeSelection *selection  = gtk_tree_view_get_selection (dataviewVars->w_selTrkTreeView);

    GtkTreeIter iter;

    GList *selected_rows = gtk_tree_selection_get_selected_rows (selection, &model);

    int num_rows_selected;

    for( num_rows_selected = g_list_length (selected_rows); num_rows_selected > 0; num_rows_selected--) {
        selected_rows = gtk_tree_selection_get_selected_rows (selection, &model);
        gtk_tree_model_get_iter (model, &iter, (GtkTreePath *) selected_rows->data);
        if(gtk_tree_selection_iter_is_selected (selection, &iter))
            gtk_tree_store_remove (GTK_TREE_STORE (model), &iter);
    }

    g_list_free_full (g_steal_pointer (&selected_rows), (GDestroyNotify) gtk_tree_path_free);
}

gboolean selTrkGrab_signal(GtkWidget *tvWidget, app_variables *dataviewVars)
{   //linenum;
    /* SET BG AND FG COLORS WHEN TREEVIEW IS NOT THE FOCUSED / GRABBED WIDGET  */
    gtk_widget_set_name (GTK_WIDGET(dataviewVars->w_dataviewTreeView), "CSS_dataBaseBG");
    gtk_widget_set_name (GTK_WIDGET(dataviewVars->w_selTrkTreeView), "CSS_selTrkTreeView");

    return FALSE;
}

gboolean dataBaseGrab_signal(GtkWidget *tvWidget, app_variables *dataviewVars)
{   //linenum;
    //* SET BG AND FG COLORS WHEN TREEVIEW IS NOT THE FOCUSED / GRABBED WIDGET  *//
    gtk_widget_set_name (GTK_WIDGET(dataviewVars->w_selTrkTreeView), "CSS_selectTrackBG");
    gtk_widget_set_name (GTK_WIDGET(dataviewVars->w_dataviewTreeView), "CSS_dataviewTreeView");

    return FALSE;
}

// ****** START OF KEYBOARD.C ******  //
// ***** KEYBOARD EVENTS *****
gboolean window_key_press_event (GtkWidget *widget, GdkEventKey *event, app_variables *dataviewVars)
{   //linenum;

    //~ // SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!
    //~ static gint x = 0;
    //~ x++;
    //~ printf("\n---KEY PRESS--- %d\n", x);
    //~ printf("KEYBOARD EVENT STATE 0x%x -- EVENT KEYVAL 0x%x -- KEY %s\n", event->state, event->keyval, gdk_keyval_name (event->keyval));
    //~ printf("---START--- X = %d\n", x);
    //~ printf("FOCUS SEL TRK %d\n", gtk_widget_is_focus (GTK_WIDGET(dataviewVars->w_selTrkTreeView)));
    //~ printf("FOCUS DATA VIEW %d\n", gtk_widget_is_focus (GTK_WIDGET(dataviewVars->w_dataviewTreeView)));
    //~ printf("FOCUS MAIN WINDOW %d\n", gtk_widget_is_focus (GTK_WIDGET(dataviewVars->w_window_main)));
    //~ printf("---END---\n\n");
    //~ // SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!

    GtkTreePath *path = NULL;

    GdkModifierType modifiers;

    modifiers = gtk_accelerator_get_default_mod_mask ();

    GdkWindow *window = gtk_widget_get_window(widget);
    GdkWindowState state = gdk_window_get_state(GDK_WINDOW(window));

    guint panePos = gtk_paned_get_position (dataviewVars->w_dataview_Pane);
    guint windowWidth = gtk_widget_get_allocated_width (GTK_WIDGET(dataviewVars->w_window_main));
    guint movePane = windowWidth / 20;

    switch(event->keyval) {

    case GDK_KEY_h:  // OPEN HELP LABEL
    case GDK_KEY_H:  // OPEN HELP LABEL
    case GDK_KEY_F1:  // OPEN HELP LABEL
        helpMe_signal(NULL, dataviewVars);
        break;

    case GDK_KEY_F10: //printf("F10 KEY\n");
        // SHIFT-F10 (WITH THE SHIFT KEY) RIGHT CLICK POPUP MENU
        if((event->state & modifiers) == GDK_SHIFT_MASK) {
            window_main_popup_menu(widget, NULL, dataviewVars);
            break;
        }

        // TOGGLES GUI  BETWEEN FULLSCREEN/MAXIMIZE/NORMAL
        if(state & GDK_WINDOW_STATE_MAXIMIZED) {	// MAXIMIZE
            gtk_window_unmaximize(GTK_WINDOW(widget));
            //~ gtk_window_fullscreen(GTK_WINDOW(widget));  // UNCOMMENT FOR FULLSCREEN FUNCTIONALITY
            break;
        }
		  // UNCOMMENT FOR FULLSCREEN FUNCTIONALITY
        //~ if(state & GDK_WINDOW_STATE_FULLSCREEN) {	// FULLSCREEN
            //~ gtk_window_unfullscreen(GTK_WINDOW(widget));
            //~ break;
        //~ }
        gtk_window_maximize(GTK_WINDOW(widget));	// NORMAL
        break;

    // SELECT TRACKS ROWS
    case GDK_KEY_x: // CTRL + x  CLEAR SELECTED ROWS FROM SELECT TRACKS TREEVIEW
    case GDK_KEY_X: // CTRL + SHIFT+ X   CLEAR ALL ROWS FROM SELECT TRACKS TREEVIEW

        // REMOVE ALL THE ROWS IN THE SELECT TRACKS TREEVIEW
        // CTRL + SHIFT + X
        if((event->state & modifiers) == (GDK_CONTROL_MASK | GDK_MOD1_MASK))
            clearAllRows_signal(NULL, dataviewVars);

        // REMOVE ONLY SELECTED ROWS IN SELECT TRACKS TREEVIEW
        // CTRL + x
        if((event->state & modifiers) ==GDK_CONTROL_MASK)
            removeSelectedRows_signal(NULL, dataviewVars);
        break;

    //CHANGE PANE MOVEMENT TO A PERCENT OF WINDOW WIDTH
    case GDK_KEY_Left:  // CTRL + Left  MOVE PANE HANDLE LEFT
        if(panePos < movePane)
            movePane = panePos;

        if((event->state & modifiers) == GDK_CONTROL_MASK)
            gtk_paned_set_position (dataviewVars->w_dataview_Pane, panePos - movePane);

        break;

    case GDK_KEY_Right:  // CTRL + Right  MOVE PANE HANDLE RIGHT
        if((event->state & modifiers) == GDK_CONTROL_MASK)
            gtk_paned_set_position (dataviewVars->w_dataview_Pane, panePos + movePane);

        break;

    case GDK_KEY_Escape:  // EXIT / QUIT
        if(gtk_widget_get_visible (dataviewVars->w_scrollHelp)) {
            helpMe_signal(NULL, dataviewVars);
            break;
        }

		gtk_main_quit();
        break;

    case GDK_KEY_q:  // CTRL + Q  EXIT / QUIT
    case GDK_KEY_Q: // CTRL + SHIFT + Q  CLEAR QUEUE
        if((event->state & modifiers) == GDK_CONTROL_MASK) {   // QUIT / EXIT
            gtk_main_quit();
            break;
        }

        //	GDK_CONTROL_MASK + GDK_SHIFT_MASK +Q -CLEAR THE PLAYER QUE
        if((event->state & modifiers) == (GDK_CONTROL_MASK | GDK_SHIFT_MASK))  // CLEAR THE PLAY QUEUE
            clearPlaylist_signal(NULL, dataviewVars);
        break;

    case GDK_KEY_p:  // TOGGLE PLAY / PAUSE MPD PLAYER
    case GDK_KEY_P:
        // CTRL + p  STOP THE MPD PLAYER (TOGGLE STOP / PLAY)
        if((event->state & modifiers) == GDK_CONTROL_MASK) {
            stopPlayer(NULL, dataviewVars);
			break;
		}
		if((event->state & modifiers) == (GDK_CONTROL_MASK | GDK_SHIFT_MASK)) {
            stopPlayer(NULL, dataviewVars);
            break;
        }
        // p OR  P   TOGGLE PLAY / PAUSE MPD PLAYER
        startPlayer(NULL, dataviewVars);
        break;


    // case GDK_KEY_Tab: // SWITCH BETEEN THE PANES DATAVIEW AND TRACK VIEW
		// DON'T PUT ANYTHING HERE. THE KEY/MOUSE EVENTS  ARE SET IN THE GLADE UI FILE
		// PRESSING THE TAB KEY WILL AUTOMATICALLY CAUSE THE TAB TO ALTERNATE
		// BETWEEN THE DATABASE AND SELECT TRACKS TREEVIEWS
        // break;

    case GDK_KEY_m:  // POPUP MENUS
    case GDK_KEY_M:  // POPUP MENUS
        // IF CTRL + M
        if((event->state & modifiers) == GDK_CONTROL_MASK)
        {
            // IF SELECT TRACK TREEVIEW IS FOCUS
            if(gtk_widget_is_focus (GTK_WIDGET(dataviewVars->w_selTrkTreeView)))
            {
                selTrack_popupMenu( NULL, dataviewVars);
                break;
            }
            // IF DATABASE TREEVIEW IS FOCUS
            if(gtk_widget_is_focus (GTK_WIDGET(dataviewVars->w_dataviewTreeView)))
                directory_popupMenu( NULL, dataviewVars);
            break;
        }

        // OTERWISE THE MAIN POPUP MENU
        window_main_popup_menu(widget, NULL, dataviewVars);
        break;

    // ADD TRACKS TO SELECT TRACKS TREEVIEW
    case GDK_KEY_a:  // IN DATABASE TREEVIEW ADD DIRECTORY SELECTED ROW TO SELECT TRACKS
    case GDK_KEY_A:  // IN SELECT TRACKS TREEVIEW ADD SELECTED ROWS TO MPD PLAY QUEUE

		// IF FOCUS IS THE DATABASE PANE
        if(true == gtk_widget_is_focus (GTK_WIDGET(dataviewVars->w_dataviewTreeView)))
        {
            if((event->state & modifiers) == GDK_CONTROL_MASK)
			{
                addDirToMpdQue( NULL, dataviewVars);
                break;
            }
			else
			{
				addDirToSelTrk(NULL, dataviewVars);
				gtk_widget_grab_focus (GTK_WIDGET(dataviewVars->w_dataviewTreeView));
				gtk_widget_grab_default (GTK_WIDGET(dataviewVars->w_dataviewTreeView));
				break;
			}
        }

		// IF FOCUS IS THE SELECT TRACK PANE
        if(gtk_widget_is_focus (GTK_WIDGET(dataviewVars->w_selTrkTreeView)))
        {
            if((event->state & modifiers) == GDK_CONTROL_MASK)
			{
				selTrk_addTracks(NULL, dataviewVars);
				break;
			}
        }
        break;

    // COLLAPSE ROWS IN THE TREEVIEW
    case GDK_KEY_c: // COLLASPE ROW SELECTED IN TREEVIEW AND SELECT TRACKS
    case GDK_KEY_C: // CTRL-C - COLLASPE ALL ROWS IN TREEVIEW AND SELECT TRACKS

        // C - COLLASPE SELECTED TOP DIRECTORY ROW
        if(gtk_widget_is_focus (GTK_WIDGET(dataviewVars->w_dataviewTreeView)))
            collapseSelectedRow(NULL, dataviewVars->w_dataviewTreeView);

        if(gtk_widget_is_focus (GTK_WIDGET(dataviewVars->w_selTrkTreeView)))
            collapseSelectedRow(NULL, dataviewVars->w_selTrkTreeView);

        // IF CTRL+C -  COLLASPE ALL ROWS
        if((event->state & modifiers) == GDK_CONTROL_MASK)
        {
            if(gtk_widget_is_focus (GTK_WIDGET(dataviewVars->w_dataviewTreeView)))
                collapseAllRows(NULL, dataviewVars->w_dataviewTreeView);

            if(gtk_widget_is_focus (GTK_WIDGET(dataviewVars->w_selTrkTreeView)))
                collapseAllRows(NULL, dataviewVars->w_selTrkTreeView);
        }
        break;

    case GDK_KEY_e:  // EXPAND ROW
    case GDK_KEY_E:  // CTRL+E - EXPAND ALL ROWS

        // EXPAND SELECTED ROW
        if(gtk_widget_is_focus (GTK_WIDGET(dataviewVars->w_dataviewTreeView)))
            expandSelectedRow(NULL, dataviewVars->w_dataviewTreeView);

        if(gtk_widget_is_focus (GTK_WIDGET(dataviewVars->w_selTrkTreeView)))
            expandSelectedRow(NULL, dataviewVars->w_selTrkTreeView);

        // IF CTRL+E  -  EXPAND ALL ROWS
        if((event->state & modifiers) == GDK_CONTROL_MASK)
        {
            if(gtk_widget_is_focus (GTK_WIDGET(dataviewVars->w_dataviewTreeView)))
                expandAllRows(NULL, dataviewVars->w_dataviewTreeView);

            if(gtk_widget_is_focus (GTK_WIDGET(dataviewVars->w_selTrkTreeView)))
                expandAllRows(NULL, dataviewVars->w_selTrkTreeView);
        }
        break;

    }

    // IF PATH WAS USED THEN FREE IT
    if(path)	gtk_tree_path_free(path);
    // printf("End of -- File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    return FALSE;
} // *** End of window_key_press_event *** //

gboolean showViewTimer(app_variables *dataviewVars)
{   //  linenum;

    // FIRST ORDER OF BUSINESS -- UPDATE THE MPD STATUS
    struct mpd_connection *conn;
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);
    struct mpd_status * status;
    mpd_send_status(conn);
    status = mpd_recv_status(conn);

    if (status == NULL) {
        gchar *tmpStr = g_strdup_printf("MPD STATUS ERR: %s\n", mpd_status_get_error(status));
        g_free(tmpStr);
        mpd_status_free (status);
        return TRUE;
    }

    dataviewVars->queueLengthStatus 	= mpd_status_get_queue_length (status);
    dataviewVars->mpdPlayerStateStatus = mpd_status_get_state (status);

    mpd_status_free(status);
    mpd_connection_free(conn);

    // SET THE MPD PLAYER STATUS BACKGROUND COLOR.
    set_mpd_status_color(dataviewVars);
    // End of SET THE MPD PLAYER STATUS BACKGROUND COLOR.

    // ***** IF THIS IS THE INITIAL START UP OF THE PROGRAM, THEN WE NEED TO UN-HIDE THE MAIN WINDOW AND BOX_0
    if(dataviewVars->startUpFirstPass)
    {
        gtk_widget_show(GTK_WIDGET(dataviewVars->w_box_0));
        gtk_widget_show(dataviewVars->w_window_main);
        dataviewVars->startUpFirstPass = 0;
    }

    dataView (dataviewVars);

    return TRUE;
} // *** End of showViewTimer ***

// ***** CATCH SIGNAL HANDLER FUNCTION *****
void signal_callback_sigFault(int signum)
{   //  linenum;

    char timeBuff[70];
    time_t current_timeNOW = time(NULL);
    struct tm *tmNOW = localtime(&current_timeNOW);
    strftime(timeBuff, sizeof timeBuff, "%A %b/%d/%Y %I:%M:%S %p", tmNOW);

    void *array[10];
    gchar **strings;
    int size, i;
    size = backtrace (array, 10);
    strings = backtrace_symbols (array, size);
    gchar *msg = NULL;

    GError *error = NULL;

    if (strings != NULL)
    {
        msg = g_strdup_printf("Backtrace -- Obtained %d stack frames.\n", size);

        for (i = 0; i < size; i++)
            msg = g_strdup_printf ("%s%d -- \t%s\n", msg, i + 1, strings[i]);
    }

    msg = g_strdup_printf("%s -- Trapped signal -- %d -- %s --\n%s", timeBuff, signum, g_strsignal(signum),msg);
    g_fprintf(stderr, "------- SIGNAL FAULT --------\n");
    g_fprintf(stderr, "-- DEBUG MESSAGE TO STDERR -- \n");
    g_fprintf(stderr, "%s\n", msg);
    g_fprintf(stderr, "-----------------------------------------------------------------------------------\n");
    // NOTE: WRITING ERR MSG TO FILE IS ONE-SHOT TO /tmp. THE NEXT FAULT WILL OVERWRITE IT
    g_file_set_contents (globalSigFaultFile, msg, -1, &error);
    // IF THERE IS AN ERROR WRITING TO TEMP FILE,
    // THERS NOT MUCH WE CAN DO ABOUT A FILE ERR AT THIS POINT.
    // JUST EXIT AND HOPE STDERR IS ENOUGH
    if(error) {
        g_fprintf(stderr, "ERROR %s\n", error->message);
        g_free(error);
    }

    g_free (strings);
    g_free(msg);

    exit(0);
}// ** End of signal_callback_handler ***

// WRITE ERROR MESSAGES TO THE DEBUG FILE IN TMP DIRECTORY FOR REFERENCE.
// NOTE:CONTENTS OF THE DEBUG FILE WILL BE LOST WITH A REBOOT
int printDebugMsg(gchar *debugMsg, gint line, gchar *file, const gchar *func,  app_variables *dataviewVars)
{   //  linenum;
    g_autofree gchar *dataviewDebugFile = g_strdup_printf("%s/%s.debug",g_get_tmp_dir (), g_get_prgname ());
    printf("Line %d -- dataviewDebugFile %s\n", __LINE__,  dataviewDebugFile);
    char timeBuff[70];
    time_t current_timeNOW = time(NULL);
    struct tm *tmNOW = localtime(&current_timeNOW);
    strftime(timeBuff, sizeof timeBuff, "\t%m/%d/%Y %I:%M:%S %p", tmNOW);
    g_autoptr(GError) error = NULL;
    GString *myMsg = g_string_new (NULL);
    g_autofree gchar *contents = NULL;

    g_string_append_printf(myMsg, "\n--------------- START ---------------\n");
    g_string_append_printf(myMsg, "%s\n\n", timeBuff);
    g_string_append_printf(myMsg, "%s\n\n", debugMsg);
    g_string_append_printf(myMsg, "\tLine %d\n", line);
    g_string_append_printf(myMsg, "\tFile %s\n", file);
    g_string_append_printf(myMsg, "\tFunction %s\n\n", func);
    g_string_append_printf(myMsg, "%s/%s -- Compiled %s -- %s\n\n", dataviewVars->cwdPath, g_get_prgname (), __DATE__, __TIME__);
    g_string_append_printf(myMsg, "-- Debug message written to file -- %s\n", dataviewDebugFile);
    g_string_append_printf(myMsg, "--------------- END ---------------\n");

    g_fprintf(stderr,"%s\n\n", myMsg->str);

    // GET CONTENTS OF THE EXISTING DEBUG FILE, IF IT EXIST,
    gint result = g_file_get_contents (dataviewDebugFile, &contents, NULL, NULL);
    // AND APPEND IT TO THE END OF THE MSG STRING SO THAT THE LATEST WILL BE AT THE VERY TOP OF THE FILE
    if(result)
        g_string_append_printf(myMsg, "\n%s\n", contents);

    // THEN WRITE THE STRING BACK TO DISK, OVERWRITING THE EXISING CONTENTS.
    g_file_set_contents (dataviewDebugFile, myMsg->str, -1, &error);

    // IF THERE IS AN ERROR WRITING TO TEMP FILE,
    if(error)
        g_fprintf(stderr, "ERROR writing to debug file %s -- %s\n", dataviewDebugFile, error->message);

    g_string_free(myMsg, TRUE);

    return 0;
}

// ********** GET A MPD CONNECTON ********
struct mpd_connection *get_connection(app_variables *dataviewVars, gchar *fileName, const gchar *funcName, int lineNum)
{   // linenum;

    struct mpd_connection *conn;
    conn = mpd_connection_new(dataviewVars->mpdHostname,  dataviewVars->mpdPortnumber, dataviewVars->mpdTimeout);
    gint atLine = __LINE__ - 1;
    int errNumber  = mpd_connection_get_error(conn);
    // IF CONNECTION SUCESSFUL, RETURN CONNECTION
    if(errNumber == MPD_ERROR_SUCCESS)
        return conn;

    // OTHERWISE FATAL ERROR, GET THE ERROR CODE
    g_autofree gchar *mpdErrMsg = g_strdup(mpd_connection_get_error_message(conn));
    // DON'T BOTHER WITH TRYING TO RECOVER
    mpd_connection_clear_error(conn);

	showErrMsgDialog (mpdErrMsg, atLine, __FILE__, __func__, dataviewVars);

    gtk_main_quit();
    exit(0);
}

void setSignals(app_variables *dataviewVars)
{   //  linenum;

    // DATAVIEW SIGNALS
    g_signal_connect(dataviewVars->w_selTrkTreeView, "button-press-event", (GCallback) dataview_selTrk_onButtonPressed, dataviewVars );

    g_signal_connect(dataviewVars->w_dataviewTreeView, "button-press-event", (GCallback) dataview_directory_onButtonPressed, dataviewVars );

    g_signal_connect(dataviewVars->w_window_main, "button-press-event", (GCallback) main_popup_menu_event_signal, dataviewVars );

    g_signal_connect(dataviewVars->w_dataviewTreeView, "row-activated", (GCallback) dataview_onRowActivated, dataviewVars);

    g_signal_connect(dataviewVars->w_selTrkTreeView, "row-activated", (GCallback) selTrack_onRowActivated, dataviewVars);

    g_signal_connect(dataviewVars->w_window_main, "key-press-event", (GCallback) window_key_press_event, dataviewVars );

    g_signal_connect(dataviewVars->w_selTrkTreeView, "grab-focus", (GCallback) selTrkGrab_signal, dataviewVars );

    g_signal_connect(dataviewVars->w_dataviewTreeView, "grab-focus", (GCallback) dataBaseGrab_signal, dataviewVars );

} //End of setSignals

void dragANDdrop(app_variables *dataviewVars)
{   //  linenum;
    //***** DRAG-N-DROP FROM DATABASE TO SELECT TRACKS
    // REFERENCE  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!
    // https://en.wikibooks.org/wiki/GTK%2B_By_Example/Tree_View/DnD
    // https://wiki.gnome.org/Newcomers/OldDragNDropTutorial
    // REFERENCE  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!

    //* SET DATABASE TREEVIEW AS THE SOURCE OF THE DRAG-N-DROP OPERATION
    static guint n_targets = 1;

    gtk_drag_source_set(GTK_WIDGET(dataviewVars->w_dataviewTreeView),GDK_BUTTON1_MASK, &drag_targets,n_targets, GDK_ACTION_COPY);

    //* ATTACH A "drag-data-get" SIGNAL TO SEND OUT THE DRAGGED DATA
    g_signal_connect(dataviewVars->w_dataviewTreeView,"drag-data-get", G_CALLBACK(on_drag_data_get),NULL);

    //* SET SELECT TRACKS TREEVIEW AS THE DESTINATION OF THE DRAG-N-DROP OPERATION
    gtk_drag_dest_set(GTK_WIDGET(dataviewVars->w_selTrkTreeView),GTK_DEST_DEFAULT_ALL,&drag_targets,n_targets,
                      GDK_ACTION_COPY /*|GDK_ACTION_MOVE*/);

    //* ATTACH A "drag-data-received" SIGNAL TO PULL IN THE DRAGGED DATA
    g_signal_connect(dataviewVars->w_selTrkTreeView,"drag-data-received", G_CALLBACK(on_drag_data_received),dataviewVars);
    //***** End of DRAG-N-DROP FROM DATABASE TO SELECT TRACKS
}


// THIS SETS THE MPD PLAYER STATUS BACKGROUND COLOR
int set_mpd_status_color(app_variables *dataviewVars)
{   //  linenum;
    // SET WINDOW BACKGROUND TO GRAY IF NO TRACKS IN THE PLAYER QUEUE.
    static gint save_mpd_state = MPD_STATE_UNKNOWN;
    gint playerState = dataviewVars->mpdPlayerStateStatus;

    if(dataviewVars->queueLengthStatus == 0)
    {
        save_mpd_state = MPD_STATE_UNKNOWN;
        gtk_widget_set_name (dataviewVars->w_window_main, "CSS_window_main-NOTRACKS");

        return 0;
    }

    // HAS THE MPD STATE CHANGED (play - pause - stop) THEN SET BACKGROUND COLOR
    if(playerState != save_mpd_state)
        save_mpd_state = playerState;
    else return 0;

    switch (dataviewVars->mpdPlayerStateStatus) {
    // GREEN IF PLAYER PLAY MODE,
    case MPD_STATE_PLAY:
        gtk_widget_set_name (dataviewVars->w_window_main, "CSS_window_main-PLAY");
        //~ gtk_widget_show (GTK_WIDGET (dataviewVars->w_btnPause));
        //~ gtk_widget_hide (GTK_WIDGET (dataviewVars->w_btnPlay));

        break;
    // BLUE IF PLAYER PAUSE MODE
    case MPD_STATE_PAUSE:
        gtk_widget_set_name (dataviewVars->w_window_main, "CSS_window_main-PAUSE");

        break;
    // RED IF PLAYER STOP MODE,
    case MPD_STATE_STOP:
        gtk_widget_set_name (dataviewVars->w_window_main, "CSS_window_main-STOP");

        break;
    case MPD_STATE_UNKNOWN:
        break;
    default:
        break;
    }

    // printf("End of -- %s -- Line %d --\n\n", __func__, __LINE__);
    return 0;
} // ***** End of set_mpd_status_color *****

// RIGHT CLICK POPUP MENU CALLBACKS
void window_main_popup_menu (GtkWidget *window_main, GdkEventButton *event, app_variables *dataviewVars)
{   //linenum;

    GtkWidget *popupMenu = gtk_menu_new();
//////////////////////////////////////////////////////////////////////////////////////////////////
	GtkWidget *titleMi = gtk_menu_item_new_with_label("Main Popup Menu");
	gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), titleMi);
	gtk_widget_set_name (titleMi, "CSS_titleMi");

//////////////////////////////////////////////////////////////////////////////////////////////////
    // ***** GET MPD CONNECTION *****
    struct mpd_connection *conn;
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);
    struct mpd_status * status;
    status = mpd_run_status(conn);

    //	******************************
    // UPDATE DATABASE MENU ITEM
    GtkWidget *updateDbMi = gtk_menu_item_new_with_label("Update Database");
    g_signal_connect(updateDbMi, "activate", (GCallback) upDateDatabase, dataviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), updateDbMi);

    // PLAYER START / PAUSE MENU ITEM
    if((mpd_status_get_state(status) == MPD_STATE_PAUSE) |  (mpd_status_get_state(status) == MPD_STATE_STOP))
    {
        GtkWidget *startPlayerMi = gtk_menu_item_new_with_label("Start Player");
        g_signal_connect(startPlayerMi, "activate", (GCallback) startPlayer, dataviewVars);
        gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), startPlayerMi);

        // IF PLAY QUEUE IS EMPTY (GRAY BACKGROUND), GREY OUT THE MENU ITEM
        if(dataviewVars->queueLengthStatus == 0)
            gtk_widget_set_sensitive (startPlayerMi, FALSE);
    }
    else
    {
        GtkWidget *pausePlayerMi = gtk_menu_item_new_with_label("Pause Player");
        g_signal_connect(pausePlayerMi, "activate", (GCallback) pausePlayer, dataviewVars);
        gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), pausePlayerMi);

        // IF PLAY QUEUE IS EMPTY (GRAY BACKGROUND), GREY OUT THE MENU ITEM
        if(dataviewVars->queueLengthStatus == 0)
            gtk_widget_set_sensitive (pausePlayerMi, FALSE);
    }

    // PLAYER STOP MENU ITEM
    GtkWidget *stopPlayerMi = gtk_menu_item_new_with_label("Stop Player");
    g_signal_connect(stopPlayerMi, "activate", (GCallback) stopPlayer, dataviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), stopPlayerMi);

    // IF PLAY QUEUE IS EMPTY (GRAY BACKGROUND), GREY OUT THE MENU ITEM
    if(dataviewVars->queueLengthStatus == 0)
        gtk_widget_set_sensitive (stopPlayerMi, FALSE);

    // ADD SEPERATOR
    GtkWidget *separator1Mi = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), separator1Mi);

    // ***** AUDIO OUTPUT MENU ITEM STARTS HERE ***** //
    GtkWidget *outputMenu = gtk_menu_new();
    GtkWidget *audioOutputMi = gtk_menu_item_new_with_label("Audio Output");
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), audioOutputMi);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(audioOutputMi), outputMenu);

    mpd_send_outputs(conn);
    struct mpd_output *output;

    // RECURSIVELY ADD AUDIO OPTIONS TO THE AUDIO SUBMENU
    while ((output = mpd_recv_output(conn)) != NULL) {

        audioOutputMi = gtk_check_menu_item_new_with_label((gchar *) mpd_output_get_name(output));
        gtk_menu_shell_append(GTK_MENU_SHELL(outputMenu), audioOutputMi);

        if(mpd_output_get_enabled(output))
            gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(audioOutputMi), TRUE);
        else  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(audioOutputMi), FALSE);

        mpd_output_free(output);
        g_signal_connect(audioOutputMi, "activate", (GCallback) toggle_mpd_output, dataviewVars)	;
    }
    // ***** AUDIO OUTPUT MENU ITEM ENDS HERE ***** //

    // ***** FREE MPD STATUS AND CONNECTION *****
    mpd_response_finish(conn);
    mpd_connection_free(conn);

    // ADD SEPERATOR
    GtkWidget *separator2Mi = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), separator2Mi);

    GtkWidget *helpMi = gtk_menu_item_new_with_label("Help");
    g_signal_connect(helpMi, "activate", (GCallback) helpMe_signal, dataviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), helpMi);

    // CLEAR THE EXISTING PLAYLIST QUEUE MENU ITEM
    GtkWidget *clearQueueMi = gtk_menu_item_new_with_label("Clear Queue Playlist");
    g_signal_connect(clearQueueMi,  "activate",     (GCallback)   clearPlaylist_signal,  dataviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), clearQueueMi);

    // IF PLAY QUEUE IS EMPTY (GRAY BACKGROUND), GREY OUT THE MENU ITEM
    if(dataviewVars->queueLengthStatus == 0)
        gtk_widget_set_sensitive (clearQueueMi, FALSE);

    // ***** EXIT MAIN WINDOW MENU ITEM *****
    GtkWidget *exitMi = gtk_menu_item_new_with_label("Quit (Exit)");
    g_signal_connect(exitMi, "activate", (GCallback) gtk_main_quit, window_main);
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), exitMi);

    // ***** NAME THE NODES ***** /
    gtk_widget_set_name (popupMenu, "CSS_popupMenu");
    gtk_widget_set_name (outputMenu, "CSS_outputMenu");

    gtk_widget_show_all(popupMenu);

    // PUT THE POPUP MENU IN THE CENTER
    gtk_menu_popup_at_widget (GTK_MENU(popupMenu), GTK_WIDGET(dataviewVars->w_window_main),
                              GDK_GRAVITY_CENTER, GDK_GRAVITY_CENTER,  NULL);

    //printf("End of File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
} // ** End of window_main_popup_menu

void toggle_mpd_output(GtkWidget *menuItem, app_variables *dataviewVars)
{   // linenum;
    struct mpd_connection *conn;
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);
    mpd_send_outputs(conn);
    struct mpd_output *output;
    while ((output = mpd_recv_output(conn)) != NULL)
    {
        if(0 == g_strcmp0 (mpd_output_get_name(output), gtk_menu_item_get_label (GTK_MENU_ITEM(menuItem))))
        {
            mpd_response_finish(conn);
            mpd_run_toggle_output 	(conn, mpd_output_get_id(output));
        }
        mpd_output_free(output);
    }
    mpd_connection_free(conn);
}

void clearPlaylist_signal	(GtkWidget *clearPlaylistMi, app_variables *dataviewVars)
{   //linenum;
    // SETUP MPD CONNECTION
    struct mpd_connection *conn;
    // GET MPD CONNECTION
    conn = get_connection(dataviewVars, __FILE__, __func__, __LINE__);
    mpd_send_clear(conn);
    mpd_connection_free(conn);
    // printf("End of File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
}

// RIGHT CLICK MOUSE BUTTON    --   CLICK - BUTTON-PRESS-EVENT
gboolean main_popup_menu_event_signal (GtkWidget *window_main, GdkEventButton *event, app_variables *dataviewVars)
{   // linenum;
    /* SINGLE CLICK WITH THE RIGHT MOUSE BUTTON- POPUP MENU */
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3) // right click
    {
        window_main_popup_menu(window_main, event, dataviewVars);
    }

    return TRUE;
}

void helpMe_signal(GtkWidget *helpBtn, app_variables *dataviewVars)
{   // linenum;
// REFERENCE	//https://cpp.hotexamples.com/examples/-/-/g_resources_open_stream/cpp-g_resources_open_stream-function-examples.html#0xe9b10f54a80d83bcee51701b634984ffd1970ffa2d77dc22c1a56c8b1c7f3bbd-534,,564,

    GInputStream * helpStream = NULL;
    g_autoptr(GError) error = NULL;
    gsize bytes_read;
    gsize helpStringLength;
    gint atLine = 0;

    g_resources_get_info("/dataview/dataview.help", G_RESOURCE_LOOKUP_FLAGS_NONE, &helpStringLength, NULL, &error);
    atLine = __LINE__ - 1;
    //  helpTxtString VARIABLE HAS TO BE DECLARED HERE
    gchar helpTxtString[helpStringLength];

    if(error)  goto failed;

    helpStream = g_resources_open_stream ("/dataview/dataview.help", G_RESOURCE_LOOKUP_FLAGS_NONE, &error);
    atLine = __LINE__ - 1;
    if(error)  goto failed;

    g_input_stream_read_all (helpStream, helpTxtString, sizeof (helpTxtString), &bytes_read, NULL, &error);
    atLine = __LINE__ - 1;
    if(error)  goto failed;

    g_object_unref (helpStream);

    g_autofree gchar *txtStr = NULL;
    txtStr =  g_strndup (helpTxtString, bytes_read);
    gtk_label_set_markup (GTK_LABEL (dataviewVars->w_lblHelp), txtStr);

    if(gtk_widget_get_visible (dataviewVars->w_scrollHelp)) {
        gtk_widget_hide(dataviewVars->w_scrollHelp);
        gtk_widget_show(GTK_WIDGET(dataviewVars->w_dataview_Pane));
    }
    else  {
        gtk_widget_show(dataviewVars->w_scrollHelp);
        gtk_widget_hide(GTK_WIDGET(dataviewVars->w_dataview_Pane));
    }
    return;

    // IF IT FAILED COME TO HERE
failed:
		showErrMsgDialog (error->message, atLine, __FILE__, __func__, dataviewVars);
    //~ g_error_free(error);
    exit(0);
} // End of helpMe_signal

// DRAG-n_DROP GET
// SAVE!!   SAVE!!   SAVE!!   SAVE!!   SAVE!!   SAVE!!   SAVE!!   SAVE!!
//* USER CALLBACK FOR GETING THE DATA FROM THE ROW THAT WAS DRAGGED 'N' DROPPED
// REFERENCE  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!  SAVE!!
// https://en.wikibooks.org/wiki/GTK%2B_By_Example/Tree_View/DnD
// https://wiki.gnome.org/Newcomers/OldDragNDropTutorial
void on_drag_data_get(	GtkWidget *widget, GdkDragContext *drag_context,
                        GtkSelectionData *sdata, guint info, guint time, app_variables *dataviewVars)
{   // linenum;

    GtkTreeIter iter;
    GtkTreeModel *tree_store;
    GtkTreeSelection *selector;
    g_autofree gchar *value = NULL;

    //* GET THE SELECTOR WIDGET FROM THE TREEVIEW IN QUESTION */
    selector = gtk_tree_view_get_selection(GTK_TREE_VIEW(widget));

    //* GET THE TREE MODEL (TREE_STORE) AND INITIALISE THE ITERATOR */
    gtk_tree_selection_get_selected(selector,&tree_store,&iter);

    //* GET THE VALUE OF A PARTICULAR COLUMN FROM THE ROW, THE ITERATOR CURRENTLY POINTS TO*/
    gtk_tree_model_get (tree_store, &iter, COL_DBPATH, &value, -1);

    //* SEND THE DATA OFF INTO THE GTKSELECTIONDATA OBJECT */
    gtk_selection_data_set(sdata, gdk_atom_intern ("struct DATA pointer", FALSE), 8, (void *)&value, sizeof (value));
}

// DRAG 'N'DROP RECEIVED
void on_drag_data_received(GtkWidget *widget, GdkDragContext *drag_context,
                           gint x, gint y, GtkSelectionData *sdata, guint info, guint time, app_variables *dataviewVars)
{   // linenum;

    addDirToSelTrk(NULL, dataviewVars);
    // DO I NEED TO FREE SOMETHING HERE
}

// GET APP VERSION
gchar * versionString(void)
{   // linenum;

    int  year, month, day, doy, woy, hour, minute, second;

    sscanf(VERSION_STR, "%u.%u.%u.%u.%u.%u.%u.%u", &year, &month, &day, &doy, &woy, &hour, &minute, &second);
    return g_strdup_printf("Version: %d.%02d.%03d.%05d -- Compiled: %02d:%02d:%d %02d:%02d:%02d", year, doy, woy, ((hour * 3600)  + (minute * 60) +  second ), month, day, year, hour, minute, second);
}

// GET COMMAND LINE OPTIONS
void getTheOptions(int argc, gchar **argv, app_variables *dataviewVars)
{   //  linenum;

    // NEED - AT START OF optstring TO CAPTURE MESSAGE FROM COMMAND LINE WITHOUT --msg OR -m OPTION
    static const char optstring[] = "-2:3:4:5:6:7:8:";
    static const struct option options_long[] = {
        /* name		has_arg			flag	val */

        { "host",		required_argument,		NULL,	'2'},
        { "port",		required_argument,		NULL,	'3' },
        { "timeout",	required_argument,		NULL,	'4' },
        { "width",	required_argument,		NULL,	'5' },
        { "height",	required_argument,		NULL,	'6' },
        { "font", 		required_argument,		NULL,	'7' },
        { "title", 		required_argument,		NULL,	'8'},

        { 0, 0, 0, 0 }
    };

    // SAVE ARGV PRINT!!!
    if(globalVerbose)
    {
        printf("\nLine %d -- ARGC = %d\n",__LINE__,  argc);
        printf("Line %d -- Command line = ", __LINE__);
        int x = 0;
        while(argv[x] != NULL)
        {
            printf("%s ", argv[x]);
            x++;
        }
        printf("\n");
    }
    // SAVE ARGV PRINT!!!

    // SAVE PRINT!!!  SAVE PRINT!!!  SAVE PRINT!!!  SAVE PRINT!!!  SAVE PRINT!!!
    //~ printf("\nLine %d -- THIS IS BEFORE READING OPTIONS\n", __LINE__);
    //~ printf("Line %d -- dataviewVars->mpdHostname = %s\n", __LINE__, dataviewVars->mpdHostname);
    //~ printf("Line %d -- dataviewVars->mpdPortnumber = %d\n", __LINE__, dataviewVars->mpdPortnumber);
    //~ printf("Line %d -- dataviewVars->mpdTimeout = %d\n", __LINE__, dataviewVars->mpdTimeout);
    //~ printf("Line %d -- dataviewVars->winWidth = %d\n", __LINE__, dataviewVars->winWidth);
    //~ printf("Line %d -- dataviewVars->winHeight = %d\n", __LINE__, dataviewVars->winHeight);
    //~ printf("Line %d -- dataviewVars->dialogFont = %s\n", __LINE__, dataviewVars->dialogFont);
    //~ printf("Line %d -- dataviewVars->windowTitle = %s\n\n", __LINE__, dataviewVars->windowTitle);
    // SAVE PRINT!!!  SAVE PRINT!!!  SAVE PRINT!!!  SAVE PRINT!!!  SAVE PRINT!!!

    // PROCESS THE COMMAND LINE ARGUMENTS
    int loop = 0;
    //~ printf("Line %d -- loop = %d\n", __LINE__,  loop);
    //~ printf("Line %d -- optstring %s\n", __LINE__, optstring);

    while ((loop = getopt_long(argc, argv, optstring, options_long, NULL))) {

        switch (loop) {  // END OF OPTIONS
        case -1:	// DONE
            //~ printf("\nLine %d -- = %d END OF OPTIONS \n", __LINE__, loop);

            return;
            break;

        case 1:
            //~ printf("Line %d -- CASE 1 = %s\n", __LINE__, optarg);
            break;

        case '2':	// HOSTNAME
            //~ printf("Line %d -- HOST\n", __LINE__);

            dataviewVars->mpdHostname = g_strdup(optarg);;
		//~ printf("HOST %s\n", dataviewVars->mpdHostname);
            break;

        case '3':	// PORT NUMBER
            //~ printf("Line %d -- PORT\n", __LINE__);
            dataviewVars->mpdPortnumber = (int) g_ascii_strtoll(optarg, NULL, 10);
            break;

        case '4':	 // TIMEOUT
            //~ printf("Line %d -- TIMEOUT\n", __LINE__);

            dataviewVars->mpdTimeout = (int) g_ascii_strtoll(optarg, NULL, 10);
            break;

        case '5':	// WIDTH
            //~ printf("Line %d -- WIDTH\n", __LINE__);

            dataviewVars->winWidth = (int) g_ascii_strtoll(optarg, NULL, 10);
            break;

        case '6':	// HEIGHT
            //~ printf("Line %d -- HEIGHT\n", __LINE__);

            dataviewVars->winHeight = (int) g_ascii_strtoll(optarg, NULL, 10);
            break;

        case '7':	// FONT
            //~ printf("Line %d -- FONT\n", __LINE__);

            dataviewVars->dialogFont = g_strdup(optarg); // DEFAULT DIALOG FONT
            break;

        case '8':	// TITLE
            //~printf("Line %d -- TITLE\n", __LINE__);

            dataviewVars->windowTitle = g_strdup(optarg);
            break;
        }
    }

} //End of getTheOptions

// LOAD THE KEYS FROM THE CONFIG FILE
void getConfigKeys(app_variables *dataviewVars)
{   // 	linenum;

    if(globalVerbose)
    {
        printf("Line %d -- GET CONFIG KEYS\n", __LINE__);
        printf("Line %d -- dataviewVars->mpdHostname = %s\n", __LINE__,dataviewVars->mpdHostname);
        printf("Line %d -- dataviewVars->mpdPortnumber = %d\n", __LINE__,dataviewVars->mpdPortnumber);
        printf("Line %d -- dataviewVars->mpdTimeout = %d\n", __LINE__,dataviewVars->mpdTimeout);
        printf("Line %d -- dataviewVars->winWidth = %d\n", __LINE__,dataviewVars->winWidth);
        printf("Line %d -- dataviewVars->winHeight = %d\n", __LINE__,dataviewVars->winHeight);
        printf("Line %d -- dataviewVars->dialogFont = %s\n", __LINE__,dataviewVars->dialogFont);
        printf("Line %d -- dataviewVars->windowTitle = %s\n\n", __LINE__,dataviewVars->windowTitle);
        printf("Line %d -- dataviewVars->gladeFile = %s\n", __LINE__,dataviewVars->gladeFile);
        printf("Line %d -- dataviewVars->cssFile = %s\n", __LINE__,dataviewVars->cssFile);
    }

    g_autofree gchar *rtn_cssFile = NULL;
    g_autofree gchar *rtn_gladeFile = NULL;

	g_autofree gchar *rtn_dirIconFile = NULL;
	g_autofree gchar *rtn_subDirIconFile = NULL;
	g_autofree gchar *rtn_trkIconFile = NULL;

    g_autofree gchar *rtn_mpdPortnumber = NULL;
    g_autofree gchar *rtn_mpdTimeout = NULL;

    g_autoptr(GError) error = NULL;
    g_autoptr(GKeyFile) keyFile = g_key_file_new ();

    gint atKeyLine;

    // GET THE KEY FILE. this is hard coded to CDW/dataview.conf
    dataviewVars->cfgFile = g_strdup_printf("%s/%s.conf", dataviewVars->cwdPath,  g_get_prgname ());

    // IF THE CONF FILE EXIS IN CWD, USE THE EXTERNAL KEY FILE
    if(g_file_test (dataviewVars->cfgFile, G_FILE_TEST_IS_REGULAR) == TRUE)
    {
        g_key_file_load_from_file(keyFile, dataviewVars->cfgFile, G_KEY_FILE_NONE, &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  goto keyFileError;

        //~ printf("Line %d -- Selected External Key File %s\n\n", __LINE__,  dataviewVars->cfgFile);
        if(globalVerbose)  printf("\nLine %d -- EXTERNAL CONFIG FILE\n", __LINE__);
        if(globalVerbose)  printf("Line %d -- CONFIG FILE %s\n", __LINE__, dataviewVars->cfgFile);
    }
    else  // OTHERWISE USE THE INTERNAL KEY FILE FROM RESOURCES
    {
        g_autoptr(GBytes) bytes = g_resources_lookup_data (CONFIG_RESOURCE, G_RESOURCE_LOOKUP_FLAGS_NONE, &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  goto keyFileError;
        g_key_file_load_from_bytes (keyFile, bytes,G_KEY_FILE_NONE, &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  goto keyFileError;
        //~ printf("Selected Internal Key File %s\n\n", CONFIG_RESOURCE);
        if(globalVerbose)  printf("Line %d -- INTERNAL CONFIG FILE\n", __LINE__);
        if(globalVerbose)  printf("Line %d -- CONFIG FILE %s\n", __LINE__, CONFIG_RESOURCE);
    }

    // ***** SPECIAL KEY ***** //	// SAVE!!   SAVE!!   SAVE!!   SAVE!!   SAVE!!   SAVE!!   SAVE!!   SAVE!!
    // THIS IS FOR DEBUGGING TO DETERMINE WHICH KEY FILE I'M USING
    // THIS KEY WILL NOT FAIL EVEN IF VARIABLE IS MISSING IN THE KEY FILE
    if(globalVerbose)
    {
        g_autofree gchar *whatAmI =  g_key_file_get_string(keyFile,  "Dataview Configuration", "whatAmI", NULL);
        if(whatAmI && (strlen(whatAmI) != 0))
            printf("\nLine %d -- What am I ? %s\n", __LINE__, whatAmI);
    }
    // ***** SPECIAL KEY ***** //	// SAVE!!   SAVE!!   SAVE!!   SAVE!!   SAVE!!   SAVE!!   SAVE!!   SAVE!!

    // MPD HOSTNAME
    if(dataviewVars->mpdHostname == NULL)  // IF NULL THEN THERE WAS NO COMMAND LINE OPTION
    {
        dataviewVars->mpdHostname = g_key_file_get_string(keyFile,  "Dataview Configuration", "mpdHostname", &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  goto keyFileError;
    }

    // MPD PORT NUMBER -IT'S EASIER TO DO IT AS A STRING RATHER THAN INTEGER
    if(dataviewVars->mpdPortnumber < 0) // IF NULL THEN THERE WAS NO COMMAND LINE OPTION
    {
        dataviewVars->mpdPortnumber = g_key_file_get_integer(keyFile, "Dataview Configuration", "mpdPortnumber", &error);
        atKeyLine =  __LINE__ - 1;
        if(error)	goto keyFileError;
    }

    // MPD TIMEOUT -IT'S EASIER TO DO IT AS A STRING RATHER THAN INTEGER
    if(dataviewVars->mpdTimeout < 0) // IF NULL THEN THERE WAS NO COMMAND LINE OPTION
    {
        dataviewVars->mpdTimeout = g_key_file_get_integer(keyFile, "Dataview Configuration", "mpdTimeout", &error);
        atKeyLine =  __LINE__ - 1;
        if(error)	goto keyFileError;
    }

    //**************************************************************************

    // GLADE FILE NAME -- ALLOW THAT THE FILENAME MAY BE ON THE COMMAND LINE
    if(dataviewVars->gladeFile == NULL)   // IF THE VARIABLE IS NULL THEN WE TRY TO READ IT FROM THE KEY FILE
    {
        rtn_gladeFile = g_key_file_get_string(keyFile, "Dataview Configuration", "dataViewGladeFile",   &error);
        atKeyLine =  __LINE__ - 1;

        if(error) 	goto keyFileError; // IF KEY DOES NOT EXIST, THEN FATAL ERROR, EXIT

        // IF FILENAME IS NOT AN ABSOLUTE PATH/FILE OR DOES NOT EXIST, THROW AND ERROR AND EXIT
        if (strlen(rtn_gladeFile) != 0)
        {

            if(globalVerbose)  printf("\nLine %d -- EXTERNAL GLADE FILE\n", __LINE__);

            // IF FILE EXIST THEN ASSIGN IT TO VARIABLE
            dataviewVars->gladeFile = g_strdup(rtn_gladeFile);
        }

        // IF KEY IS EMPTY (ZERO STRING LEN), THEN ASSIGN INTERNAL RESOURCE TO VARIABLE
        else {
            if(globalVerbose)  printf("\nLine %d -- INTERNAL GLADE FILE\n", __LINE__);
            dataviewVars->gladeFile = g_strdup(GLADE_RESOURCE);
        }
    }

    if(globalVerbose)  printf("Line %d -- GLADE FILE %s\n", __LINE__, dataviewVars->gladeFile);
    //printf("Line %d -- dataviewVars->cfgFile %s\n", __LINE__, dataviewVars->cfgFile);

    // CSS FILE NAME
    if(dataviewVars->cssFile == NULL)   // IF THE VARIABLE IS NULL THEN WE TRY TO READ IT FROM THE KEY FILE
    {
        rtn_cssFile = g_key_file_get_string(keyFile, "Dataview Configuration", "dataViewCssFile",   &error);
        atKeyLine =  __LINE__ - 1;

        if(error) 	goto keyFileError; // IF KEY DOES NOT EXIST, THEN FATAL ERROR, EXIT

        // IF FILENAME IS NOT AN ABSOLUTE PATH/FILE OR DOES NOT EXIST, THROW AND ERROR AND EXIT
        if (strlen(rtn_cssFile) != 0)
        {
            // IF FILE EXIST THEN ASSIGN IT TO VARIABLE
            dataviewVars->cssFile = g_strdup(rtn_cssFile);

            if(globalVerbose)  printf("\nLine %d -- EXTERNAL CSS FILE\n", __LINE__);
            if(globalVerbose)  printf("Line %d -- CSS FILE %s\n", __LINE__, dataviewVars->cssFile);
        }

        // IF KEY IS EMPTY (ZERO STRING LEN), THEN ASSIGN INTERNAL RESOURCE TO VARIABLE
        else {
            if(globalVerbose)  printf("\nLine %d -- INTERNAL CSS FILE\n", __LINE__);
            if(globalVerbose)  printf("Line %d -- CSS FILE %s\n", __LINE__, CSS_RESOURCE);
            dataviewVars->cssFile = g_strdup(CSS_RESOURCE);
        }
    }

    // WINDOW TITLE
    if(dataviewVars->windowTitle == NULL)
    {
        dataviewVars->windowTitle = g_key_file_get_string(keyFile, "Dataview Configuration", "dataViewWindowTitle", &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  	goto keyFileError;
    }

    // WINDOW HEIGHT
    if(dataviewVars->winHeight < 0)
    {
        dataviewVars->winHeight = g_key_file_get_integer(keyFile, "Dataview Configuration", "dataViewHeight", &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  	goto keyFileError;
    }

    // WINDOW WIDTH
    if(dataviewVars->winWidth < 0)
    {
        dataviewVars->winWidth = g_key_file_get_integer(keyFile, "Dataview Configuration", "dataViewWidth", &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  	goto keyFileError;
    }

	// ICON SIZE
    dataviewVars->iconSize = g_key_file_get_integer(keyFile, "Dataview Configuration", "dataViewiconSize", &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;

    // NOTIFY DIALOG TIMEOUT
    dataviewVars->notifyDialogTimeout = g_key_file_get_integer(keyFile, "Dataview Configuration", "timeoutNotifyDialog", &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;

    // FONT STRING
    if(dataviewVars->dialogFont == NULL)
    {
        dataviewVars->dialogFont = g_key_file_get_string(keyFile, "Dataview Configuration", "dialogFontString", &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  	goto keyFileError;
    }

    // SET PANED POSITION AT X PERCENT OF WINDOW WIDTH
    dataviewVars->panedPosition = g_key_file_get_integer(keyFile, "Dataview Configuration", "setPanePercent", &error);
    atKeyLine =  __LINE__ - 1;
    if((dataviewVars->panedPosition < 0) || (dataviewVars->panedPosition > 100))
    {
        atKeyLine =  __LINE__ - 2;
        g_set_error (&error, 0, 0, "Key file variable setPanePercent = %d is out of range.\nMust be between 0%% and 100%%\n",\
                     dataviewVars->panedPosition);
    }
    if(error)  	goto keyFileError;

//*****                                      DIR ICON                                ****//
	// IS THE DIRECORY ICON SPECIFED IN THE KEY CONFIG FILE?
	if(globalVerbose)  printf("\nLine %d -- SELECT TRACK TREEVIEW ICONS \n", __LINE__);
		atKeyLine =  __LINE__ + 1;
        rtn_dirIconFile = g_key_file_get_string(keyFile, "Dataview Configuration", "topDirectoryIconFile",   &error);
        		if(error) 	goto keyFileError; // IF KEYWORD ERROR

		if(strlen(rtn_dirIconFile) != 0)	 // DOES THE ICON FILE EXIST?
		{
			if(globalVerbose)  printf("Line %d -- DIR ICON FILE \"%s\"\n", __LINE__, rtn_dirIconFile);

			if(g_file_test (rtn_dirIconFile, G_FILE_TEST_IS_REGULAR) == 0)
			{
				atKeyLine = __LINE__+ 1;
				g_set_error (&error, G_IO_ERROR_NOT_FOUND, 0, "Key file variable -- %s --\nFile\n-- %s --\nNot found.\nLine %d", "topDirectoryIconFile", rtn_dirIconFile, atKeyLine);
				goto keyFileError;
			}

			atKeyLine = __LINE__ + 1;
			dataviewVars->dir_Icon_pixBuf = gdk_pixbuf_new_from_file_at_scale( rtn_dirIconFile, dataviewVars->iconSize, dataviewVars->iconSize, TRUE, &error );
		}

		else	// IF ICON FILE NOT SPECIFIED IN CONFIG FILE, THEN SET  THE RESOURCE ICON IF IT EXIST
		{
			if(g_resources_get_info (TOP_FOLDER_ICON_RESOURCE, G_RESOURCE_LOOKUP_FLAGS_NONE, NULL, NULL, NULL) == TRUE)
			{
				if(globalVerbose)  printf("Line %d -- FOLDER ICON RESOURCE \"%s\"\n", __LINE__, TOP_FOLDER_ICON_RESOURCE);
				atKeyLine = __LINE__ + 1;
				dataviewVars->dir_Icon_pixBuf = gdk_pixbuf_new_from_resource_at_scale (TOP_FOLDER_ICON_RESOURCE, dataviewVars->iconSize, dataviewVars->iconSize, TRUE, &error);
			}
			else	// IF RESOURCE ICON DOES NOT EXIST, THEN SET THE STOCK ICON, IF IT EXIST
			{
				if(globalVerbose)  printf("Line %d -- FOLDER ICON STOCK \"%s\"\n", __LINE__, STOCK_TOP_FOLDER_ICON);
				atKeyLine = __LINE__ + 1;
				dataviewVars->dir_Icon_pixBuf = gtk_icon_theme_load_icon (gtk_icon_theme_get_default (), STOCK_TOP_FOLDER_ICON  , dataviewVars->iconSize , GTK_ICON_LOOKUP_USE_BUILTIN, &error);
			}
		}

		if(error) 	goto keyFileError;
// ** DIR ICON DONE *	 ** DIR ICON DONE *	 ** DIR ICON DONE *	 ** DIR ICON DONE *	 ** DIR ICON DONE *

//*****                                      SUB DIR ICON                                ****//
	// IS THE SUB DIRECTORY ICON SPECIFED IN THE KEY CONFIG FILE?
		atKeyLine =  __LINE__ + 1;
        rtn_subDirIconFile = g_key_file_get_string(keyFile, "Dataview Configuration", "subDirectoryIconFile",   &error);
        		if(error) 	goto keyFileError; // IF KEYWORD ERROR

		if(strlen(rtn_subDirIconFile) != 0)	 // DOES THE ICON FILE EXIST?
		{
			if(globalVerbose)  printf("Line %d -- SUB DIR ICON FILE \"%s\"\n", __LINE__, rtn_subDirIconFile);

			if(g_file_test (rtn_subDirIconFile, G_FILE_TEST_IS_REGULAR) == 0)
			{
				atKeyLine = __LINE__+ 1;
				g_set_error (&error, G_IO_ERROR_NOT_FOUND, 0, "Key file variable -- %s --\nFile\n-- %s --\nNot found.\nLine %d", "subDirectoryIconFile", rtn_subDirIconFile, atKeyLine);
				goto keyFileError;
			}

			atKeyLine = __LINE__ + 1;
			dataviewVars->sub_dir_Icon_pixBuf = gdk_pixbuf_new_from_file_at_scale( rtn_subDirIconFile, dataviewVars->iconSize, dataviewVars->iconSize, TRUE, &error );
		}

		else	// IF ICON FILE NOT SPECIFIED, THEN SET  THE RESOURCE ICON IF IT EXIST
		{
			if(g_resources_get_info (SUB_FOLDER_ICON_RESOURCE, G_RESOURCE_LOOKUP_FLAGS_NONE, NULL, NULL, NULL) == TRUE)
			{
				if(globalVerbose)  printf("Line %d -- SUB FOLDER ICON RESOURCE \"%s\"\n", __LINE__, SUB_FOLDER_ICON_RESOURCE);
				atKeyLine = __LINE__ + 1;
				dataviewVars->sub_dir_Icon_pixBuf = gdk_pixbuf_new_from_resource_at_scale (SUB_FOLDER_ICON_RESOURCE, dataviewVars->iconSize, dataviewVars->iconSize, TRUE, &error);
			}
			else	// IF RESOURCE ICON DOES NOT EXIST, THEN SET THE STOCK ICON, IF IT EXIST
			{
				if(globalVerbose)  printf("Line %d -- SUB FOLDER ICON STOCK \"%s\"\n", __LINE__, STOCK_SUB_FOLDER_ICON);
				atKeyLine = __LINE__ + 1;
				dataviewVars->sub_dir_Icon_pixBuf = gtk_icon_theme_load_icon (gtk_icon_theme_get_default (), STOCK_SUB_FOLDER_ICON  , dataviewVars->iconSize , GTK_ICON_LOOKUP_USE_BUILTIN, &error);
			}
		}

		if(error) 	goto keyFileError;
// ** SUB DIR DONE *	 ** SUB DIR DONE *	 ** SUB DIR DONE *	 ** SUB DIR DONE *	 ** SUB DIR DONE *

//*****                                      TRACK ICON                                ****//

	// IS THE TRACK ICON SPECIFED IN THE KEY CONFIG FILE?
		atKeyLine =  __LINE__ + 1;
        rtn_trkIconFile = g_key_file_get_string(keyFile, "Dataview Configuration", "trackIconFile",   &error);
        		if(error) 	goto keyFileError; // IF KEYWORD ERROR

		if(strlen(rtn_trkIconFile) != 0)	 // DOES THE TRACK ICON FILE EXIST?
		{
			if(globalVerbose)  printf("Line %d -- TRACK ICON FILE \"%s\"\n", __LINE__, rtn_trkIconFile);

			if(g_file_test (rtn_trkIconFile, G_FILE_TEST_IS_REGULAR) == 0)
			{
				atKeyLine = __LINE__+ 1;
				g_set_error (&error, G_IO_ERROR_NOT_FOUND, 0, "Key file variable -- %s --\nFile\n-- %s --\nNot found.\nLine %d", "trackIconFile", rtn_trkIconFile, atKeyLine);
				goto keyFileError;
			}

			atKeyLine = __LINE__ + 1;
			dataviewVars->trk_Icon_pixBuf = gdk_pixbuf_new_from_file_at_scale( rtn_trkIconFile, dataviewVars->iconSize, dataviewVars->iconSize, TRUE, &error );
		}

		else	// IF ICON FILE NOT SPECIFIED, THEN SET  THE RESOURCE ICON IF IT EXIST
		{
			if(g_resources_get_info (TRACK_ICON_RESOURCE, G_RESOURCE_LOOKUP_FLAGS_NONE, NULL, NULL, NULL) == TRUE)
			{
				if(globalVerbose)  printf("Line %d -- TRACK ICON RESOURCE \"%s\"\n", __LINE__, TRACK_ICON_RESOURCE);
				atKeyLine = __LINE__ + 1;
				dataviewVars->trk_Icon_pixBuf = gdk_pixbuf_new_from_resource_at_scale (TRACK_ICON_RESOURCE, dataviewVars->iconSize, dataviewVars->iconSize, TRUE, &error);
			}
			else	// IF RESOURCE ICON DOES NOT EXIST, THEN SET THE STOCK ICON, IF IT EXIST
			{
				if(globalVerbose)  printf("Line %d -- TRACK ICON STOCK \"%s\"\n", __LINE__, STOCK_TRACK_ICON);
				atKeyLine = __LINE__ + 1;
				dataviewVars->trk_Icon_pixBuf = gtk_icon_theme_load_icon (gtk_icon_theme_get_default (), STOCK_TRACK_ICON  , dataviewVars->iconSize , GTK_ICON_LOOKUP_USE_BUILTIN, &error);
			}
		}

		if(error) 	goto keyFileError;

// ** TRACK ICON DONE ** ** TRACK ICON DONE ** ** TRACK ICON DONE ** ** TRACK ICON DONE ** ** TRACK ICON DONE **
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // END OF GET KEYS

    return;

keyFileError:
    ;

    g_key_file_free (keyFile);

	showErrMsgDialog (error->message, atKeyLine, __FILE__, __func__, dataviewVars);

    exit(0);

}  // End of getConfigKeys

void initilizeGladeWidgets(app_variables *dataviewVars)
{   //	linenum;

    // ****** INITALIZE THE GLADE VARIABLES ******
    g_autoptr(GError) error = NULL;
    gint atLine;
    GtkBuilder  *builder = gtk_builder_new();

    // LOAD GLAD FROM INTERNAL RESOURCCE
    if(0 == g_strcmp0(dataviewVars->gladeFile, GLADE_RESOURCE))
    {
        atLine = __LINE__ + 1;
        gtk_builder_add_from_resource (builder, GLADE_RESOURCE, &error);
        if(error)
			showErrMsgDialog (error->message, atLine, __FILE__, __func__, dataviewVars);
    }
    // LOAD GLADE FROM EXTERNAL FILE
    else
    {
        atLine = __LINE__ + 1;
        checkKeyPathFile("Glade File", atLine, dataviewVars->gladeFile, dataviewVars);
        atLine = __LINE__ + 1;
        gtk_builder_add_from_file (builder, dataviewVars->gladeFile, &error);
        if(error)
			showErrMsgDialog (error->message, atLine, __FILE__, __func__, dataviewVars);
    }

    dataviewVars->w_dataviewTreeStore = GTK_TREE_STORE(gtk_builder_get_object(builder, "dataviewTreeStore"));
    dataviewVars->w_dataviewTreeView = GTK_TREE_VIEW(gtk_builder_get_object(builder, "dataviewTreeView"));

    // ***** dataview SELECT TRACKS *****
    dataviewVars->w_selTrkTreeStore = GTK_TREE_STORE				(gtk_builder_get_object(builder, "selTrkTreeStore"));
    dataviewVars->w_selTrkTreeView   = GTK_TREE_VIEW				(gtk_builder_get_object(builder, "selTrkTreeView"));

    //******* INITILAZE THE WIDGETS *********
    GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    dataviewVars->w_window_main = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    dataviewVars->w_lblHostname    = GTK_WIDGET(gtk_builder_get_object(builder, "lblHostname"));
    dataviewVars->w_box_0 = GTK_WIDGET(gtk_builder_get_object(builder, "box_0"));
    dataviewVars->w_dataview_Pane = GTK_PANED (gtk_builder_get_object (builder, "dataview_Pane"));
    dataviewVars->w_scrollHelp    = GTK_WIDGET(gtk_builder_get_object(builder, "scrollHelp"));
    dataviewVars->w_lblHelp    = GTK_LABEL(gtk_builder_get_object(builder, "lblHelp"));

	dataviewVars->dataviewFirstPass = 1;

    // CONNECT SIGNALS
    gtk_builder_connect_signals(builder, dataviewVars);
    g_clear_object(&builder);

    // INITILAZE SOME OF THE STUFF THAT DEPENDS ON THE GLADE FILE

    // SET WINDOW TITLE
    gtk_window_set_title (GTK_WINDOW (dataviewVars->w_window_main), dataviewVars->windowTitle);

    // SET THE WINDOW WIDTH AND HEIGHT
    g_object_set (dataviewVars->w_window_main, "default-width", dataviewVars->winWidth, NULL);
    g_object_set (dataviewVars->w_window_main, "default-height", dataviewVars->winHeight, NULL);

    // SET SET STUFF IN THE HOSTNAME MSG LABEL
    gtk_label_set_text (GTK_LABEL (dataviewVars->w_lblHostname), dataviewVars->mpdHostname);

	// TREEVIEW FOCUS?
	gtk_widget_set_can_focus (GTK_WIDGET(dataviewVars->w_dataviewTreeView), true);
	gtk_widget_set_can_focus (GTK_WIDGET(dataviewVars->w_selTrkTreeView), true);

	gtk_widget_set_focus_on_click (GTK_WIDGET(dataviewVars->w_dataviewTreeView), true);
	gtk_widget_set_focus_on_click (GTK_WIDGET(dataviewVars->w_selTrkTreeView), true);

	gtk_widget_set_can_default (GTK_WIDGET(dataviewVars->w_dataviewTreeView), false);
	gtk_widget_set_can_default (GTK_WIDGET(dataviewVars->w_selTrkTreeView), false);

    // SAVE!!!!!!!  SAVE!!!!!!!  SAVE!!!!!!!  SAVE!!!!!!!  SAVE!!!!!!!  SAVE!!!!!!!
    if(globalVerbose)
    {
        printf("\nLine %d -- dataviewVars->mpdHostname = %s\n", __LINE__, dataviewVars->mpdHostname);
        printf("Line %d -- dataviewVars->mpdPortnumber = %d\n", __LINE__, dataviewVars->mpdPortnumber);
        printf("Line %d -- dataviewVars->mpdTimeout = %d\n", __LINE__, dataviewVars->mpdTimeout);
        printf("Line %d -- dataviewVars->winWidth = %d\n", __LINE__, dataviewVars->winWidth);
        printf("Line %d -- dataviewVars->winHeight = %d\n", __LINE__, dataviewVars->winHeight);
        printf("Line %d -- dataviewVars->dialogFont = %s\n", __LINE__, dataviewVars->dialogFont);
        printf("Line %d -- dataviewVars->windowTitle = %s\n\n", __LINE__, dataviewVars->windowTitle);
        printf("Line %d -- dataviewVars->gladeFile = %s\n", __LINE__, dataviewVars->gladeFile);
        printf("Line %d -- dataviewVars->cssFile = %s\n", __LINE__, dataviewVars->cssFile);
        //~ printf("Line %d -- dataviewVars->showHideButtons = %s\n", __LINE__, dataviewVars->showHideButtons ? "TRUE" : "FALSE");
        printf("Line %d -- End of CONFIG FILE\n", __LINE__);
    }
} //End of initilizeGladeVariable
//******* End of INITILAZE THE WIDGETS *********

// ********* THIS LOADS THE CSS STYLE OVERRIDE OF THE THEME *********
void loadCCSProvider(app_variables *dataviewVars)
{   // linenum;

    // LOAD THE CSS FILE
    GtkCssProvider *provider = gtk_css_provider_new ();
    GdkDisplay *display = gdk_display_get_default ();
    GdkScreen *screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), \
            GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_autoptr(GError) error = NULL;
    gint atLine;
    // LOAD CSS FROM INTERNAL RESOURCE
    if(0 == g_strcmp0(dataviewVars->cssFile, CSS_RESOURCE))
    {
        atLine = __LINE__ + 1;
        gtk_css_provider_load_from_resource (provider, CSS_RESOURCE);
        if(error)
			showErrMsgDialog (error->message, atLine, __FILE__, __func__, dataviewVars);
    }
    // LOAD CSS FROM EXTERAL FILE
    else
    {
        atLine = __LINE__ + 1;
        checkKeyPathFile("Css File", atLine, dataviewVars->cssFile, dataviewVars);
        atLine = __LINE__ + 1;
        gtk_css_provider_load_from_path(provider, dataviewVars->cssFile, &error);
        if(error)
			showErrMsgDialog (error->message, atLine, __FILE__, __func__, dataviewVars);
    }

    // SET WINDOW FONT IF ON THE COMMAND LINE
    if(dataviewVars->dialogFont)
    {
        g_autofree gchar *windowFont = g_strdup_printf("window {font: %s; }\n", dataviewVars->dialogFont);
        if(globalVerbose)  printf("Line %d -- WINDOW FONT %s\n", __LINE__, windowFont);

        GtkCssProvider *fontProvider = gtk_css_provider_new ();
        gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (fontProvider), \
                GTK_STYLE_PROVIDER_PRIORITY_USER);
        gtk_css_provider_load_from_data (fontProvider, windowFont, -1, &error);
        g_object_unref(fontProvider);
    }

    // WE'RE DONE, UNREF THE PROVIDER
    g_object_unref(provider);
}
// ***** End of THIS SETS UP THE CSS STYLE OVERRIDE OF THE THEME

int checkKeyPathFile(gchar *keyName, gint atLine, gchar *fileName, app_variables *dataviewVars)
{   //  linenum;

    g_autofree gchar *txtStr = NULL;
    g_autoptr(GError) error = NULL;
    gint atKeyLine;

    // IS THE PATH/FILENAME ABSOLUTE?
    if(g_path_is_absolute (fileName) == 0) { //    printf("FILENAME %s\n", fileName);
        atKeyLine = __LINE__ - 1;
        g_set_error (&error, 0, 0, "Key file variable -- %s --\n-- %s -- is not a valid filename.\nFilename must be an absolute path/filename.\nLine %d",  keyName, fileName, atKeyLine);

        goto fileCheckKey;
    }

    // DOES THE FILE EXIST?
    if(g_file_test (fileName, G_FILE_TEST_IS_REGULAR) == 0) {   //     printf("FILENAME %s\n", fileName);
        atKeyLine = __LINE__ - 1;
        g_set_error (&error, 0, 0, "Key file variable -- %s --\nFile\n-- %s --\nNot found.\nLine %d", keyName, fileName, atKeyLine);
        goto fileCheckKey;
    }

    return 1;

fileCheckKey:
    ;

	showErrMsgDialog (error->message, atLine, __FILE__, __func__, dataviewVars);
    exit(0);
}  // End of checkKeyPathFile

// ERROR DIALOG
void showErrMsgDialog (gchar *msg, gint line, gchar *file, const gchar *func, app_variables *dataviewVars)
{   // linenum;
    GtkWidget *errorDialog;

    errorDialog = gtk_message_dialog_new_with_markup (NULL,  0,  GTK_MESSAGE_ERROR,  GTK_BUTTONS_CLOSE,\
                  "<span foreground='red' weight=\"bold\" size=\"larger\">%s\n\nFile %s\nFunction %s\nLine %d\nClick \"Close\" to exit.</span>", msg, file, func, line);

    gtk_window_set_title (GTK_WINDOW (errorDialog), "Error");
    gtk_dialog_run (GTK_DIALOG (errorDialog));
    gtk_widget_destroy (errorDialog);
    exit(0);
}

void showNotify (gchar *notifyMsg, app_variables *dataviewVars)
{   // linenum;
	gtk_label_set_text (GTK_LABEL (dataviewVars->w_lblHostname), notifyMsg);
    g_timeout_add_seconds (dataviewVars->notifyDialogTimeout,  (GSourceFunc) destroyNotifyDialog,  dataviewVars);
}

gboolean destroyNotifyDialog(app_variables *dataviewVars)
{   //  linenum;
    gtk_label_set_text (GTK_LABEL (dataviewVars->w_lblHostname), dataviewVars->mpdHostname);
    return FALSE;
}


// EXPAND ALL ROWS
void	expandAllRows(GtkMenuItem *menuitem, GtkTreeView *tvWidget)
{   // linenum;
    gtk_tree_view_expand_all (GTK_TREE_VIEW(tvWidget));
}

// COLLAPSE ALL THE ROWS
void collapseAllRows(GtkMenuItem *menuitem, GtkTreeView *tvWidget)
{   // linenum;
    gtk_tree_view_collapse_all (GTK_TREE_VIEW(tvWidget));
}

// EXPAND SELECTED ROW AT CURSOR
void	expandSelectedRow(GtkMenuItem *menuitem, GtkTreeView *tvWidget)
{   // linenum;
// SAVE!!
//gtk_tree_view_row_expanded (GTK_TREE_VIEW(tvWidget),path)
    GtkTreePath *path = NULL;
    gtk_tree_view_get_cursor (GTK_TREE_VIEW(tvWidget), &path, NULL);
    gtk_tree_view_expand_row (GTK_TREE_VIEW(tvWidget), path, TRUE);
}

// COLLASPE SELECTED ROW AT CURSOR
void collapseSelectedRow(GtkMenuItem *menuitem, GtkTreeView *tvWidget)
{   // linenum;
    GtkTreePath *path = NULL;
    gtk_tree_view_get_cursor (GTK_TREE_VIEW(tvWidget), &path, NULL);
    gtk_tree_view_collapse_row (GTK_TREE_VIEW(tvWidget), path);
}
