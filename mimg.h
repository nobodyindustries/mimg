#ifndef MIMG_H
#define MIMG_H

#include <stdlib.h>
#include <stdbool.h>

#ifdef MIMG_DEBUG

#include <stdio.h>

#endif

#define STB_IMAGE_IMPLEMENTATION

#include "lib/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "lib/stb_image_write.h"

#define RANDOM_H_IMPLEMENTATION

#include "lib/cauldron_random.h"

/** CONSTANT DEFINITIONS **/
#pragma mark Constants

#define MIMG_EXPECTED_N_CHANNELS 3
#define MIMG_OUTPUT_JPG_QUALITY 100
#define MIMG_UC_MAX 255

#define MIMG_LAPLACIAN_KERNEL_SIZE 3
#define MIMG_DIFF_DIRECTIONAL_KERNEL_SIZE 3
#define MIMG_PREWITT_KERNEL_SIZE 3

/** MACRO DEFINITIONS **/
#pragma mark Macros

#define MIMG_COORD_IDX(w, x, y) (3 * (((y) * (w)) + (x)))
#define MIMG_COORDS_VALID(w, h, x, y) ((x) >= 0) && ((x) < (w)) && ((y) >= 0) && ((y) < (h))

/** TYPE DEFINITIONS **/
#pragma mark Types

typedef enum mimg_direction {
    MIMG_VERTICAL = 0,
    MIMG_HORIZONTAL = 1,
    MIMG_DIAGONAL_LR = 2,
    MIMG_DIAGONAL_RL = 3,
} MIMG_DIRECTION;

/**
 * STBImg
 *
 * Auxiliary struct for ease of interaction with stb_image_*, not intended to use it as a
 * general abstraction of an image, as it is not really necessary.
 */
typedef struct stb_img {
    stbi_uc *pixels;
    int width;
    int height;
    int n_channels;
} STBImg;

/** PIXEL MANIPULATION FUNCTIONS **/
#pragma mark Pixel manipulation

extern inline void mimg_set_pixel(stbi_uc *pixels, int w, int x, int y, stbi_uc r, stbi_uc g, stbi_uc b) {
    pixels[MIMG_COORD_IDX(w, x, y)] = r;
    pixels[MIMG_COORD_IDX(w, x, y) + 1] = g;
    pixels[MIMG_COORD_IDX(w, x, y) + 2] = b;
}

extern inline void mimg_get_pixel(stbi_uc *pixels, int w, int x, int y, stbi_uc *r, stbi_uc *g, stbi_uc *b) {
    *r = pixels[MIMG_COORD_IDX(w, x, y)];
    *g = pixels[MIMG_COORD_IDX(w, x, y) + 1];
    *b = pixels[MIMG_COORD_IDX(w, x, y) + 2];
}

/** AUXILIARY DEBUG FUNCTIONS **/
#pragma mark Debug functions

#ifdef MIMG_DEBUG

