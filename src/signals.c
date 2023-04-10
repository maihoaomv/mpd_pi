// *** signals.c ***
#include "mpd_pi.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

// DOUBLE CLICKED A ROW CALLBACK
// DOUBLE CLICKING ON A TREEVIEW ROW, WILL POINT QUEUE TO THAT POSITION AND START PLAYING THE TRACK
gboolean trackview_onRowActivated_signal (GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn  *col, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    GtkTreeSelection *selection;
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    GtkTreeModel *model;
    GtkTreeIter   iter;

    guint value;

    // *** IF THE ROW IS DOUBLE CLICKED THEN GET THE NEW SELECTION
    gtk_tree_selection_get_selected(GTK_TREE_SELECTION( selection), &model, &iter); // won't work without this

    gtk_tree_model_get(model, &iter, TRACKVIEW_COL_QUEUE, &value,  -1);

    // SETUP MPD CONNECTION
    struct mpd_connection *conn;
    // GET MPD CONNECTION
    conn = get_connection(mpd_piVars);

    mpd_send_play_pos ( conn, value - 1) ;

    mpd_connection_free(conn);
    // UN-SELECT THE SELECTED TRACK
    gtk_tree_selection_unselect_all(selection);

    return 1;
}

////////// NEED BOTH OF THESE TO GET DOUBLE CLICK ON ROW TO WORK/////////////////////////////////////////////////////////////////////////////////////////////
gboolean trackview_onButtonPressed_signal (GtkWidget *window_main, GdkEventButton *event, app_variables *mpd_piVars)
{    //	printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    // SINGLE CLICK WITH THE RIGHT MOUSE BUTTON- POPUP MENU
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3) // right click
    {   // g_print("Right mouse button clicked\n");
        view_popup_menu(window_main, event, mpd_piVars);
    }

    // THE RETURN NEEDS TO BE FALSE SO BOTH POPUP MENU AND DOUBLE CLICK WILL WORK AT THE SAME TIME
    //	https://lists.gt.net/maemo/developers/27463
    return FALSE;
}
////////// NEED BOTH OF THESE TO GET DOUBLE CLICK ON ROW TO WORK/////////////////////////////////////////////////////////////////////////////////////////////
// RIGHT CLICK MOUSE BUTTON  ON GUI  TO ENVOKE POPUP MENU
gboolean window_main_button_press_event (GtkWidget *window_main, GdkEventButton *event, app_variables *mpd_piVars)
{     //	printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    /* single click with the right mouse button- popup menu */
    if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3) // right click
    {   // g_print("Right mouse button clicked\n");
        view_popup_menu(window_main, event, mpd_piVars);
    }

    return TRUE; /* we did handle this */
// WHAT WAS THE REASON THAT I HAD SET THIS TO RETURN FALSE?
    //~ return FALSE; /* we did handle this */
}
////////// NEED BOTH OF THESE TO GET DOUBLE CLICK ON ROW TO WORK/////////////////////////////////////////////////////////////////////////////////////////////

