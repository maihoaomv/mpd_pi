// playview.c

/*
*  MIT License
*
*  Copyright (c) 2023 maihoaomv
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  The above copyright notice and this permission notice shall be included in all
*  copies or substantial portions of the Software.
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*  SOFTWARE.
*/

#include "playview.h"
#include "playview_version.h"

int main(int argc, gchar **argv)
{   //printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    gtk_init(&argc, &argv);

    //****** INITILIZE PLAYVIEWI VARS STRUCT ******
    app_variables *playviewVars = g_slice_new0(app_variables);

    // IS THE HELP OR VERSION OPTION ON THE COMMAND LINE OR IS VERBOSE SET?
    // IF VERSION OPTION -v OR --version THEN PRINT VERSION STRING AND EXIT
    // IF VERBOSE OPTION IS SET THEN SET THE GLOBAL VERBOSE FLAG TO PRINT SOME STATUS ALONG THE WAY
    int loop = 1;

    if(argc > 1)
    {
        while(loop < argc)
        {
            // IF REQUESTING VERBOSE THEN SET THE FLAG
            if((g_strcmp0(argv[loop], "--verbose") == 0) || (g_strcmp0(argv[loop], "-V") == 0) )
                globalVerbose++;
            if(globalVerbose)		printf("Line %4d -- VERBOSE ON %d\n", __LINE__, globalVerbose);

            // IF THE VERSION OPTION ON COMMAND LINE THAN PRINT STRING TO STDOUT AND EXIT
            if((g_strcmp0(argv[loop], "--version") == 0) || (g_strcmp0(argv[loop], "-v") == 0) )
            {
                fprintf(stdout, "%s\n", versionString());
                exit(0);
            }

            if((g_strcmp0(argv[loop], "--help") == 0) || (g_strcmp0(argv[loop], "-h") == 0))
            {
                GString *helpStr = NULL;
                helpStr= g_string_new("");

                g_string_append_printf(helpStr, "\n Usage: \t%s  [ --host ]  [ --port ] ", g_get_prgname ());
                g_string_append_printf(helpStr, " [ --timeout ]  [ --width ]  [ --height ]  [ --font ]  [ --title ]\n");

                g_string_append_printf(helpStr, " Example: \t%s --host=\"localhost\" --port=6600 --timeout=30000 ", g_get_prgname ());

                g_string_append_printf(helpStr, "--width=1200 --height=800 --font=\"Bold 20px Sans\" ");
                g_string_append_printf(helpStr, "--title=\"Display MPD Playlist\"\n\n");
                g_string_append_printf(helpStr, "\n This app will display the MPD playlist files and is intended to be used as a helper, but it can be used standalone.\n Default is localhost unless specified othewise with the --host= option.\n");

                g_string_append_printf(helpStr, "\n %s  [ -h ]  -Print help to command line then exit\n", g_get_prgname ());
                g_string_append_printf(helpStr, " %s  [ -v ]  -Print version to command line then exit\n", g_get_prgname ());
                g_string_append_printf(helpStr, "\n %s  [ -V ]  -Print verbose useful stats to stdout command line\n\n", g_get_prgname ());

				g_string_append_printf(helpStr, " Press F1 for Help, Press ESC to exit %s app\n", g_get_prgname ());

				g_string_append_printf(helpStr, "\n NOTE: Optionally an external config file may be used with %s. If the config file  \"%s.conf\"  exist in the\n       current working directory, then the app will automatically read configuation from the external configuration file\n       instead of the internal resouce configuration file.\n\n", g_get_prgname (), g_get_prgname ());

                g_printf("%s", helpStr->str);

                g_string_free(helpStr, TRUE);
				//~ g_fprintf(stderr, "\n %s\n\n", versionString());
                exit(0);
            }

            loop++;
            if(loop > argc) break;
        }
    }
    //~ if(globalVerbose) printf("\nVERBOSE = %d\n\n", globalVerbose);

    //~ //****** INITILIZE PLAYVIEWI VARS STRUCT ******
    //~ app_variables *playviewVars = g_slice_new0(app_variables);

    playviewVars->mpdHostname = NULL;  // MUST BE SET TO NULL TO ACCOMMODATE COMMAND ARGS
    playviewVars->mpdPortnumber = -1;  // MUST BE SET TO -1 TO ACCOMMODATE COMMAND ARGS
    playviewVars->mpdTimeout = -1;  // MUST BE SET TO -1 TO ACCOMMODATE COMMAND ARGS
    playviewVars->winWidth = -1;  // MUST BE SET TO -1 TO ACCOMMODATE COMMAND ARGS
    playviewVars->winHeight = -1;  // MUST BE SET TO -1 TO ACCOMMODATE COMMAND ARGS

    playviewVars->cwdPath = NULL;  //SET TO NULL TO BE ON SAFE SIDE
    playviewVars->gladeFile = NULL;  //SET TO NULL TO BE ON SAFE SIDE
    playviewVars->cssFile = NULL;  //SET TO NULL TO BE ON SAFE SIDE
    playviewVars->helpFile = NULL;  //SET TO NULL TO BE ON SAFE SIDE
    playviewVars->cfgFile = NULL;  //SET TO NULL TO BE ON SAFE SIDE
    playviewVars->simpleMsgDialog = NULL;  //SET TO NULL TO BE ON SAFE SIDE

    playviewVars->windowTitle = NULL;  // MUST BE SET TO NULL TO ACCOMMODATE COMMAND ARGS
    playviewVars->dialogFont = NULL;   // MUST BE SET TO NULL TO ACCOMMODATE COMMAND ARGS
    playviewVars->notifyDialogTimeout = 1;

    // IS THIS EXECUTABLE A LINKED FILENAME, RELATIVE FILENAME OR AN ABSOULTE PATH/FILENAME?
    gchar *isLink = g_file_read_link (argv[0], NULL);
    gchar *isDir = g_path_get_dirname (argv[0]);

    if(isLink == NULL)
    {
        if(0 == g_strcmp0(isDir, "."))
            playviewVars->cwdPath = g_get_current_dir ();
        else playviewVars->cwdPath = g_strdup(isDir);
    }
    else {
        playviewVars->cwdPath = g_path_get_dirname(isLink);
    }
    g_free(isDir);
    g_free(isLink);

    // GET AND PROCESS THE COMMAND LINE ARGUMENTS
    getTheOptions(argc, argv, playviewVars);

    // GET THE CONFIG KEYS
    getConfigKeys(playviewVars);

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
    initilizeGladeWidgets(playviewVars);

    // LOAD THE CSS STYLE FILE
    loadCCSProvider(playviewVars);

    // SET UP THE SIGNALS
    setSignals(playviewVars);

    // **** TEST MPD CONNECTION
    struct mpd_connection *conn  = NULL;
    conn = get_connection(playviewVars, __FILE__, __func__, __LINE__);
    if(conn == NULL) {
        mpd_connection_free(conn);
        gtk_main_quit();
    }
    mpd_connection_free(conn);

    // TELL THE TREEVIEW TO INITILIZE THE TREEVIEW FOR THE FIRST TIME AND SHOW ON MAIN WINDOW AND BOX_0
    playviewVars->startUpFirstPass = 1;

    // START A MILLISECOND TIMER TO GET STATUS AND TRACK DATA
    g_timeout_add_full (G_PRIORITY_HIGH, 250, (GSourceFunc) showViewTimer, playviewVars, NULL);

    // PUT THE APP WINDOW ON TOP
    gtk_window_set_keep_above (GTK_WINDOW(playviewVars->w_window_main), TRUE);

    //SHOW THE WINDOW
    gtk_main();

    if(playviewVars->mpdHostname)		g_free(	playviewVars->mpdHostname);
    if(playviewVars->cwdPath)					g_free(	playviewVars->cwdPath);
    if(playviewVars->gladeFile)				g_free(	playviewVars->gladeFile);
    if(playviewVars->cssFile)					g_free(	 playviewVars->cssFile);
    if(playviewVars->helpFile)					g_free(	 playviewVars->helpFile );
    if(playviewVars->cfgFile)					g_free(	playviewVars->cfgFile );
    if(playviewVars->simpleMsgDialog)	g_free(	playviewVars->simpleMsgDialog);
    if(playviewVars->windowTitle)			g_free(	playviewVars->windowTitle);
    if(playviewVars->dialogFont)				g_free(	 playviewVars->dialogFont);


    // FREE THE STRUCT
    g_slice_free(app_variables, playviewVars);

    //    printf("\nEnd of -- %s -- Line %d --\n", __func__, __LINE__);
    exit(0);
} // *** End of main ***

