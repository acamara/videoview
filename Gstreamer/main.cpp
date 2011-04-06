#include <QtCore/QCoreApplication>
#include <QDebug>
#include <iostream>

#include <glib.h>
#include <gst/gst.h>


int main(int argc, char *argv[])
{
    //filesrc location=music.ogg ! decodebin ! audioconvert ! directsoundsink
    GstElement *pipeline;
    GstElement *source,*decoder,*conv, *sink;

    /* Initicialització */
    gst_init (&argc, &argv);

    /* Creació pipeline */
    pipeline = gst_pipeline_new ("audio-pipeline");

    /* Creació gstreamer elements */
    source = gst_element_factory_make ("filesrc", "file-source");
    decoder = gst_element_factory_make ("decodebin", "audio-decode");
    conv = gst_element_factory_make ("audioconvert", "audio-converter");
    sink = gst_element_factory_make ("directsoundsink", "directsound-output");

    if (!source || !decoder|| !conv || !sink) {
        std::cout<<"Un dels elements no es pot crear";
    }

    g_object_set (G_OBJECT(source), "location", "music.ogg", NULL);

    /* Afegim els elements al pipeline abans de linkar-los*/
    gst_bin_add_many (GST_BIN_CAST(pipeline), source, decoder, conv, sink, NULL);

    /* Linkem els elements */
    if (!gst_element_link_many (source, decoder, conv, sink, NULL)) {
        std::cout<<"No s'han pogut linkar els elements! ";
    }

    gst_element_set_state(pipeline,GST_STATE_PLAYING);
    std::cout<<"Reproduint fitxer d'àudio per la sortida directsoundsink";

    gst_object_unref(GST_OBJECT_CAST(pipeline));

    QCoreApplication a(argc, argv);
    return a.exec();
}