static void mimg_print_array(const char *name, stbi_uc *values, size_t length) {
    printf("%s - [", name);
    for(int i = 0; i < length; i ++) {
        printf("%d", values[i]);
        if(i != length - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

#endif

/** MATHEMATICAL AIDS **/
#pragma mark Mathematical aids

extern inline stbi_uc mimg_clampi(int i) {
    if (i < 0) {
        return (stbi_uc) 0;
    }
    if (i > MIMG_UC_MAX) {
        return (stbi_uc) MIMG_UC_MAX;
    }
    return (stbi_uc) (i);
}

extern inline stbi_uc mimg_clampd(double i) {
    i = round(i);
    if (i < 0.) {
        return (stbi_uc) 0;
    }
    if (i > (double) MIMG_UC_MAX) {
        return (stbi_uc) MIMG_UC_MAX;
    }
    return (stbi_uc) (i);
}

extern inline uint32_t mimg_random_range(uint32_t min, uint32_t max) {
    uint32_t r = dist_uniform_u32(max - min, trng_u32, NULL);
    return r + min;
}

extern inline int mimg_uint32_cmp(const void *a, const void *b) {
    return (int) (*(stbi_uc *) a - *(stbi_uc *) b);
}

extern inline stbi_uc mimg_mediani(const stbi_uc *values, const stbi_uc length) {
    // TODO: Code our own quicksort so we don't need to include stdlib.h
    qsort((void *) values, length, sizeof(stbi_uc), mimg_uint32_cmp);
    if (length % 2 == 0) {
        return (stbi_uc) round(((double) values[length / 2 - 1] + (double) values[length / 2]) / 2);
    }
    return values[length / 2];
}

void mimg_linspace(stbi_uc *values, const size_t length, const stbi_uc min, const stbi_uc max) {
    assert(max >= min);
    assert(length > 0);
    double increment = ((double) (max - min)) / ((double) (length - 1));
    for(size_t i = 0; i < length; i++) {
        values[i] = mimg_clampd(((double) min) + (increment * ((double) i)));
    }
}

void mimg_logspace(stbi_uc *values, const size_t length, const stbi_uc min, const stbi_uc max) {
    assert(max >= min);
    assert(length > 0);

    bool substract = false;
    double maxd =(double) max;
    if(min == 0) {
        maxd += 1;
        substract = true;
    }

    double *exps = calloc(length, sizeof(double));
    double exp_inc = 1.0 / ((double) length - 1);
    for(int i = 0; i < length; i++) {
        exps[i] = i * exp_inc;
    }

    double *d_values = calloc(length, sizeof(double));
    for(int i = 0; i < length; i++) {
        d_values[i] = round(pow(maxd, exps[i]));
    }

    if(substract) {
        for(int i = 0; i < length; i++) {
            values[i] = mimg_clampd(d_values[i] - 1);
        }
    } else {
        for(int i = 0; i < length; i++) {
            values[i] = mimg_clampd(d_values[i]);
        }
    }

    free(d_values);
    free(exps);
}

extern inline stbi_uc mimg_add_clamp(stbi_uc v1, stbi_uc v2) {
    int i1 = (int) v1;
    int i2 = (int) v2;
    return mimg_clampi(i1 + i2);
}

extern inline stbi_uc mimg_sub_clamp(stbi_uc v1, stbi_uc v2) {
    int i1 = (int) v1;
    int i2 = (int) v2;
    return mimg_clampi(i1 - i2);
}

/** COLOR CALCULATIONS */
#pragma mark Color calculations

extern inline stbi_uc mimg_pixel_luminosity(stbi_uc r, stbi_uc g, stbi_uc b) {
	double aux = (((double) r) * 0.299) + (((double) g) * 0.587) + (((double) b) * 0.114);
	return (stbi_uc)round(aux);
}

extern inline stbi_uc mimg_pixel_average(stbi_uc r, stbi_uc g, stbi_uc b) {
	double aux = (((double) r) + ((double) g) + ((double) b)) / 3;
	return (stbi_uc)round(aux);
}

/** COLOR SPACE CHANGES */
#pragma mark Color space changes

void mimg_convert_grayscale_average(int w, int h, stbi_uc *px, stbi_uc *out) {
	stbi_uc cr, cg, cb, avg;
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++) {
			mimg_get_pixel(px, w, x, y, &cr, &cg, &cb);
			avg = mimg_pixel_average(cr, cg, cb);
			mimg_set_pixel(out, w, x, y, avg, avg, avg);
		}
}

void mimg_convert_grayscale_luminosity(int w, int h, stbi_uc *px, stbi_uc *out) {
	stbi_uc cr, cg, cb, l;
	for (int y = 0; y < h; y++)
		for (int x = 0; x < w; x++) {
			mimg_get_pixel(px, w, x, y, &cr, &cg, &cb);
			l = mimg_pixel_luminosity(cr, cg, cb);
			mimg_set_pixel(out, w, x, y, l, l, l);
		}
}

/** IMAGE ALGEBRA OPERATORS **/
#pragma mark Image algebra operators

void mimg_add(int w, int h, stbi_uc *px1, stbi_uc *px2, stbi_uc *out) {
    stbi_uc r1, g1, b1, r2, g2, b2;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            mimg_get_pixel(px1, w, x, y, &r1, &g1, &b1);
            mimg_get_pixel(px2, w, x, y, &r2, &g2, &b2);
            mimg_set_pixel(out, w, x, y, mimg_add_clamp(r1, r2), mimg_add_clamp(g1, g2), mimg_add_clamp(b1, b2));
        }
}

void mimg_sub(int w, int h, stbi_uc *px1, stbi_uc *px2, stbi_uc *out) {
    stbi_uc r1, g1, b1, r2, g2, b2;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            mimg_get_pixel(px1, w, x, y, &r1, &g1, &b1);
            mimg_get_pixel(px2, w, x, y, &r2, &g2, &b2);
            mimg_set_pixel(out, w, x, y, mimg_sub_clamp(r1, r2), mimg_sub_clamp(g1, g2), mimg_sub_clamp(b1, b2));
        }
}

