#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <gst/gst.h>
#include <glib.h>
#include <gst/interfaces/xoverlay.h>

#include <QFileDialog>

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

void MainWindow::closeEvent(QCloseEvent *)
{
    stopVideo();
}



static gboolean bus_call (GstBus *bus, GstMessage *msg, gpointer data)
{
  GMainLoop *loop = (GMainLoop *) data;

  switch (GST_MESSAGE_TYPE (msg)) {

    case GST_MESSAGE_EOS:
      g_print ("Final del stream\n");
      g_main_loop_quit (loop);
      break;

    case GST_MESSAGE_ERROR: {
      gchar  *debug;
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

GstElement *pipeline, *audio, *video;

static void cb_newpad_audio (GstElement *decodebin, GstPad *pad, gboolean last, gpointer data)
{
  GstCaps *caps;
  GstStructure *str;
  GstPad *audiopad;

  /* Linkem només una vegada */
  audiopad = gst_element_get_static_pad (audio, "sink");
  if (GST_PAD_IS_LINKED (audiopad)) {
    g_object_unref (audiopad);
    return;
  }

  /* Mirem els tipus de dades multimedia */
  caps = gst_pad_get_caps (pad);
  str = gst_caps_get_structure (caps, 0);
  if (!g_strrstr (gst_structure_get_name (str), "audio")) {
    gst_caps_unref (caps);
    gst_object_unref (audiopad);
    return;
  }
  gst_caps_unref (caps);

  /* link'n'play */
  gst_pad_link (pad, audiopad);

  g_object_unref (audiopad);
}

static void cb_newpad_video (GstElement *decodebin, GstPad *pad, gboolean last, gpointer data)
{
  GstCaps *caps;
  GstStructure *str;
  GstPad *videopad;

  /* Linkem només una vegada */
  videopad = gst_element_get_static_pad (video, "sink");
  if (GST_PAD_IS_LINKED (videopad)) {
    g_object_unref (videopad);
    return;
  }

  /* Mirem els tipus de dades multimedia*/
  caps = gst_pad_get_caps (pad);
  str = gst_caps_get_structure (caps, 0);
  if (!g_strrstr (gst_structure_get_name (str), "video")) {
    gst_caps_unref (caps);
    gst_object_unref (videopad);
    return;
  }
  gst_caps_unref (caps);

  /* link'n'play */
  gst_pad_link (pad, videopad);

  g_object_unref (videopad);
}

void MainWindow::startVideo()
{
    GMainLoop *loop;
    GstElement *src, *dec, *conv_audio, *sink_audio, *conv_video, *sink_video;
    GstPad *audiopad, *videopad;
    GstBus *bus;

    /* Inicialització de GStreamer */
    gst_init (NULL, NULL);
    loop = g_main_loop_new (NULL, FALSE);

    /* Creem el pipeline */
    pipeline = gst_pipeline_new ("pipeline");

    bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
    gst_bus_add_watch (bus,bus_call, loop);
    gst_object_unref (bus);

    /* Creem els elements i els afegim al pipeline */
    //Creem entrada de fitxer i el decodebin, els afegim al pipeline i els linkem.
    src = gst_element_factory_make ("filesrc", "source");
    dec = gst_element_factory_make ("decodebin", "decoder");
    g_signal_connect (dec, "new-decoded-pad", G_CALLBACK (cb_newpad_audio), NULL);
    g_signal_connect (dec, "new-decoded-pad", G_CALLBACK (cb_newpad_video), NULL);
    gst_bin_add_many (GST_BIN (pipeline), src, dec, NULL);
    gst_element_link (src, dec);

    // Creem la sortida d'àudio
    audio = gst_bin_new ("audiobin");
    conv_audio = gst_element_factory_make ("audioconvert", "aconv");
    audiopad = gst_element_get_static_pad (conv_audio, "sink");
    sink_audio = gst_element_factory_make ("autoaudiosink", "sink");
    gst_bin_add_many (GST_BIN (audio), conv_audio, sink_audio, NULL);
    gst_element_link (conv_audio, sink_audio);
    gst_element_add_pad (audio,
          gst_ghost_pad_new ("sink", audiopad));
    gst_object_unref (audiopad);
    gst_bin_add (GST_BIN (pipeline), audio);

    // Creem la sortida de vídeo
    video = gst_bin_new ("videobin");
    conv_video = gst_element_factory_make ("ffmpegcolorspace", "vconv");
    videopad = gst_element_get_static_pad (conv_video, "sink");
    sink_video = gst_element_factory_make ("xvimagesink", "sink");
    gst_bin_add_many (GST_BIN (video), conv_video, sink_video, NULL);
    gst_element_link (conv_video, sink_video);
    gst_element_add_pad (video,
          gst_ghost_pad_new ("sink", videopad));
    gst_object_unref (videopad);
    gst_bin_add (GST_BIN (pipeline), video);

    /* Seleccionem el fitxer d'entrada*/
    QString nom_fitxer = QFileDialog::getOpenFileName();
    const char *c_nom_fitxer = nom_fitxer.toStdString().c_str();
    g_object_set (G_OBJECT (src), "location", c_nom_fitxer, NULL);
    gst_element_set_state(sink_video, GST_STATE_READY);

    //---------------------------------------------------------------
    //No acaba de funciona per aix en principi hauria de servir per veure el video en el widget
    //---------------------------------------------------------------
    gst_x_overlay_set_xwindow_id(GST_X_OVERLAY (sink_video),gulong(ui->centralWidget->winId()));
    QApplication::syncX();
    //---------------------------------------------------------------

    /* run */
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
    g_main_loop_run (loop);

    stopVideo();

}

void MainWindow::stopVideo()
{
    /* cleanup */
    gst_element_set_state (pipeline, GST_STATE_NULL);
    gst_object_unref (GST_OBJECT (pipeline));
}
