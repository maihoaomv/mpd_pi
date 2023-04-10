// popup_menu.c
// NOTE: popup menu is done with setting signals "button-press-event"(mouse RIGHT CLICK) and "popup-menu"(keyboard SHIFT-F10) in the GLADE window_main signals
// SIGNAL "BUTTON-PRESS-EVENT"

#include "mpd_pi.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

extern gint globalUpdateTrackCenter;
extern gint globalUpdateSongview;

// RIGHT CLICK POPUP MENU CALLBACKS
void view_popup_menu (GtkWidget *window_main, GdkEventButton *event, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    // ***** TOOGLE HELP DIALOG SUBMENU STARTS HERE ***** //
    GtkWidget *popupMenu, *menuItem;


    popupMenu = gtk_menu_new();

    // THIS ONLY A LABEL TO IDENTIFY THIS POPUP MENU
    GtkWidget *titleMi = gtk_menu_item_new_with_label("Popup Menu");
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), titleMi);
    gtk_widget_set_name (titleMi, "CSS_Popup_Menu_TitleMi");

    // ***** HIDE / SHOW SUBMENU STARTS HERE ***** //
    //***** MODE SUBMENU SETUP ***** THIS WORKS -- DO NOT CHANGE *****
    GtkWidget *displayMenu;							//GtkWidget *modeMenu;
    GtkWidget *displayMi;								//GtkWidget *modeMi;
    //~ GtkWidget *hostNameMi;						//GtkWidget *repeatMi;
    GtkWidget *playerControlsMi;					//GtkWidget *randomMi;
    GtkWidget *statusDisplayMi;						//GtkWidget *singleMi;

    displayMenu = gtk_menu_new();
    displayMi = gtk_menu_item_new_with_label("Display Options");

    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), displayMi);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(displayMi), displayMenu);

    // ***** TOGGLE PLAYER CONTROLS DISPLAY *****
    playerControlsMi = gtk_check_menu_item_new_with_label("Player Controls");
    gtk_menu_shell_append(GTK_MENU_SHELL(displayMenu), playerControlsMi);

    // ***** TOGGLE STATUS DISPLAY *****
    statusDisplayMi = gtk_check_menu_item_new_with_label("Status");
    gtk_menu_shell_append(GTK_MENU_SHELL(displayMenu), statusDisplayMi);

    // ***** SET CHECK MARKS *****
    if (gtk_widget_get_visible (GTK_WIDGET (mpd_piVars->w_btnBoxPlayerControls)))
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(playerControlsMi), TRUE);
    else  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(playerControlsMi), FALSE);

    if (gtk_widget_get_visible (GTK_WIDGET (mpd_piVars->w_boxProgressBars)))
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(statusDisplayMi), TRUE);
    else  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(statusDisplayMi), FALSE);

    // ***** SET SIGNALS HERE -DO NOT MOVE ******
    g_signal_connect(playerControlsMi,  "activate", (GCallback)  toggle_PlayerDisplay,  mpd_piVars);
    g_signal_connect(statusDisplayMi,  "activate",  (GCallback)  toggle_ProgressDisplay,  mpd_piVars);
    //***** End of TOGGLE SUBMENU ITEMS ***** THIS WORKS -- DO NOT CHANGE *****
    // ***** HIDE / SHOW SUBMENU ENDS HERE ***** //
    ///////////////////////////////////////////////////////////////////////////////

    // ***** PLAYER MODE SUBMENU STARTS HERE ***** //
    //***** MODE SUBMENU SETUP ***** THIS WORKS -- DO NOT CHANGE *****
    GtkWidget *modeMenu;
    GtkWidget *modeMi;
    GtkWidget *repeatMi;
    GtkWidget *randomMi;
    GtkWidget *singleMi;
    GtkWidget *consumeMi;

    modeMenu = gtk_menu_new();
    modeMi = gtk_menu_item_new_with_label("Player Mode");

    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), modeMi);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(modeMi), modeMenu);
    // ***** DON'T NEED A SIGNAL FOR MODE MENU *****

    // ***** TOGGLE REPEAT MODE *****
    repeatMi = gtk_check_menu_item_new_with_label("Repeat");
    gtk_menu_shell_append(GTK_MENU_SHELL(modeMenu), repeatMi);

    // ***** TOGGLE RANDOM MODE *****
    randomMi = gtk_check_menu_item_new_with_label("Random");
    gtk_menu_shell_append(GTK_MENU_SHELL(modeMenu), randomMi);

    // ***** TOGGLE SINGLE MODE *****
    singleMi = gtk_check_menu_item_new_with_label("Single");
    gtk_menu_shell_append(GTK_MENU_SHELL(modeMenu), singleMi);

    // ***** TOGGLE CONSUME MODE *****
    consumeMi = gtk_check_menu_item_new_with_label("Consume");
    gtk_menu_shell_append(GTK_MENU_SHELL(modeMenu), consumeMi);

    // ***** GET MPD CONNECTION *****
    struct mpd_connection *conn;
    conn = get_connection(mpd_piVars);
    struct mpd_status * status;
    mpd_send_status(conn);
    status = mpd_recv_status(conn);

    // ***** SET MODE CHECK MARKS *****
    if (mpd_status_get_repeat(status))  // REPEAT
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(repeatMi), TRUE);
    else  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(repeatMi), FALSE);

    if (mpd_status_get_random(status))  // RANDOM
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(randomMi), TRUE);
    else  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(randomMi), FALSE);

    if (mpd_status_get_single(status))  // SINGLE
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(singleMi), TRUE);
    else  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(singleMi), FALSE);

    if (mpd_status_get_consume(status))  // CONSUME
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(consumeMi), TRUE);
    else  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(consumeMi), FALSE);

    // ***** SET SIGNALS HERE -DO NOT MOVE ******
    g_signal_connect(repeatMi,  "activate", (GCallback) mode_repeat,  mpd_piVars);
    g_signal_connect(randomMi,  "activate", (GCallback) mode_random,  mpd_piVars);
    g_signal_connect(singleMi,  "activate", (GCallback) mode_single,  mpd_piVars);
    g_signal_connect(consumeMi, "activate", (GCallback) mode_consume, mpd_piVars);
    //***** End of MODE SUBMENU SETUP ***** THIS WORKS -- DO NOT CHANGE *****

    // ***** SCREEN SIZE SUBMENU STARTS HERE ***** //
    //***** SCREEN SUBMENU SETUP ***** THIS WORKS -- DO NOT CHANGE *****
    GtkWidget *screenMenu;
    GtkWidget *screenMi;
    GtkWidget *maximizeMi;
    GtkWidget *minimizeMi;
    GtkWidget *fullscreenMi;
    GtkWidget *normalMi;

    screenMenu = gtk_menu_new();
    screenMi = gtk_menu_item_new_with_label("Screen Size");

    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), screenMi);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(screenMi), screenMenu);
    // ***** DON'T NEED A SIGNAL FOR VIEW MENU *****

    // ***** FULL SCREEN *****
    fullscreenMi = gtk_check_menu_item_new_with_label("Fullscreen");
    gtk_menu_shell_append(GTK_MENU_SHELL(screenMenu), fullscreenMi);

    // ***** NORMAL SCREEN *****
    normalMi = gtk_check_menu_item_new_with_label("Normal");
    gtk_menu_shell_append(GTK_MENU_SHELL(screenMenu), normalMi);

    // ***** MAXIMIXE SCREEN *****
    maximizeMi = gtk_check_menu_item_new_with_label("Maximize");
    gtk_menu_shell_append(GTK_MENU_SHELL(screenMenu), maximizeMi);

    // ***** MINIMIZE (ICONIFY) SCREEN *****
    minimizeMi = gtk_check_menu_item_new_with_label("Minimize");
    gtk_menu_shell_append(GTK_MENU_SHELL(screenMenu), minimizeMi);

    GdkWindowState state = gdk_window_get_state(gtk_widget_get_window(GTK_WIDGET(mpd_piVars->w_window_main)));

    if(0 == (state  & (GDK_WINDOW_STATE_MAXIMIZED | GDK_WINDOW_STATE_FULLSCREEN | GDK_WINDOW_STATE_ICONIFIED)))
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(normalMi), TRUE);
    else  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(normalMi), FALSE);

    if(state & GDK_WINDOW_STATE_MAXIMIZED)
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(maximizeMi), TRUE);
    else  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(maximizeMi), FALSE);

    if(state & GDK_WINDOW_STATE_FULLSCREEN)
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(fullscreenMi), TRUE);
    else  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(fullscreenMi), FALSE);

    if(state & GDK_WINDOW_STATE_ICONIFIED)
        gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(minimizeMi), TRUE);
    else  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(minimizeMi), FALSE);

    g_signal_connect(fullscreenMi,  "activate", (GCallback) setFullscreen, mpd_piVars);
    g_signal_connect(normalMi,  "activate", (GCallback) setNormal, mpd_piVars);
    g_signal_connect(maximizeMi,  "activate", (GCallback) setMaximize, mpd_piVars);
    g_signal_connect(minimizeMi,  "activate", (GCallback) setMinimize, mpd_piVars);
    //***** End of SCREEN SUBMENU SETUP ***** THIS WORKS -- DO NOT CHANGE *****

    // ***** AUDIO OUTPUT SUBMENU STARTS HERE ***** //
    //***** OUTPUT SUBMENU SETUP ***** THIS WORKS -- DO NOT CHANGE *****
    GtkWidget *outputMenu;
    GtkWidget *outputMi;

    outputMenu = gtk_menu_new();
    outputMi = gtk_menu_item_new_with_label("Audio Output");

    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), outputMi);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(outputMi), outputMenu);

    mpd_send_outputs(conn);

    struct mpd_output *output;
    while ((output = mpd_recv_output(conn)) != NULL) {

        menuItem = gtk_check_menu_item_new_with_label((gchar *) mpd_output_get_name(output));
        gtk_menu_shell_append(GTK_MENU_SHELL(outputMenu), menuItem);

        if(mpd_output_get_enabled(output))
            gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuItem), TRUE);
        else  gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(menuItem), FALSE);

        mpd_output_free(output);
        g_signal_connect(menuItem, "activate", (GCallback) toggle_mpd_output, mpd_piVars)	;
    }

    GtkWidget *separatorMi1 = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), separatorMi1);
    //***** End of OUTPUT SUBMENU SETUP ***** THIS WORKS -- DO NOT CHANGE *****

    //~ // **** CREATE/OVERWRITE NEW CONFIG KEY FILE
    //~ menuItem = gtk_menu_item_new_with_label("Create Config File");
    //~ g_signal_connect(menuItem, "activate", (GCallback) createKeyFile, mpd_piVars);
    //~ gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);

    // UPDATE DATABASE
    menuItem = gtk_menu_item_new_with_label("Update Database");
    g_signal_connect(menuItem, "activate", (GCallback) upDateDatabase, mpd_piVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);

    GtkWidget *separatorMi2 = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), separatorMi2);
    // PLAYER PLAY MENU ITEM
    if(mpd_status_get_state(status) == MPD_STATE_PLAY)
    {
        menuItem = gtk_menu_item_new_with_label("Pause Player");
        g_signal_connect(menuItem, "activate", (GCallback) popupMenu_startPlayer, mpd_piVars);
        gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);

        menuItem = gtk_menu_item_new_with_label("Stop Player");
        g_signal_connect(menuItem, "activate", (GCallback) popupMenu_stopPlayer, mpd_piVars);
        gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);

        menuItem = gtk_menu_item_new_with_label("Next Track");
        g_signal_connect(menuItem, "activate", (GCallback) popupMenu_nextTrack, mpd_piVars);
        gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);

        menuItem = gtk_menu_item_new_with_label("Previous Track");
        g_signal_connect(menuItem, "activate", (GCallback) popupMenu_previousTrack, mpd_piVars);
        gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);

        menuItem = gtk_menu_item_new_with_label("Restart Track");
        g_signal_connect(menuItem, "activate", (GCallback) popupMenu_restartTrack, mpd_piVars);
        gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);

        menuItem = gtk_menu_item_new_with_label("Increment Queue");
        g_signal_connect(menuItem, "activate", (GCallback) popupMenu_incrementQueue, mpd_piVars);
        gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);

        menuItem = gtk_menu_item_new_with_label("Decrement Queue");
        g_signal_connect(menuItem, "activate", (GCallback) popupMenu_decrementQueue, mpd_piVars);
        gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);
    }

    // PLAYER PAUSE MENU ITEM
    if(mpd_status_get_state(status) == MPD_STATE_PAUSE)
    {
        menuItem = gtk_menu_item_new_with_label("Start Player");
        g_signal_connect(menuItem, "activate", (GCallback) popupMenu_startPlayer, mpd_piVars);
        gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);

        menuItem = gtk_menu_item_new_with_label("Stop Player");
        g_signal_connect(menuItem, "activate", (GCallback) popupMenu_stopPlayer, mpd_piVars);
        gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);
    }

    // PLAYER STOP MENU ITEM
    if(mpd_status_get_state(status) == MPD_STATE_STOP)
    {
        menuItem = gtk_menu_item_new_with_label("Start Player");
        g_signal_connect(menuItem, "activate", (GCallback) popupMenu_stopPlayer, mpd_piVars);
        gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);
    }

    mpd_response_finish(conn);
    mpd_status_free(status);
    mpd_connection_free(conn);

    GtkWidget *separatorMi3 = gtk_separator_menu_item_new();
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), separatorMi3);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//~ linenum;	printf("Show View Mode %d\n", mpd_piVars->showViewMode);
//~ linenum;	printf("Show View Mode %s\n", (mpd_piVars->showViewMode) ? "TRACKVIEW" : "SONGVIEW");
	// TOGGLE VIEW MODE