void mimg_mul(int w, int h, stbi_uc *px, int multiplier, stbi_uc *out) {
    stbi_uc r, g, b;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            mimg_get_pixel(px, w, x, y, &r, &g, &b);
            mimg_set_pixel(out, w, x, y, mimg_clampi(r * multiplier), mimg_clampi(g * multiplier),
                           mimg_clampi(b * multiplier));
        }
}

void mimg_div(int w, int h, stbi_uc *px, int divider, stbi_uc *out) {
    stbi_uc r, g, b;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            mimg_get_pixel(px, w, x, y, &r, &g, &b);
            mimg_set_pixel(out, w, x, y, mimg_clampi(r / divider), mimg_clampi(g / divider), mimg_clampi(b / divider));
        }
}

void mimg_and(int w, int h, stbi_uc *px1, stbi_uc *px2, stbi_uc *out) {
    stbi_uc r1, g1, b1, r2, g2, b2;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            mimg_get_pixel(px1, w, x, y, &r1, &g1, &b1);
            mimg_get_pixel(px2, w, x, y, &r2, &g2, &b2);
            mimg_set_pixel(out, w, x, y, r1 & r2, g1 & g2, b1 & b2);
        }
}

void mimg_or(int w, int h, stbi_uc *px1, stbi_uc *px2, stbi_uc *out) {
    stbi_uc r1, g1, b1, r2, g2, b2;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            mimg_get_pixel(px1, w, x, y, &r1, &g1, &b1);
            mimg_get_pixel(px2, w, x, y, &r2, &g2, &b2);
            mimg_set_pixel(out, w, x, y, r1 | r2, g1 | g2, b1 | b2);
        }
}

void mimg_not(int w, int h, stbi_uc *px, stbi_uc *out) {
    stbi_uc r, g, b;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            mimg_get_pixel(px, w, x, y, &r, &g, &b);
            mimg_set_pixel(out, w, x, y, ~r, ~g, ~b);
        }
}

/** GENERALIZED CONVOLUTION OPERATOR **/
#pragma mark Generalized convolution

void mimg_convolve(int w, int h, stbi_uc *px, stbi_uc *out, int kernel_size, const double *kernel_values) {
    assert(kernel_size % 2 == 1);

    int kernel_idx, delta_limit;
    double sum_r, sum_g, sum_b, kernel_value;
    stbi_uc cr, cg, cb;

    delta_limit = (kernel_size - 1) / 2;

    // Square kernel is assumed, so if kernel_size is 3, then we need 3x3 = 9 kernel values
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            kernel_idx = 0;
            sum_r = 0;
            sum_g = 0;
            sum_b = 0;
            for (int dy = -delta_limit; dy <= delta_limit; dy++) {
                for (int dx = -delta_limit; dx <= delta_limit; dx++) {

                    if (MIMG_COORDS_VALID(w, h, x + dx, y + dy)) {
                        kernel_value = kernel_values[kernel_idx];
                        mimg_get_pixel(px, w, x + dx, y + dy, &cr, &cg, &cb);
                        sum_r += ((double) cr) * kernel_value;
                        sum_g += ((double) cg) * kernel_value;
                        sum_b += ((double) cb) * kernel_value;
                    }

                    kernel_idx++;
                }
            }
            mimg_set_pixel(out, w, x, y, mimg_clampd(sum_r), mimg_clampd(sum_g), mimg_clampd(sum_b));
        }
}

/** NOISE **/
#pragma mark Noise

void mimg_noise_saltpepper(int w, int h, stbi_uc *px, stbi_uc *out, uint32_t p_change, uint32_t p_white) {
    assert(p_change >= 0 && p_change <= 100);
    assert(p_white >= 0 && p_white <= 100);
    uint32_t current_p;
#ifdef MIMG_DEBUG
    uint32_t pixels_changed = 0, pixels_white = 0, pixels_black = 0;
#endif
    stbi_uc r, g, b;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            // Decide whether the pixel changes or not
            current_p = mimg_random_range(1, 101);
            if (current_p <= p_change) {
#ifdef MIMG_DEBUG
                pixels_changed++;
#endif
                current_p = mimg_random_range(1, 101);
                // Decide whether the changed pixel changes to either white or black
                if (current_p <= p_white) {
#ifdef MIMG_DEBUG
                    pixels_white++;
#endif
                    mimg_set_pixel(out, w, x, y, 0xFF, 0xFF, 0xFF);
                } else {
#ifdef MIMG_DEBUG
                    pixels_black++;
#endif
                    mimg_set_pixel(out, w, x, y, 0x00, 0x00, 0x00);
                }
            } else {
                mimg_get_pixel(px, w, x, y, &r, &g, &b);
                mimg_set_pixel(out, w, x, y, r, g, b);
            }
        }
