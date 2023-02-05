#include "../mimg.h"

int main() {
    STBImg img1;

    img1.pixels = stbi_load("./test/original/flowers.jpg", &(img1.width),
                            &(img1.height), &(img1.n_channels), MIMG_EXPECTED_N_CHANNELS);


    stbi_uc *new_pixels = calloc(MIMG_EXPECTED_N_CHANNELS * img1.height * img1.width, sizeof(stbi_uc));

    mimg_mean_filter(img1.width, img1.height, img1.pixels, new_pixels, 3);

    stbi_write_jpg("./test/mean_filter_test_3.jpg", img1.width,
                   img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

    mimg_mean_filter(img1.width, img1.height, img1.pixels, new_pixels, 9);

    stbi_write_jpg("./test/mean_filter_test_9.jpg", img1.width,
                   img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

    mimg_mean_filter(img1.width, img1.height, img1.pixels, new_pixels, 15);

    stbi_write_jpg("./test/mean_filter_test_15.jpg", img1.width,
                   img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

    mimg_mean_filter(img1.width, img1.height, img1.pixels, new_pixels, 31);

    stbi_write_jpg("./test/mean_filter_test_31.jpg", img1.width,
                   img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

    free(new_pixels);
    stbi_image_free(img1.pixels);

    return 0;
}
