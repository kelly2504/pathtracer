#ifndef COLOR_H
#define COLOR_H

#include "common_utility.h"
#include "vec3.h" // match actual filename/casing
#include <fstream>

using color = vec3;

void write_color(std::ostream& out, const color& pixel_color);

#endif // !COLOR_H
