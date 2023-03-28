#include "pch.h"

#include <cassert>




namespace cc {
	bool base_point::operator ==(const base_point& other) const noexcept {
		return Hash() == other.Hash();
	}

	bool base_point::operator <(const base_point& other) const noexcept {
		return Hash() < other.Hash();
	}

	int base_point::Hash() const noexcept {
		int result = (red << three_bytes) + (blue << two_bytes) + (green << one_byte) + value;
		return result;
	}

	void swap(base_point& a, base_point& b) {
		std::swap(a.red, b.red);
		std::swap(a.green, b.green);
		std::swap(a.blue, b.blue);
		std::swap(a.value, b.value);
		return;
	}




	/* -- Point -- */

	Point::Point(int red, int green, int blue) noexcept :
		base_point(red, green, blue, InvalidBucket) {
		return;
	}

	int Point::GetBucket() const noexcept {
		return value;
	}

	int Point::Red() const noexcept {
		return red;
	}

	int Point::Green() const noexcept {
		return green;
	}

	int Point::Blue() const noexcept {
		return blue;
	}

	void Point::SetBucket(size_t index) noexcept {
		value = static_cast<int>(index);
		return;
	}

	void Point::SetToCentroid(const Centroid& other) noexcept {
		red = other.red;
		green = other.green;
		blue = other.blue;
		return;
	}

	void Point::MakeGreyscale() noexcept {
		int white = (red + green + blue) / 3;
		red = green = blue = white;
		return;
	}




	/* -- Centroid -- */

	Centroid::Centroid(const Point& point) noexcept :
		base_point(point.red, point.green, point.blue, 1) {
		return;
	}

	void Centroid::Zero() noexcept {
		red = 0;
		green = 0;
		blue = 0;
		value = 0;
		return;
	}

	void Centroid::AddPoint(const Point& other) noexcept {
		red += other.red;
		green += other.green;
		blue += other.blue;
		value++;
		return;
	}

	void Centroid::SetToAveragePointValue() noexcept {
		assert(red >= 0 && green >= 0 && blue >= 0);

		if (value == 0) {
			return;
		}
		red /= value;
		green /= value;
		blue /= value;

		// value /= value;
		value = 1;
		return;
	}

	int Centroid::SquaredDistance(const Point& other) const noexcept {
		int r = red - other.red;
		int b = blue - other.blue;
		int g = green - other.green;

		int result = r * r + b * b + g * g;
		return result;
	}

	bool Centroid::IsUnused() const noexcept {
		return value <= 0;
	}
}
