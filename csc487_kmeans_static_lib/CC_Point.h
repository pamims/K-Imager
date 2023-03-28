#ifndef CC_POINT_H
#define CC_POINT_H


namespace cc {
	class base_point {
	private:
		int  Hash() const noexcept;
	protected:
		int red;
		int green;
		int blue;
		int value;
		base_point(int red, int green, int blue, int value) noexcept :
			red(red), green(green), blue(blue), value(value) {};
	public:
		//void Print() const noexcept;
		bool operator ==(const base_point& other) const noexcept;
		bool operator <(const base_point& other) const noexcept;
	public:
		friend void swap(base_point& a, base_point& b);
	};



	class Centroid;  // intellisense gets mad about SetToCentroid def in .cpp if this isn't forward declared
	class Point final : public base_point {
		friend class Centroid;
	public:
		Point(int red, int green, int blue) noexcept;
		int  Red() const noexcept;
		int  Green() const noexcept;
		int  Blue() const noexcept;
		int  GetBucket() const noexcept;
		void SetBucket(size_t index) noexcept;
		void SetToCentroid(const Centroid& other) noexcept;
		void MakeGreyscale() noexcept;
	public:
		static constexpr int InvalidBucket = -1;
		static constexpr int BeyondMaxSquaredDistance = 195076; //(3 * 255 * 255) + 1;
	};



	class Centroid final : public base_point {
		friend class Point;
	public:
		Centroid(const Point& point) noexcept;
		void AddPoint(const Point& other) noexcept;
		void SetToAveragePointValue() noexcept;
		void Zero() noexcept;
		int  SquaredDistance(const Point& other) const noexcept;
		bool IsUnused() const noexcept;
	};
}

#endif
