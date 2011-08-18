#include "eprocessat.h"

static void cb_newpad_audio (GstElement *decodebin, GstPad *pad, gboolean last, gpointer data)
{
  EntradaFitxer *grup = (EntradaFitxer*)data;
  GstCaps *caps;
  GstStructure *str;
  GstPad *audiopad;

  //Linkem només una vegada
  audiopad = gst_element_get_static_pad (grup->a.bin, "sink");
  if (GST_PAD_IS_LINKED (audiopad)) {
    g_object_unref (audiopad);
    return;
  }

  //Mirem els tipus de dades multimedia
  caps = gst_pad_get_caps (pad);
  str = gst_caps_get_structure (caps, 0);
  if (!g_strrstr (gst_structure_get_name (str), "audio")) {
    gst_caps_unref (caps);
    gst_object_unref (audiopad);
    return;
  }
  gst_caps_unref (caps);

  //Link'n'play
  gst_pad_link (pad, audiopad);

  g_object_unref (audiopad);
}

static void cb_newpad_video (GstElement *decodebin, GstPad *pad, gboolean last, gpointer data)
{
  EntradaFitxer *grup = (EntradaFitxer*)data;
  GstCaps *caps;
  GstStructure *str;
  GstPad *videopad;

  //Linkem només una vegada
  videopad = gst_element_get_static_pad (grup->v.bin, "sink");
  if (GST_PAD_IS_LINKED (videopad)) {
    g_object_unref (videopad);
    return;
  }

  //Mirem els tipus de dades multimedia
  caps = gst_pad_get_caps (pad);
  str = gst_caps_get_structure (caps, 0);
  if (!g_strrstr (gst_structure_get_name (str), "video")) {
    gst_caps_unref (caps);
    gst_object_unref (videopad);
    return;
  }
  gst_caps_unref (caps);

  //Link'n'play
  gst_pad_link (pad, videopad);

  g_object_unref (videopad);
}

/*
inline char *gennom(QString s, int k) {
  return (char*)s.arg(k).toStdString().c_str();
}
*/

static gboolean link_elements_with_filter (GstElement *element1, GstElement *element2, GstCaps *caps)
{
  gboolean link_ok;

  link_ok = gst_element_link_filtered (element1, element2, caps);

  if (!link_ok) {
    g_warning ("No s'ha pogut linkar element1 i element2 amb filtre demanat");
  }

  return link_ok;
}

