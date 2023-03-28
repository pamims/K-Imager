#ifndef CC_IMAGE_MAPPER_H
#define CC_IMAGE_MAPPER_H

#include <utility>
#include <vector>
#include "CC_point.h"
#include "CImg/CImg.h"


namespace cc {
	class ImageConverter {
	private:
		ImageConverter();
	public:
		static std::vector<Point> BuildData(const cimg_library::CImg<int>& image, bool make_greyscale = false);
		static cimg_library::CImg<int> BuildImage(const std::vector<Point>& data, const unsigned int width, const unsigned int height);
	};
}

#endif
