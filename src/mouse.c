// *** mouse.c ***

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
#pragma GCC diagnostic ignored "-Wunused-parameter"

// LEFT CLICK PROGRESS (ELASPED / TOTAL / REMAINING)  TIME TO RESET TRACK TIME TO ZERO
void trackTime_click_event (GtkWidget *widget, GdkEventButton *event, app_variables *mpd_piVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    // FOR THE FOLLOWING USE EITHER MPD_PI_FLAG_PLAY OR MPD_PI_FLAG_PAUSE. EITHER WILL WORK.
    // DO NOT USE (MPD_PI_FLAG_PLAY | MPD_PI_FLAG_PAUSE) or (MPD_PI_FLAG_PLAY & MPD_PI_FLAG_PAUSE).
    // THAT WILL CAUSE IT TO SEND MPD_PI_FLAG_TRACK_DEC or MPD_PI_FLAG_PREVIOUS.

    if((event->type == GDK_BUTTON_PRESS)  &&  (event->button == 1)) // left mouse button click
        changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_ZERO_TRACK);
}

// LEFT CLICK PROGRESS VOLUME TO MUTE / UNMUTE VOLUME
void volumePercent_click_event (GtkWidget *widget, GdkEventButton *event, app_variables *mpd_piVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    if ((event->type == GDK_BUTTON_PRESS)  &&  (event->button == 1)) // left mouse button click
        changePlayerStatus(mpd_piVars, VOLUME_MUTE, MPD_PI_FLAG_VOLMUTE);
}

// ROLL MOUSE WHEEL OVER PROGRESS VOLUME CHANGE VOLUME UP / DOWN
void volume_scroll_event (GtkWidget *widget, GdkEventButton *event, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);;
    // https://github.com/Keruspe/Germinal/blob/master/src/germinal/germinal-terminal.c
    GdkScrollDirection direction;

    gdk_event_get_scroll_direction ((GdkEvent *)event, &direction);

    switch (direction)
    {
    case GDK_SCROLL_UP:
        changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_VOLUP);
        break;
    case GDK_SCROLL_DOWN:
        changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_VOLDN);
        break;

    default:
        break;
    }
}

// ROLL MOUSE WHEEL OVER PROGRESS QUEUE TO INCREMENT / DECEMENT QUEUE POSTION
void queue_scroll_event (GtkWidget *widget, GdkEventButton *event, app_variables *mpd_piVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    GdkScrollDirection direction;

    gdk_event_get_scroll_direction ((GdkEvent *)event, &direction);

    switch (direction)
    {
    case GDK_SCROLL_UP:
        changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_QUEUE_INC);
        break;
    case GDK_SCROLL_DOWN:
        changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_QUEUE_DEC);
        break;

    default:
        break;
    }
}

// ROLL MOUSE WHEEL OVER PROGRESS (ELASPED / TOTAL / REMAINING)  TIME TO INCREMANT / DECREMENT TRACK TIME
void elasped_scroll_event (GtkWidget *widget, GdkEventButton *event, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    GdkScrollDirection direction;

    gdk_event_get_scroll_direction ((GdkEvent *)event, &direction);

    switch (direction)
    {
    case GDK_SCROLL_UP:
        changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_ELASPED_INC);
        break;
    case GDK_SCROLL_DOWN:
        changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_ELASPED_DEC);
        break;

    default:
        break;
    }
}