//*****************************************************************************************************
gboolean showViewTimer(app_variables *playviewVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    g_autoptr(GError) error = NULL;
    // FIRST ORDER OF BUSINESS -- UPDATE THE MPD STATUS
    struct mpd_connection *conn;
    conn = get_connection(playviewVars, __FILE__, __func__, __LINE__);
    struct mpd_status * status;
    mpd_send_status(conn);
    status = mpd_recv_status(conn);
    gint atLine = __LINE__;
    if (status == NULL) {
        g_set_error (&error, 0, 0, "MPD STATUS ERR: %s\n", mpd_status_get_error(status));
        mpd_status_free (status);
        showErrMsgDialog (error->message,  atLine, __FILE__, __func__,  playviewVars);
        //~ spawnSimpleMessageDialog(error->message,  atLine, __FILE__, __func__, ERROR_MSG_MODE,  playviewVars);
        g_error_free(error);
        return TRUE;
    }

    playviewVars->queueLengthStatus 	= mpd_status_get_queue_length (status);
    playviewVars->mpdPlayerStateStatus = mpd_status_get_state (status);

    mpd_status_free(status);
    mpd_connection_free(conn);

    // SET THE MPD PLAYER STATUS BACKGROUND COLOR. RED = PLAYER STOP MODE, GREEN = PLAYER PLAY MODE,
    // BLUE = PLAYER PAUSE MODE,  GRAY = NO TRACKS IN THE PLAYER QUEUE.
    set_mpd_status_color(playviewVars);

    // IF THIS IS THE INITIAL START UP OF THE PROGRAM, THEN WE NEED TO UN-HIDE (SHOW) THE MAIN WINDOW AND BOX_0
    if(playviewVars->startUpFirstPass)
    {
        gtk_widget_show(GTK_WIDGET(playviewVars->w_box_0));
        gtk_widget_show(playviewVars->w_window_main);
        playviewVars->startUpFirstPass = 0;
    }

    playView (playviewVars);

    return TRUE;
} // *** End of showViewTimer ***

gint playView (app_variables *playviewVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    // FIRST PASS THRU PLAYVIEW. NEEDS TO BE HERE AFTER BOX 6 SHOW
    if(playviewVars->playviewFirstPass)
        playviewVars->playviewFirstPass = 0;
    else return 1;

    GtkTreeIter    iter;

    gchar *playviewInfo = NULL;

    // GET MPD CONNECTION
    struct mpd_connection *conn;
    conn = get_connection(playviewVars, __FILE__, __func__, __LINE__);
    struct mpd_playlist *playlist;
    mpd_send_list_playlists(conn);

    // CLEAR THE LISTSTORE
    gtk_list_store_clear (playviewVars->w_playviewListStore);

    while ((playlist = mpd_recv_playlist(conn)) != NULL) {

        playviewInfo = g_strdup(mpd_playlist_get_path(playlist));

        // APPEND A ROW AND FILL IN PLAYLIST DATA
        gtk_list_store_append (playviewVars->w_playviewListStore, &iter);
        gtk_list_store_set (playviewVars->w_playviewListStore, &iter, PLAYVIEW_COL_ZERO, playviewInfo, -1);

        if(playviewInfo) {
            g_free(playviewInfo);
            playviewInfo = NULL;
        }
        mpd_playlist_free(playlist);
    }

    if(playviewInfo) g_free(playviewInfo);
    // FREE MPD CONNECTION
    mpd_connection_free(conn);
    // *** End of FILL FROM MPD PLAYLIST ***

    // SORT LISTSTORE
    GtkTreeSortable *sortable;
    sortable = GTK_TREE_SORTABLE(playviewVars->w_playviewListStore);
    gtk_tree_sortable_set_sort_column_id(sortable, PLAYVIEW_COL_ZERO, GTK_SORT_ASCENDING);

    // printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
    return 1;

} // *** End of CREATE_AND_FILL_MODEL ***

gboolean playview_onRowActivated (GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn  *col, app_variables *playviewVars)
{   // g_printf ("A PLAYVIEW row has been double-clicked!\n");
    //printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    // GET MPD CONNECTION
    struct mpd_connection *conn;
    conn = get_connection(playviewVars, __FILE__, __func__, __LINE__);

    GtkTreeSelection *selection;
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));

    GtkTreeIter iter;
    GtkTreeModel *model;
    g_autofree gchar *value;

    // *** IF THE ROW IS CLICKED THEN GET THE NEW SELECTION
    gtk_tree_selection_get_selected(GTK_TREE_SELECTION( selection), &model, &iter); // WON'T WORK WITHOUT THIS
    // GET THE SELECTED PLAYLIST FROM THE LISTSTORE
    gtk_tree_model_get(model, &iter, PLAYVIEW_COL_ZERO, &value,  -1);

    mpd_run_clear(conn);  // CLEAR THE QUEUE
    mpd_run_load(conn, value);   // LOAD PLAYLIST
    mpd_run_play(conn);   // START THE PLAYER

    // CLEAN UP BEFORE RETURNING
    mpd_connection_free(conn); // FREE MPD CONNECTION

    // AND SET HEADER TO PLAYLIST NAME
    gtk_tree_view_column_set_title(col, value);
    /////////////////////////////////////////////////////////////////////
    g_autofree gchar *textStr = g_strdup_printf("Replaced MPD Queue\n%s", value);
    showNotify (textStr, playviewVars);
    //~ spawnSimpleMessageDialog(textStr,  __LINE__, __FILE__, __func__, NOTIFY_MSG_MODE,  playviewVars);
    //~ g_free(textStr);
    /////////////////////////////////////////////////////////////////////
    //~ g_free(value); // FREE STRING
    // printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
    return FALSE;
}

