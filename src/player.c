/****** player.c ******/
#include "mpd_pi.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

// THIS FUNCTION HANDLES THE MPD PLAYER FUNTIONS
int changePlayerStatus(app_variables *mpd_piVars, int data, int bitFlag)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    static int save_volume;
    struct mpd_connection *conn;
    struct mpd_status * status;
    conn = get_connection(mpd_piVars);
    //~ if(conn == NULL) {
    //~ gtk_main_quit();
    //~ }

    mpd_send_status(conn);
    status = mpd_recv_status(conn);

    unsigned elasped_time_sec = mpd_status_get_elapsed_time(status);
    unsigned total_time_sec = mpd_status_get_total_time(status);

    int statusFlag = mpd_status_get_state(status);

    switch(bitFlag) {

    case MPD_PI_FLAG_STOP:   //printf("--MPD_STATE_STOP--\n");
        if (statusFlag == MPD_STATE_STOP)
            mpd_run_play(conn);
        else
            mpd_run_stop(conn);
        break;

    case MPD_PI_FLAG_PLAY:  // COMBINE THE PLAY AND PAUSE FUCTION
    case MPD_PI_FLAG_PAUSE:  //printf("--MPD_STATE_PLAY--\n");
        if (statusFlag == MPD_STATE_STOP)
            mpd_run_play(conn);
        if (statusFlag == MPD_STATE_PLAY)
            mpd_run_pause(conn, true);
        if (statusFlag == MPD_STATE_PAUSE)
            mpd_run_play(conn);
        break;

    case MPD_PI_FLAG_VOLUP:
        mpd_run_change_volume(conn, +MPD_ADJUST);
        break;

    case MPD_PI_FLAG_VOLDN:
        mpd_run_change_volume(conn, -MPD_ADJUST);
        break;

    case MPD_PI_FLAG_VOLMUTE:
        if (mpd_status_get_volume(status) > 0)
        {
            save_volume = mpd_status_get_volume(status);
            mpd_run_set_volume(conn, 0);
        }
        if (mpd_status_get_volume(status) == 0)
            mpd_run_set_volume(conn, save_volume);
        break;
    }

    if (!(statusFlag == MPD_STATE_STOP))
    {
        switch(bitFlag) {

        case MPD_PI_FLAG_PREVIOUS:	// move queue back one track
            mpd_run_previous(conn);
            break;

        case MPD_PI_FLAG_NEXT:	// move queue forward one track
            mpd_run_next(conn);
            break;

        case MPD_PI_FLAG_ZERO_TRACK:
            mpd_run_play_pos(conn, mpd_status_get_song_pos(status));
            break;

        // RESET CURRENT TRACK TIME BACK TO ZERO
        case MPD_PI_FLAG_REWIND:
            if (((int)elasped_time_sec - (int)data) < 0)
                mpd_run_play_pos(conn, mpd_status_get_song_pos(status));
            else
                mpd_run_seek_pos (conn, mpd_status_get_song_pos(status), elasped_time_sec - data);
            break;

        case MPD_PI_FLAG_FORWARD: // skip current track time forward X number of seconds
            if (((int)elasped_time_sec + (int)data) > (int)total_time_sec)
                mpd_run_play_pos(conn, mpd_status_get_song_pos(status));
            else
                mpd_run_seek_pos (conn, mpd_status_get_song_pos(status), elasped_time_sec + data);
            break;

        case MPD_PI_FLAG_ELASPED_INC:
            if (statusFlag == MPD_STATE_STOP)
                break;
            if(elasped_time_sec == total_time_sec) mpd_run_seek_pos (conn,\
                        mpd_status_get_song_pos(status),0);
            else mpd_run_seek_pos (conn, mpd_status_get_song_pos(status), elasped_time_sec + 1);
            break;

        case MPD_PI_FLAG_ELASPED_DEC:
            if(elasped_time_sec == 0)
                break;
            else mpd_run_seek_pos (conn, mpd_status_get_song_pos(status), elasped_time_sec - 1);
            break;

        case MPD_PI_FLAG_QUEUE_DEC:
            if (statusFlag == MPD_STATE_STOP)
                break;
            if(mpd_status_get_song_pos(status)	== 0)
                mpd_run_seek_pos(conn,mpd_piVars->queueLengthStatus-1,0);
            else
                mpd_run_seek_pos(conn,mpd_status_get_song_pos(status)-1,0);
            break;

        case MPD_PI_FLAG_QUEUE_INC:
            if(mpd_status_get_song_pos(status)	== (int)mpd_piVars->queueLengthStatus-1)
                mpd_run_seek_pos(conn,0,0);
            else
                mpd_run_seek_pos(conn,mpd_status_get_song_pos(status)+1,0);
            break;

        case MPD_PI_FLAG_TRACK_FIRST:
            mpd_send_play_pos (conn, 0);
            break;

        case MPD_PI_FLAG_TRACK_LAST:
            mpd_send_play_pos (conn, mpd_piVars->queueLengthStatus- 1);
            break;

        default:
            break;
        }
    }

    mpd_status_free(status);
    mpd_connection_free(conn);

    return 0;
} // *** End of changePlayerStatus *** //

// *** BUTTONS *** -- THESE ARE THE PLAYER BUTTON SIGNAL CALLBACKS //
// take a look at this     https://prognotes.net/2016/12/gtk-glade-get-pointers-to-widgets/
// NEXT TRACK BUTTON - MOVE QUEUE FORWARD TO NEXT TRACK
void btnNext_clicked(GtkWidget *widget, app_variables *mpd_piVars) {
    // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__)
    changePlayerStatus(mpd_piVars, mpd_piVars->trackSkipTime, MPD_PI_FLAG_NEXT);
}

// FORWARD TRACK TIME BUTTON -MOVE TRACK TIME FORWARD X NUMBER OF SECONDS
void btnForward_clicked(GtkWidget *widget, app_variables *mpd_piVars) {
    // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__)
    changePlayerStatus(mpd_piVars, mpd_piVars->trackSkipTime, MPD_PI_FLAG_FORWARD);
}

void btnStop_clicked(GtkWidget *widget, app_variables *mpd_piVars) {
    // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__)
    changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_STOP);
}

void btnPause_clicked(GtkWidget *widget, app_variables *mpd_piVars) {
    // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__)
    changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_PAUSE);
}

void btnPlay_clicked(GtkWidget *widget, app_variables *mpd_piVars) {
    // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__)
    changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_PLAY);
}

//  PREVIOUS TRACK BUTTON -MOVE QUEUE BACK ONE TO PREVIOUS TRACK
void btnPrevious_clicked(GtkWidget *widget, app_variables *mpd_piVars) {
    // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__)
    changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_PREVIOUS);
}

//  REWIND BUTTON -RESETS CURRENT TRACK BACK TO PLAY POSITION ZERO
void btnRewind_clicked(GtkWidget *widget, app_variables *mpd_piVars) {
    // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    changePlayerStatus(mpd_piVars, mpd_piVars->trackSkipTime, MPD_PI_FLAG_REWIND);
}
