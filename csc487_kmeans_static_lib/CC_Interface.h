#ifndef CC_CC_INTERFACE_H
#define CC_CC_INTERFACE_H

#include "pch.h"

namespace cc {
	enum class AlgorithmType : int {
		kMeans = 0,
		kMeansUnweighted = 1,
		kMeansPP = 2,
		kMeansUnweightedPP = 3
		//kMedians
	};

	class CC_Interface {
	private:
		cimg_library::CImg<int> original_image;
		cimg_library::CImg<int> result_image;
		cimg_library::CImgDisplay original_display;
		cimg_library::CImgDisplay result_display;
		HICON hIcon;
	public:
		CC_Interface();
		CC_Interface(HICON hIcon);
		void ImageLoad(const char* filename);
		void Process(unsigned int k, AlgorithmType algorithm_type, bool make_greyscale = false);
		void Display();
		void Close();
		void Save(const char* filename) const;
		bool ImageIsValid() const noexcept;
		bool ImageIsTooLarge() const noexcept;
		bool CanSave() const noexcept;
	};
}
#endif
