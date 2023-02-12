#include "../mimg.h"

int main() {
    STBImg img1;

    img1.pixels = stbi_load("./test/original/flowers.jpg", &(img1.width),
                            &(img1.height), &(img1.n_channels), MIMG_EXPECTED_N_CHANNELS);


    stbi_uc *new_pixels = calloc(MIMG_EXPECTED_N_CHANNELS * img1.height * img1.width, sizeof(stbi_uc));

    mimg_threshold_color_quantize(img1.width, img1.height, img1.pixels, new_pixels, 70);

    stbi_write_jpg("./test/threshold_quantize_70.jpg", img1.width, img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

    free(new_pixels);
    stbi_image_free(img1.pixels);

    return 0;
}
