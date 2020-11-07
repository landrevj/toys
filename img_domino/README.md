# img_domino
Should probably be called img_braille instead but here we are.

## Example usage
```img_domino image_path target_height effect_cutoff invert_output```

* ```image_path``` - string - path to input image. (currently only images with only 3 channels until I fix that.)
* ```target_height``` - uint - dot height of the output text. (multiply by 4 to get how many rows of text it will be.)
* ```effect_cutoff``` - 0-255 - this allows you to tune the threshold for which pixels become the foreground and which become the background. (140 seems to be a good starting point.)
* ```invert_output``` - 0 or 1 - lets you invert which pixels get dots and which dont.

The program creates a file called output.txt for the resulting text.

## Example output
```
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⠉⠁⠀⠀⠀⠀⠈⠉⠛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀   ⠀⠀⠈⠙⢿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⠟⠀⠀⠀⣶⣤⣀⣀⣀⣀⣀⣀⣀⣠⣤⣶⠀⠀⠀⠻⣿⣿⣿⣿⣿
⣿⣿⣿⣿⠏⠀⠀⠀⠘⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⢹⣿⣿⣿⣿
⣿⣿⣿⣿⠀⠀⠀⠀⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⠀⠀⠀⠀⣿⣿⣿⣿
⣿⣿⣿⡇⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⣻⣿⣿⣿
⣿⣿⣿⣷⠀⠀⠀⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀⠀⣿⣿⣿⣿
⣿⣿⣿⣿⡆⠀⠀⡀⠀⠙⠿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠋⠀⠀⠀⠀⢰⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣄⠀⠙⢷⣄⠀⠀⣸⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⣠⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣦⡀⠈⠛⠻⠟⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⣀⣼⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣷⣤⣀⠀⣿⣿⣿⣿⣿⣿⢀⣀⣤⣾⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
```
It seems that the empty braille characters are not the same width as the ones with dots, which is why the right side is not flush. Not really sure what to do about that.

## Dependencies
- Uses various .h files from the [stb_image.c](http://nothings.org/stb_image.c) repo for handling image inputs.
- Some C++ compiler.
- I wrote this to compile with mingw32 so you might have to change a couple things for it to work on linux.
- If you set ```-DOUTPUT_STEP_IMAGES``` in the makefile the program will output several images which show the manipulations the program is doing to the input image before creating the text.