void setSignals(app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    // NOTE: POPUP MENU IS DONE WITH SETTING SIGNALS "button-press-event"(mouse RIGHT CLICK) AND "popup-menu"(keyboard SHIFT-F10) IN THE GLADE WINDWO_MAIN SIGNALS
    // NOTE: SOME SIGNALS ARE ALSO SET IN GLADE GUI INTERFACE

    // ********* ENABLE THE GDK_SCROLL_MASK MASK TO RECEIVE THE SCROLL EVENTS **********
    // https://stackoverflow.com/questions/10355779/how-to-catch-gtk-scroll-event-on-menu-item
    gtk_widget_add_events(GTK_WIDGET(mpd_piVars->w_eventBoxProgressBarVolume), GDK_SCROLL_MASK);
    gtk_widget_add_events(GTK_WIDGET(mpd_piVars->w_eventBoxProgressBarTime), GDK_SCROLL_MASK);
    gtk_widget_add_events(GTK_WIDGET(mpd_piVars->w_eventBoxProgressBarQueue), GDK_SCROLL_MASK);

    // THIS CATCHES THE KEYBOARD KEYS
    g_signal_connect(mpd_piVars->w_window_main, "key-press-event", (GCallback) window_main_key_press_event, mpd_piVars);

    // PROGRESS BAR EVENTS
    g_signal_connect(mpd_piVars->w_eventBoxProgressBarQueue, "button-press-event", (GCallback) centerTrackClickEvent,  mpd_piVars);
    g_signal_connect(mpd_piVars->w_eventBoxProgressBarTime, "button-press-event", (GCallback) trackTime_click_event,  mpd_piVars);
    g_signal_connect(mpd_piVars->w_eventBoxProgressBarVolume, "button-press-event", (GCallback) volumePercent_click_event,  mpd_piVars);
    g_signal_connect(mpd_piVars->w_eventBoxProgressBarVolume, "scroll-event", (GCallback) volume_scroll_event, mpd_piVars);
    g_signal_connect(mpd_piVars->w_eventBoxProgressBarTime, "scroll-event", (GCallback) elasped_scroll_event, mpd_piVars);
    g_signal_connect(mpd_piVars->w_eventBoxProgressBarQueue, "scroll-event", (GCallback) queue_scroll_event, mpd_piVars);

    // NEED BOTH OF THESE TO MAKE THE POPUP MENU WORK ANYWHERE IN THE WINDOW
    ////////// NEED BOTH OF THESE TO GET DOUBLE CLICK ON ROW TO WORK/////////////////////////////////////////////////////////////////////////////////////////////
    g_signal_connect(mpd_piVars->w_window_main, "button-press-event", (GCallback) window_main_button_press_event, mpd_piVars);
    g_signal_connect(mpd_piVars->w_trackviewTreeView, "button-press-event", (GCallback) trackview_onButtonPressed_signal, mpd_piVars );
    ////////// NEED BOTH OF THESE TO GET DOUBLE CLICK ON ROW TO WORK/////////////////////////////////////////////////////////////////////////////////////////////


    // DOUBLE CLICKING ON A ROW, WILL MOVE QUEUE TO POSITION AND START PLAYING THIS TRACK
    g_signal_connect(mpd_piVars->w_trackviewTreeView, "row-activated", (GCallback) trackview_onRowActivated_signal, mpd_piVars);

    // THESE ARE PLAYER BUTTON SIGNALS
    g_signal_connect(mpd_piVars->w_btnPrevious, 	"clicked", (GCallback)	btnPrevious_clicked, 	mpd_piVars);
    g_signal_connect(mpd_piVars->w_btnRewind, 		"clicked", (GCallback)	btnRewind_clicked, 	mpd_piVars);
    g_signal_connect(mpd_piVars->w_btnPlay, 			"clicked", (GCallback)	btnPlay_clicked, 			mpd_piVars);
    g_signal_connect(mpd_piVars->w_btnPause, 		"clicked", (GCallback)	btnPause_clicked, 		mpd_piVars);
    g_signal_connect(mpd_piVars->w_btnStop, 			"clicked", (GCallback)	btnStop_clicked, 			mpd_piVars);
    g_signal_connect(mpd_piVars->w_btnForward, 	"clicked", (GCallback)	btnForward_clicked, 	mpd_piVars);
    g_signal_connect(mpd_piVars->w_btnNext, 			"clicked", (GCallback)	btnNext_clicked, 			mpd_piVars);

    // THESE ARE THE TREEVIEW SORT SIGNALS
    g_signal_connect(mpd_piVars->w_trackViewModelSort, 	"sort-column-changed", (GCallback)	trackViewModelSort_sort_column_changed_cb, 			mpd_piVars);

    // NOTE: popup menu is done with setting signals "button-press-event"(mouse RIGHT CLICK) and "popup-menu"(keyboard SHIFT-F10) in the GLADE windwo_main signals

}
