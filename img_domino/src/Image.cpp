#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION 
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#include "Image.h"

Image::Image() : path(""), data(NULL), width(0), height(0), channels(0), has_image_loaded(false), modified_in_memory(false)
{

}

Image::Image(string filepath)
{
  has_image_loaded   = false;
  modified_in_memory = false;
  load_from_file(filepath);
}

Image::~Image()
{
  clear_data();
}

bool Image::load_from_file(string filepath)
{
  if (has_image_loaded == false)
  {
    path = filepath;
    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    if (data == NULL)
    {
      std::cerr << "Unable to load image: " << filepath << std::endl;
      return false;
    }
    has_image_loaded = true;
    return true;
  }
  else
  {
    clear_data();
    return load_from_file(filepath);
  }
}

bool Image::write_to_file(string filepath)
{
  std::cout << "\tsaving: " << filepath << std::endl;
  stbi_write_jpg(filepath.c_str(), width, height, channels, data, 100);
  return true;
}

void Image::resize_image(int target_height)
{
  int target_width = (target_height * width) / height;
  unsigned char* new_data = new unsigned char[target_width * target_height * channels];

  stbir_resize_uint8(data, width, height, 0, new_data, target_width, target_height, 0, channels);
  set_new_data(new_data, target_width, target_height, channels);
}

void Image::apply_effect(ImageEffect *effect)
{
  effect->apply_effect(this);
}

void Image::apply_effects(ImageEffect **effect_sequence, int num_effects)
{
  for (int i = 0; i < num_effects; i++)
  {
    effect_sequence[i]->apply_effect(this);
#ifdef OUTPUT_STEP_IMAGES
    write_to_file("image_" + effect_sequence[i]->get_name() + ".jpg");
#endif
  }
}

void Image::clear_data()
{
  stbi_image_free(data);
  path  = "";
  data  = NULL;
  width = height = channels = 0;
  has_image_loaded   = false;
  modified_in_memory = false;
}

unsigned char *Image::get_data() { return data; }

void Image::set_new_data(unsigned char *new_data, int new_width, int new_height, int new_channels)
{
  stbi_image_free(data);
  width              = new_width;
  height             = new_height;
  channels           = new_channels;
  data               = new_data;
  modified_in_memory = true;
}

string Image::get_filepath() { return path; }

int Image::get_width() { return width; }

int Image::get_height() { return height; }

int Image::get_channels() { return channels; }

bool Image::is_image_loaded() { return has_image_loaded; }

bool Image::has_been_modified() { return modified_in_memory; }
