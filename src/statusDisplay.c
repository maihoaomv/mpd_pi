// **** statusDisplay ****

#include "mpd_pi.h"

//~ gboolean setStatusDisplay(app_variables *mpd_piVars)
void setStatusDisplay(app_variables *mpd_piVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    gdouble fraction;
    g_autofree gchar *queStr = g_strdup_printf("Que %d of %d", mpd_piVars->currentSongPos + 1, mpd_piVars->queueLengthStatus);
    g_autofree gchar *volStr1 = g_strdup_printf("Volume %d%%", mpd_piVars->volumeStatus);
    g_autofree gchar *volStr2 = g_strdup((const gchar *)"Volume Mute");
    g_autofree gchar *volStr3 = g_strdup((const gchar *)"Volume");
    g_autofree gchar *timeStr = NULL;

    // *** CONVERT ELASPED SECONDS TO HRS, MIN, SEC STRINGS *** //
    g_autofree gchar *calET	= convertTimeToString(mpd_piVars->elaspedTimeStatus);
    g_autofree gchar *calTT	= convertTimeToString(mpd_piVars->totalTimeStatus);
    g_autofree gchar *calRT	= convertTimeToString(mpd_piVars->totalTimeStatus - mpd_piVars->elaspedTimeStatus);

    // *** VOLUME STATUS *** //
    gtk_progress_bar_set_fraction (mpd_piVars->w_progressBarVolume, (gdouble) mpd_piVars->volumeStatus / (gdouble)100);

    // DON'T CHANGE THIS
    // I HAD TO DO IT THIS WAY  HERE TO FIX VALGRIND blocks are definitely lost
    if(mpd_piVars->volumeStatus)
        gtk_progress_bar_set_text (mpd_piVars->w_progressBarVolume, volStr1);

    if(mpd_piVars->volumeStatus == 0)
        gtk_progress_bar_set_text (mpd_piVars->w_progressBarVolume, volStr2);

    if(mpd_piVars->volumeStatus == -1)
        gtk_progress_bar_set_text (mpd_piVars->w_progressBarVolume, volStr3);

    // *** PROGRESS BAR QUEUE *** //
    fraction = (gdouble)( mpd_piVars->currentSongPos + 1)  / (gdouble)mpd_piVars->queueLengthStatus;
    // IS THE FRACTION NORMAL?
    if(!isnormal(fraction))
        fraction = 0;

    gtk_progress_bar_set_fraction (mpd_piVars->w_progressBarQueue, fraction);
    //~ queStr = g_strdup_printf("Que %d of %d", mpd_piVars->currentSongPos + 1, mpd_piVars->queueLengthStatus);
    gtk_progress_bar_set_text (mpd_piVars->w_progressBarQueue, queStr);

    // *** PROGRESS BAR ELASPED TIME / TOTAL TIME / REMAINING TIME *** //
    fraction =  (gdouble) mpd_piVars->elaspedTimeStatus / (gdouble) mpd_piVars->totalTimeStatus;
    // IS THE FRACTION NORMAL?
    if(!isnormal(fraction))
        fraction = 0;

    gtk_progress_bar_set_fraction (mpd_piVars->w_progressBarTime, fraction);

    if(mpd_piVars->totalTimeStatus) 	timeStr = g_strdup_printf("%s<%s>%s", calET, calTT, calRT);
    else   		timeStr = g_strdup(calET);

    gtk_progress_bar_set_text (mpd_piVars->w_progressBarTime, timeStr);

    //  printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
    //~ return TRUE;
    return;
}  // ***** End of setStatusDisplay *****