//~ linenum;
    if(mpd_piVars->showViewMode == SHOW_TRACKVIEW_MODE)
	{
        menuItem = gtk_menu_item_new_with_label("Songview Mode");
        //~ g_signal_connect(menuItem, "button-press-event", (GCallback) toggleViewMode, mpd_piVars);
        g_signal_connect(menuItem, "activate", (GCallback) toggleViewMode, mpd_piVars);
        gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);
	}
//~ linenum;
	if(mpd_piVars->showViewMode == SHOW_SONGVIEW_MODE)
	{
		menuItem = gtk_menu_item_new_with_label("Trackview Mode");
		//~ g_signal_connect(menuItem, "button-press-event", (GCallback) toggleViewMode, mpd_piVars);
		g_signal_connect(menuItem, "activate", (GCallback) toggleViewMode, mpd_piVars);
		gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);
	}
//~ linenum;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ***** PLAYVIEW HELPER DIALOG ***** //
    menuItem = gtk_menu_item_new_with_label("Playview Dialog");
    g_signal_connect(menuItem, "activate", (GCallback) spawn_PlayviewDialog, mpd_piVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);

    // ***** DATAVIEW HELPER DIALOG ***** //
    menuItem = gtk_menu_item_new_with_label("Dataview Dialog");
    g_signal_connect(menuItem, "activate", (GCallback) spawn_DataviewDialog, mpd_piVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);

    // ***** HELP POPUP ***** //
    menuItem = gtk_menu_item_new_with_label("Help Dialog");
    g_signal_connect(menuItem, "activate", (GCallback) popupHelpMessage, mpd_piVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);

    //~ // ***** STATUS DIALOG ***** //
    //~ menuItem = gtk_menu_item_new_with_label("Status Dialog");
    //~ g_signal_connect(menuItem, "activate", (GCallback) popupStatusMessage, mpd_piVars);
    //~ gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);

    // ***** CENTER TRACK MENU *****
    menuItem = gtk_menu_item_new_with_label("Center Track");
    g_signal_connect(menuItem, "button-press-event", (GCallback) centerTrackClickEvent, mpd_piVars);
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);

    // ***** EXIT MAIN WINDOW MENU *****
    menuItem = gtk_menu_item_new_with_label("Quit (Exit)");
    g_signal_connect(menuItem, "activate", (GCallback) gtk_main_quit, window_main);
    gtk_menu_shell_append(GTK_MENU_SHELL(popupMenu), menuItem);

    // ***** NAME THE NODES ***** /
    // KEEP!! KEEP!! THIS ALLOWS ME TO COLOR THE POPUP MENU
    gtk_widget_set_name (popupMenu, "CSS_popupMenu");
    gtk_widget_set_name (displayMenu, "CSS_displayMenu");
    gtk_widget_set_name (modeMenu, "CSS_modeMenu");
    gtk_widget_set_name (screenMenu, "CSS_screenMenu");
    gtk_widget_set_name (outputMenu, "CSS_outputMenu");

    gtk_widget_show_all(popupMenu);
    gtk_menu_popup_at_pointer (GTK_MENU(popupMenu), NULL);

