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
            g_print ("End of stream\n");
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
    GstElement *pipeline, *source, *demuxer, *queue_audio, *queue_video;
    GstElement *decoder_audio, *decoder_video, *conv_audio, *conv_video, *sink_audio, *sink_video;
    GstBus *bus;

    /* Initialisation */
    gst_init (NULL, NULL);
    loop = g_main_loop_new (NULL, FALSE);

    /* Create gstreamer elements */
    pipeline = gst_pipeline_new ("video-player");
    source = gst_element_factory_make ("filesrc", "file-source");
    demuxer = gst_element_factory_make ("oggdemux", "avi-demuxer");
    queue_audio = gst_element_factory_make("queue", "thread-audio");
    queue_video = gst_element_factory_make("queue", "thread-video");
    decoder_audio = gst_element_factory_make ("vorbisdec", "vorbis-decoder");
    decoder_video = gst_element_factory_make ("theoradec", "theora-decoder");
    conv_audio = gst_element_factory_make ("audioconvert", "audio-converter");
    conv_video = gst_element_factory_make ("ffmpegcolorspace", "video-converter");
    sink_audio = gst_element_factory_make ("autoaudiosink", "audio-output");
    sink_video = gst_element_factory_make("directdrawsink", "sink");

    gst_element_set_state(sink_video, GST_STATE_READY);

    if (!pipeline || !source || !demuxer || !queue_audio || !decoder_audio || !conv_audio || !sink_audio
                                         || !queue_video || !decoder_video || !conv_video || !sink_video) {
        g_printerr ("One element could not be created. Exiting.\n");
    //return -1;
    }

    /* Set up the pipeline */
    /* we set the input filename to the source element */

    g_object_set (G_OBJECT (source), "location","mpeg2video.ogg", NULL);

    /* we add a message handler */
    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    gst_bus_add_watch (bus, bus_call, loop);
    gst_object_unref (bus);

    /* we add all elements into the pipeline */
    /* file-source | ogg-demuxer | queue | vorbis-decoder | audio-converter | audio-output
                                   queue | theora-decoder | video-converter | video-output */
    gst_bin_add_many (GST_BIN (pipeline),source, demuxer, queue_audio, decoder_audio, conv_audio, sink_audio,
                                                          queue_video, decoder_video, conv_video, sink_video, NULL);

    /* we link the elements together */
    /* file-source -> ogg-demuxer ~> vorbis-decoder -> converter -> audio-output */
    gst_element_link (source, demuxer);
    gst_element_link_many (queue_audio, decoder_audio, conv_audio, sink_audio, NULL);
    gst_element_link_many (queue_video, decoder_video, conv_video, sink_video, NULL);
    g_signal_connect (demuxer, "pad-added", G_CALLBACK (on_pad_added), queue_audio);
    g_signal_connect (demuxer, "pad-added", G_CALLBACK (on_pad_added), queue_video);

    /* note that the demuxer will be linked to the decoder dynamically.
    The reason is that Ogg may contain various streams (for example
    audio and video). The source pad(s) will be created at run time,
    by the demuxer when it detects the amount and nature of streams.
    Therefore we connect a callback function which will be executed
    when the "pad-added" is emitted.*/

    /* Set the pipeline to "playing" state*/
    g_print ("Now playing: %s\n","mpeg2video");
    gst_element_set_state (pipeline, GST_STATE_PLAYING);

    //---------------------------------------------------------------
    //No acaba de funciona però això en principi hauria de servir per veure el video en el widget
    //---------------------------------------------------------------
    gst_x_overlay_set_xwindow_id(GST_X_OVERLAY (sink_video),gulong(ui->widget->winId()));
    QApplication::syncX();
    //---------------------------------------------------------------

    /* Iterate */
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

