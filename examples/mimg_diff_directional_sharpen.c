#include "../mimg.h"

int main() {

    STBImg img1;

    img1.pixels = stbi_load("./test/original/flowers.jpg", &(img1.width),
                            &(img1.height), &(img1.n_channels), MIMG_EXPECTED_N_CHANNELS);

    stbi_uc *new_pixels = calloc(MIMG_EXPECTED_N_CHANNELS * img1.height * img1.width, sizeof(stbi_uc));

    mimg_diff_directional_sharpen(img1.width, img1.height, img1.pixels, new_pixels, MIMG_HORIZONTAL);

    stbi_write_jpg("./test/diff_directional_sharpen_test_H.jpg", img1.width,
                   img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

    mimg_diff_directional_sharpen(img1.width, img1.height, img1.pixels, new_pixels, MIMG_VERTICAL);

    stbi_write_jpg("./test/diff_directional_sharpen_test_V.jpg", img1.width,
                   img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

    mimg_diff_directional_sharpen(img1.width, img1.height, img1.pixels, new_pixels, MIMG_DIAGONAL_RL);

    stbi_write_jpg("./test/diff_directional_sharpen_test_DRL.jpg", img1.width,
                   img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

    mimg_diff_directional_sharpen(img1.width, img1.height, img1.pixels, new_pixels, MIMG_DIAGONAL_LR);

    stbi_write_jpg("./test/diff_directional_sharpen_test_DLR.jpg", img1.width,
                   img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

    free(new_pixels);
    stbi_image_free(img1.pixels);

    return 0;
}