// ***** KEYBOARD EVENTS *****
gboolean window_key_press_event (GtkWidget *widget, GdkEventKey *event, app_variables *playviewVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    GdkModifierType modifiers;
    modifiers = gtk_accelerator_get_default_mod_mask ();
    GdkWindow *window = gtk_widget_get_window(widget);
    gtk_accelerator_set_default_mod_mask (GDK_CONTROL_MASK | GDK_SHIFT_MASK | GDK_MOD1_MASK );
    GdkWindowState state = gdk_window_get_state(GDK_WINDOW(window));

    switch(event->keyval) {

    case GDK_KEY_Escape:  // CLOSE APPLICATION
        ////////////////////////////////////////////////////////////////////////////////////////
        if(gtk_widget_get_visible (playviewVars->w_scrollHelp)) {
            helpMe_signal(NULL, playviewVars);
            break;
        }
        ////////////////////////////////////////////////////////////////////////////////////////
        gtk_main_quit();
        break;

    case GDK_KEY_h:  // OPEN / CLOSE HELP LABEL
    case GDK_KEY_H:  // OPEN / CLOSE HELP LABEL
    case GDK_KEY_F1:  // OPEN / CLOSE HELP LABEL
        helpMe_signal(NULL, playviewVars);
        break;

    // POPUP MENU AND TOGGLE MINIMIZE/MAXIMAZE THE GUI
    case GDK_KEY_F10: //printf("F10 KEY\n");
        // SHIFT-F10 (WITH THE SHIFT KEY) RIGHT CLICK POPUP MENU
        if((event->state & modifiers) == GDK_SHIFT_MASK) {
            view_popup_menu(widget, NULL, playviewVars);
            break;
        }

        // TOGGLES APPLICATION  BETWEEN FULLSCREEN/MAXIMIZE/NORMAL
        if(state & GDK_WINDOW_STATE_MAXIMIZED) {	// MAXIMIZE
            gtk_window_unmaximize(GTK_WINDOW(widget));
            //~ gtk_window_fullscreen(GTK_WINDOW(widget));
            break;
        }
        //~ if(state & GDK_WINDOW_STATE_FULLSCREEN) {	// FULLSCREEN
            //~ gtk_window_unfullscreen(GTK_WINDOW(widget));
            //~ break;
        //~ }
        gtk_window_maximize(GTK_WINDOW(widget));	// NORMAL
        break;

    case GDK_KEY_q:  // EXIT / QUIT
    case GDK_KEY_Q:
        if((event->state & modifiers) == GDK_CONTROL_MASK) {
            gtk_main_quit();
            break;
        }
        // GDK_CONTROL_MASK + GDK_SHIFT_MASK +Q -CLEAR THE PLAYER QUE
        if((event->state & modifiers) == (GDK_CONTROL_MASK | GDK_SHIFT_MASK))
            clearPlaylist_signal(NULL, playviewVars);
        break;

    case GDK_KEY_m:  // INVOKE RIGHT CLICK POPUP MENU
    case GDK_KEY_M: // KEY CTRL+m	= POPUP MAIN WINDOW RIGHT CLICK MENU
        view_popup_menu(widget, NULL, playviewVars);

        break;

    // ADD PLAYLIST TO MPD QUE
    case GDK_KEY_a:
    case GDK_KEY_A:
        if((event->state & modifiers) == GDK_CONTROL_MASK) {
            addPlaylist_signal( NULL, playviewVars);
            break;
        }

        break;

    case GDK_KEY_r:  // REPLACE, CLEAR MPD QUE, AND START PLAYING
    case GDK_KEY_R:
        if((event->state & modifiers) == GDK_CONTROL_MASK) {
            replacePlaylist_signal(NULL, playviewVars);
            startPlayer(NULL, playviewVars);
            break;
        }
        break;

	case GDK_KEY_p:
	case GDK_KEY_P:
            startPlayer(NULL, playviewVars);;
            break;

		case GDK_KEY_s:
		case GDK_KEY_S:
        stopPlayer(NULL, playviewVars);
        break;
    }
    // printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
    return FALSE;
} // *** End of window_key_press_event *** //

// *** other.c ***
// ***** CATCH SIGNAL HANDLER FUNCTION *****
void signal_callback_sigFault(int signum)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    g_autofree gchar *globalSigFaultFile = g_strdup_printf("%s/%s.sigfault",g_get_tmp_dir (), g_get_prgname ());
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

    g_autoptr(GError) error = NULL;

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
        //		exit(0);
    }

    g_free (strings);
    g_free(msg);

    //	printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
    exit(0);
}// ** End of signal_callback_handler ***

// WRITE ERROR MESSAGES TO THE DEBUG FILE IN TMP DIRECTORY FOR REFERENCE.
// NOTE:CONTENTS OF THE DEBUG FILE WILL BE LOST WITH A REBOOT
int printDebugMsg(gchar *debugMsg, gint line, gchar *file, const gchar *func,  app_variables *playviewVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    g_autofree gchar *playviewDebugFile = g_strdup_printf("%s/%s.debug",g_get_tmp_dir (), g_get_prgname ());
    printf("Line %d -- playviewDebugFile %s\n", __LINE__,  playviewDebugFile);
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
    g_string_append_printf(myMsg, "%s/%s -- Compiled %s -- %s\n\n", playviewVars->cwdPath, g_get_prgname (), __DATE__, __TIME__);
    g_string_append_printf(myMsg, "-- Debug message written to file -- %s\n", playviewDebugFile);
    g_string_append_printf(myMsg, "--------------- END ---------------\n");

    g_fprintf(stderr,"%s\n\n", myMsg->str);

    // GET CONTENTS OF THE EXISTING DEBUG FILE, IF IT EXIST,
    gint result = g_file_get_contents (playviewDebugFile, &contents, NULL, NULL);
    // AND APPEND IT TO THE END OF THE MSG STRING SO THAT THE LATEST WILL BE AT THE VERY TOP OF THE FILE
    if(result)
        g_string_append_printf(myMsg, "\n%s\n", contents);

    // THEN WRITE THE STRING BACK TO DISK, OVERWRITING THE EXISING CONTENTS.
    g_file_set_contents (playviewDebugFile, myMsg->str, -1, &error);

    // IF THERE IS AN ERROR WRITING TO TEMP FILE,
    if(error)
        g_fprintf(stderr, "ERROR writing to debug file %s -- %s\n", playviewDebugFile, error->message);

    g_string_free(myMsg, TRUE);

    return 0;
}

void map_playviewTreeView_signal(GtkWidget *widget, app_variables *playviewVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    gtk_list_store_clear (playviewVars->w_playviewListStore);
    playviewVars->playviewFirstPass = 1;
}

void unmap_playviewListView_signal(GtkWidget *widget, app_variables *playviewVars) {
    // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    gtk_list_store_clear (playviewVars->w_playviewListStore);
    //printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
}

void setSignals(app_variables *playviewVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    //~ g_signal_connect(playviewVars->w_btnExit, "clicked", (GCallback) gtk_main_quit, playviewVars);
    //~ g_signal_connect(playviewVars->w_btnHelp, "clicked", (GCallback) helpMe_signal, playviewVars);
    //~ g_signal_connect(playviewVars->w_btnPlay, "clicked", (GCallback) startPlayer, playviewVars);
    //~ g_signal_connect(playviewVars->w_btnPause, "clicked", (GCallback) pausePlayer, playviewVars);
    //~ g_signal_connect(playviewVars->w_btnStop, "clicked", (GCallback) stopPlayer, playviewVars);


    g_signal_connect(playviewVars->w_playviewTreeView, "row-activated", (GCallback) playview_onRowActivated, playviewVars);

    g_signal_connect(playviewVars->w_playviewTreeView, "map", (GCallback)  map_playviewTreeView_signal, playviewVars);

    g_signal_connect(playviewVars->w_playviewTreeView, 	"unmap", (GCallback)  	unmap_playviewListView_signal, playviewVars);

    g_signal_connect(playviewVars->w_window_main, "key-press-event", (GCallback) window_key_press_event, playviewVars );

    g_signal_connect(playviewVars->w_window_main, "button-press-event", (GCallback) popup_menu_event_signal, playviewVars );

    g_signal_connect(playviewVars->w_playviewTreeView, "button-press-event", (GCallback) popup_menu_event_signal, playviewVars );
}

