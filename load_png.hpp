#ifndef _LOAD_PNG_HPP_
#define _LOAD_PNG_HPP_
#include <cstdint>
#include <GL/glew.h>

// Load PNG file, format, width and height must be exact, pixels are stored into data
void load_png(const char* filename, GLenum expected_format, size_t expected_width, size_t expected_height, uint8_t* data);

#endif