void ElementsComuns::creacomuns(int k, QString ref)
{
    QString sbin("_bin_%1"), stee("_tee_%1"), squeue("_queue_%1"),squeue_m("_queue_mix_%1");

    bin =       gst_bin_new ((char*)sbin.prepend(ref).arg(k).toStdString().c_str());
    tee =       gst_element_factory_make("tee",    (char*)stee.prepend(ref).arg(k).toStdString().c_str());
    queue =     gst_element_factory_make("queue2", (char*)squeue.prepend(ref).arg(k).toStdString().c_str());
    queue_mix = gst_element_factory_make("queue2", (char*)squeue_m.prepend(ref).arg(k).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if(!bin || !tee || !queue || !queue_mix){
      g_printerr ("Un dels elements de comuns no s'ha pogut crear. Sortint.\n");
    }
}

void EntradaVideo::crea(int k, GstElement *pipeline, const char* type, QSize resolucio, int framerate) {
  //Elements de font d'entrada
  QString ssource("_source_%1"), ssink("_sink_%1");

  creacomuns(k,"video");
  creatransformadors(k);
  source =      gst_element_factory_make(type,          (char*)ssource.prepend("video").arg(k).toStdString().c_str());
  sink =        gst_element_factory_make("xvimagesink", (char*)ssink.prepend("video").arg(k).toStdString().c_str());

  //Comprovem que s'han pogut crear tots els elements d'entrada
  if(!source || !sink){
    g_printerr ("Un dels elements de l'entrada de vídeo no s'ha pogut crear. Sortint.\n");
  }

  //Afegim tots els elements al bin corresponent
  gst_bin_add_many (GST_BIN (bin), source, tee, queue, color_conv, scale, sink, queue_mix, scale_mix, NULL);

  //Afegim els bin al pipeline
  gst_bin_add (GST_BIN (pipeline), bin);

  //Linkem els elements
  caps = gst_caps_new_simple ("video/x-raw-yuv", "width", G_TYPE_INT, resolucio.width(),
                              "height", G_TYPE_INT, resolucio.height(), "framerate", GST_TYPE_FRACTION,framerate, 1, NULL);
  link_elements_with_filter (source,tee,caps);
  gst_element_link_many (tee, queue, scale, sink, NULL);
}

void EntradaVideo::creatransformadors(int k)
{
    QString svconv("colorconverter_%1"), svscale("videoscale_%1"), svscale_m("videoscale_mix_%1");

    color_conv =    gst_element_factory_make("ffmpegcolorspace",(char*)svconv.arg(k).toStdString().c_str());
    scale =         gst_element_factory_make("videoscale",      (char*)svscale.arg(k).toStdString().c_str());
    scale_mix =     gst_element_factory_make("videoscale",      (char*)svscale_m.arg(k).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if(!color_conv || !scale || !scale_mix){
      g_printerr ("Un dels elements transformadors de vídeo no s'ha pogut crear. Sortint.\n");
    }

}

void EntradaFitxer::crea(int k, GstElement *pipeline, QString nom_fitxer)
{
    //Elements de font d'entrada de fitxer
    QString sbin("bin_font%1"),  ssource("source_%1"), sdec("decoder%1"), svolumen_m("volumen_mix%1"), squeue("audio_queue%1");
    QString saconv("audio_conv_%1"), sabin("bin_audio_%1"), sconv("video_conv_%1"), ssink("video_sink_%1");

    //Creem entrada de fitxer i el decodebin, els afegim al pipeline i els linkem.
    bin_font = gst_bin_new ((char*)sbin.arg(k).toStdString().c_str());

    source = gst_element_factory_make ("filesrc",   (char*)ssource.arg(k).toStdString().c_str());
    dec = gst_element_factory_make ("decodebin2",   (char*)sdec.arg(k).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if(!bin_font || !source || !dec){
      g_printerr ("Un dels elements de l'entrada de fitxer no s'ha pogut crear. Sortint.\n");
    }

    g_signal_connect (dec, "new-decoded-pad", G_CALLBACK (cb_newpad_audio), this);
    g_signal_connect (dec, "new-decoded-pad", G_CALLBACK (cb_newpad_video), this);
    gst_bin_add_many (GST_BIN (bin_font), source, dec, NULL);
    gst_element_link (source, dec);

    //Creem l'entrada d'àudio
    a.bin = gst_bin_new ((char*)sabin.arg(k).toStdString().c_str());
    conv_audio =    gst_element_factory_make("audioconvert",    (char*)saconv.arg(k).toStdString().c_str());
    audiopad =      gst_element_get_static_pad (conv_audio, "sink");
    a.queue_mix=    gst_element_factory_make("queue2",          (char*)squeue.arg(k).toStdString().c_str());
    a.volume_mix =  gst_element_factory_make("volume",          (char*)svolumen_m.arg(k).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if(!a.bin || !conv_audio || !audiopad || !a.queue_mix || !a.volume_mix){
      g_printerr ("Un dels elements de l'entrada de fitxer d'àudio no s'ha pogut crear. Sortint.\n");
    }

    gst_bin_add_many (GST_BIN (a.bin), conv_audio, a.queue_mix, a.volume_mix, NULL);
    gst_element_link_many (conv_audio, a.queue_mix, a.volume_mix, NULL);
    gst_element_add_pad (a.bin, gst_ghost_pad_new ("sink", audiopad));
    gst_object_unref (audiopad);
    gst_bin_add (GST_BIN (bin_font), a.bin);

    //Creem l'entrada de vídeo
    v.creacomuns(k,"video_fitxer");
    v.creatransformadors(k);
    conv_video =    gst_element_factory_make ("ffmpegcolorspace",   (char*)sconv.arg(k).toStdString().c_str());
    videopad =      gst_element_get_static_pad (conv_video,         "sink");
    v.sink =        gst_element_factory_make ("xvimagesink",        (char*)ssink.arg(k).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if( !videopad || !conv_video || !v.sink){
      g_printerr ("Un dels elements de l'entrada de fitxer de vídeo no s'ha pogut crear. Sortint.\n");
    }

    gst_bin_add_many (GST_BIN (v.bin), conv_video, v.tee, v.queue, v.scale, v.sink, v.queue_mix, v.color_conv, v.scale_mix, NULL);
    gst_element_link_many (conv_video, v.tee, v.queue, v.scale, v.sink, NULL);
    gst_element_add_pad (v.bin, gst_ghost_pad_new ("sink", videopad));
    gst_object_unref (videopad);
    gst_bin_add (GST_BIN (bin_font), v.bin);

    //Seleccionem el fitxer d'entrada
    const char *c_nom_fitxer = nom_fitxer.toStdString().c_str();
    g_object_set (G_OBJECT (source), "location", c_nom_fitxer, NULL);
    gst_element_set_state(v.sink, GST_STATE_READY);

    //Afegim el bin_video_pgm al pipeline
    gst_bin_add (GST_BIN (pipeline),bin_font);
}

void EntradaAudio::crea(int k, GstElement *pipeline)
{
    //Elements de font d'entrada d'àudio
    QString sbin("bin_audio_%1"), ssource_a("audio_source_%1"), squeue("audio_queue_%1"), svolumen_m("volumen_mix_%1");

    bin = gst_bin_new ((char*)sbin.arg(k).toStdString().c_str());
    source =        gst_element_factory_make("audiotestsrc",    (char*)ssource_a.arg(k).toStdString().c_str());
    queue_mix =     gst_element_factory_make("queue2",          (char*)squeue.arg(k).toStdString().c_str());
    volume_mix =    gst_element_factory_make("volume",          (char*)svolumen_m.arg(k).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if (!source || !queue_mix || !volume_mix){
        g_printerr ("Un dels elements comuns no s'ha pogut crear. Sortint.\n");
    }

    //Canvi de les propietats d'alguns elements
    g_object_set (G_OBJECT (source), "wave",4, NULL);
    g_object_set (G_OBJECT (volume_mix), "volume",0, NULL);

    //Afegim tots els elements al bin_font corresponent
    gst_bin_add_many (GST_BIN (bin), source, queue_mix, volume_mix, NULL);

    //Afegim els bin_font al pipeline
    gst_bin_add (GST_BIN (pipeline), bin);

    //Linkem els elements
    gst_element_link_many (source, queue_mix, volume_mix,NULL);

}

void EntradaLogo::crea(GstElement *pipeline, QString nomfitxer)
{
    //Creem entrada de fitxer
    bin_logo = gst_bin_new ("bin_logo");
    source =        gst_element_factory_make ("filesrc", "fitxer_logo");
    dec =           gst_element_factory_make ("pngdec", "decoder_logo");
    imagefreeze =   gst_element_factory_make("imagefreeze", "imagefreeze_logo");
    queue =         gst_element_factory_make("queue2","queue_logo");
    conv_logo =     gst_element_factory_make("ffmpegcolorspace", "color_conv_logo");

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if(!bin_logo || !source || !dec || !imagefreeze || !queue || !conv_logo){
      g_printerr ("Un dels elements de l'entrada de logo no s'ha pogut crear. Sortint.\n");
    }

    gst_bin_add_many (GST_BIN (bin_logo), source, dec, imagefreeze, queue, conv_logo, NULL);

    const char *c_nom_fitxer = nomfitxer.toStdString().c_str();
    g_object_set (G_OBJECT (source), "location", c_nom_fitxer, NULL);

    //Afegim els elements al pipeline corresponent
    gst_bin_add_many (GST_BIN (pipeline),bin_logo, NULL);

    //Linkem els elements
    gst_element_link_many(source, dec, imagefreeze, queue, conv_logo, NULL);
}

void VideoPGM::crea(int k, GstElement *pipeline){

    QString smixer("videomixer_%1"), ssink("pgm_sink_%1");

    creacomuns(k,"videomixer");
    mixer =         gst_element_factory_make("videomixer" , (char*)smixer.arg(k).toStdString().c_str());
    color_conv =    gst_element_factory_make("ffmpegcolorspace","color_converer_pgm");
    textoverlay =   gst_element_factory_make("textoverlay", "textoverlay");
    sink =          gst_element_factory_make("xvimagesink", (char*)ssink.arg(k).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements
    if (!mixer || !color_conv || !textoverlay || !sink) {
       g_printerr ("Un dels elements de videoPGM no s'ha pogut crear. Sortint.\n");
    }

    g_object_set(G_OBJECT(textoverlay),"silent",true, NULL);

    //Afegim tots els elements al bin_pgm corresponent
    gst_bin_add_many (GST_BIN (bin), mixer, color_conv, textoverlay, tee, queue, queue_mix, sink, NULL);

    //Afegim el bin_pgm al pipeline
    gst_bin_add (GST_BIN (pipeline),bin);

    //Linkem els elements
    gst_element_link_many(mixer, color_conv, textoverlay, tee, queue, sink, NULL);
    gst_element_link(tee, queue_mix);
}

void AudioPGM::crea(int k, GstElement *pipeline){

    QString smixer("audiomixer_%1"), ssink("pgm_sink_%1");

    creacomuns(k,"audiomixer");
    mixer =    gst_element_factory_make("adder" ,       (char*)smixer.arg(k).toStdString().c_str());
    volum =    gst_element_factory_make("volume",       "volum_pgm");
    level =    gst_element_factory_make("level",        "level_pgm");
    sink =     gst_element_factory_make("autoaudiosink",(char*)ssink.arg(k).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements
    if (!mixer || !volum || !level || !sink) {
       g_printerr ("Un dels elements d'AudioPGM no s'ha pogut crear. Sortint.\n");
    }

    GstCaps *caps;
    caps = gst_caps_new_simple ("audio/x-raw-int","rate", G_TYPE_INT, 44100, "channels", G_TYPE_INT, 2, NULL);
    g_object_set(G_OBJECT(mixer),"caps",caps, NULL);
    g_object_set (G_OBJECT (level), "interval",100000000,"message", TRUE, NULL);

    //Afegim tots els elements al bin_pgm corresponent
    gst_bin_add_many (GST_BIN (bin), mixer, tee, queue, queue_mix, volum, level, sink, NULL);

    //Afegim el bin_pgm al pipeline
    gst_bin_add (GST_BIN (pipeline),bin);

    //Linkem els elements
    gst_element_link_many(mixer, tee, queue, volum, level, sink, NULL);
    gst_element_link(tee, queue_mix);
}

void SortidaFitxer::crea(GstElement *pipeline, GstElement *queue,  GstElement *mux_pgm,
                         const char* type, const char* typeconverter, const char* typeencoder)
{
    QString sbin("_bin_fitxer_%1"), sconv("_fitxer_%1"), sencoder("_encoder_%1");

    bin_fitxer = gst_bin_new ((char*)sbin.prepend(type).toStdString().c_str());
    conv =      gst_element_factory_make(typeconverter, (char*)sconv.prepend(typeconverter).toStdString().c_str());
    //encoder =   gst_element_factory_make(typeencoder,   (char*)sencoder.prepend(typeencoder).toStdString().c_str());

    //Comprovem que s'han pogut crear tots els elements d'entrada
    if(!bin_fitxer || !conv){
      g_printerr ("Un dels elements de la sortida a fitxer no s'ha pogut crear. Sortint.\n");
    }

    //Afegim tots els elements al bin corresponent
    gst_bin_add_many (GST_BIN (bin_fitxer), conv, NULL);

    //Afegim els bin_fitxer al pipeline
    gst_bin_add (GST_BIN (pipeline), bin_fitxer);

    //Linkem els elements
    gst_element_link_many (queue, conv, mux_pgm, NULL);
}