//~ linenum;

} // ** End of VIEW_POPUP_MENU

void mode_repeat(GtkWidget *repeatMi, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    struct mpd_connection *conn = get_connection(mpd_piVars);

    struct mpd_status * status;
    mpd_send_status(conn);
    status = mpd_recv_status(conn);

    if (mpd_status_get_repeat(status)) mpd_send_repeat (conn, FALSE);
    else mpd_send_repeat (conn, TRUE);

    mpd_status_free(status);
    mpd_connection_free(conn);
}

void mode_random(GtkWidget *randomMi, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    struct mpd_connection *conn = get_connection(mpd_piVars);

    struct mpd_status * status;
    mpd_send_status(conn);
    status = mpd_recv_status(conn);

    if (mpd_status_get_random(status)) mpd_send_random (conn, FALSE);
    else mpd_send_random (conn, TRUE);

    mpd_status_free(status);
    mpd_connection_free(conn);
}

void mode_single(GtkWidget *singleMi, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    struct mpd_connection *conn = get_connection(mpd_piVars);

    struct mpd_status * status;
    mpd_send_status(conn);
    status = mpd_recv_status(conn);

    if (mpd_status_get_single(status)) mpd_send_single (conn, FALSE);
    else mpd_send_single (conn, TRUE);

    mpd_status_free(status);
    mpd_connection_free(conn);
}