// THIS SETS THE MPD PLAYER STATUS BACKGROUND COLOR
int set_mpd_status_color(app_variables *playviewVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    static gint save_mpd_state = MPD_STATE_UNKNOWN;
    gint playerState = playviewVars->mpdPlayerStateStatus;

    if(playviewVars->queueLengthStatus == 0)
    {
        save_mpd_state = MPD_STATE_UNKNOWN;
        gtk_widget_set_name (playviewVars->w_window_main, "CSS_window_main-NOTRACKS");

        GtkTreeViewColumn *col;
        col = gtk_tree_view_get_column (playviewVars->w_playviewTreeView, PLAYVIEW_COL_ZERO);
        gtk_tree_view_column_set_title(col, "Select Playlist");

        return 0;
    }

    // HAS THE MPD STATE CHANGED (play - pause - stop - empty queue) THEN SET BACKGROUND COLOR
    if(playerState != save_mpd_state)
        save_mpd_state = playerState;
    else return 0;

    switch (playviewVars->mpdPlayerStateStatus) {
    case MPD_STATE_PLAY:
        gtk_widget_set_name (playviewVars->w_window_main, "CSS_window_main-PLAY");
        //~ gtk_widget_show (GTK_WIDGET (playviewVars->w_btnPause));
        //~ gtk_widget_hide (GTK_WIDGET (playviewVars->w_btnPlay));
        break;
    case MPD_STATE_PAUSE:
        gtk_widget_set_name (playviewVars->w_window_main, "CSS_window_main-PAUSE");
        //~ gtk_widget_hide (GTK_WIDGET (playviewVars->w_btnPause));
        //~ gtk_widget_show (GTK_WIDGET (playviewVars->w_btnPlay));
        break;
    case MPD_STATE_STOP:
        gtk_widget_set_name (playviewVars->w_window_main, "CSS_window_main-STOP");
        //~ gtk_widget_hide (GTK_WIDGET (playviewVars->w_btnPause));
        //~ gtk_widget_show (GTK_WIDGET (playviewVars->w_btnPlay));
        break;
    case MPD_STATE_UNKNOWN:
        break;
    default:
        break;
    }

    // printf("End of -- %s -- Line %d --\n\n", __func__, __LINE__);
    return 0;
} // ***** End of set_mpd_status_color *****

// ********** GET A MPD CONNECTON ********
struct mpd_connection *get_connection(app_variables *playviewVars, gchar *fileName, const gchar *funcName, int lineNum)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    struct mpd_connection *conn;
    conn = mpd_connection_new(playviewVars->mpdHostname,  playviewVars->mpdPortnumber, playviewVars->mpdTimeout);
    int errNumber  = mpd_connection_get_error(conn);
    // IF CONNECTION SUCESSFUL, RETURN CONNECTION
    if(errNumber == MPD_ERROR_SUCCESS)
        return conn;
    gint atLine = __LINE__;
    // OTHERWISE FATAL ERROR, GET THE ERROR CODE
    //    gchar *mpdErrMsg = g_strdup_printf("MPD CONNECTION ERR:\n%s\nFunction %s\nFilename %s\nLine %d", mpd_connection_get_error_message(conn), funcName, fileName, lineNum);
    gchar *mpdErrMsg = g_strdup_printf("MPD CONNECTION ERROR:\n%s\nHostname \t%s\nPortnumber \t%d\nTimeout \t\t%d", mpd_connection_get_error_message(conn),playviewVars->mpdHostname, playviewVars->mpdPortnumber, playviewVars->mpdTimeout);

    mpd_connection_clear_error(conn);

    showErrMsgDialog (mpdErrMsg,  atLine, __FILE__, __func__,  playviewVars);
    //~ spawnSimpleMessageDialog(mpdErrMsg,  atLine, __FILE__, __func__, ERROR_MSG_MODE,  playviewVars);

    g_free(mpdErrMsg);
    gtk_main_quit();
    exit(0);
}

//////////////////////// POPUP_MENU.C /////////////////////////////////
// RIGHT CLICK MOUSE BUTTON    --   CLICK - BUTTON-PRESS-EVENT
gboolean popup_menu_event_signal (GtkWidget *window_main, GdkEventButton *event, app_variables *playviewVars)
{   //g_print("POPUP MENU Inside ON_WINDOW_MAIN_BUTTON_PRESS_EVENT Line %d, BUTTON = %d\n", __LINE__, event->button);
    /* single click with the right mouse button- popup menu */
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3) // right click
    {   // g_print("Right mouse button clicked\n");
        view_popup_menu(window_main, event, playviewVars);
    }

    return FALSE; /* we did handle this */
}

// RIGHT CLICK POPUP MENU CALLBACKS
void view_popup_menu (GtkWidget *window_main, GdkEventButton *event, app_variables *playviewVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__););

    GtkWidget *popupMenu;
    popupMenu = gtk_menu_new();

    // THIS ONLY A LABEL TO IDENTIFY THIS POPUP MENU
    GtkWidget *titleMi = gtk_menu_item_new_with_label("Playlist Popup Menu");
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), titleMi);
    gtk_widget_set_name (titleMi, "CSS_playlistTitleMi");

    // ADD PLAYLIST TO EXISTING QUEUE
    GtkWidget *addPlaylistMi = gtk_menu_item_new_with_label("Add Playlist to MPD Queue");
    g_signal_connect(addPlaylistMi,  "activate",     (GCallback)   addPlaylist_signal,  playviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), addPlaylistMi);

    // REPLACE THE EXISTING QUEUE WITH NEW PLAYLIST
    GtkWidget *replacePlaylistMi = gtk_menu_item_new_with_label("Replace MPD Queue with Playlist");
    g_signal_connect(replacePlaylistMi,  "activate",     (GCallback)   replacePlaylist_signal,  playviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), replacePlaylistMi);

    // ***** GET MPD CONNECTION *****
    struct mpd_connection *conn;
    conn = get_connection(playviewVars, __FILE__, __func__, __LINE__);
    struct mpd_status * status;
    status = mpd_run_status(conn);

    GtkWidget *separatorMi1 = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), separatorMi1);

    // PLAYER START / PAUSE MENU ITEM
    GtkWidget *startPauseMi;
    if((mpd_status_get_state(status) == MPD_STATE_PAUSE) |  (mpd_status_get_state(status) == MPD_STATE_STOP))
    {
        startPauseMi = gtk_menu_item_new_with_label("Start Player");
        g_signal_connect(startPauseMi, "activate", (GCallback) startPlayer, playviewVars);
        gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), startPauseMi);
    }
    else
    {
        startPauseMi = gtk_menu_item_new_with_label("Pause Player");
        g_signal_connect(startPauseMi, "activate", (GCallback) pausePlayer, playviewVars);
        gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), startPauseMi);
    }

    // PLAYER STOP MENU ITEM
    GtkWidget *stopPlayerMi = gtk_menu_item_new_with_label("Stop Player");
    g_signal_connect(stopPlayerMi, "activate", (GCallback) stopPlayer, playviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), stopPlayerMi);

    GtkWidget *separatorMi2 = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), separatorMi2);

    // ***** AUDIO OUTPUT SUBMENU STARTS HERE ***** //
    GtkWidget *outputMenu = gtk_menu_new();
    GtkWidget *outputMi = gtk_menu_item_new_with_label("Audio Output");
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), outputMi);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(outputMi), outputMenu);

    mpd_send_outputs(conn);
    struct mpd_output *output;

    while ((output = mpd_recv_output(conn)) != NULL) {

        GtkWidget *audioOutputMi = gtk_check_menu_item_new_with_label((gchar *) mpd_output_get_name(output));
        gtk_menu_shell_append(GTK_MENU_SHELL(outputMenu), audioOutputMi);

        if(mpd_output_get_enabled(output))
            gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(audioOutputMi), TRUE);
        else
            gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(audioOutputMi), FALSE);

        mpd_output_free(output);
        g_signal_connect(audioOutputMi, "activate", (GCallback) toggle_mpd_output, playviewVars)	;
    }

    // ***** FREE MPD STATUS AND CONNECTION *****
    mpd_response_finish(conn);
    mpd_connection_free(conn);

    // ***** EXIT MAIN WINDOW MENU *****
    GtkWidget *separatorMi3 = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), separatorMi3);

    // HELP
    GtkWidget *helpMi = gtk_menu_item_new_with_label("Help");
    g_signal_connect(helpMi, "activate", (GCallback) helpMe_signal, playviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), helpMi);

    // CLEAR THE EXISTING PLAYLIST QUEUE
    GtkWidget *clearQueueMi = gtk_menu_item_new_with_label("Clear Queue Playlist");
    g_signal_connect(clearQueueMi,  "activate",     (GCallback)   clearPlaylist_signal,  playviewVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), clearQueueMi);

    // QUIT/EXIT
    GtkWidget *quitExitMi = gtk_menu_item_new_with_label("Quit (Exit)");
    g_signal_connect(quitExitMi, "activate", (GCallback) gtk_main_quit, window_main);
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), quitExitMi);

    // ***** NAME THE NODES ***** /
    gtk_widget_set_name (popupMenu, "CSS_popupMenu");
    gtk_widget_set_name (outputMenu, "CSS_outputMenu");

    gtk_widget_show_all(popupMenu);
    //    gtk_menu_popup_at_pointer (GTK_MENU(popupMenu), NULL);
    gtk_menu_popup_at_widget (GTK_MENU(popupMenu), GTK_WIDGET(playviewVars->w_window_main),
                              GDK_GRAVITY_CENTER,
                              GDK_GRAVITY_CENTER,
                              NULL);

    //printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
} // ** End of VIEW_POPUP_MENU

