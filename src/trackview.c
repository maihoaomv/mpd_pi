// trackview.c

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
//~ extern globalUpdateTrackview;
//~ extern globalUpdateTrackCenter;
// TRACKVIEW
// THIS IS THE TRACK VIEW LISTING OF TRACKS IN THE PLAY QUEUE

void trackView(app_variables *mpd_piVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    // GET MPD CONNECTION
    struct mpd_connection *conn;
    conn = get_connection(mpd_piVars);
    if(conn == NULL) {
        gtk_main_quit();
    }

    // GET MPD STATUS
    struct mpd_status * status;
    mpd_send_status(conn);
    status = mpd_recv_status(conn);

    // CLEAR THE LIST STORE
    gtk_list_store_clear (mpd_piVars->w_trackviewListStore);

    // GET TAG INFO FROM MPD
    struct mpd_entity * entity;

    // TAG INFO FOR USE WITH THE TREEVIEW
    gchar *trackInfo = NULL;
    gchar *titleInfo = NULL;
    gchar *artistInfo = NULL;
    gchar *albumArtistInfo = NULL;
    gchar *albumInfo = NULL;
    gchar *discInfo = NULL;
    gchar *nameInfo = NULL;

    // TRACK INFO
    gchar *timeInfo = NULL;

    gchar *textStr  = NULL;

    GtkTreeIter iter;

    mpd_send_list_queue_meta(conn);

    gint isDisk = 0, isTrack = 0, isTime = 0, isAlbum = 0, isArtist = 0;

    while ((entity = mpd_recv_entity(conn)) != NULL) {
        const struct mpd_song *song;

        song = mpd_entity_get_song(entity);

        // GET THE TRACK TAGS
        // NAME INFO MAY APPEAR ON A HTTP TRACK (INTERNET RADIO). POSSIBLY, BUT UNLIKELY ON A SONG TRACK
        nameInfo = g_strdup( mpd_song_get_tag(song,   MPD_TAG_NAME, 0));
        titleInfo =   g_strdup( mpd_song_get_tag(song,    MPD_TAG_TITLE, 0));
        trackInfo =    g_strdup( mpd_song_get_tag(song,   MPD_TAG_TRACK, 0));
        artistInfo =    g_strdup( mpd_song_get_tag(song,   MPD_TAG_ARTIST, 0));
        albumArtistInfo = g_strdup( mpd_song_get_tag(song,   MPD_TAG_ALBUM_ARTIST, 0));
        albumInfo = g_strdup( mpd_song_get_tag(song,   MPD_TAG_ALBUM, 0));
        discInfo =    g_strdup( mpd_song_get_tag(song,    MPD_TAG_DISC, 0));
        timeInfo = convertTimeToString(mpd_song_get_duration(song));

/////////////////////////////////////////////////////////////
//printf("ALBUM = %s\nALBUM_ARTIST = %s\n", artistInfo, albumArtistInfo);

        // IF THE ALBUM_ARTIST TAG EXIST AND THE ALBUM TAG DOES NOT,
        // THEN COPY THE albumArtistInfo TAG INTO artistInfo
        if(((albumArtistInfo)) && (!(artistInfo)))   // TITLEINFO SHOULD BE NULL
        {
            artistInfo = g_strdup(albumArtistInfo);
            g_free(albumArtistInfo);
            albumArtistInfo = NULL;
        }
        // WE ARE DONE WITH albumArtistInfo
/////////////////////////////////////////////////////////////


        // THE NAME TAG IS A SPECIAL CASE. IT WILL PROBABLY BE NULL WITH A SONG URI, BUT MAY EXIST IN AN HTTP URI.
        // IF NAME TAG AND TITLE TAG BOTH EXIST, THEN COMBINE THE NAME TAG AND TITLE TAG
        // INTO A SINGLE STRING, (WITH A CR) THEN COPY THE NEW STRING INTO TITLEINFO
        if(((nameInfo)) && ((titleInfo))) {
            textStr = g_strdup_printf("%s\n%s", nameInfo, titleInfo);
            g_free(titleInfo);
            titleInfo = NULL;
            g_free(nameInfo);
            nameInfo = NULL;
            titleInfo = g_strdup(textStr);
            g_free(textStr);
            textStr = NULL;
        }

        // IF THE NAME TAG EXIST AND THE TITLE TAG DOES NOT,
        // THEN COPY THE NAME TAG INTO TITLEINFO
        if(((nameInfo)) && (!(titleInfo)))   // TITLEINFO SHOULD BE NULL
        {
            titleInfo = g_strdup(nameInfo);
            g_free(nameInfo);
            nameInfo = NULL;
        }
        // WE ARE DONE WITH NAMEINFO

        // INITIALIZE  THE FIRST ROW
        gtk_list_store_append (mpd_piVars->w_trackviewListStore, &iter);

        // WRITE THE DATA TO THE TREEVIEW
        // SET TRACK POSITION IN THE QUEUE AS A GINT SO THAT THE ORIGINAL "QUEUE UNSORTED POSITION"  CAN BE NUMERICALLY REESTABLISHED
        gtk_list_store_set (mpd_piVars->w_trackviewListStore,  &iter, TRACKVIEW_COL_QUEUE, mpd_song_get_pos(song) + 1,  -1);

        gtk_list_store_set (mpd_piVars->w_trackviewListStore,  &iter, TRACKVIEW_COL_DISC, discInfo,  -1);

        gtk_list_store_set (mpd_piVars->w_trackviewListStore,  &iter, TRACKVIEW_COL_TRACK, trackInfo,  -1);

        gtk_list_store_set (mpd_piVars->w_trackviewListStore,  &iter, TRACKVIEW_COL_TIME, timeInfo, -1);

        gtk_list_store_set (mpd_piVars->w_trackviewListStore,  &iter, TRACKVIEW_COL_TITLE, titleInfo, -1);

        gtk_list_store_set (mpd_piVars->w_trackviewListStore,  &iter, TRACKVIEW_COL_ARTIST, artistInfo, -1);

        gtk_list_store_set (mpd_piVars->w_trackviewListStore,  &iter, TRACKVIEW_COL_ALBUM, albumInfo,  -1);

        gtk_list_store_set (mpd_piVars->w_trackviewListStore,  &iter, TRACKVIEW_COL_PATH_FILENAME, mpd_song_get_uri(song),  -1);

        // FREE THE VARIABLES
        if(trackInfo)   	{
            isTrack++;
            g_free(trackInfo);
            trackInfo 		= NULL;
        }
        if(titleInfo)   	{
            g_free(titleInfo);
            titleInfo   		= NULL;
        }
        if(artistInfo)   	{
            isArtist++;
            g_free(artistInfo);
            artistInfo  	= NULL;
        }
        if(albumInfo) 	{
            isAlbum++;
            g_free(albumInfo);
            albumInfo 	= NULL;
        }
        if(discInfo)   	{
            isDisk++;
            g_free(discInfo);
            discInfo    	= NULL;
        }

        if(timeInfo)   	{
            isTime++;
            g_free(timeInfo);
            timeInfo   	= NULL;
        }
        if(nameInfo)   {
            g_free(nameInfo);
            nameInfo   	= NULL;
        }

        // FREE MPD ENTITY
        mpd_entity_free(entity);
    }   // LET'S DO IT ALL OVER AGAIN FOR THE NEXT TRACK

    // FREE THE STATUS
    mpd_status_free(status);

    // FREE MPD CONNECTION
    mpd_connection_free(conn);

    // HIDE THE DISK COLUMN IF THERE IS NOTHING IN THE COLOMN
    GtkTreeViewColumn *column;
    column= gtk_tree_view_get_column (mpd_piVars->w_trackviewTreeView, TRACKVIEW_COL_DISC);
    if(isDisk  == 0)
        gtk_tree_view_column_set_visible (column, false);
    else
        gtk_tree_view_column_set_visible (column, true);

    // HIDE THE TRACK COLUMN IF THERE IS NOTHING IN THE COLOMN
    column = gtk_tree_view_get_column (mpd_piVars->w_trackviewTreeView,TRACKVIEW_COL_TRACK);
    if(isTrack == 0)
        gtk_tree_view_column_set_visible (column, false);
    else
        gtk_tree_view_column_set_visible (column, true);

    // HIDE THE ALBUM COLUMN IF THERE IS NOTHING IN THE COLOMN
    column = gtk_tree_view_get_column (mpd_piVars->w_trackviewTreeView,TRACKVIEW_COL_ALBUM);
    if(isAlbum == 0)
        gtk_tree_view_column_set_visible (column, false);
    else
        gtk_tree_view_column_set_visible (column, true);

    // HIDE THE ARTIST COLUMN IF THERE IS NOTHING IN THE COLOMN
    column = gtk_tree_view_get_column (mpd_piVars->w_trackviewTreeView,TRACKVIEW_COL_ARTIST);
    if(isArtist == 0)
        gtk_tree_view_column_set_visible (column, false);
    else
        gtk_tree_view_column_set_visible (column, true);

    return;
}