#ifdef MIMG_DEBUG
    printf("Total pixels %d | Pixels changed %u | To white: %u | To black: %u\n", w * h, pixels_changed,
           pixels_white, pixels_black);
#endif
}

/** FILTERS **/
#pragma mark Filters

void mimg_mean_filter(int w, int h, stbi_uc *px, stbi_uc *out, int kernel_size) {
    double *kernel_values = calloc((size_t) kernel_size * kernel_size, sizeof(double));
    double kernel_n = kernel_size * kernel_size;
    double value = 1.0 / kernel_n;
    for (int idx = 0; idx < kernel_size * kernel_size; idx++) {
        kernel_values[idx] = value;
    }
    mimg_convolve(w, h, px, out, kernel_size, kernel_values);
    free(kernel_values);
}

void mimg_median_filter(int w, int h, stbi_uc *px, stbi_uc *out, int kernel_size) {
    size_t array_size = kernel_size * kernel_size;

    stbi_uc *values_r = calloc(array_size, sizeof(stbi_uc));
    stbi_uc *values_g = calloc(array_size, sizeof(stbi_uc));
    stbi_uc *values_b = calloc(array_size, sizeof(stbi_uc));

    int delta_limit = ((int) round((double) kernel_size / 2.0)) - 1;
    int changed_values;

    stbi_uc cr, cg, cb, nr, ng, nb;

    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            changed_values = 0;
            memset((void *) values_r, 0, array_size);
            memset((void *) values_g, 0, array_size);
            memset((void *) values_b, 0, array_size);
            for (int dy = -delta_limit; dy <= delta_limit; dy++) {
                for (int dx = -delta_limit; dx <= delta_limit; dx++) {

                    if (MIMG_COORDS_VALID(w, h, x + dx, y + dy)) {
                        mimg_get_pixel(px, w, x + dx, y + dy, &cr, &cg, &cb);
                        values_r[changed_values] = cr;
                        values_g[changed_values] = cg;
                        values_b[changed_values] = cb;
                        changed_values++;
                    }

                }
            }
            nr = mimg_mediani(values_r, changed_values);
            ng = mimg_mediani(values_g, changed_values);
            nb = mimg_mediani(values_b, changed_values);
            mimg_set_pixel(out, w, x, y, nr, ng, nb);
        }


    free(values_r);
    free(values_g);
    free(values_b);
}

void mimg_laplacian_sharpen(int w, int h, stbi_uc *px, stbi_uc *out) {
    double kernel_values[] = {
            0, -1, 0,
            -1, 5, -1,
            0, -1, 0
    };
    mimg_convolve(w, h, px, out, MIMG_LAPLACIAN_KERNEL_SIZE, (double *) kernel_values);
}

void mimg_diff_directional_sharpen(int w, int h, stbi_uc *px, stbi_uc *out, MIMG_DIRECTION dir) {
    double kernel_values[MIMG_DIFF_DIRECTIONAL_KERNEL_SIZE * MIMG_DIFF_DIRECTIONAL_KERNEL_SIZE];
    switch(dir) {
        case MIMG_VERTICAL:
            kernel_values[0] = 0; kernel_values[1] = 1; kernel_values[2] = 0;
            kernel_values[3] = 0; kernel_values[4] = 1; kernel_values[5] = 0;
            kernel_values[6] = 0; kernel_values[7] = -1; kernel_values[8] = 0;
            break;
        case MIMG_HORIZONTAL:
            kernel_values[0] = 0; kernel_values[1] = 0; kernel_values[2] = 0;
            kernel_values[3] = 1; kernel_values[4] = 1; kernel_values[5] = -1;
            kernel_values[6] = 0; kernel_values[7] = 0; kernel_values[8] = 0;
            break;
        case MIMG_DIAGONAL_LR:
            kernel_values[0] = 1; kernel_values[1] = 0; kernel_values[2] = 0;
            kernel_values[3] = 0; kernel_values[4] = 1; kernel_values[5] = 0;
            kernel_values[6] = 0; kernel_values[7] = 0; kernel_values[8] = -1;
            break;
        case MIMG_DIAGONAL_RL:
            kernel_values[0] = 0;  kernel_values[1] = 0; kernel_values[2] = 1;
            kernel_values[3] = 0;  kernel_values[4] = 1; kernel_values[5] = 0;
            kernel_values[6] = -1; kernel_values[7] = 0; kernel_values[8] = 0;
            break;
        default:
            break;
    }
    mimg_convolve(w, h, px, out, MIMG_DIFF_DIRECTIONAL_KERNEL_SIZE, (double *) kernel_values);
}

