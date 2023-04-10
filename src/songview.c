// songview.c
#include "mpd_pi.h"
extern gint globalVerbose;
void songView(app_variables *mpd_piVars)
{   // printf("Inside File %s -- Func %s -- Line %d --\n",__FILE__,  __func__, __LINE__);

    g_autofree gchar *songUri 							= NULL;
    g_autofree gchar *songMetaName 				= NULL;
    g_autofree gchar *songMetaTitle  				= NULL;
    g_autofree gchar *songMetaArtist 				= NULL;
    g_autofree gchar *songMetaAlbumArtist		= NULL;
    g_autofree gchar *songMetaAlbum 			= NULL;
    g_autofree gchar *songMetaDisc 				= NULL;
    g_autofree gchar *songMetaTrack 				= NULL;
    g_autofree gchar *discNumber 					= g_strdup("Disc #");
    g_autofree gchar *trackNumber 					= g_strdup("Track #");
    g_autofree gchar *seperator 						= g_strdup("----");

    //~ GString *songStr = g_string_new (NULL);

    // GET MPD CONNECTION
    struct mpd_connection *conn;
    conn = get_connection(mpd_piVars);

    struct mpd_status * status;
    mpd_send_status(conn);
    status = mpd_recv_status(conn);

    mpd_status_free(status);

    struct mpd_song *song = NULL;
    mpd_send_current_song(conn);
    song = mpd_recv_song(conn);
    if(!song) {
        linenum;
        printf("NOT SONG\n");
    }


    songUri 	            		= g_strdup( mpd_song_get_uri(song));
    songMetaName			= g_strdup(mpd_song_get_tag(song, 	MPD_TAG_NAME, 	0));
    songMetaTitle 				= g_strdup(mpd_song_get_tag(song, 	MPD_TAG_TITLE, 		0));
    songMetaArtist			= g_strdup( mpd_song_get_tag(song,	MPD_TAG_ARTIST, 	0));
    songMetaAlbumArtist	= g_strdup( mpd_song_get_tag(song,	MPD_TAG_ALBUM_ARTIST, 	0));
    songMetaAlbum			= g_strdup( mpd_song_get_tag(song,	MPD_TAG_ALBUM, 	0));
    songMetaDisc  			= g_strdup( mpd_song_get_tag(song, MPD_TAG_DISC, 		0));
    songMetaTrack 			= g_strdup( mpd_song_get_tag(song, MPD_TAG_TRACK, 	0));

    mpd_song_free(song);

    mpd_connection_free(conn);

    g_autofree gchar *diskTrack = NULL;

    if(songMetaDisc)   diskTrack = g_strdup_printf("Track # %s", songMetaTrack);
    if(songMetaTrack)   diskTrack = g_strdup_printf( "Track # %s", songMetaTrack);
    if(songMetaDisc && songMetaTrack)   diskTrack = g_strdup_printf("%s %s %s %s %s", discNumber, songMetaDisc, seperator, trackNumber, songMetaTrack);


    if((globalVerbose == 4) || (globalVerbose > 9)) {
        linenum;
        printf("songMetaName %s\n", songMetaName);
    }
    if((globalVerbose == 4) || (globalVerbose > 9)) {
        linenum;
        printf("songMetaTitle %s\n", songMetaTitle);
    }
    if((globalVerbose == 4) || (globalVerbose > 9)) {
        linenum;
        printf("songMetaArtist %s\n", songMetaArtist);
    }
    if((globalVerbose == 4) || (globalVerbose > 9)) {
        linenum;
        printf("songMetaAlbumArtist %s\n", songMetaAlbumArtist);
    }
    if((globalVerbose == 4) || (globalVerbose > 9)) {
        linenum;
        printf("songMetaAlbum %s\n", songMetaAlbum);
    }
    if((globalVerbose == 4) || (globalVerbose > 9)) {
        linenum;
        printf("diskTrack %s\n", diskTrack);
    }
    if((globalVerbose == 4) || (globalVerbose > 9)) {
        linenum;
        printf("songUri %s\n\n", songUri);
    }

    setSongLabel(songMetaName, mpd_piVars->w_lblSongName, "songMetaName");
    setSongLabel(songMetaTitle, mpd_piVars->w_lblSongTitle, "songMetaTitle");
    setSongLabel(songMetaArtist, mpd_piVars->w_lblSongArtist, "songMetaArtist");
    setSongLabel(songMetaAlbum, mpd_piVars->w_lblSongAlbum, "songMetaAlbum");
    setSongLabel(songMetaAlbumArtist, mpd_piVars->w_lblSongAlbumArtist, "songMetaAlbumArtist");
    setSongLabel(diskTrack, mpd_piVars->w_lblSongDiskTrack, "diskTrack");
    setSongLabel(songUri, mpd_piVars->w_lblSongURI, "songUri");

//   printf("End of -- %s -- Line %d --\n", __func__, __LINE__);
    return;
}

gboolean setSongLabel(gchar *meta, GtkLabel *widget, gchar * name)
{

    gint result = 1234;
    if(meta)
    {
        gtk_label_set_text(GTK_LABEL(widget), meta);
        gtk_widget_show(GTK_WIDGET(widget));
    }
    else
        gtk_widget_hide(GTK_WIDGET(widget));

    result = gtk_widget_get_visible (GTK_WIDGET(widget));

    if((globalVerbose == 4) || (globalVerbose > 9)) {
        linenum;
        printf("%s -- META %s -- %s --\n", (result) ? "Show" : "Hide", name, meta);
    }


    return result;
}