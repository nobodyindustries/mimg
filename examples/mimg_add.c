#include "../mimg.h"

int main() {
    STBImg img1;
    STBImg img2;

    img1.pixels = stbi_load("./test/original/fingers.jpg", &(img1.width),
                            &(img1.height), &(img1.n_channels), MIMG_EXPECTED_N_CHANNELS);
    img2.pixels = stbi_load("./test/original/flowers.jpg", &(img2.width),
                            &(img2.height), &(img2.n_channels), MIMG_EXPECTED_N_CHANNELS);


    stbi_uc *new_pixels = calloc(MIMG_EXPECTED_N_CHANNELS * img1.height * img1.width, sizeof(stbi_uc));

    mimg_add(img1.width, img1.height, img1.pixels, img2.pixels, new_pixels);

    stbi_write_jpg("./test/add_test.jpg", img1.width, img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);


    free(new_pixels);
    stbi_image_free(img1.pixels);
    stbi_image_free(img2.pixels);

    return 0;
}