void mimg_prewitt_edges(int w, int h, stbi_uc *px, stbi_uc *out) {

	double kernel_values_h[] = {
		1, 1, 1,
		0, 0, 0,
		-1, -1, -1
	};
	double kernel_values_v[] = {
		1, 0, -1,
		1, 0, -1,
		1, 0, -1
	};

	stbi_uc *gs = calloc(MIMG_EXPECTED_N_CHANNELS * h * w, sizeof(stbi_uc));
	stbi_uc *np1 = calloc(MIMG_EXPECTED_N_CHANNELS * h * w, sizeof(stbi_uc));
	stbi_uc *np2 = calloc(MIMG_EXPECTED_N_CHANNELS * h * w, sizeof(stbi_uc));

	mimg_convert_grayscale_luminosity(w, h, px, gs);

	mimg_convolve(w, h, gs, np1, MIMG_PREWITT_KERNEL_SIZE, (double *) kernel_values_h);
	mimg_convolve(w, h, gs, np2, MIMG_PREWITT_KERNEL_SIZE, (double *) kernel_values_v);

	mimg_add(w, h, np1, np2, out);

	free(gs);
	free(np1);
	free(np2);
}

void mimg_sobel_edges(int w, int h, stbi_uc *px, stbi_uc *out) {

	double kernel_values_h[] = {
		1, 2, 1,
		0, 0, 0,
		-1, -2, -1
	};
	double kernel_values_v[] = {
		1, 0, -1,
		2, 0, -2,
		1, 0, -1
	};

	stbi_uc *gs = calloc(MIMG_EXPECTED_N_CHANNELS * h * w, sizeof(stbi_uc));
	stbi_uc *np1 = calloc(MIMG_EXPECTED_N_CHANNELS * h * w, sizeof(stbi_uc));
	stbi_uc *np2 = calloc(MIMG_EXPECTED_N_CHANNELS * h * w, sizeof(stbi_uc));

	mimg_convert_grayscale_luminosity(w, h, px, gs);

	mimg_convolve(w, h, gs, np1, MIMG_PREWITT_KERNEL_SIZE, (double *) kernel_values_h);
	mimg_convolve(w, h, gs, np2, MIMG_PREWITT_KERNEL_SIZE, (double *) kernel_values_v);

	mimg_add(w, h, np1, np2, out);

	free(gs);
	free(np1);
	free(np2);
}

/** COLOR QUANTIZATION **/
#pragma mark Color quantization

void mimg_threshold_color_quantize(int w, int h, stbi_uc *px, stbi_uc *out, stbi_uc threshold) {
    stbi_uc r, g, b, nr, ng, nb;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            nr = 0;
            ng = 0;
            nb = 0;
            mimg_get_pixel(px, w, x, y, &r, &g, &b);
            if(r > threshold) nr = 255;
            if(g > threshold) ng = 255;
            if(b > threshold) nb = 255;
            mimg_set_pixel(out, w, x, y, nr, ng, nb);
        }
}

void mimg_mask_color_quantize(int w, int h, stbi_uc *px, stbi_uc *out, stbi_uc mask_len) {
    assert(mask_len >= 0 && mask_len <= 8);
    // We keep the high bits which contain more information
    stbi_uc mask = 0xFF << mask_len;
    stbi_uc r, g, b;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            mimg_get_pixel(px, w, x, y, &r, &g, &b);
            mimg_set_pixel(out, w, x, y, r & mask, g & mask, b & mask);
        }
}

