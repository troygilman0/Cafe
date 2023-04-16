#include "Image.h"

#include <iostream>
#include <png.h>
#include "StringUtils.h"


bool Image::loadImage(Image* image, const char* filePath) {
    FILE* fp = fopen(filePath, "rb");
    if (!fp) {
        std::cout << "Error opening PNG file: " << image->filePath << "\n";
        return false;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        std::cout << "Error creating PNG read structure.\n";
        fclose(fp);
        return false;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        std::cout << "Error creating PNG info structure.\n";
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return false;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    size_t width = png_get_image_width(png_ptr, info_ptr);
    size_t height = png_get_image_height(png_ptr, info_ptr);
    size_t channels = png_get_channels(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    if (bit_depth == 16)
        png_set_strip_16(png_ptr);
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png_ptr);

    png_read_update_info(png_ptr, info_ptr);

    size_t row_size = png_get_rowbytes(png_ptr, info_ptr);
    size_t imageSizeBytes = row_size * height;
    if (imageSizeBytes > MAX_IMAGE_SIZE_BYTES) {
        std::cout << "Image is too big: " << imageSizeBytes << " bytes\n";
        fclose(fp);
        return false;
    }

    image->width = width;
    image->height = height;
    image->channels = channels;
    StringUtils::copyStringToBuffer(filePath, image->filePath, CHAR_BUFFER_SIZE);

    png_bytep row_pointers[height];
    for (unsigned y = 0; y < height; y++) {
        row_pointers[y] = &image->buffer[y * row_size];
    }

    png_read_image(png_ptr, row_pointers);
    png_read_end(png_ptr, NULL);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return true;
}