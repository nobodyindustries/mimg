#define MIMG_DEBUG
#include "../mimg.h"

int main() {
    STBImg img1;

    img1.pixels = stbi_load("./test/original/flowers.jpg", &(img1.width),
                            &(img1.height), &(img1.n_channels), MIMG_EXPECTED_N_CHANNELS);


    stbi_uc *new_pixels = calloc(MIMG_EXPECTED_N_CHANNELS * img1.height * img1.width, sizeof(stbi_uc));

    mimg_noise_saltpepper(img1.width, img1.height, img1.pixels, new_pixels, 5, 50);

    stbi_write_jpg("./test/flowers_saltpepper_5.jpg", img1.width, img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);


    mimg_noise_saltpepper(img1.width, img1.height, img1.pixels, new_pixels, 20, 50);

    stbi_write_jpg("./test/flowers_saltpepper_20.jpg", img1.width, img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

    mimg_noise_saltpepper(img1.width, img1.height, img1.pixels, new_pixels, 100, 50);

    stbi_write_jpg("./test/flowers_saltpepper_100.jpg", img1.width, img1.height,
                   MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);



    free(new_pixels);
    stbi_image_free(img1.pixels);

    return 0;
}
