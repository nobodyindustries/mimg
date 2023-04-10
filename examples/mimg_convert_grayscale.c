#include "../mimg.h"

int main() {
	STBImg img;

	img.pixels = stbi_load("./test/original/flowers.jpg", &(img.width),
		&(img.height), &(img.n_channels), MIMG_EXPECTED_N_CHANNELS);

	stbi_uc *new_pixels = calloc(MIMG_EXPECTED_N_CHANNELS * img.height * img.width, sizeof(stbi_uc));

	mimg_convert_grayscale_average(img.width, img.height, img.pixels, new_pixels);

	stbi_write_jpg("./test/convert_grayscale_average.jpg", img.width, img.height,
		MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

	mimg_convert_grayscale_luminosity(img.width, img.height, img.pixels, new_pixels);

	stbi_write_jpg("./test/convert_grayscale_luminosity.jpg", img.width, img.height,
		MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

	free(new_pixels);
	stbi_image_free(img.pixels);

	return 0;
}