void toggle_mpd_output(GtkWidget *menuItem, app_variables *playviewVars)
{   //printf("\n\t\tInside toggle_mpd_output\n");
    struct mpd_connection *conn;
    conn = get_connection(playviewVars, __FILE__, __func__, __LINE__);
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

void startPlayer(GtkMenuItem *menuItem, app_variables *playviewVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    struct mpd_connection *conn;
    struct mpd_status * status;

    conn = get_connection(playviewVars, __FILE__, __func__, __LINE__);

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

void pausePlayer(GtkMenuItem *menuItem, app_variables *playviewVars)
{   //printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    startPlayer(menuItem, playviewVars);
    //	printf("Inside -- %s -- Line %d --\n", __func__, __LINE__);
    return;
}

//~ void stopPlayer(GtkMenuItem *menuItem, app_variables *playviewVars)
//~ {   //  printf("Inside -- %s -- Line %d --\n", __func__, __LINE__);

    //~ struct mpd_connection *conn;
    //~ struct mpd_status * status;
    //~ conn = get_connection(playviewVars, __FILE__, __func__, __LINE__);
    //~ mpd_send_status(conn);
    //~ status = mpd_recv_status(conn);

    //~ mpd_run_stop(conn);

    //~ mpd_status_free(status);
    //~ mpd_connection_free(conn);
    //~ //printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
//~ }

void stopPlayer(GtkMenuItem *menuItem, app_variables *playviewVars)
{   // printf("Inside -- %s -- Line %d --\n", __func__, __LINE__);
    struct mpd_connection *conn;
    struct mpd_status * status;
    conn = get_connection(playviewVars, __FILE__, __func__, __LINE__);
    mpd_send_status(conn);
    status = mpd_recv_status(conn);

    if(mpd_status_get_state(status) == MPD_STATE_STOP)
        mpd_run_play(conn);

    else
        mpd_run_stop(conn);

    mpd_status_free(status);
    mpd_connection_free(conn);
    //printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
}

void addPlaylist_signal	(GtkWidget *addPlaylistMi, app_variables *playviewVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    // GET MPD CONNECTION
    struct mpd_connection *conn;
    conn = get_connection(playviewVars, __FILE__, __func__, __LINE__);

    GtkTreeSelection *selection;
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(playviewVars->w_playviewTreeView));

    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar *value;

    // *** IF THE ROW IS CLICKED THEN GET THE NEW SELECTION
    gtk_tree_selection_get_selected(GTK_TREE_SELECTION( selection), &model, &iter); // WON'T WORK WITHOUT THIS
    // GET THE SELECTED PLAYLIST FROM THE LISTSTORE
    gtk_tree_model_get(model, &iter, PLAYVIEW_COL_ZERO, &value,  -1);

    mpd_run_load(conn, value);   // LOAD PLAYLIST

    // CLEAN UP BEFORE RETURNING
    mpd_connection_free(conn); // FREE MPD CONNECTION

    // AND SET HEADER TO PLAYLIST NAME
    GtkTreeViewColumn *col;
    col = gtk_tree_view_get_column (playviewVars->w_playviewTreeView, PLAYVIEW_COL_ZERO);
    gtk_tree_view_column_set_title(col, value);

    gchar *textStr = g_strdup_printf("Added Playlist to MPD Queue\n%s", value);
    showNotify (textStr, playviewVars);
    //~ spawnSimpleMessageDialog(textStr,  __LINE__, __FILE__, __func__, NOTIFY_MSG_MODE,  playviewVars);
    g_free(textStr);

    g_free(value); // FREE STRING
    // printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
}

void replacePlaylist_signal	(GtkWidget *replacePlaylistMi, app_variables *playviewVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    // GET MPD CONNECTION
    struct mpd_connection *conn;
    conn = get_connection(playviewVars, __FILE__, __func__, __LINE__);

    GtkTreeSelection *selection;

    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(playviewVars->w_playviewTreeView));

    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar *value;

    // *** IF THE ROW IS CLICKED THEN GET THE NEW SELECTION
    gtk_tree_selection_get_selected(GTK_TREE_SELECTION( selection), &model, &iter); // WON'T WORK WITHOUT THIS
    // GET THE SELECTED PLAYLIST FROM THE LISTSTORE
    gtk_tree_model_get(model, &iter, PLAYVIEW_COL_ZERO, &value,  -1);

    mpd_run_clear(conn);  // CLEAR THE QUEUE
    mpd_run_load(conn, value);   // LOAD PLAYLIST

    // CLEAN UP BEFORE RETURNING
    mpd_connection_free(conn); // FREE MPD CONNECTION

    // AND SET HEADER TO PLAYLIST NAME
    GtkTreeViewColumn *col;
    col = gtk_tree_view_get_column (playviewVars->w_playviewTreeView, PLAYVIEW_COL_ZERO);
    gtk_tree_view_column_set_title(col, value);

    gchar *textStr = g_strdup_printf("Replaced MPD Queue\n%s", value);
    showNotify (textStr, playviewVars);
    //~ spawnSimpleMessageDialog(textStr,  __LINE__, __FILE__, __func__, NOTIFY_MSG_MODE,  playviewVars);
    g_free(textStr);
    g_free(value); // FREE STRING
    // printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
}

void clearPlaylist_signal	(GtkWidget *clearPlaylistMi, app_variables *playviewVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    // SETUP MPD CONNECTION
    struct mpd_connection *conn;
    // GET MPD CONNECTION
    conn = get_connection(playviewVars, __FILE__, __func__, __LINE__);

    mpd_send_clear(conn);

    mpd_connection_free(conn);

    gchar *textStr = g_strdup("Cleared MPD Play Queue");
    showNotify (textStr, playviewVars);
    //~ spawnSimpleMessageDialog(textStr,  __LINE__, __FILE__, __func__, NOTIFY_MSG_MODE,  playviewVars);
    g_free(textStr);
}