void highLightTrack (app_variables *mpd_piVars)
{   //  printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    // HIGHLIGHT THE CURRENT PLAYING TRACK
    GtkTreePath *pathPrev = gtk_tree_path_new_from_indices (mpd_piVars->prevSongPos, -1);
    GtkTreeIter iterPrev;

    gtk_tree_model_get_iter (GTK_TREE_MODEL(mpd_piVars->w_trackviewListStore), &iterPrev,   pathPrev);

	// SOMETIMES GETS A VALIST Gtk-CRITICAL WHEN TRACKVIEW_COL_ROW_COLOR IS SET AT THE FOLLOWING LINE
	// (mpd_pi:7433): Gtk-CRITICAL **: 14:37:13.795: gtk_list_store_set_valist: assertion 'iter_is_valid (iter, list_store)' failed
	// IT APPEARS TO HAPPEN ONLY WHEN WHEN LOADING TRACKS FROM THE DATABASE AND PLAYLIST.
	// DOENS'T HAPPEN EVERTTIME, JUST SOME TIMES OR RANDOMLY. NOT SURE???
    gtk_list_store_set (mpd_piVars->w_trackviewListStore,  &iterPrev, TRACKVIEW_COL_ROW_COLOR, NULL,  -1);
    gtk_list_store_set (mpd_piVars->w_trackviewListStore,  &iterPrev, TRACKVIEW_COL_ROW_FONT, NULL,  -1);

    GtkTreePath *pathCur = gtk_tree_path_new_from_indices (mpd_piVars->currentSongPos, -1);
    GtkTreeIter iterCur;
    gtk_tree_model_get_iter (GTK_TREE_MODEL(mpd_piVars->w_trackviewListStore), &iterCur,   pathCur);

    gtk_list_store_set (mpd_piVars->w_trackviewListStore,  &iterCur, TRACKVIEW_COL_ROW_COLOR, mpd_piVars->trackviewHiLiteColor,  -1);
    gtk_list_store_set (mpd_piVars->w_trackviewListStore,  &iterCur, TRACKVIEW_COL_ROW_FONT, "Bold",  -1);

    gtk_tree_path_free (pathCur);
    gtk_tree_path_free (pathPrev);
    //	printf("End of File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);
    return;
}

