#include <filesystem>
#include <cstdio>
#include <cstring>
#include <csetjmp>
#include <png.h>
#include "Texture.hpp"
#include "build_config.h"

Texture::Texture(GLenum type_) {
  type = type_;
  glGenTextures(1, &id);
}

Texture::~Texture() {
  glDeleteTextures(1, &id);
}

void Texture::bind() {
  glBindTexture(type, id);
}

void Texture::loadFromFile(const std::string& filename) {
  std::filesystem::path path(APP_RESOURCE_PATH);
  path.append(filename);

  FILE* file = fopen(path.c_str(), "rb");
  if (!file) {
    std::string msg = strerror(errno);
    throw TextureException("Cannot open texture file " + path.string() + ": " + msg);
  }

  // Initialize libpng
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr) {
    throw TextureException("Unable to initialize libpng");
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
    png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
    throw TextureException("Unable to initialize libpng");
  }

  if (setjmp(png_jmpbuf(png_ptr))) {
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
    throw TextureException("Error reading texture file " + path.string());
  }

  // Read image info
  png_init_io(png_ptr, file);
  png_read_info(png_ptr, info_ptr);

  int width = png_get_image_width(png_ptr, info_ptr);
  int height = png_get_image_height(png_ptr, info_ptr);
  png_byte color_type = png_get_color_type(png_ptr, info_ptr);
  png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);

  // Image must be RGB8 format, and square
  if (
    color_type != PNG_COLOR_TYPE_RGB ||
    bit_depth != 8 ||
    width != height ||
    rowbytes != width * 3
  ) {
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
    throw TextureException("Unsupported image format in texture file " + path.string());
  }

  // Allocate pixel data buffer
  png_byte* data = new png_byte[rowbytes * height];
  png_bytep* row_pointers = new png_bytep[height];
  for (size_t i = 0; i < height; i++) {
    row_pointers[i] = &data[i * rowbytes];
  }

  // Read image
  png_read_image(png_ptr, row_pointers);

  // Cleanup libpng stuff, only data buffer is left
  png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
  fclose(file);
  delete[] row_pointers;

  // Send image to GPU
  bind();
  glTexImage2D(type, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(type);

  delete[] data;
}
