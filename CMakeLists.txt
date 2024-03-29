cmake_minimum_required(VERSION 3.24)
project(mimgtools C)

set(CMAKE_C_STANDARD 23)

set(MIMG_LIBRARIES
        "mimg.h"
        "lib/stb_image.h"
        "lib/stb_image_write.h"
        "lib/cauldron_random.h")

add_executable(mimg_add examples/mimg_add.c ${MIMG_LIBRARIES})

add_executable(mimg_sub examples/mimg_sub.c ${MIMG_LIBRARIES})

add_executable(mimg_mul examples/mimg_mul.c ${MIMG_LIBRARIES})

add_executable(mimg_div examples/mimg_div.c ${MIMG_LIBRARIES})

add_executable(mimg_and examples/mimg_and.c ${MIMG_LIBRARIES})

add_executable(mimg_or examples/mimg_or.c ${MIMG_LIBRARIES})

add_executable(mimg_not examples/mimg_not.c ${MIMG_LIBRARIES})

add_executable(mimg_mean_filter examples/mimg_mean_filter.c ${MIMG_LIBRARIES})

add_executable(mimg_random_range_test examples/mimg_random_range_test.c ${MIMG_LIBRARIES})

add_executable(mimg_saltpepper_test examples/mimg_saltpepper.c ${MIMG_LIBRARIES})

add_executable(mimg_median examples/mimg_median.c ${MIMG_LIBRARIES})

add_executable(mimg_median_filter examples/mimg_median_filter.c ${MIMG_LIBRARIES})

add_executable(mimg_laplacian_sharpen examples/mimg_laplacian_sharpen.c ${MIMG_LIBRARIES})

add_executable(mimg_diff_directional_sharpen examples/mimg_diff_directional_sharpen.c ${MIMG_LIBRARIES})

add_executable(mimg_threshold_quantize examples/mimg_threshold_quantize.c ${MIMG_LIBRARIES})

add_executable(mimg_mask_quantize examples/mimg_mask_quantize.c ${MIMG_LIBRARIES})

add_executable(mimg_rnd_mask_quantize examples/mimg_rnd_mask_quantize.c ${MIMG_LIBRARIES})

add_executable(mimg_uniform_bin_quantize examples/mimg_uniform_bin_quantize.c ${MIMG_LIBRARIES})

add_executable(mimg_logarithmic_bin_quantize examples/mimg_logarithmic_bin_quantize.c ${MIMG_LIBRARIES})

add_executable(mimg_linspace examples/mimg_linspace.c ${MIMG_LIBRARIES})

add_executable(mimg_logspace examples/mimg_logspace.c ${MIMG_LIBRARIES})

add_executable(mimg_convert_grayscale examples/mimg_convert_grayscale.c ${MIMG_LIBRARIES})

add_executable(mimg_edges examples/mimg_edges.c ${MIMG_LIBRARIES})

file(COPY test/original DESTINATION test)