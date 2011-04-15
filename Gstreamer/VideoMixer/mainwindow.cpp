#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <gst/gst.h>
#include <glib.h>
#include <gst/interfaces/xoverlay.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionStart,SIGNAL(triggered()),this,SLOT(startVideo()));
    connect(ui->actionStop,SIGNAL(triggered()),this,SLOT(stopVideo()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

static gboolean bus_call (GstBus *bus,GstMessage *msg,gpointer data)
{
    GMainLoop *loop = (GMainLoop *) data;
    switch (GST_MESSAGE_TYPE (msg)) {
        case GST_MESSAGE_EOS:
            g_print ("Final del stream\n");
            g_main_loop_quit (loop);
        break;
        case GST_MESSAGE_ERROR: {
            gchar *debug;
            GError *error;
            gst_message_parse_error (msg, &error, &debug);
            g_free (debug);
            g_printerr ("Error: %s\n", error->message);
            g_error_free (error);
            g_main_loop_quit (loop);
        break;
        }
        default:
        break;
    }
    return TRUE;
}

static void on_pad_added (GstElement *element,GstPad *pad,gpointer data)
{
    GstPad *sinkpad;
    GstElement *decoder = (GstElement *) data;

    /* We can now link this pad with the vorbis-decoder sink pad */
    g_print ("Dynamic pad created, linking demuxer/decoder\n");
    sinkpad = gst_element_get_static_pad (decoder, "sink");
    gst_pad_link (pad, sinkpad);
    gst_object_unref (sinkpad);
}

void MainWindow::startVideo()
{
    GMainLoop *loop;
    GstElement *pipeline;

    //Elements de les fonts d'entrada
    GstElement *bin_font1, *source_1, *tee_1, *queue_1, *sink_1;
    GstElement *bin_font2, *source_2, *tee_2, *queue_2, *sink_2;
    GstElement *bin_font3, *source_3, *demuxer, *queue_audio_3, *queue_video_3, *decoder_audio, *decoder_video;
    GstElement *conv_audio, *conv_video, *tee_3, *queue_3, *sink_audio_3, *sink_3;
    
    //Elementa de sortida
    GstElement *bin_pgm, *queue_12, *queue_22, *queue_32, *videomixer, *tee_pgm, *queue_pgm, *sink_pgm;
    GstElement *queue_fitxer, *conv, *encoder, *mux, *sink_fitxer;
    
    GstBus *bus;

    /* Inicialització */
    gst_init (NULL, NULL);
    loop = g_main_loop_new (NULL, FALSE);

    /* Creació dels elements gstreamer*/
    pipeline = gst_pipeline_new ("video-mixer");

    bin_font1 = gst_bin_new ("bin_font1");
    bin_font2 = gst_bin_new ("bin_font2");
    bin_font3 = gst_bin_new ("bin_font3");
    bin_pgm = gst_bin_new ("bin_pgm");

    source_1 = gst_element_factory_make ("videotestsrc", "test-source1");
    source_2 = gst_element_factory_make ("videotestsrc", "test-source2");
    source_3 = gst_element_factory_make ("filesrc", "file-source3");

    demuxer = gst_element_factory_make ("oggdemux", "ogg-demuxer");
    queue_audio_3 = gst_element_factory_make("queue", "thread-audio");
    queue_video_3 = gst_element_factory_make("queue", "thread-video");
    decoder_audio = gst_element_factory_make ("vorbisdec", "vorbis-decoder");
    decoder_video = gst_element_factory_make ("theoradec", "theora-decoder");
    conv_audio = gst_element_factory_make ("audioconvert", "audio-converter");
    conv_video = gst_element_factory_make ("ffmpegcolorspace","video-converter");
    sink_audio_3 = gst_element_factory_make ("autoaudiosink", "audio-output");
    sink_3 = gst_element_factory_make("xvimagesink", "sink");

    tee_1 = gst_element_factory_make ("tee", "tee-source1");
    tee_2 = gst_element_factory_make ("tee", "tee-source2");
    tee_3 = gst_element_factory_make ("tee", "tee-source3");

    queue_1 = gst_element_factory_make("queue", "thread-video1");
    queue_2 = gst_element_factory_make("queue", "thread-video2");
    queue_3 = gst_element_factory_make("queue", "thread-video3");

    queue_12 = gst_element_factory_make("queue", "thread-video12");
    queue_22 = gst_element_factory_make("queue", "thread-video22");
    queue_32 = gst_element_factory_make("queue", "thread-video32");

    sink_1 = gst_element_factory_make ("xvimagesink", "sink1");
    sink_2 = gst_element_factory_make("xvimagesink", "sink2");
    sink_pgm = gst_element_factory_make("xvimagesink", "sinkpgm");

    videomixer = gst_element_factory_make("videomixer", "video-mixer");

    tee_pgm = gst_element_factory_make ("tee", "tee-pgm");
    queue_pgm = gst_element_factory_make("queue", "thread-pgm");
    queue_fitxer = gst_element_factory_make("queue", "thread-fitxersortida");

    encoder = gst_element_factory_make ("theoraenc", "video-encoder");
    mux = gst_element_factory_make ("oggmux", "ogg-mux");
    conv = gst_element_factory_make ("ffmpegcolorspace","color-converter");
    sink_fitxer = gst_element_factory_make ("filesink", "file-output");

    /*Comprovem que s'han pogut crear tots els elements */
    if (!pipeline || !bin_font1 || !source_1 || !tee_1 || !queue_1 || !sink_1 ||
                     !bin_font2 || !source_2 || !tee_2 || !queue_2 || !sink_2 ||
                     !queue_12 || !queue_22 || !queue_32 || !videomixer || !tee_pgm ||
                     !queue_pgm || !sink_pgm ||
                     !queue_fitxer || !conv || !encoder || !mux || !sink_fitxer ||
                     !bin_font3 || !source_3 || !demuxer || !queue_audio_3 ||
                     !decoder_audio || !conv_audio || !conv_video || !tee_3 || !queue_3|| !sink_audio_3 ||
                     !queue_video_3 || !decoder_video || !sink_3) {
        g_printerr ("Un dels elements no s'ha pogut crear. Sortint.\n");
    //return -1;
    }

    /*Canvi de les propietats d'alguns elements */
    g_object_set (G_OBJECT (source_2), "pattern", 15 , NULL);
    g_object_set (G_OBJECT (source_3), "location","video.ogg", NULL);
    g_object_set (G_OBJECT (videomixer), "background", 1 , NULL);
    g_object_set (G_OBJECT(sink_fitxer), "location", "sortida.ogg", NULL);

    /*Establim a ready l'estat dels element de sortida */
    gst_element_set_state(sink_1, GST_STATE_READY);
    gst_element_set_state(sink_2, GST_STATE_READY);
    gst_element_set_state(sink_3, GST_STATE_READY);
    gst_element_set_state(sink_pgm, GST_STATE_READY);


    /* Afegim un controlador de missatges */
    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    gst_bus_add_watch (bus, bus_call, loop);
    gst_object_unref (bus);

    /* Afegim tots els elements al bin corresponent */
    /* test-source | tee | queue | video-output
       test-source | tee | queue | video-output
                           queue | video-mixer | tee | queue | video-output
                                                             | conv | encoder | mux | sink*/

    gst_bin_add_many (GST_BIN (bin_font1), source_1, tee_1, queue_1, queue_12, sink_1, NULL);
    gst_bin_add_many (GST_BIN (bin_font2), source_2, tee_2, queue_2, queue_22, sink_2, NULL);
    gst_bin_add_many (GST_BIN (bin_pgm),videomixer, tee_pgm, queue_fitxer, queue_pgm,
                      sink_pgm, encoder, mux, sink_fitxer, NULL);

    /* file-source | ogg-demuxer | queue | vorbis-decoder | audio-converter | audio-output
                                   queue | theora-decoder | video-converter | tee | queue | video-output
                                                                                    queue | videomixer */
    gst_bin_add_many (GST_BIN (bin_font3),source_3, demuxer,
                      queue_audio_3, decoder_audio, conv_audio, sink_audio_3,
                      queue_video_3, decoder_video, conv_video, tee_3, queue_3, queue_32, sink_3, NULL);

    /* Afegim els bin al pipeline */
    gst_bin_add_many (GST_BIN (pipeline), bin_font1, bin_font2, bin_font3, bin_pgm, NULL);

    /* Linkem els elements entre ells */
    /* source_1 -> tee_1 -> queue_1 -> sink_1
       source_2 -> tee_2 -> queue_2 -> sink_2
                            queue -> video-mixer -> tee -> queue -> video-output
                                                                 -> conv -> encoder -> mux -> sink*/
    gst_element_link_many (source_1, tee_1, queue_1, sink_1, NULL);
    gst_element_link_many (source_2, tee_2, queue_2, sink_2, NULL);
    gst_element_link_many (tee_1, queue_12, videomixer, NULL);
    gst_element_link_many (tee_2, queue_22, videomixer, NULL);
    gst_element_link_many (videomixer, tee_pgm, NULL);
    gst_element_link_many (tee_pgm, queue_pgm, sink_pgm, NULL);
    gst_element_link_many (tee_pgm, queue_fitxer, encoder, mux, sink_fitxer, NULL);

    /* file-source -> ogg-demuxer ~> queue -> vorbis-decoder -> converter -> audio-output
                                  ~> queue -> theora-decoder -> converter -> tee -> queue -> video-output*/
    gst_element_link (source_3, demuxer);
    gst_element_link_many (queue_audio_3, decoder_audio, conv_audio, sink_audio_3, NULL);
    gst_element_link_many (queue_video_3, decoder_video, conv_video, tee_3, NULL);
    gst_element_link_many (tee_3, queue_3, sink_3, NULL);
    gst_element_link_many (tee_3, queue_32, videomixer, NULL);
    g_signal_connect (demuxer, "pad-added", G_CALLBACK (on_pad_added), queue_audio_3);
    g_signal_connect (demuxer, "pad-added", G_CALLBACK (on_pad_added), queue_video_3);

    /* Canviem l'estat del pipeline a "playing" */
    g_print ("Now playing: %s\n","Mixer example");
    gst_element_set_state (pipeline, GST_STATE_PLAYING);

    //---------------------------------------------------------------
    //Una cosa similar a això hauria de permetre fer el MIX
    GstPad *mixerpad=gst_element_get_pad(videomixer,"sink_1");
    g_object_set (G_OBJECT(mixerpad), "alpha",0,NULL);
    mixerpad=gst_element_get_pad(videomixer,"sink_2");
    g_object_set (G_OBJECT(mixerpad), "alpha",0,NULL);

    /*---------------------------------------------------------------
    //No acaba de funciona però això en principi hauria de servir per veure el video en el widget
    //---------------------------------------------------------------
    gst_x_overlay_set_xwindow_id(GST_X_OVERLAY (sink_1),gulong(ui->widget->winId()));
    gst_x_overlay_set_xwindow_id(GST_X_OVERLAY (sink_2),gulong(ui->widget_2->winId()));
    gst_x_overlay_set_xwindow_id(GST_X_OVERLAY (sink_pgm),gulong(ui->widget_3->winId()));
    QApplication::syncX();
    //---------------------------------------------------------------*/

    /* Iterem */
    g_print ("Running...\n");
    g_main_loop_run (loop);

    /* Out of the main loop, clean up nicely */
    g_print ("Returned, stopping playback\n");
    gst_element_set_state (pipeline, GST_STATE_NULL);
    g_print ("Deleting pipeline\n");
    gst_object_unref (GST_OBJECT (pipeline));
}

void MainWindow::stopVideo()
{

}
