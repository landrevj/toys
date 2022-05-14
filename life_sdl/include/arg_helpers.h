#ifndef ARG_HELPERS
#define ARG_HELPERS

#include <unordered_map>
#include <string>
#include "game.h"

void handle_args ( game_options&, int, char* const* );
void show_help ();
std::string strip_spaces ( const std::string& );
void parse_file ( std::string, game_options& );
void parse_line ( const std::string&, std::string[] );
void set_options_from_file ( game_options&, std::unordered_map<std::string, std::string>& );

#endif
