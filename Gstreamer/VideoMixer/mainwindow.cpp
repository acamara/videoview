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

void MainWindow::startVideo()
{
    GMainLoop *loop;
    GstElement *pipeline;

    //Elements de les fonts d'entrada
    GstElement *bin_font1, *source_1, *tee_1, *queue_1, *sink_1;
    GstElement *bin_font2, *source_2, *tee_2, *queue_2, *sink_2;
    
    //Elementa de sortida
    GstElement *bin_pgm, *queue_12, *queue_22, *videomixer, *tee_pgm, *sink_pgm;
    GstElement *queue_pgm, *queue_fitxer, *encoder, *mux, *conv, *sink_fitxer;
    
    GstBus *bus;

    /* Initialisation */
    gst_init (NULL, NULL);
    loop = g_main_loop_new (NULL, FALSE);

    /* Creació dels elements gstreamer*/
    pipeline = gst_pipeline_new ("video-mixer");

    bin_font1 = gst_bin_new ("bin_font1");
    bin_font2 = gst_bin_new ("bin_font2");
    bin_pgm = gst_bin_new ("bin_pgm");

    source_1 = gst_element_factory_make ("videotestsrc", "test-source1");
    source_2 = gst_element_factory_make ("videotestsrc", "test-source2");

    tee_1 = gst_element_factory_make ("tee", "tee-source1");
    tee_2 = gst_element_factory_make ("tee", "tee-source2");

    queue_1 = gst_element_factory_make("queue", "thread-video1");
    queue_2 = gst_element_factory_make("queue", "thread-video2");

    queue_12 = gst_element_factory_make("queue", "thread-video12");
    queue_22 = gst_element_factory_make("queue", "thread-video22");

    sink_1 = gst_element_factory_make ("directdrawsink", "sink1");
    sink_2 = gst_element_factory_make("directdrawsink", "sink2");
    sink_pgm = gst_element_factory_make("directdrawsink", "sinkpgm");

    videomixer = gst_element_factory_make("videomixer", "video-mixer");

    tee_pgm = gst_element_factory_make ("tee", "tee-pgm");
    queue_pgm = gst_element_factory_make("queue", "thread-pgm");
    queue_fitxer = gst_element_factory_make("queue", "thread-fitxersortida");

    encoder = gst_element_factory_make ("theoraenc", "video-encoder");
    mux = gst_element_factory_make ("oggmux", "ogg-mux");
    conv = gst_element_factory_make ("ffmpegcolorspace","color-converter");
    sink_fitxer = gst_element_factory_make ("filesink", "file-output");

    g_object_set (G_OBJECT (source_2), "pattern", 1 , NULL);

    gst_element_set_state(sink_1, GST_STATE_READY);
    gst_element_set_state(sink_2, GST_STATE_READY);
    gst_element_set_state(sink_pgm, GST_STATE_READY);


    if (!pipeline || !bin_font1 || !source_1 || !tee_1 || !queue_1 || !sink_1 ||
                     !bin_font2 || !source_2 || !tee_2 || !queue_2 || !sink_2 ||
                     !queue_12 || !queue_22 || !videomixer || !tee_pgm ||
                     !queue_pgm || !sink_pgm ||
                     !queue_fitxer || !conv || !encoder || !mux || !sink_fitxer) {
        g_printerr ("One element could not be created. Exiting.\n");
    //return -1;
    }

    g_object_set (G_OBJECT(sink_fitxer), "location", "sortida.ogg", NULL);

    /* we add a message handler */
    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    gst_bus_add_watch (bus, bus_call, loop);
    gst_object_unref (bus);

    /* we add all elements into the pipeline */
    /* test-source | tee | queue | video-output
       test-source | tee | queue | video-output
                           queue | video-mixer | tee | queue | video-output
                                                             | conv | encoder | mux | sink*/

    gst_bin_add_many (GST_BIN (bin_font1), source_1, tee_1, queue_1, queue_12, sink_1, NULL);
    gst_bin_add_many (GST_BIN (bin_font2), source_2, tee_2, queue_2, queue_22, sink_2, NULL);
    gst_bin_add_many (GST_BIN (bin_pgm),videomixer, tee_pgm, queue_fitxer, queue_pgm,
                      sink_pgm, conv, encoder, mux, sink_fitxer, NULL);

    /* add the bin to the pipeline */
    gst_bin_add_many (GST_BIN (pipeline), bin_font1, bin_font2, bin_pgm, NULL);

    /* we link the elements together */
    /* test-source -> tee -> queue -> video-output */
    gst_element_link_many (source_1, tee_1, queue_1, sink_1, NULL);
    gst_element_link_many (source_2, tee_2, queue_2, sink_2, NULL);
    gst_element_link_many (tee_1, queue_12, videomixer, NULL);
    gst_element_link_many (tee_2, queue_22, videomixer, NULL);
    gst_element_link_many (videomixer, tee_pgm, NULL);
    gst_element_link_many (tee_pgm, queue_pgm, sink_pgm, NULL);
    gst_element_link_many (tee_pgm, queue_fitxer, conv, encoder, mux, sink_fitxer, NULL);

    /* Set the pipeline to "playing" state*/
    g_print ("Now playing: %s\n","Mixer example");
    gst_element_set_state (pipeline, GST_STATE_PLAYING);

    //---------------------------------------------------------------
    //Una cosa similar a això hauria de permetre fer el MIX
    g_object_set (G_OBJECT (videomixer), "sink_0::alpha", 0 , NULL);

    //---------------------------------------------------------------
    //No acaba de funciona però això en principi hauria de servir per veure el video en el widget
    //---------------------------------------------------------------
    //gst_x_overlay_set_xwindow_id(GST_X_OVERLAY (sink_1),gulong(ui->widget->winId()));
    //gst_x_overlay_set_xwindow_id(GST_X_OVERLAY (sink_2),gulong(ui->widget_2->winId()));
    //gst_x_overlay_set_xwindow_id(GST_X_OVERLAY (sink_pgm),gulong(ui->widget_3->winId()));
    //QApplication::syncX();
    //---------------------------------------------------------------*/

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