void showErrMsgDialog (gchar *msg, gint line, gchar *file, const gchar *func, app_variables *playviewVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    GtkWidget *errorDialog;

    errorDialog = gtk_message_dialog_new_with_markup (NULL,  0,  GTK_MESSAGE_ERROR,  GTK_BUTTONS_CLOSE,\
                  "<span foreground='red' weight=\"bold\" size=\"larger\">%s\n\nFile %s\nFunction %s\nLine %d\nClick \"Close\" to exit.</span>",\
                  msg, file, func, line);

    gtk_window_set_title (GTK_WINDOW (errorDialog), "Error");
    gtk_dialog_run (GTK_DIALOG (errorDialog));
    gtk_widget_destroy (errorDialog);
    exit(0);
}

void showNotify (gchar *notifyMsg, app_variables *playviewVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
	gtk_label_set_text (GTK_LABEL (playviewVars->w_lblHostname), notifyMsg);
    g_timeout_add_seconds (playviewVars->notifyDialogTimeout,  (GSourceFunc) destroyNotifyDialog,  playviewVars);
}

gboolean destroyNotifyDialog(app_variables *playviewVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    gtk_label_set_text (GTK_LABEL (playviewVars->w_lblHostname), playviewVars->mpdHostname);
    return FALSE;
}


// READ HELP FROM FILE AND PUT ON THE LABEL
void helpMe_signal(GtkWidget *helpBtn, app_variables *playviewVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    GError *error = NULL;
    GBytes *bytes = NULL;
    gsize len;
    gconstpointer data = NULL;

    bytes = g_resources_lookup_data (HELP_RESOURCE, G_RESOURCE_LOOKUP_FLAGS_NONE, &error);
    gint atHelpLine = __LINE__  - 1;
    if(error)
    {
        showErrMsgDialog (error->message,  atHelpLine, __FILE__, __func__,  playviewVars);
        g_error_free(error);
        exit(0);
    }

    if (bytes != NULL)
        data = g_bytes_get_data (bytes, &len);

    g_markup_escape_text ((gchar *)data, -1);
    gtk_label_set_markup (GTK_LABEL (playviewVars->w_lblHelp), (gchar *)data);

    g_bytes_unref (bytes);

    if(gtk_widget_get_visible (playviewVars->w_scrollHelp)) {
        gtk_widget_hide(playviewVars->w_scrollHelp);
        gtk_widget_show(playviewVars->w_tv_scrollwin_playview);
    }
    else  {
        gtk_widget_show(playviewVars->w_scrollHelp);
        gtk_widget_hide(playviewVars->w_tv_scrollwin_playview);
    }
}

gchar * versionString(void)
{   //	printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    int  year, month, day, doy, woy, hour, minute, second;

    sscanf(VERSION_STR, "%u.%u.%u.%u.%u.%u.%u.%u", &year, &month, &day, &doy, &woy, &hour, &minute, &second);
    return g_strdup_printf("Version: %d.%02d.%03d.%05d -- Compiled: %02d:%02d:%d %02d:%02d:%02d", year, doy, woy, ((hour * 3600)  + (minute * 60) +  second ), month, day, year, hour, minute, second);

}