// CENTER CURRENT TRACK IN TRACKVIEW BY CLICKING ON ???
// THIS FUNCTION HAS TO RETURN TRUE TO ALLOW TRUE/FALSE FOR THE BUTTON PRESS EVENT
gboolean centerTrackClickEvent (GtkWidget *widget, GdkEventButton *event, app_variables *mpd_piVars)
{   //   printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    // HAS THE "CENTER BUTTON" BEEN RIGHT CLICKED WITH THE MOUSE?
    if(event)
        if (event->type == GDK_BUTTON_PRESS  &&  event->button == 3) // right click
        {
            mpd_piVars->centerTrackEnable =  !mpd_piVars->centerTrackEnable;
            //~ // NEEDS TO RETURN TRUE HERE SO RIGHT CLICK DOESN'T FALL THROUGH TO THE WINDOW AND POPUP MENU
            return TRUE;
        }

    GtkTreePath * sortPath;
    GtkTreePath * childPath;

    childPath = gtk_tree_path_new_from_indices (mpd_piVars->currentSongPos, -1);
    sortPath = gtk_tree_model_sort_convert_child_path_to_path(mpd_piVars->w_trackViewModelSort, childPath);

    gtk_tree_view_scroll_to_cell(GTK_TREE_VIEW(mpd_piVars->w_trackviewTreeView), sortPath, NULL, true, 0.5, 0.0);

    gtk_tree_path_free (sortPath);
    gtk_tree_path_free (childPath);

    return TRUE;
}

// THIS IS THE SORT SIGNAL WHEN TREEVIEW HEADER IS CLICKED.
void trackViewModelSort_sort_column_changed_cb(GtkTreeSortable *sortable, app_variables *mpd_piVars)
{   //	printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    // IF ENABLED THEN CENTER THE TRACK IN THE TREEVIEW
    if(mpd_piVars->centerTrackEnable)
        centerTrackClickEvent (NULL, NULL, mpd_piVars);
}
