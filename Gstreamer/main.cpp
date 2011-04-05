#include <QtCore/QCoreApplication>
#include <QDebug>
#include <iostream>

#include <glib.h>
#include <gst/gst.h>


int main(int argc, char *argv[])
{
    GstElement *pipeline;
    GstElement *source,*sink;

    /* Initicialització */
    gst_init (&argc, &argv);

    /* Creació pipeline */
    pipeline = gst_pipeline_new ("my-pipeline");

    /* Creació gstreamer elements */
    source = gst_element_factory_make ("videotestsrc", "videotest-source");
    sink = gst_element_factory_make ("directdrawsink", "direct-output");


    if (!source || !sink) {
        std::cout<<"Un dels elements no es pot crear";
    }

    std::cout<<"S'han creat dos elements, un videotest i un directdrawsink";

    QCoreApplication a(argc, argv);
    return a.exec();
}
