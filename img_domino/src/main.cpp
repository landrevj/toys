#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

#include "Image.h"
#include "GrayscaleEffect.h"
#include "DominoEffect.h"
#include "QuantizeEffect.h"

int main(int argc, char** argv)
{

  if (argc < 4)
  {
    std::cout << "Usage: img_domino image_path(string) target_height(uint) effect_cutoff(0-255) invert_output(0/1)" << std::endl;
    return 1;
  }

  Image *img = new Image(argv[1]);
  std::cout << "input dimensions: " << '\t' << img->get_width() << '\t' << img->get_height() << '\t' << img->get_channels() << std::endl;
  
  int num_effects = 3;
  ImageEffect **effect_sequence = new ImageEffect*[num_effects];
  GrayscaleEffect *ge = new GrayscaleEffect();
  QuantizeEffect  *qe = new QuantizeEffect(atoi(argv[3]), atoi(argv[4]));
  DominoEffect    *de = new DominoEffect();
  effect_sequence[0]  = ge;
  effect_sequence[1]  = qe;
  effect_sequence[2]  = de;

  int target_height = atoi(argv[2]);
  if (target_height)
  {
    img->resize_image(target_height);
    std::cout << "resized dimensions: \t" << img->get_width() << '\t' << img->get_height() << '\t' << img->get_channels() << std::endl;
  }

  img->apply_effects(effect_sequence, num_effects);

  std::ofstream ofs("output.txt");
  ofs << de->get_output();
  ofs.close();

  delete ge;
  delete qe;
  delete de;
  delete[] effect_sequence;
  delete img;
  
  return 0;
}
