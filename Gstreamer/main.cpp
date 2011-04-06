#include <QtCore/QCoreApplication>
#include <QDebug>
#include <iostream>

#include <glib.h>
#include <gst/gst.h>


int main(int argc, char *argv[])
{
    GstElement *pipeline;
    GstElement *bin;
    GstElement *source,*conv, *sink;

    /* Initicialització */
    gst_init (&argc, &argv);

    /* Creació pipeline */
    pipeline = gst_pipeline_new ("my-pipeline");
    bin=gst_bin_new("my-bin");

    /* Creació gstreamer elements */
    source = gst_element_factory_make ("dshowvideosrc", "videotest-source");
    conv= gst_element_factory_make ("ffmpegcolorspace", "color-converter");
    sink = gst_element_factory_make ("directdrawsink", "direct-output");

    if (!source|| !conv || !sink) {
        std::cout<<"Un dels elements no es pot crear";
    }



    /* Afegim els elements al pipeline abans de linkar-los*/
    gst_bin_add_many (GST_BIN_CAST(pipeline), source, conv, sink, NULL);

    /* Linkem els elements */
    if (!gst_element_link_many (source,conv, sink, NULL)) {
        std::cout<<"Failed to link elements!";
    }

    gst_element_set_state(pipeline,GST_STATE_PLAYING);
    std::cout<<"Reproduint l'entrada de video dshowvideosrc per la sortida directdrawsink";

    gst_object_unref(GST_OBJECT_CAST(pipeline));

    QCoreApplication a(argc, argv);
    return a.exec();
}
