#include <QtCore/QCoreApplication>
#include <QDebug>
#include <iostream>

#include <glib.h>
#include <gst/gst.h>


int main(int argc, char *argv[])
{
    //filesrc location=music.ogg ! decodebin ! audioconvert ! audioresample
    //! vorbisenc ! oggmux name=mux ! filesink location=file.ogg
    GstElement *pipeline;
    GstElement *source, *encoder, *mux, *sink;

    /* Initicialització */
    gst_init (&argc, &argv);

    /* Creació pipeline */
    pipeline = gst_pipeline_new ("video-pipeline");

    /* Creació gstreamer elements */
    source = gst_element_factory_make ("videotestsrc", "video-source");
    encoder = gst_element_factory_make ("theoraenc", "video-encoder");
    mux = gst_element_factory_make ("oggmux", "ogg-mux");
    sink = gst_element_factory_make ("filesink", "file-output");

    if (!source || !encoder || !mux || !sink) {
        std::cout<<"Un dels elements no es pot crear";
    }

    g_object_set (G_OBJECT(sink), "location", "sortida.ogg", NULL);

    /* Afegim els elements al pipeline abans de linkar-los*/
    gst_bin_add_many (GST_BIN(pipeline), source, encoder, mux, sink, NULL);

    /* Linkem els elements */
    if (!gst_element_link_many (source, encoder, mux, sink, NULL)) {
        std::cout<<"No s'han pogut linkar els elements! ";
    }

    gst_element_set_state(pipeline,GST_STATE_PLAYING);
    std::cout<<"Gravant fitxer...";

    gst_object_unref(GST_OBJECT(pipeline));

    QCoreApplication a(argc, argv);
    return a.exec();
}
