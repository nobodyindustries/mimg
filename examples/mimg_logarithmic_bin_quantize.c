#define MIMG_DEBUG
#include "../mimg.h"

int main() {
    STBImg img1;

    img1.pixels = stbi_load("./test/original/flowers.jpg", &(img1.width),
                            &(img1.height), &(img1.n_channels), MIMG_EXPECTED_N_CHANNELS);

    stbi_uc *new_pixels = calloc(MIMG_EXPECTED_N_CHANNELS * img1.height * img1.width, sizeof(stbi_uc));

    mimg_logarithmic_bin_color_quantize(img1.width, img1.height, img1.pixels, new_pixels, 4);

    stbi_write_jpg("./test/mimg_logarithmic_bin_quantize_4.jpg", img1.width, img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

    mimg_logarithmic_bin_color_quantize(img1.width, img1.height, img1.pixels, new_pixels, 8);

    stbi_write_jpg("./test/mimg_logarithmic_bin_quantize_8.jpg", img1.width, img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

    mimg_logarithmic_bin_color_quantize(img1.width, img1.height, img1.pixels, new_pixels, 16);

    stbi_write_jpg("./test/mimg_logarithmic_bin_quantize_16.jpg", img1.width, img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

    mimg_logarithmic_bin_color_quantize(img1.width, img1.height, img1.pixels, new_pixels, 32);

    stbi_write_jpg("./test/mimg_logarithmic_bin_quantize_32.jpg", img1.width, img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

    mimg_logarithmic_bin_color_quantize(img1.width, img1.height, img1.pixels, new_pixels, 128);

    stbi_write_jpg("./test/mimg_logarithmic_bin_quantize_128.jpg", img1.width, img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

    mimg_logarithmic_bin_color_quantize(img1.width, img1.height, img1.pixels, new_pixels, 255);

    stbi_write_jpg("./test/mimg_logarithmic_bin_quantize_255.jpg", img1.width, img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

    free(new_pixels);
    stbi_image_free(img1.pixels);

    return 0;
}