# MIMG

A small, multiplatform, didactic image processing library with minimal dependencies implemented in the C programming language. You can think of it as a more lightweight, easier to use and understand OpenCV with only image manipulation features. A poor man's OpenCV for images, you can say.

All the images are expected to have three channels (RGB, no alpha). Maybe this can change in the future. All the dependencies are self-contained and stored in the `/lib` directory

If you want to collaborate, just drop pull requests. That would make me very happy. Please also report as well issues via Github.

For licensing (MIT License), see [LICENSE](LICENSE)

## Current features

### Mathematical aids

- Clamp (integer, double)
- Random number in range (statistically tested)
- Median (unsigned char)

### Image algebra

- Addition
- Substraction
- Multiplication
- Division
- And
- Or
- Not

### Operators

- Convolution with variable kernel size

### Noise

- Salt and pepper

### Filters

- Mean filter
- Median filter
- Laplacian sharpen
- Directional sharpen by difference

## Quantization

- Threshold quantization
- Mask quantization