void mode_consume(GtkWidget *consumeMi, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    struct mpd_connection *conn = get_connection(mpd_piVars);

    struct mpd_status * status;
    mpd_send_status(conn);
    status = mpd_recv_status(conn);

    if (mpd_status_get_consume(status)) mpd_send_consume (conn, FALSE);
    else mpd_send_consume (conn, TRUE);

    mpd_status_free(status);
    mpd_connection_free(conn);
}

void setFullscreen(GtkWidget *fullscreenMi, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    gtk_window_fullscreen(GTK_WINDOW(mpd_piVars->w_window_main));
}
void setNormal(GtkWidget *normalMi, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    gtk_window_unfullscreen(GTK_WINDOW(mpd_piVars->w_window_main));
    gtk_window_unmaximize(GTK_WINDOW(mpd_piVars->w_window_main));
}

void setMaximize(GtkWidget *maximizeMi, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    gtk_window_unfullscreen(GTK_WINDOW(mpd_piVars->w_window_main));
    gtk_window_maximize(GTK_WINDOW(mpd_piVars->w_window_main));
}

void setMinimize(GtkWidget *minimizeMi, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    gtk_window_unfullscreen(GTK_WINDOW(mpd_piVars->w_window_main));
    gtk_window_iconify(GTK_WINDOW(mpd_piVars->w_window_main));
}

