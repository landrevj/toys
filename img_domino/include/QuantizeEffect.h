#ifndef QUANTIZE_EFFECT_H
#define QUANTIZE_EFFECT_H

#include <iostream>
#include <string>
#include "ImageEffect.h"
using std::string;

class QuantizeEffect : public ImageEffect
{
private:
  uint8_t cutoff;
  bool invert_output_colors;

  unsigned char *effect(unsigned char *data, int *width, int *height, int *channels)
  {
    if ((*channels) != 1)
    {
      std::cerr << "Image data to QuantizeEffect must have only one channel." << std::endl;
      return NULL;
    }

    int size = (*width) * (*height);
    unsigned char *new_data = new unsigned char[size];
    std::memcpy(new_data, data, size);

    for (unsigned char *pixel = new_data; pixel != new_data + size; pixel++)
    {
      uint8_t first_color = invert_output_colors ? 255 : 0;
      uint8_t second_color = 255 - first_color;

      if (*pixel <= cutoff)
        *pixel = first_color;
      else
        *pixel = second_color;
    }

    return new_data;
  }

public:
  QuantizeEffect() : ImageEffect("QuantizeEffect"), cutoff(255 / 2), invert_output_colors(false) {}
  QuantizeEffect(uint8_t effect_cutoff, bool invert_colors) : ImageEffect("QuantizeEffect"), cutoff(effect_cutoff), invert_output_colors(invert_colors) {}
};

#endif // QUANTIZE_EFFECT_H
