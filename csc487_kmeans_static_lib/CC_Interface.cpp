#include "pch.h"

#include <thread>
#include <memory>

namespace cc {
	CC_Interface::CC_Interface() {
		hIcon = NULL;
	}

	CC_Interface::CC_Interface(HICON hicon) {
		hIcon = hicon;
	}

	std::unique_ptr<IClusterAlgorithm> GetAlgorithm(AlgorithmType algorithm_type);
	void DisplayImage(const unsigned int x, const unsigned int y, const cimg_library::CImg<int>& image, cimg_library::CImgDisplay& display, const char* title);

	void CC_Interface::ImageLoad(const char* filename) {
		if (strlen(filename) > 0)
			original_image = cimg_library::CImg<int>(filename);
		return;
	}

	void CC_Interface::Process(unsigned int k, AlgorithmType algorithm_type, bool is_greyscale) {

		std::unique_ptr<IClusterAlgorithm> algorithm = GetAlgorithm(algorithm_type);
		if (algorithm.get() == nullptr) {
			return;
		}

		auto data = ImageConverter::BuildData(original_image, is_greyscale);
		auto result_pair = algorithm->Process(static_cast<size_t>(k), data);
		auto result_data = DataMapper::BuildData(result_pair);

		const unsigned int width = static_cast<unsigned int>(original_image.width());
		const unsigned int height = static_cast<unsigned int>(original_image.height());
		result_image = ImageConverter::BuildImage(result_data, width, height);

		return;
	}

	void CC_Interface::Display() {
		if (!original_image.is_empty())
			DisplayImage(0, 0, original_image, original_display, "Original Image");
		if (!result_image.is_empty())
			DisplayImage(200, 200, result_image, result_display, "Processed Image");
		original_display.set_icon(hIcon);
		result_display.set_icon(hIcon);
	}

	void CC_Interface::Close() {
		original_display.assign();
		result_display.assign();
		original_image.assign();
		result_image.assign();
	}

	void CC_Interface::Save(const char* filename) const {
		result_image.save(filename);
		return;
	}

	bool CC_Interface::ImageIsValid() const noexcept {
		return !(original_image.is_empty() || original_image.is_inf() || original_image.is_nan());
	}

	bool CC_Interface::ImageIsTooLarge() const noexcept {
		int width = original_image.width();
		int height = original_image.height();
		int size = width * height;
		if (size > max_image_size) {
			return true;
		}
		if (width != 0 && size / width != height) {
			// some smart guy tried to overflow
			return true;
		}
		return false;
	}

	bool CC_Interface::CanSave() const noexcept {
		return !result_image.is_empty();
	}


	std::unique_ptr<IClusterAlgorithm> GetAlgorithm(AlgorithmType algorithm_type) {
		std::unique_ptr<IClusterAlgorithm> algorithm;

		switch (algorithm_type) {
		case AlgorithmType::kMeansUnweighted:
			algorithm = std::make_unique<KMeansUnweighted>();
			break;
		case AlgorithmType::kMeansUnweightedPP:
			algorithm = std::make_unique<KMeansUnweightedPP>();
			break;
		case AlgorithmType::kMeansPP:
			algorithm = std::make_unique<KMeansPP>();
			break;
		case AlgorithmType::kMeans:
			algorithm = std::make_unique<KMeans>();
			break;
		default:
			algorithm = std::make_unique<KMeans>();
			break;
		}

		return algorithm;
	}

	void DisplayImage(
		const unsigned int x,
		const unsigned int y,
		const cimg_library::CImg<int>& image,
		cimg_library::CImgDisplay& display,
		const char* title)
	{

		unsigned int width = image.width();
		unsigned int height = image.height();

		float f_width = static_cast<float>(width);
		float f_height = static_cast<float>(height);
		float aspect_ratio = f_width / f_height;

		float target_width = max_display_size;
		float target_height = max_display_size;

		if (aspect_ratio > 1.0) {
			target_height = target_height / aspect_ratio;
		}
		else if (aspect_ratio < 1.0) {
			target_width = target_width * aspect_ratio;
		}

		f_width = target_width;
		f_height = target_height;

		width = static_cast<unsigned int>(f_width);
		height = static_cast<unsigned int>(f_height);

		display.assign(width, height, title, 0, false, false);
		display.move(x, y);
		display.display(image);
		return;
	}
}