void toggle_mpd_output(GtkWidget *menuItem, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    struct mpd_connection *conn;
    conn = get_connection(mpd_piVars);
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

void toggle_PlayerDisplay	(GtkWidget *playerControlsMi, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    if(gtk_widget_get_visible (GTK_WIDGET (mpd_piVars->w_btnBoxPlayerControls)))
        gtk_widget_hide(GTK_WIDGET(mpd_piVars->w_btnBoxPlayerControls));
    else
        gtk_widget_show(GTK_WIDGET(mpd_piVars->w_btnBoxPlayerControls));
}

void toggle_ProgressDisplay	(GtkWidget *statusDisplayMi, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    if(gtk_widget_get_visible (GTK_WIDGET (mpd_piVars->w_boxProgressBars)))
        gtk_widget_hide(GTK_WIDGET(mpd_piVars->w_boxProgressBars));
    else
        gtk_widget_show(GTK_WIDGET(mpd_piVars->w_boxProgressBars));
}

void popupMenu_startPlayer(GtkMenuItem *menuItem, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_PLAY);
}

void popupMenu_stopPlayer(GtkMenuItem *menuItem, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_STOP);
}

void popupMenu_nextTrack(GtkMenuItem *menuItem, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_NEXT);
}

void popupMenu_previousTrack(GtkMenuItem *menuItem, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_PREVIOUS);
}