// LOAD THE KEYS FROM THE CONFIG FILE
void getConfigKeys(app_variables *playviewVars)
{   //	printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    g_autofree gchar *rtn_cssFile = NULL;
    g_autofree gchar *rtn_gladeFile = NULL;

    g_autofree gchar *rtn_mpdPortnumber = NULL;
    g_autofree gchar *rtn_mpdTimeout = NULL;
    g_autofree gchar *rtn_simpleMsgDialog = NULL;

    g_autoptr(GError) error = NULL;
    g_autoptr(GKeyFile) keyFile = g_key_file_new ();

    gint atKeyLine;

    // GET THE KEY FILE. this is hard coded to CDW/playview.conf
    playviewVars->cfgFile = g_strdup_printf("%s/%s.conf", playviewVars->cwdPath,  g_get_prgname ());

    // IF THE CONF FILE EXIS IN CWD, USE THE EXTERNAL KEY FILE
    if(g_file_test (playviewVars->cfgFile, G_FILE_TEST_IS_REGULAR) == TRUE)
    {
        g_key_file_load_from_file(keyFile, playviewVars->cfgFile, G_KEY_FILE_NONE, &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  goto keyFileError;

        if(globalVerbose > 0)  printf("Line %d -- EXTERNAL CONFIG FILE\n", __LINE__);
        if(globalVerbose > 0)  printf("Line %d -- CONFIG FILE %s\n", __LINE__, playviewVars->cfgFile);
    }
    else  // OTHERWISE USE THE INTERNAL KEY FILE FROM RESOURCES
    {
        g_free(playviewVars->cfgFile);
        playviewVars->cfgFile = NULL;
        playviewVars->cfgFile = g_strdup(CONFIG_RESOURCE);

        g_autoptr(GBytes) bytes = g_resources_lookup_data (playviewVars->cfgFile, G_RESOURCE_LOOKUP_FLAGS_NONE, &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  goto keyFileError;
        g_key_file_load_from_bytes (keyFile, bytes,G_KEY_FILE_NONE, &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  goto keyFileError;

        if(globalVerbose > 0)  printf("Line %d -- INTERNAL CONFIG FILE\n", __LINE__);
        if(globalVerbose > 0)  printf("Line %d -- CONFIG FILE %s\n", __LINE__, CONFIG_RESOURCE);
    }

    // THIS IS MOSTLY FOR DEBUGGING TO DETERMINE WHICH KEY FILE I'M USING
    // THIS KEY WILL NOT FAIL EVEN IF VARIABLE IS MISSING IN THE KEY FILE
	if(globalVerbose > 1)
    {
        g_autofree gchar *whatAmI =  g_key_file_get_string(keyFile,  "Playiew Configuration", "whatAmI", NULL);
        if(whatAmI && (strlen(whatAmI) != 0))
            printf("\nLine %d -- What am I ? %s\n", __LINE__, whatAmI);
    }

    // MPD HOSTNAME
	if(playviewVars->mpdHostname == NULL)  // IF NULL THEN THERE WAS NO COMMAND LINE OPTION
    {
        playviewVars->mpdHostname = g_key_file_get_string(keyFile,  "Playview Configuration", "mpdHostname", &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  goto keyFileError;
    }
    //printf("Line %d -- HOSTNAME %s -- PORT %d -- TIMEOUT %d\n", __LINE__, playviewVars->mpdHostname,  playviewVars->mpdPortnumber, playviewVars->mpdTimeout);

    // MPD PORT NUMBER -IT'S EASIER TO DO IT AS A STRING RATHER THAN INTEGER
    if(playviewVars->mpdPortnumber < 0) // IF NULL THEN THERE WAS NO COMMAND LINE OPTION
    {
        playviewVars->mpdPortnumber = g_key_file_get_integer(keyFile, "Playview Configuration", "mpdPortnumber", &error);
        atKeyLine =  __LINE__ - 1;
        if(error)	goto keyFileError;
    }


    //printf("Line %d -- HOSTNAME %s -- PORT %d -- TIMEOUT %d\n", __LINE__, playviewVars->mpdHostname,  playviewVars->mpdPortnumber, playviewVars->mpdTimeout);
    // MPD TIMEOUT -IT'S EASIER TO DO IT AS A STRING RATHER THAN INTEGER
    if(playviewVars->mpdTimeout < 0) // IF NULL THEN THERE WAS NO COMMAND LINE OPTION
    {
        playviewVars->mpdTimeout = g_key_file_get_integer(keyFile, "Playview Configuration", "mpdTimeout", &error);
        atKeyLine =  __LINE__ - 1;
        if(error)	goto keyFileError;
    }
    //printf("Line %d -- HOSTNAME %s -- PORT %d -- TIMEOUT %d\n", __LINE__, playviewVars->mpdHostname,  playviewVars->mpdPortnumber, playviewVars->mpdTimeout);

    //**************************************************************************

    // GLADE FILE NAME -- ALLOW THAT THE FILENAME MAY BE ON THE COMMAND LINE
    if(playviewVars->gladeFile == NULL)   // IF THE VARIABLE IS NULL THEN WE TRY TO READ IT FROM THE KEY FILE
    {
        rtn_gladeFile = g_key_file_get_string(keyFile, "Playview Configuration", "playViewGladeFile",   &error);
        atKeyLine =  __LINE__ - 1;

        if(error) 	goto keyFileError; // IF KEY DOES NOT EXIST, THEN FATAL ERROR, EXIT

        // IF FILENAME IS NOT AN ABSOLUTE PATH/FILE OR DOES NOT EXIST, THROW AND ERROR AND EXIT
        if (strlen(rtn_gladeFile) != 0)
        {
            // IF FILE EXIST THEN ASSIGN IT TO VARIABLE
            playviewVars->gladeFile = g_strdup(rtn_gladeFile);

            if(globalVerbose > 0)  printf("\nLine %d -- EXTERNAL GLADE FILE\n", __LINE__);
            if(globalVerbose > 0)  printf("Line %d -- GLADE FILE %s\n", __LINE__, playviewVars->gladeFile);
        }

        // IF KEY IS EMPTY (ZERO STRING LEN), THEN ASSIGN INTERNAL RESOURCE TO VARIABLE
        else {
            if(globalVerbose > 0)  printf("\nLine %d -- INTERNAL GLADE FILE\n", __LINE__);
            if(globalVerbose > 0)  printf("Line %d -- GLADE FILE %s\n", __LINE__, GLADE_RESOURCE);
            playviewVars->gladeFile = g_strdup(GLADE_RESOURCE);
        }
    }

    // CSS FILE NAME
    if(playviewVars->cssFile == NULL)   // IF THE VARIABLE IS NULL THEN WE TRY TO READ IT FROM THE KEY FILE
    {
        rtn_cssFile = g_key_file_get_string(keyFile, "Playview Configuration", "playViewCssFile",   &error);
        atKeyLine =  __LINE__ - 1;

        if(error) 	goto keyFileError; // IF KEY DOES NOT EXIST, THEN FATAL ERROR, EXIT

        // IF FILENAME IS NOT AN ABSOLUTE PATH/FILE OR DOES NOT EXIST, THROW AND ERROR AND EXIT
        if (strlen(rtn_cssFile) != 0)
        {
            // IF FILE EXIST THEN ASSIGN IT TO VARIABLE
            playviewVars->cssFile = g_strdup(rtn_cssFile);

            if(globalVerbose > 0)  printf("\nLine %d -- EXTERNAL CSS FILE\n", __LINE__);
            if(globalVerbose > 0)  printf("Line %d -- CSS FILE %s\n", __LINE__, playviewVars->cssFile);
        }

        // IF KEY IS EMPTY (ZERO STRING LEN), THEN ASSIGN INTERNAL RESOURCE TO VARIABLE
        else {
            if(globalVerbose > 0)  printf("\nLine %d -- INTERNAL CSS FILE\n", __LINE__);
            if(globalVerbose > 0)  printf("Line %d -- CSS FILE %s\n", __LINE__, CSS_RESOURCE);
            playviewVars->cssFile = g_strdup(CSS_RESOURCE);
        }
    }


    // WINDOW TITLE
    if(playviewVars->windowTitle == NULL)
    {
        playviewVars->windowTitle = g_key_file_get_string(keyFile, "Playview Configuration", "playViewWindowTitle", &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  	goto keyFileError;
    }

    // WINDOW HEIGHT
    if(playviewVars->winHeight < 0)
    {
        playviewVars->winHeight = g_key_file_get_integer(keyFile, "Playview Configuration", "playViewHeight", &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  	goto keyFileError;
    }

    // WINDOW WIDTH
    if(playviewVars->winWidth < 0)
    {
        playviewVars->winWidth = g_key_file_get_integer(keyFile, "Playview Configuration", "playViewWidth", &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  	goto keyFileError;
    }

    // NOTIFY TIMEOUT
    playviewVars->notifyDialogTimeout = g_key_file_get_integer(keyFile, "Playview Configuration", "timeoutNotifyDialog", &error);
    atKeyLine =  __LINE__ - 1;
    if(error)  	goto keyFileError;

    // FONT STRING
    if(playviewVars->dialogFont == NULL)
    {
        playviewVars->dialogFont = g_key_file_get_string(keyFile, "Playview Configuration", "dialogFontString", &error);
        atKeyLine =  __LINE__ - 1;
        if(error)  	goto keyFileError;
    }

    // END OF GET KEYS

    if (error) {
        g_error_free(error);
    }

    return;

keyFileError:
    ;

    showErrMsgDialog (error->message,  atKeyLine, __FILE__, __func__,  playviewVars);
    if(error) g_error_free(error);
    exit(0);

}  // End of getKeyVarsgetConfigKeys

void initilizeGladeWidgets(app_variables *playviewVars)
{   //	printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    if(globalVerbose > 1)
    {
        printf("Line %d -- INITILIZE GLADE VARIABLES\n", __LINE__);
        printf("GLADE FILE %s\n", playviewVars->gladeFile);
    }


    // ****** INITALIZE THE GLADE VARIABLES ******
    g_autoptr(GError) error = NULL;
    gint atLine;
    GtkBuilder  *builder = gtk_builder_new();

    // LOAD GLADE FROM INTERNAL RESOURCCE
    if(0 == g_strcmp0(playviewVars->gladeFile, GLADE_RESOURCE))
    {
        atLine = __LINE__ + 1;
        gtk_builder_add_from_resource (builder, GLADE_RESOURCE, &error);
        if(error)
            showErrMsgDialog (error->message,  atLine, __FILE__, __func__,  playviewVars);
    }
    // LOAD GLADE FROM EXTERNAL FILE
    else
    {
        atLine = __LINE__ + 1;
        checkKeyPathFile("Glade File", atLine, playviewVars->gladeFile, playviewVars);
        atLine = __LINE__ + 1;
        gtk_builder_add_from_file (builder, playviewVars->gladeFile, &error);
        if(error)
            showErrMsgDialog (error->message,  atLine, __FILE__, __func__,  playviewVars);
    }

    //******* INITILAZE THE WIDGETS *********
    GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    playviewVars->w_window_main = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    playviewVars->w_lblExit    = GTK_WIDGET(gtk_builder_get_object(builder, "lblExit"));
    playviewVars->w_lblHostname    = GTK_WIDGET(gtk_builder_get_object(builder, "lblHostname"));
    playviewVars->w_box_0 = GTK_WIDGET(gtk_builder_get_object(builder, "box_0"));
    playviewVars->w_playviewListStore = GTK_LIST_STORE (gtk_builder_get_object (builder, "playviewListStore"));
    playviewVars->w_playviewTreeView   = GTK_TREE_VIEW (gtk_builder_get_object (builder, "playviewTreeView"));

    playviewVars->w_scrollHelp    = GTK_WIDGET(gtk_builder_get_object(builder, "scrollHelp"));
    playviewVars->w_tv_scrollwin_playview    = GTK_WIDGET(gtk_builder_get_object(builder, "tv_scrollwin_playview"));

    playviewVars->w_lblHelp    = GTK_LABEL(gtk_builder_get_object(builder, "lblHelp"));

    playviewVars->playviewFirstPass = 1;

    // CONNECT SIGNALS
    gtk_builder_connect_signals(builder, playviewVars);
    g_clear_object(&builder);

    // SET WINDOW TITLE
    gtk_window_set_title (GTK_WINDOW (playviewVars->w_window_main), playviewVars->windowTitle);

    // SET THE WINDOW WIDTH AND HEIGHT
    g_object_set (playviewVars->w_window_main, "default-width", playviewVars->winWidth, NULL);
    g_object_set (playviewVars->w_window_main, "default-height", playviewVars->winHeight, NULL);

    // SET SET STUFF IN THE HOSTNAME MSG LABEL
    gtk_label_set_text (GTK_LABEL (playviewVars->w_lblHostname), playviewVars->mpdHostname);

    if(globalVerbose > 1)
    {	printf("\n\n");
        printf("Line %d -- playviewVars->mpdHostname = %s\n", __LINE__, playviewVars->mpdHostname);
        printf("Line %d -- playviewVars->mpdPortnumber = %d\n", __LINE__, playviewVars->mpdPortnumber);
        printf("Line %d -- playviewVars->mpdTimeout = %d\n", __LINE__, playviewVars->mpdTimeout);
        printf("Line %d -- playviewVars->winWidth = %d\n", __LINE__, playviewVars->winWidth);
        printf("Line %d -- playviewVars->winHeight = %d\n", __LINE__, playviewVars->winHeight);
        printf("Line %d -- playviewVars->dialogFont = %s\n", __LINE__, playviewVars->dialogFont);
        printf("Line %d -- playviewVars->windowTitle = %s\n", __LINE__, playviewVars->windowTitle);
        printf("Line %d -- playviewVars->notifyDialogTimeout = %d\n", __LINE__, playviewVars->notifyDialogTimeout);

        printf("Line %d -- playviewVars->cfgFile = %s\n", __LINE__, playviewVars->cfgFile);
        printf("Line %d -- playviewVars->gladeFile = %s\n", __LINE__, playviewVars->gladeFile);
        printf("Line %d -- playviewVars->cssFile = %s\n", __LINE__, playviewVars->cssFile);
    }

} //End of initilizeGladeVariable
//******* End of INITILAZE THE WIDGETS *********

int checkKeyPathFile(gchar *keyName, gint atLine, gchar *fileName, app_variables *playviewVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    //~ gint atKeyLine;
    g_autofree gchar *txtStr = NULL;
    g_autoptr(GError) error = NULL;

    // IS THE PATH/FILENAME ABSOLUTE?
    if(g_path_is_absolute (fileName) == 0) { //    printf("FILENAME %s\n", fileName);
        //~ atKeyLine = __LINE__ - 1;
        g_set_error (&error, 0, 0, "Key file variable -- %s --\n-- %s -- is not a valid filename.\nFilename must be an absolute path/filename.", keyName, fileName);

        goto fileCheckKey;
    }

    // DOES THE FILE EXIST?
    if(g_file_test (fileName, G_FILE_TEST_IS_REGULAR) == 0) {   //     printf("FILENAME %s\n", fileName);
        //~ atKeyLine = __LINE__ - 1;
        g_set_error (&error, 0, 0, "Key file variable -- %s --\nFile\n-- %s --\nNot found.", keyName, fileName);
        goto fileCheckKey;
    }

    return 1;

fileCheckKey:
    ;

    showErrMsgDialog (error->message,  atLine, __FILE__, __func__,  playviewVars);

    exit(0);
}  // End of checkKeyPathFile

void getTheOptions(int argc, gchar **argv, app_variables *playviewVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

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

    //////////////////////////////////////////////////////////////////
    // SAVE ARGV PRINT!!!
    if(globalVerbose > 0)
    {
        printf("\nLine %d -- ARGC = %d\n",__LINE__,  argc);
        printf("Line %d -- Command line = ", __LINE__);
        int x = 0;
        while(argv[x] != NULL)
        {
            printf("%s ", argv[x]);
            x++;
        }
        printf("\n\n");
    }

    // PROCESS THE COMMAND LINE ARGUMENTS
    int loop = 0;

    while ((loop = getopt_long(argc, argv, optstring, options_long, NULL))) {

        switch (loop) {  // END OF OPTIONS
        case -1:

            return;
            break;

        case 1:
            //~ if(globalVerbose) printf("Line %d -- CASE 1 = %s\n", __LINE__, optarg);
            break;

        case '2': // HOSTNAME
            //~ if(globalVerbose) printf("Line %d -- HOST\n", __LINE__);
            playviewVars->mpdHostname = g_strdup(optarg);;
            break;

        case '3':  // MPD PORT NUMBER
            //~ if(globalVerbose) printf("Line %d -- PORT\n", __LINE__);
            playviewVars->mpdPortnumber = (int) g_ascii_strtoll(optarg, NULL, 10);
            break;

        case '4': // MPD TIMEOUT
            //~ if(globalVerbose) printf("Line %d -- TIMEOUT\n", __LINE__);
            playviewVars->mpdTimeout = (int) g_ascii_strtoll(optarg, NULL, 10);
            break;

        case '5':  // WIDTH
            //~ if(globalVerbose) printf("Line %d -- WIDTH\n", __LINE__);
            playviewVars->winWidth = (int) g_ascii_strtoll(optarg, NULL, 10);
            break;

        case '6':  //HEIGHT
            //~ if(globalVerbose) printf("Line %d -- HEIGHT\n", __LINE__);
            playviewVars->winHeight = (int) g_ascii_strtoll(optarg, NULL, 10);
            break;

        case '7':  // FONT
            //~ if(globalVerbose) printf("Line %d -- FONT\n", __LINE__);
            playviewVars->dialogFont = g_strdup(optarg); // DEFAULT DIALOG FONT
            break;

        case '8': // TITLE
            //~ if(globalVerbose)printf("Line %d -- TITLE\n", __LINE__);
            playviewVars->windowTitle = g_strdup(optarg);
            break;
        }
    }

} //End of getTheOptions

// ********* THIS LOADS THE CSS STYLE OVERRIDE OF THE THEME *********
void loadCCSProvider(app_variables *playviewVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    //~ if(globalVerbose) printf("Line %d -- CSS FILE %s\n", __LINE__, playviewVars->cssFile);

    // LOAD THE CSS FILE
    GtkCssProvider *provider = gtk_css_provider_new ();
    GdkDisplay *display = gdk_display_get_default ();
    GdkScreen *screen = gdk_display_get_default_screen (display);
    gtk_style_context_add_provider_for_screen (screen, GTK_STYLE_PROVIDER (provider), \
            GTK_STYLE_PROVIDER_PRIORITY_USER);

    g_autoptr(GError) error = NULL;
    gint atLine;
    // LOAD CSS FROM INTERNAL RESOURCE
    if(0 == g_strcmp0(playviewVars->cssFile, CSS_RESOURCE))
    {
        atLine = __LINE__ + 1;
        gtk_css_provider_load_from_resource (provider, CSS_RESOURCE);
        if(error)
            showErrMsgDialog (error->message,  atLine, __FILE__, __func__,  playviewVars);
    }
    // LOAD CSS FROM EXTERAL FILE
    else
    {
        atLine = __LINE__ + 1;
        checkKeyPathFile("Css File", atLine, playviewVars->cssFile, playviewVars);
        atLine = __LINE__ + 1;
        gtk_css_provider_load_from_path(provider, playviewVars->cssFile, &error);
        if(error)
            showErrMsgDialog (error->message,  atLine, __FILE__, __func__,  playviewVars);
    }

    // SET WINDOW FONT IF ON THE COMMAND LINE
    if(playviewVars->dialogFont)
    {
        g_autofree gchar *windowFont = g_strdup_printf("window {font: %s; }\n", playviewVars->dialogFont);
		if(globalVerbose > 1)  printf("Line %d -- WINDOW FONT STRING %s\n", __LINE__, windowFont);

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

