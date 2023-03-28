#include "pch.h"

#include <algorithm>
#include <execution>
#include <cassert>

namespace cc {
	std::vector<Point> DataMapper::BuildData(const std::pair<std::vector<Point>, std::vector<Centroid>>& input) {
		std::vector<Point> result = input.first;
		std::vector<Centroid> centroids = input.second;

		auto map_points_to_centroid_value = [centroids](Point& point) {
			assert(point.GetBucket() >= 0 && point.GetBucket() < centroids.size());
			const size_t index = static_cast<size_t>(point.GetBucket());
			const Centroid& centroid = centroids[index];
			point.SetToCentroid(centroid);
		};

		std::for_each(std::execution::par, result.begin(), result.end(), map_points_to_centroid_value);
		return result;
	}
}