void popupMenu_restartTrack(GtkMenuItem *menuItem, app_variables *mpd_piVars)
{   //printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_ZERO_TRACK);
}

void popupMenu_incrementQueue(GtkMenuItem *menuItem, app_variables *mpd_piVars)
{   //printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_QUEUE_INC);
}

void popupMenu_decrementQueue(GtkMenuItem *menuItem, app_variables *mpd_piVars)
{   //printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_QUEUE_DEC);
}

void spawn_PlayviewDialog(GtkMenuItem *menuItem, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    spawnPlayviewDialog(mpd_piVars);
}

void spawn_DataviewDialog(GtkMenuItem *menuItem, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    spawnDataviewDialog(mpd_piVars);
}

void upDateDatabase(GtkMenuItem *menuItem, app_variables *mpd_piVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    // GET MPD CONNECTION
    struct mpd_connection *conn;
    conn = get_connection(mpd_piVars);

    gchar *textStr =  textStr = g_strdup_printf("Updating Database (#%u) ... ",  mpd_run_update(conn, NULL));
    // NOTE: THIS IS AN ASYNC SPAWN FUNCTION
    messageDialog (textStr, __LINE__, __FILE__, __func__, NOTIFY_MSG_MODE, FIVE_SEC_TIMEOUT, FALSE, mpd_piVars);
    gint atLine = __LINE__ - 1;
    printDebugMsg(textStr, atLine, __FILE__, __func__, mpd_piVars);
    g_free(textStr);

    mpd_connection_free(conn);
    //	 printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
}

void popupHelpMessage(GtkMenuItem *menuitem, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    showHelpMessage(mpd_piVars);
}

//~ gboolean toggleViewMode (GtkMenuItem *menuItem, GdkEventButton *event, app_variables *mpd_piVars)
void  toggleViewMode (GtkMenuItem *menuItem,  app_variables *mpd_piVars)
{	// printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

	if(mpd_piVars->showViewMode == SHOW_TRACKVIEW_MODE)
	{
		mpd_piVars->showViewMode = SHOW_SONGVIEW_MODE;
		globalUpdateSongview++;
		return;
	}

		mpd_piVars->showViewMode = SHOW_TRACKVIEW_MODE;
		globalUpdateTrackCenter++;

}