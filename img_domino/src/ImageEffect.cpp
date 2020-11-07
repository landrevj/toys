#include "ImageEffect.h"
#include "Image.h"

void ImageEffect::apply_effect(Image *img)
{
  int width    = img->get_width();
  int height   = img->get_height();
  int channels = img->get_channels();
  unsigned char *data = effect(img->get_data(), &width, &height, &channels);

  img->set_new_data(data, width, height, channels);
}
