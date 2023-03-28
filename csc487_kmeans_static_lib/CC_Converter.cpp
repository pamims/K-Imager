#include "pch.h"
#include <algorithm>
#include <execution>


namespace cc {
	std::vector<Point> ImageConverter::BuildData(const cimg_library::CImg<int>& image, bool is_greyscale) {

		std::vector<Point> result;
		if (image.is_empty() || image.is_inf() || image.is_nan()) {
			return result;
		}
		const unsigned int width = static_cast<unsigned int>(image.width());
		const unsigned int height = static_cast<unsigned int>(image.height());
		const unsigned int size = width * height;
		result.reserve(size);

		for (unsigned int y = 0; y < height; y++) {
			for (unsigned int x = 0; x < width; x++) {
				int red = image(x, y, flat_2D_z_value, cimg_red_index);
				int green = image(x, y, flat_2D_z_value, cimg_green_index);
				int blue = image(x, y, flat_2D_z_value, cimg_blue_index);

				result.emplace_back(red, green, blue);
			}
		}

		if (is_greyscale) {
			for_each(std::execution::par, result.begin(), result.end(), [](Point& point) {
				point.MakeGreyscale();
			});
		}

		return result;
	}

	cimg_library::CImg<int> ImageConverter::BuildImage(const std::vector<Point>& data, const unsigned int width, const unsigned int height) {

		cimg_library::CImg<int> result(width, height, flat_2d_image_depth, num_color_channels, cimg_black_pixel_value);

		for (unsigned int y = 0; y < height; y++) {
			for (unsigned int x = 0; x < width; x++) {
				unsigned int index = x + width * y;
				result(x, y, flat_2D_z_value, cimg_red_index) = data[index].Red();
				result(x, y, flat_2D_z_value, cimg_green_index) = data[index].Green();
				result(x, y, flat_2D_z_value, cimg_blue_index) = data[index].Blue();
			}
		}

		return result;
	}
}
