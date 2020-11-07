#ifndef DOMINO_EFFECT_H
#define DOMINO_EFFECT_H
// i didnt even plan for it to be named this lol

#include <bitset>
#include <codecvt>
#include <iostream>
#include <locale>
#include <string>
#include "ImageEffect.h"
using std::string;

#define BRAILLE_CODEPOINT_START 0x2800
#define BRAILLE_CODEPOINT_END   0x28FF

class DominoEffect : public ImageEffect
{
private:
  string output;
  uint8_t cutoff;
  bool invert_output_colors;

  unsigned char *effect(unsigned char *data, int *width, int *height, int *channels)
  {
    // this way this function is set up is inefficient but i am going a bit
    // crazy attempting to index the original array, so we'll do it like this

    if ((*channels) != 1)
    {
      std::cerr << "Image data to DominoEffect must have only one channel." << std::endl;
      return NULL;
    }

    int size = (*width) * (*height);
    unsigned char *new_data = new unsigned char[size];
    std::memcpy(new_data, data, size);

    int wdb2 = (*width) % 2;
    int hdb4 = (*height) % 4;

    int width_sections = ((*width) / 2) + wdb2;
    int height_sections = ((*height) / 4) + hdb4;
    int groups = width_sections * height_sections;
    std::cout << "output dimensions: \t" << width_sections << '*' << height_sections << '=' << groups << " (characters)" << std::endl;

    // allocate a 2d array which we will parse our image data into
    unsigned char **pixel2d = new unsigned char *[(*height)];
    for (int i = 0; i < (*height); i++)
      pixel2d[i] = new unsigned char[(*width)];

    // parse the image data into the 2d array
    for (int i = 0; i < (*height); i++)
      for (int j = 0; j < (*width); j++)
        pixel2d[i][j] = data[(i * (*width)) + j];

    // allocate a 2d array for our list pixel sequences
    unsigned char **pixel_groups = new unsigned char *[groups];
    for (int i = 0; i < groups; i++)
      pixel_groups[i] = new unsigned char[8];

    ////////////////////////////////////////////////////////// Grab 2x4 chunks of pixels from the 2d array and turn them into 1x8 arrays
    int curr_group = 0;
    for (int hs = 0; hs < (*height); hs += 4)
    {
      for (int ws = 0; ws < (*width); ws += 2)
      {
        int rows = ((*height) - hs < 4) ? (*height) - hs : 4; // do 4 rows each time except for the last chunk which might consist of less than 4 rows
        for (int r = 0; r < rows; r++)
        {
          uint8_t a = 0, b = 0;

          a = pixel2d[hs + r][ws];
          if (ws < (*width)) // similarly dont do the last column if it isnt there
            b = pixel2d[hs + r][ws + 1];

          pixel_groups[curr_group][r * 2] = a;
          pixel_groups[curr_group][(r * 2) + 1] = b;
        }
        curr_group++;
      }
    }

    ////////////////////////////////////////////////////////// Send each group to be turned into a braille character and appended to output
    for (int i = 0; i < groups; i++)
    {
      string group_str = "";
      for (int j = 0; j < 8; j++)
        group_str += (pixel_groups[i][j] == 255) ? "1" : "0";

      output += pixel_group_to_braille(group_str);
      if ((i+1) % width_sections == 0) output += '\n'; // newline at end of rows
    }

    ////////////////////////////////////////////////////////// Delete the arrays we created
    for (int i = 0; i < groups; i++)
      delete[] pixel_groups[i];
    delete[] pixel_groups;

    for (int i = 0; i < (*height); i++)
      delete[] pixel2d[i];
    delete[] pixel2d;

    return new_data;
  }

  string pixel_group_to_braille(string group_str)
  {
    /* the way bits for braille are organized is kinda jumbled and backwards:
      https://en.wikipedia.org/wiki/Braille_Patterns#Identifying,_naming_and_ordering

      so this next bit just reorders them from how i grabbed them to match they way they are layed out in unicode.
    */
    string braille_order = "";
    braille_order += group_str[7];
    braille_order += group_str[6];
    braille_order += group_str[5];
    braille_order += group_str[3];
    braille_order += group_str[1];
    braille_order += group_str[4];
    braille_order += group_str[2];
    braille_order += group_str[0];

    std::bitset<8> braille_bits(braille_order);

    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    string u8str = converter.to_bytes(BRAILLE_CODEPOINT_START + braille_bits.to_ulong());

    return u8str;
  }

public:
  DominoEffect() : ImageEffect("DominoEffect"), output("") {}
  string get_output() { return output; }
};

#endif // DOMINO_EFFECT_H
