/* getStatus.c */

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

#include "mpd_pi.h"
extern gint globalUpdateTrackview;
extern gint globalUpdateTrackCenter;

// THIS FUNCTION GETS THE MPD STATUS VARIABLES FROM THE MPD SERVER
gboolean getMpdStatus(app_variables *mpd_piVars)
{   //   printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    struct mpd_connection *conn;
    conn = get_connection(mpd_piVars);

    struct mpd_status * status;
    mpd_send_status(conn);
    status = mpd_recv_status(conn);
    gint atLine = __LINE__ - 1;
    if (status == NULL)
    {
        gchar *str = NULL;
        str = g_strdup( mpd_status_get_error(status));
        messageDialog(str,  atLine, __FILE__, __func__, ERROR_MSG_MODE, 0, TRUE, mpd_piVars);
        mpd_status_free (status);
        g_free(str);
        exit(0);
    }

    // THESE ARE THE STATUS VARIABLES
    mpd_piVars->elaspedTimeStatus 		= mpd_status_get_elapsed_ms(status) / 1000;
    mpd_piVars->totalTimeStatus 				= mpd_status_get_total_time (status);
    mpd_piVars->volumeStatus 					= mpd_status_get_volume(status);

    mpd_piVars->queueLengthStatus 		= mpd_status_get_queue_length (status);

    mpd_piVars->mpdPlayerStateStatus 	= mpd_status_get_state (status);

    // IF UPDATE TRACK CENTER THEN UPDATE SONG POSITION AND PREVIOUS SONG POSITION
    if(globalUpdateTrackCenter)
    {
        mpd_piVars->prevSongPos = mpd_piVars->currentSongPos;
        mpd_piVars->currentSongPos = mpd_status_get_song_pos (status);
    }

    mpd_status_free(status);
    mpd_connection_free(conn);

    // printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
    return TRUE;
} // *** End of getMpdStatus
