#ifndef EPROCESSAT_H
#define EPROCESSAT_H

#include <QtGui>
#include <gst/gst.h>
#include <glib.h>
#include <gst/interfaces/xoverlay.h>

struct ElementsComuns {
  GstElement *bin, *tee, *queue, *queue_mix;
  GstCaps *caps;
  void creacomuns(int k, QString ref);
};

struct EntradaVideo: public ElementsComuns {
  GstElement *source, *color_conv, *scale, *scale_mix, *sink;
  void crea(int k, GstElement *, const char *type,QSize resolucio, int framerate);
  void creatransformadors(int k);
};

struct EntradaAudio: public ElementsComuns {
  GstElement *source, *volume_mix;
  void crea(int k, GstElement *);
};

struct EntradaFitxer {
  EntradaVideo v;
  EntradaAudio a;

  GstElement *bin_font, *source, *dec, *conv_audio, *conv_video;
  GstPad     *audiopad, *videopad;
  void crea(int k, GstElement *,QString nomfitxer);
};

struct EntradaLogo {
  GstElement *bin_logo, *source, *dec, *imagefreeze, *queue, *conv_logo;
  void crea(GstElement *,QString nomfitxer);
};

struct VideoPGM: public ElementsComuns {
  GstElement  *mixer, *color_conv, *textoverlay, *sink;
  void crea(int k, GstElement *);
};

struct AudioPGM: public ElementsComuns {
  GstElement  *mixer, *volum, *level, *sink;
  void crea(int k, GstElement *);
};

struct SortidaFitxer {
    GstElement  *bin_fitxer, *conv; // "*encoder;" Es podria ficar en cas de voler codificar al instant
    void crea(GstElement *, GstElement *, GstElement *, const char* type, const char* typeconverter, const char* typeencoder);
};


#endif // EPROCESSAT_H
