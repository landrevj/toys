#ifndef GRAYSCALE_H
#define GRAYSCALE_H

#include <cstring>
#include <iostream>
#include "ImageEffect.h"

class GrayscaleEffect : public ImageEffect
{
private:
  // this is the only thing it does so I guess ill just put it in the header?
  unsigned char *effect(unsigned char *data, int *width, int *height, int *channels)
  {
    int size = (*width) * (*height) * (*channels);

    int new_channels = (*channels) == 4 ? 2 : 1; // reduce to one channel, two if the image had an alpha channel
    int new_size = (*width) * (*height) * new_channels;

    unsigned char *new_data = new unsigned char[new_size];

    // https://solarianprogrammer.com/2019/06/10/c-programming-reading-writing-images-stb_image-libraries/
    for (unsigned char *p = data, *pg = new_data; p != data + size; p += *channels, pg += new_channels)
    {
      *pg = (uint8_t)((*p + *(p + 1) + *(p + 2)) / 3.0);
      if ((*channels) == 4)
      {
        *(pg + 1) = *(p + 3);
      }
    }

    *channels = new_channels;
    return new_data;
  }

public:
  GrayscaleEffect() : ImageEffect("GrayscaleEffect") {}
};

#endif // GRAYSCALE_H
