// *** keyboard.c ***

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
extern gint globalUpdateSongview;
extern gint globalUpdateTrackCenter;
extern gint globalVerbose;

// ***** KEYBOARD EVENTS *****
gboolean window_main_key_press_event (GtkWidget *widget, GdkEventKey *event, app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    GdkWindow *window = gtk_widget_get_window(widget);
    // SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! SAVE!!
    //https://developer.gnome.org/gtk3/stable/checklist-modifiers.html
    //https://stackoverflow.com/questions/10134956/in-simple-gtk-key-press-event-example-gdk-shift-mask-seems-to-be-ignored
    GdkModifierType modifiers;

    modifiers = gtk_accelerator_get_default_mod_mask ();

    gtk_accelerator_set_default_mod_mask (GDK_CONTROL_MASK | GDK_SHIFT_MASK | GDK_LOCK_MASK | GDK_MOD1_MASK );

    GdkWindowState state = gdk_window_get_state(GDK_WINDOW(window));

    // SAVE!! FOR DEBUGGING	SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! SAVE!!
    // SET ARROW KEYS  TO EMULATE  TRACK FORWARD,  TRACK REWIND, TRACK PREVIOUS, TRACK NEXT
    //	if( event->keyval == GDK_KEY_Up) event->keyval = GDK_KEY_AudioForward;
    //	if( event->keyval == GDK_KEY_Down) event->keyval = GDK_KEY_AudioRewind;
    //	if( event->keyval == GDK_KEY_Left) event->keyval = GDK_KEY_AudioPrev;
    //	if( event->keyval == GDK_KEY_Right) event->keyval = GDK_KEY_AudioNext;
    // SAVE!! FOR DEBUGGING	SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! SAVE!!
    //~ printf("GDK_LOCK_MASK %d\n", GDK_LOCK_MASK);
    //~ printf("GDK_CONTROL_MASK %d\n", GDK_CONTROL_MASK);
    //~ printf("GDK_MODIFIER_MASK %d\n", GDK_MODIFIER_MASK);
    //~ printf("GDK_RELEASE_MASK %d\n", GDK_RELEASE_MASK);
    //~ printf("GDK_MOD1_MASK %d\n", GDK_MOD1_MASK);
    //~ printf("GDK_MOD2_MASK %d\n", GDK_MOD2_MASK);
    //~ printf("GDK_MOD3_MASK %d\n", GDK_MOD3_MASK);
    //~ printf("GDK_MOD4_MASK %d\n", GDK_MOD4_MASK);
    //~ printf("GDK_MOD5_MASK %d\n", GDK_MOD5_MASK);
    //~ printf("GDK_SHIFT_MASK %d\n", GDK_SHIFT_MASK);
	//~ printf("\n\n");
    // SAVE!! FOR DEBUGGING SAVE!! SAVE!! SAVE!! SAVE!! FOR DEBUGGING
    // printf("Line %d\n", __LINE__);
	//~ linenum;
    //~ printf("modifiers 0x%X\n", modifiers);
    //~ printf("event->keyval 0x%X\n\n", event->keyval);
    //~ switch (event->state & modifiers) {
    //~ case GDK_SHIFT_MASK:
    //~ printf("SHIFT KEY\n");
    //~ break;
    //~ case GDK_CONTROL_MASK:
    //~ printf("CONTROL KEY\n");
    //~ break;
    //~ case GDK_MOD1_MASK:
    //~ case GDK_MOD2_MASK:
    //~ case GDK_MOD3_MASK:
    //~ case GDK_MOD4_MASK:
    //~ case GDK_MOD5_MASK:
    //~ printf("ALT KEY\n");
    //~ break;
    //~ default:
    //~ printf("DEFAULT\n");
    //~ break;
    //~ }
    //printf("Line %d\n", __LINE__);
    //~ printf("KEYBOARD.C ***** EVENT->IS_MODIFIER ========= 0x%X -- LINE %d ***** \n", event->is_modifier, __LINE__);
    //~ printf("KEYBOARD.C ***** EVENT->GROUP ========= 0x%X -- LINE %d ***** \n", event->group, __LINE__);
    //~ printf("KEYBOARD.C ***** EVENT->KEYVAL ========= 0x%X -- LINE %d ***** \n", event->keyval, __LINE__);
    //~ printf("KEYBOARD.C ***** EVENT->STATE ========== 0x%X -- LINE %d ***** \n", event->state, __LINE__);
    //~ printf("KEYBOARD.C ***** EVENT->HARDWARE_KEYCODE =0x%X	-- LINE %d ***** \n", event->hardware_keycode, __LINE__);
    //~ printf("KEYBOARD.C ***** EVENT->IS_MODIFIER 0x%X -- LINE %d ***** \n", event->is_modifier, __LINE__);
    // SAVE!! FOR DEBUGGING SAVE!! SAVE!! SAVE!! SAVE!! FOR DEBUGGING //

    int result = 0;
    int mpdFlag = -1;
    int data = 0;
    g_autofree gchar *tmpStr = NULL;
	//~ GString *myStr = NULL;
	//~ g_autoptr(GString) myStr = NULL;
	static gint toggleWidget = 1; // START COUNTING AT ONE NOT ZERO
	gint atLine = 0;

    // SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! FOR DEBUGGING //
    //~ printf("GDK_KEY_%s\n\n", gdk_keyval_name (event->keyval));
    //~ printf("CAP LOCK STATE %d\n", gdk_keymap_get_caps_lock_state(gdk_keymap_get_default()));
    //~ printf("CAP LOCK STATE %d\n", gdk_keymap_get_caps_lock_state(gdk_keymap_get_for_display(gtk_widget_get_display (widget))));
    // SAVE!! SAVE!! SAVE!! SAVE!! SAVE!! FOR DEBUGGING //

    switch(event->keyval) {

    case GDK_KEY_Tab:
		toggleViewMode (NULL, mpd_piVars);

        break;

    case GDK_KEY_asciitilde: // FORCE SIG FAULT FOR TESTING PURPOSES !!!!! CTRL+SHIFT+~
        // ** this intentionally causes a sig fault **
        if((event->state & modifiers) == (GDK_CONTROL_MASK + GDK_SHIFT_MASK))   // CTRL KEY MASK
        {
            fprintf(stderr, "Line %d -- File %s -- Func %s --TEST SEG FAULT and sigfault function\n", __LINE__, __FILE__,  __func__);

            int* p = NULL;
            *p = 10;
        }
        break;

    case GDK_KEY_U: // UPDATE MPD DATABASE  CTRL+SHIFT+U
        if((event->state & modifiers) == (GDK_CONTROL_MASK + GDK_SHIFT_MASK))
            upDateDatabase(NULL, mpd_piVars);
        break;

    case GDK_KEY_Escape:  // EXIT APP
        gtk_main_quit();
        gtk_widget_destroy(GTK_WIDGET(mpd_piVars->w_window_main));
        break;

    case GDK_KEY_F1:  // HELP DIALOG
        showHelpMessage(mpd_piVars);
        break;

    case GDK_KEY_F2:  // PLAYVIEW DIALOG
        spawnPlayviewDialog(mpd_piVars);
        break;

    case GDK_KEY_F3:  // DATAVIEW DIALOG
        spawnDataviewDialog(mpd_piVars);
        break;

    // TOGGLE HIDE/SHOW PLAYER BUTTONS AND PROGRESS BARS
    case GDK_KEY_F4:
            if((globalVerbose == 4) || (globalVerbose > 9)) {
                printf("\n");
                linenum;
                printf("F4 KEY - SHOW / HIDE WIDGETS = %d\n", toggleWidget);
            }

        switch (toggleWidget++)
        {
        case 0:
            gtk_widget_show(GTK_WIDGET(mpd_piVars->w_boxProgressBars));
            gtk_widget_show(GTK_WIDGET(mpd_piVars->w_btnBoxPlayerControls));
            break;

        case 1:
            gtk_widget_hide(GTK_WIDGET(mpd_piVars->w_boxProgressBars));
            gtk_widget_show(GTK_WIDGET(mpd_piVars->w_btnBoxPlayerControls));
            break;

        case 2:
            gtk_widget_show(GTK_WIDGET(mpd_piVars->w_boxProgressBars));
            gtk_widget_hide(GTK_WIDGET(mpd_piVars->w_btnBoxPlayerControls));
            break;

        case 3:
            gtk_widget_hide(GTK_WIDGET(mpd_piVars->w_boxProgressBars));
            gtk_widget_hide(GTK_WIDGET(mpd_piVars->w_btnBoxPlayerControls));
            toggleWidget = 0;
            break;
        }
        break;

    // ROTATE THE WIDGETS THROUGH THE VARIOUS MODES
    case GDK_KEY_F5:
        if((globalVerbose == 4) || (globalVerbose > 9)) {
            printf("\n");
            linenum;
            printf("BEFORE F5 KEY FLIP MODE = %d\n", mpd_piVars->flipMode);
        }
        if(mpd_piVars->flipMode > 9)
            mpd_piVars->flipMode = 0;
        setTheWidgets(mpd_piVars->flipMode + 1, mpd_piVars);
        mpd_piVars->flipMode++;
        if((globalVerbose == 4) || (globalVerbose > 9)) {
            linenum;
            printf("AFTER F5 KEY FLIP MODE = %d\n\n", mpd_piVars->flipMode);
        }
        break; // End of GDK_KEY_F5


    //~ case GDK_KEY_F6:
    //~ break; // End of GDK_KEY_F6


    //~ case GDK_KEY_F7:
    //~ break;       // End of GDK_KEY_F7

    //~ case GDK_KEY_F8:
    //~ break;  // End of GDK_KEY_F8

    //~ case GDK_KEY_F9:
    //~ break;  // End of GDK_KEY_F9

    // POPUP MENU (SHIFT+F10) AND TOGGLE (F10) MINIMIZE/MAXIMAZE THE GUI
    case GDK_KEY_F10: //printf("F10 KEY\n");
        // SHIFT-F10 (WITH THE SHIFT KEY) RIGHT CLICK POPUP MENU
        if((event->state & modifiers) == GDK_SHIFT_MASK) {
            view_popup_menu(widget, NULL, mpd_piVars);
            break;
        }

        // TOGGLES GUI  BETWEEN FULLSCREEN/MAXIMIZE/NORMAL
        if(state & GDK_WINDOW_STATE_MAXIMIZED) {	// MAXIMIZE
            gtk_window_unmaximize(GTK_WINDOW(widget));
            gtk_window_fullscreen(GTK_WINDOW(widget));
            break;
        }
        if(state & GDK_WINDOW_STATE_FULLSCREEN) {	// FULLSCREEN
            gtk_window_unfullscreen(GTK_WINDOW(widget));
            break;
        }
        gtk_window_maximize(GTK_WINDOW(widget));	// NORMAL
        break;

    case GDK_KEY_C: // TOGGLE CENTER CURRENT TRACK ENABLE / ENABLE
    case GDK_KEY_c: // CENTER CURRENT TRACK IN CENTER OF TREEVIEW
		if((event->state & modifiers) == GDK_CONTROL_MASK)   // CTRL KEY MASK
			mpd_piVars->centerTrackEnable =  !mpd_piVars->centerTrackEnable;
		else if((event->state & modifiers) == GDK_CONTROL_MASK + GDK_LOCK_MASK)   // CTRL KEY MASK
			mpd_piVars->centerTrackEnable =  !mpd_piVars->centerTrackEnable;
		else
			centerTrackClickEvent (NULL, NULL, mpd_piVars);
        break;

    // INVOLK THE POPUP MENU
    case GDK_KEY_Menu:
    case GDK_KEY_m:  // POPUP MENU
        view_popup_menu(widget, NULL, mpd_piVars);
        break;

    case GDK_KEY_E:  // *** TEST THE "SHOW ERR MSG DIALOG" AS SYNCHRONOUS DIALOG// ***
		atLine = __LINE__ - 1;
        if((event->state & modifiers) == (GDK_CONTROL_MASK + GDK_SHIFT_MASK))
        {
            fprintf(stderr, "Keyboard Line %d -- Test \"Error Message Dialog\"\n", __LINE__);
			// tmpStr is an autofree variable
			tmpStr = g_strdup("** This is an ERROR message to test simpleMsgDialog **\n** This is NOT a real error **\n** Click EXIT to close app **\n** OK to continue **");

		result = messageDialog (tmpStr, atLine, __FILE__, __func__, ERROR_MSG_MODE, DEFAULT_TIMEOUT, true, mpd_piVars);

            printf("\n");	linenum;	fprintf(stderr, "\nKEYBOARD Line %d -- errMsgMacro returned %d\n", __LINE__, result);
            if(result == GTK_RESPONSE_CLOSE)
            {
                linenum;fprintf(stderr, "KEYBOARD Line %d -- EXIT button was clicked. Exiting %s ..... \n",__LINE__, g_get_prgname ());
                //~ exit(0);
				exitatline;
            }
			else fprintf(stderr, "KEYBOARD Line %d -- OK  button was clicked or (Return or ESC key was pressed). Continuing %s ..... \n",__LINE__, g_get_prgname ());
            fprintf(stderr, "\n");
        }

        break;

    // *** TEST THE "NOTIFY MSG DIALOG" - ASYNCHRONOUS DIALOG
    case GDK_KEY_I:
        if((event->state & modifiers) == (GDK_CONTROL_MASK + GDK_SHIFT_MASK))
        {
            fprintf(stderr, "Test \"Information Message Dialog\"\n");
            tmpStr = g_strdup_printf("** This is a test of the **\n** Information Message Dialog **\n");
            messageDialog (tmpStr, __LINE__, __FILE__, __func__, INFORMATION_MSG_MODE, DEFAULT_TIMEOUT, FALSE, mpd_piVars);
        }
        break;

    // *** TEST THE "NOTIFY MSG DIALOG" - ASYNCHRONOUS DIALOG
    case GDK_KEY_N:
        if((event->state & modifiers) == (GDK_CONTROL_MASK + GDK_SHIFT_MASK))
        {
            fprintf(stderr, "Test \"Notify Message Dialog\"\n");
            tmpStr = g_strdup_printf("** This is a test of the **\n** Notify Message Dialog **\n** With a %d second timeout **", FIVE_SEC_TIMEOUT);
            messageDialog (tmpStr, __LINE__, __FILE__, __func__, NOTIFY_MSG_MODE, FIVE_SEC_TIMEOUT, FALSE, mpd_piVars);

        }
        break;

    case GDK_KEY_Q:  // *** TEST THE "QUESTION MSG DIALOG" SYNCHRONOUS DIALOG // ***
        if((event->state & modifiers) == (GDK_CONTROL_MASK + GDK_SHIFT_MASK))
        {
            fprintf(stderr, "Test \"Question Message Dialog\"\n");
            tmpStr = NULL;
            tmpStr = g_strdup("** This is a test of the **\n** Question Message Dialog **\n** Click \"Yes\" or \"No\". **");

            gint result = messageDialog (tmpStr, __LINE__, __FILE__, __func__, QUESTION_MSG_MODE, DEFAULT_TIMEOUT, TRUE, mpd_piVars);

            fprintf(stderr, "\nError Dialog returned %d\n", result);
            if(result == GTK_RESPONSE_NO)
                fprintf(stderr, "\nKEYBOARD Line %d -- \"NO\" button was clicked.\n",__LINE__);
            if(result == GTK_RESPONSE_YES)
                fprintf(stderr, "\nKEYBOARD Line %d -- \"YES\" button was clicked.\n",__LINE__);
            fprintf(stderr, "\n");
        }
        break;

    // THE FOLLOWING ARE THE AUDIO KEYS
    case GDK_KEY_p:  // TOGGLE PLAY/PAUSE
    case GDK_KEY_P:  // TOGGLE PLAY/PAUSE
        mpdFlag = MPD_PI_FLAG_PLAY;
        break;

    case GDK_KEY_s:  // TOGGLE PLAY/STOP
    case GDK_KEY_S:  // TOGGLE PLAY/STOP
        mpdFlag = MPD_PI_FLAG_STOP;
        break;

    case GDK_KEY_AudioPlay:  // TOGGLE PLAY/PAUSE
        if((event->state & modifiers) == GDK_SHIFT_MASK) {
            mpdFlag = MPD_PI_FLAG_PLAY;
            break;
        }

        if((event->state & modifiers) == GDK_CONTROL_MASK) {  // TOGGLES STOP/PLAY
            mpdFlag = MPD_PI_FLAG_STOP;
            break;
        }

        //~ if((event->state & modifiers) == GDK_MOD1_MASK) {  // SET TRACK TIME TO ZERO
            //~ mpdFlag = MPD_PI_FLAG_ZERO_TRACK;
            //~ break;
        //~ }

        mpdFlag = MPD_PI_FLAG_PLAY;	 // TOGGLE PLAY/PAUSE
        break;

    case GDK_KEY_AudioPause:   // TOGGLE PLAY/PAUSE
        mpdFlag = MPD_PI_FLAG_PAUSE;
        break;

    case GDK_KEY_AudioStop:  // TOGGLE STOP /PLAY
        mpdFlag = MPD_PI_FLAG_STOP;
        break;

    case GDK_KEY_AudioRewind: // REWIND TRACK TIME TO TIME ZERO
        mpdFlag = MPD_PI_FLAG_REWIND;
        data = mpd_piVars->trackSkipTime;
        break;

    case GDK_KEY_f:
        // NUDGE TRACK TIME FORWARD X NUMBER OF SECONDS PER trackSkipTime SETTING IN CONF FILE
        if((event->state & modifiers) == GDK_CONTROL_MASK) {
            mpdFlag = MPD_PI_FLAG_FORWARD;
            data = mpd_piVars->trackSkipTime;
            break;
        }

        // MOVE TRACK FORWARD ONE TRACK IN RESPECT TO THE RANDOM (OR NOT RANDOM) MODE QUE OPTION
        mpdFlag = MPD_PI_FLAG_NEXT;
        break;

    // MOVE QUEUE POINTER FORWARD SEQUENTIALLY ONE TRACK REGARDLESS OF THE RANDOME (OR NOT RANDOM) QUE OPTION
    case GDK_KEY_F: // SHIFT F
        changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_QUEUE_INC);
        break;

    case GDK_KEY_b:
        // NUDGE TRACK TIME BACKWARD X NUMBER OF SECONDS PER trackSkipTime SETTING IN CONF FILE
        if((event->state & modifiers) == GDK_CONTROL_MASK) {
            mpdFlag = MPD_PI_FLAG_REWIND;
            data = mpd_piVars->trackSkipTime;
            break;
        }

        // MOVE TRACK BACKWARD ONE TRACK IN RESPECT TO THE RANDOM (OR NOT RANDOM) MODE QUE OPTION
        mpdFlag = MPD_PI_FLAG_PREVIOUS;
        break;

    case GDK_KEY_B:  // MOVE QUEUE POINTER BACKWARD ONE TRACK REGARDLESS OF THE RANDOM (OR NOT RANDOM) QUE OPTION
        changePlayerStatus(mpd_piVars, 0, MPD_PI_FLAG_QUEUE_DEC);
        break;

    case GDK_KEY_AudioForward: // SKIP TRACK TIME FORWARD X NUMBER OF SECONDS PER trackSkipTime SETTING IN CONF FILE
        mpdFlag = MPD_PI_FLAG_FORWARD;
        data = mpd_piVars->trackSkipTime;
        break;

        if((event->state & modifiers) == GDK_CONTROL_MASK) {
            mpdFlag = MPD_PI_FLAG_REWIND;
            data = mpd_piVars->trackSkipTime;
            break;
        }

        mpdFlag = MPD_PI_FLAG_PREVIOUS;
        break;

    case GDK_KEY_AudioNext: // JUMP QUEUE FORWARD ONE TRACK
        if((event->state & modifiers) == GDK_CONTROL_MASK) {
            mpdFlag = MPD_PI_FLAG_FORWARD;
            data = mpd_piVars->trackSkipTime;
            break;
        }

        if((event->state & modifiers) == GDK_MOD1_MASK) {  // ALT KEY
            mpdFlag = MPD_PI_FLAG_TRACK_LAST;
            break;
        }
        mpdFlag = MPD_PI_FLAG_NEXT;
        break;

    case GDK_KEY_AudioPrev: // JUMP QUEUE FORWARD ONE TRACK
        if((event->state & modifiers) == GDK_CONTROL_MASK) {
            mpdFlag = MPD_PI_FLAG_REWIND;
            data = mpd_piVars->trackSkipTime;
            break;
        }

        if((event->state & modifiers) == GDK_MOD1_MASK) {  // ALT KEY
            mpdFlag = MPD_PI_FLAG_TRACK_FIRST;
            break;
        }
        mpdFlag = MPD_PI_FLAG_PREVIOUS;
        break;

    case GDK_KEY_Z: // SET QUEUE TO TRACK ONE
    case GDK_KEY_z: // RESET CURRENT TRACK TIME TO ZERO
        mpdFlag = MPD_PI_FLAG_ZERO_TRACK;
        break;

    case GDK_KEY_V:
    case GDK_KEY_AudioLowerVolume:  // DECREASE AUDIO VOLUME
    case GDK_KEY_KP_Subtract:
    case GDK_KEY_minus:
        mpdFlag = MPD_PI_FLAG_VOLDN;
        break;

    case GDK_KEY_v:
    case GDK_KEY_AudioRaiseVolume:  // INCREASE AUDIO VOLUME
    case GDK_KEY_plus:
    case GDK_KEY_KP_Add:
        mpdFlag = MPD_PI_FLAG_VOLUP;
        break;

    case GDK_KEY_AudioMute:  // MUTE AUDIO VOLUME
    //~ case GDK_KEY_M:
    case GDK_KEY_0:
    case GDK_KEY_KP_0:
        mpdFlag = MPD_PI_FLAG_VOLMUTE;
        break;

    }

    // ****** SAVE!! SAVE!! KEEP THESE PRINTF FOR DEBUGGING
    //	printf("KEYBOARD.C ***** EVENT->KEYVAL 0x%X -- LINE %d ***** \n", event->keyval, __LINE__);
    //	printf("GDK_KEYVAL_NAME %s\n", gdk_keyval_name(event->keyval));
    //	printf("EVENT->KEYVAL 0x%X\n", event->keyval);
    //	printf("\n");
    // ****** SAVE!! SAVE!! KEEP THESE PRINTF FOR DEBUGGING

    if(mpdFlag != -1) changePlayerStatus(mpd_piVars, data, mpdFlag);

    return FALSE;
} // *** End of window_main_key_press_event *** //
