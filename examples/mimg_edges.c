#include "../mimg.h"

int main() {
	STBImg img;

	img.pixels = stbi_load("./test/original/flowers.jpg", &(img.width),
		&(img.height), &(img.n_channels), MIMG_EXPECTED_N_CHANNELS);

	stbi_uc *new_pixels = calloc(MIMG_EXPECTED_N_CHANNELS * img.height * img.width, sizeof(stbi_uc));

	mimg_prewitt_edges(img.width, img.height, img.pixels, new_pixels);

	stbi_write_jpg("./test/prewitt_edges.jpg", img.width, img.height,
		MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

	mimg_sobel_edges(img.width, img.height, img.pixels, new_pixels);

	stbi_write_jpg("./test/sobel_edges.jpg", img.width, img.height,
		MIMG_EXPECTED_N_CHANNELS, new_pixels, MIMG_OUTPUT_JPG_QUALITY);

	free(new_pixels);
	stbi_image_free(img.pixels);

	return 0;
}
