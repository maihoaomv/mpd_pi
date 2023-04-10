// *** timer_handler.c ***
#include "mpd_pi.h"
extern gint globalUpdateTrackview;
extern gint globalUpdateSongview;
extern gint globalUpdateTrackCenter;

// THIS FUNCTION RUNS IN A LOOP TIMER
gboolean showViewTimer(app_variables *mpd_piVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    // UPDATE THE MPD STATUS VARIABLES
    getMpdStatus(mpd_piVars);

    // SET THE PROGRESS BAR STATUS DISPLAYS
    setStatusDisplay(mpd_piVars);

    setMpdPlayerState(mpd_piVars); // SET THE PLAYER MODE BACKGROUND COLORS

// IF THE MAIN WINDOWS IS NOT VISIBLE THEN TURN IT ON -THIS SHOULD ONLY HAPPEN AT INITIAL STARTUP
    if(gtk_widget_get_visible (GTK_WIDGET (mpd_piVars->w_window_main)) == FALSE)
        gtk_widget_show(GTK_WIDGET(mpd_piVars->w_window_main));

    // IF THERE ARE NO TRACKS IN THE PLAY QUEUE
    if((mpd_piVars->queueLengthStatus) == 0)
    {   // CLEAR THE TRACKVIEW AND HIDE SONGVIEW LABELS
        gtk_list_store_clear (mpd_piVars->w_trackviewListStore);
        gtk_widget_hide(GTK_WIDGET(mpd_piVars->w_box_for_SongviewLabel));
        goto weAreDone;
    }
    else  // UNHIDE SONGVIEW LABLES IF IT'S NOT VISIBLE
        if (gtk_widget_get_visible (GTK_WIDGET (mpd_piVars->w_box_for_SongviewLabel)) == FALSE)
            gtk_widget_show(GTK_WIDGET(mpd_piVars->w_box_for_SongviewLabel));

    // IF WE ARE IN THE SONGVIEW MODE THEN CALL SONGVIEW AND IGNORE TRACKVIEW
    if(mpd_piVars->showViewMode == SHOW_SONGVIEW_MODE)
    {	//linenum;

        gtk_widget_set_name (GTK_WIDGET(mpd_piVars->w_boxProgressBars), "CSS_SongviewMode");

        if(globalUpdateSongview)
        {  // linenum; gtk_widget_set_name (GTK_WIDGET(mpd_piVars->w_boxProgressBars), "CSS_SongviewMode");
            songView(mpd_piVars);
        }

        if(gtk_widget_get_visible (GTK_WIDGET (mpd_piVars->w_boxTrackview)) == TRUE)
        {
            gtk_widget_show(GTK_WIDGET(mpd_piVars->w_boxSongview));
            gtk_widget_hide(GTK_WIDGET(mpd_piVars->w_boxTrackview));
        }
        goto weAreDone;
    }

    // IF WE ARE IN THE TRACKVIEW MODE THEN CALL TRACKVIEW AND IGNORE SONGVIEW
    if(mpd_piVars->showViewMode == SHOW_TRACKVIEW_MODE)
    {   //linenum;
        gtk_widget_set_name (GTK_WIDGET(mpd_piVars->w_boxProgressBars), "CSS_TrackviewMode");

        if(gtk_widget_get_visible (GTK_WIDGET (mpd_piVars->w_boxSongview)) == TRUE)
        {
            gtk_widget_hide(GTK_WIDGET(mpd_piVars->w_boxSongview)); // HIDE THE SONGVIEW
            gtk_widget_show(GTK_WIDGET(mpd_piVars->w_boxTrackview)); // SHOW THE TRACKVIEW
        }
        // IF QUEUE IS EMPTY THEN NO NEED TO CALL THE TRACKVIEW
        if(mpd_piVars->queueLengthStatus == 0)
            goto weAreDone;

        // LOAD TRACKS INTO THE TRACKVIEW DISPLAY
        if(globalUpdateTrackview)
        { //  linenum;	gtk_widget_set_name (GTK_WIDGET(mpd_piVars->w_boxProgressBars), "CSS_TrackviewMode");
            trackView (mpd_piVars);
            highLightTrack (mpd_piVars);
            setMpdPlayerState(mpd_piVars);
        }

        // IF CENTER TRACK FLAG IS ENABLED THEN CENTER THE TRACK IN THE TREEVIEW
        if(globalUpdateTrackCenter)
        {  // linenum;	gtk_widget_set_name (GTK_WIDGET(mpd_piVars->w_boxProgressBars), "CSS_TrackviewMode");
            // IS THE FLAG SET TO DO AUTO CENTERING?
            if(mpd_piVars->centerTrackEnable)
                centerTrackClickEvent (NULL, NULL, mpd_piVars);
            // HIGHLIGHT THE CURRENT PLAYING TRACK
            highLightTrack (mpd_piVars);
        }
    }

weAreDone:
    ;
    //~ /*linenum;*/   printf("\n\n");
    if(globalUpdateSongview)
        globalUpdateSongview = 0;

    if(globalUpdateTrackview)
        globalUpdateTrackview = 0;

    if(globalUpdateTrackCenter)
        globalUpdateTrackCenter = 0;

    // printf("End of -- %s -- Line %d --\n\n", __func__, __LINE__);
    // ALWAYS RETURN TRUE SO IT DOESN'T KILL THE TIMER
    return TRUE;
} // *** End of showViewTimer ***