void mimg_rnd_mask_color_quantize(int w, int h, stbi_uc *px, stbi_uc *out, stbi_uc mask_len, stbi_uc random_max) {
    assert(mask_len >= 0 && mask_len <= 8);
    assert(random_max >= 0 && random_max <= 255);
    // We keep the high bits which contain more information
    stbi_uc mask = 0xFF << mask_len;
    stbi_uc r, g, b;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            mimg_get_pixel(px, w, x, y, &r, &g, &b);
            r = (stbi_uc) mimg_clampi((int) r + mimg_random_range(0, (uint32_t) random_max));
            g = (stbi_uc) mimg_clampi((int) g + mimg_random_range(0, (uint32_t) random_max));
            b = (stbi_uc) mimg_clampi((int) b + mimg_random_range(0, (uint32_t) random_max));
            mimg_set_pixel(out, w, x, y, r & mask, g & mask, b & mask);
        }
}

// This function can be used for arbitrary bin quantification.
static void mimg_bin_classify(int w, int h, stbi_uc *px, stbi_uc *out, stbi_uc n_bins, stbi_uc *limits, stbi_uc *values) {
    stbi_uc r, g, b, nr, ng, nb, idx;
    for (int y = 0; y < h; y++)
        for (int x = 0; x < w; x++) {
            mimg_get_pixel(px, w, x, y, &r, &g, &b);
            // We determine channels and values
            for(idx = 0; idx < n_bins; idx++) {
                if(r <= limits[idx]) {
                    nr = values[idx];
                    break;
                }
            }
            for(idx = 0; idx < n_bins; idx++) {
                if(g <= limits[idx]) {
                    ng = values[idx];
                    break;
                }
            }
            for(idx = 0; idx < n_bins; idx++) {
                if(b <= limits[idx]) {
                    nb = values[idx];
                    break;
                }
            }
            mimg_set_pixel(out, w, x, y, nr, ng, nb);
        }
}

void mimg_uniform_bin_color_quantize(int w, int h, stbi_uc *px, stbi_uc *out, stbi_uc n_bins) {
    // Upper limit of each bin
    stbi_uc *limits = calloc((size_t) n_bins, sizeof(stbi_uc));
    // Mid value of each bin
    stbi_uc *values = calloc((size_t) n_bins, sizeof(stbi_uc));

    stbi_uc *base = calloc((size_t) n_bins + 1, sizeof(stbi_uc));
    mimg_linspace(base, (size_t) n_bins + 1, 0, 255);
    for(int i = 0; i < n_bins; i++) {
        limits[i] = base[i + 1];
        values[i] = ((stbi_uc) round((double) (base[i + 1] - base[i]) / 2.0)) + base[i];
    }

#ifdef MIMG_DEBUG
    mimg_print_array("UNIFORM BIN QUANTIZE - BASE", base, n_bins + 1);
    mimg_print_array("UNIFORM BIN QUANTIZE - LIMITS", limits, n_bins);
    mimg_print_array("UNIFORM BIN QUANTIZE - VALUES", values, n_bins);
#endif

    mimg_bin_classify(w, h, px, out, n_bins, limits, values);
    free(base);
    free(limits);
    free(values);
}

void mimg_logarithmic_bin_color_quantize(int w, int h, stbi_uc *px, stbi_uc *out, stbi_uc n_bins) {
    assert(n_bins > 0);

    // Upper limit of each bin
    stbi_uc *limits = calloc((size_t) n_bins + 1, sizeof(stbi_uc));
    // Mid value of each bin
    stbi_uc *values = calloc((size_t) n_bins, sizeof(stbi_uc));

    stbi_uc *base = calloc((size_t) n_bins + 1, sizeof(stbi_uc));
    mimg_logspace(base, (size_t) n_bins + 1, 0, 255);
    for(int i = 0; i < n_bins; i++) {
        limits[i] = base[i + 1];
        values[i] = ((stbi_uc) round((double) (base[i + 1] - base[i]) / 2.0)) + base[i];
    }

#ifdef MIMG_DEBUG
    mimg_print_array("LOGARITHMIC BIN QUANTIZE - BASE", base, n_bins + 1);
    mimg_print_array("LOGARITHMIC BIN QUANTIZE - LIMITS", limits, n_bins);
    mimg_print_array("LOGARITHMIC BIN QUANTIZE - VALUES", values, n_bins);
#endif

    mimg_bin_classify(w, h, px, out, n_bins, limits, values);
    free(base);
    free(limits);
    free(values);
}

#endif //MIMG_H
