// *** playerState.c ***

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

// SET THE MPD PLAYER STATUS GUI BACKGROUND COLOR.
// RED = PLAYER STOP MODE,
// GREEN = PLAYER PLAY MODE,
// BLUE = PLAYER PAUSE MODE,
// OR TO UGLY GRAY IF THE PLAYER QUEUE IS EMPTY.
int setMpdPlayerState(app_variables *mpd_piVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    // IF THERE NO TRACKS IN THE QUEUE THEN SET THE GUI BACKGROUND COLOR TO GREY
    if((mpd_piVars->queueLengthStatus) == 0)
    {
        gtk_widget_set_name (GTK_WIDGET(mpd_piVars->w_window_main), "CSS_window_main-NOTRACKS");
        return 0;
    }

    switch (mpd_piVars->mpdPlayerStateStatus) {

    case MPD_STATE_PLAY:
        gtk_widget_set_name (GTK_WIDGET(mpd_piVars->w_window_main), "CSS_window_main-PLAY");
        gtk_widget_show(GTK_WIDGET(mpd_piVars->w_btnPause));
        gtk_widget_hide(GTK_WIDGET(mpd_piVars->w_btnPlay));

        gtk_widget_set_sensitive (GTK_WIDGET(mpd_piVars->w_btnStop), TRUE);
        gtk_widget_set_sensitive (GTK_WIDGET(mpd_piVars->w_btnNext), TRUE);
        gtk_widget_set_sensitive (GTK_WIDGET(mpd_piVars->w_btnForward), TRUE);
        gtk_widget_set_sensitive (GTK_WIDGET(mpd_piVars->w_btnPrevious), TRUE);
        gtk_widget_set_sensitive (GTK_WIDGET(mpd_piVars->w_btnRewind), TRUE);
        gtk_widget_set_sensitive (GTK_WIDGET(mpd_piVars->w_btnPlay), TRUE);
        gtk_widget_set_sensitive (GTK_WIDGET(mpd_piVars->w_btnPause), TRUE);
        break;

    case MPD_STATE_PAUSE:
        gtk_widget_set_name (GTK_WIDGET(mpd_piVars->w_window_main), "CSS_window_main-PAUSE");
        gtk_widget_show(GTK_WIDGET(mpd_piVars->w_btnPlay));
        gtk_widget_hide(GTK_WIDGET(mpd_piVars->w_btnPause));

        gtk_widget_set_sensitive (GTK_WIDGET(mpd_piVars->w_btnNext), FALSE);
        gtk_widget_set_sensitive (GTK_WIDGET(mpd_piVars->w_btnPrevious), FALSE);
        gtk_widget_set_sensitive (GTK_WIDGET(mpd_piVars->w_btnForward), FALSE);
        gtk_widget_set_sensitive (GTK_WIDGET(mpd_piVars->w_btnRewind), FALSE);
        break;

    case MPD_STATE_STOP:
        gtk_widget_set_name (GTK_WIDGET(mpd_piVars->w_window_main), "CSS_window_main-STOP");
        gtk_widget_show(GTK_WIDGET(mpd_piVars->w_btnPlay));
        gtk_widget_hide(GTK_WIDGET(mpd_piVars->w_btnPause));

        gtk_widget_set_sensitive (GTK_WIDGET(mpd_piVars->w_btnStop), FALSE);
        gtk_widget_set_sensitive (GTK_WIDGET(mpd_piVars->w_btnNext), FALSE);
        gtk_widget_set_sensitive (GTK_WIDGET(mpd_piVars->w_btnForward), FALSE);
        gtk_widget_set_sensitive (GTK_WIDGET(mpd_piVars->w_btnPrevious), FALSE);
        gtk_widget_set_sensitive (GTK_WIDGET(mpd_piVars->w_btnRewind), FALSE);
        gtk_widget_set_sensitive (GTK_WIDGET(mpd_piVars->w_btnPlay), TRUE);
        gtk_widget_set_sensitive (GTK_WIDGET(mpd_piVars->w_btnPause), FALSE);
        break;

    case MPD_STATE_UNKNOWN:

        break;

    default:

        break;
    }

    //	printf("End of -- %s -- Line %d --\n\n", __func__, __LINE__);
    return 0;
} // ***** End of setMpdPlayerState *****
