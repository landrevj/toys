#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include "ImageEffect.h"
using std::string;

class Image
{
private:
  string path;
  unsigned char *data;
  int width, height, channels;
  bool has_image_loaded;
  bool modified_in_memory;

  void clear_data();

public:
  Image();
  Image(string);
  ~Image();

  bool load_from_file(string);
  bool write_to_file(string);
  void resize_image(int);

  void apply_effect(ImageEffect *);
  void apply_effects(ImageEffect **, int);

  unsigned char *get_data();
  void set_new_data(unsigned char *, int, int, int);
  string get_filepath();
  int get_width();
  int get_height();
  int get_channels();
  bool is_image_loaded();
  bool has_been_modified();
};

#endif // IMAGE_H
