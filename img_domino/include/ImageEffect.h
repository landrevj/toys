#ifndef IMAGE_EFFECT_H
#define IMAGE_EFFECT_H

#include <string>
using std::string;
class Image;

class ImageEffect
{
private:
  string name;
  virtual unsigned char *effect(unsigned char *, int *, int *, int *) = 0;

public:
  ImageEffect() : name("ImageEffect") {}
  ImageEffect(string effect_name) : name(effect_name) {}

  string get_name() { return name; }
  void apply_effect(Image *);
};
#endif // IMAGE_EFFECT_H
