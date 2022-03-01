#include <stdexcept>
#include <string>
#include <sstream>
#include <cstring>
#include <png.h>
#include "load_png.hpp"

void load_png(const char* filename, GLenum expected_format, size_t expected_width, size_t expected_height, uint8_t* data) {
  // Should minimize the amount of non trivially destructable types, because setjmp/longjmp is involved

  png_byte expected_color_type;
  size_t expected_pixel_bytes;
  switch (expected_format) {
    case GL_R8:
      expected_color_type = PNG_COLOR_TYPE_GRAY;
      expected_pixel_bytes = 1;
      break;
    case GL_RGB8:
      expected_color_type = PNG_COLOR_TYPE_RGB;
      expected_pixel_bytes = 3;
      break;
    case GL_RGBA8:
      expected_color_type = PNG_COLOR_TYPE_RGBA;
      expected_pixel_bytes = 4;
      break;
    default:
      throw std::invalid_argument("unsupported texture format");
  }

  // Construct an array of pointer that libpng needs
  // Invert Y axis of the image because PNG stores pixels from top to bottom, while we want them to be layed out bottom to top
  png_bytep* row_pointers = new png_bytep[expected_height];
  for (size_t i = 0; i < expected_height; i++) {
    row_pointers[i] = data + (expected_height - i - 1) * expected_width * expected_pixel_bytes;
  }


  FILE* file = fopen(filename, "rb");
  if (!file) {
    delete[] row_pointers;
    std::ostringstream msg;
    msg << "Cannot open PNG file " << filename << ": " << strerror(errno);
    throw std::runtime_error(msg.str());
  }

  // Initialize libpng
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr) {
    fclose(file);
    delete[] row_pointers;
    throw std::runtime_error("Unable to initialize libpng");
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
    fclose(file);
    delete[] row_pointers;
    throw std::runtime_error("Unable to initialize libpng");
  }

  if (setjmp(png_jmpbuf(png_ptr))) { // Source of evil
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
    fclose(file);
    delete[] row_pointers;
    std::ostringstream msg;
    msg << "Error reading PNG file " << filename;
    throw std::runtime_error(msg.str());
  }

  // Read image info
  png_init_io(png_ptr, file);
  png_read_info(png_ptr, info_ptr);

  size_t width = png_get_image_width(png_ptr, info_ptr);
  size_t height = png_get_image_height(png_ptr, info_ptr);
  png_byte color_type = png_get_color_type(png_ptr, info_ptr);
  png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);

  // Image format, width, height must match expectation
  if (
    width != expected_width ||
    height != expected_height ||
    color_type != expected_color_type ||
    bit_depth != 8 ||
    rowbytes != width * expected_pixel_bytes
  ) {
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
    fclose(file);
    delete[] row_pointers;
    std::ostringstream msg;
    msg << "Unsupported image format in texture file " << filename;
    throw std::runtime_error(msg.str());
  }

  // Read image
  png_read_image(png_ptr, row_pointers);

  // Cleanup
  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
  fclose(file);
  delete[] row_pointers;
}
