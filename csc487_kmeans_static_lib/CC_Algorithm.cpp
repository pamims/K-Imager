#include "pch.h"

#include <algorithm>
#include <execution>
#include <mutex>
#include <random>
#include <ctime>

namespace cc {

	/* -- Helper Functions Forward Declarations -- */
	//  general helpers
	inline size_t clamp_k(const size_t k);
	inline void inline_put_data_in_buckets(const std::vector<Centroid>& centroids, size_t k, Point& point);
	inline bool are_the_same_data_set(const std::vector<Point>& a, const std::vector<Point>& b);
	// alternatives for GetScrubbedData()
	inline std::vector<Point> return_unique_data(const std::vector<Point>& data);
	inline std::vector<Point> return_full_data(const std::vector<Point>& data);
	// alternatives for GetCentroids()
	inline std::vector<Centroid> get_naive_centroids(const size_t _k, std::vector<Point>& unique_data);
	inline std::vector<Centroid> get_plus_plus_centroids(const size_t _k, std::vector<Point>& data);


	/* -------------- Class Method Definitions -------------- */

	/* -- KMeans -- */
	std::pair<std::vector<Point>, std::vector<Centroid>>
		KMeans::Process(const size_t _k, std::vector<Point> data) const {

		// return early if there is no data
		if (data.empty()) {
			auto result = std::make_pair(data, std::vector<Centroid>());
			return result;
		}

		// Prepare the data
		const size_t temp_k = clamp_k(_k);
		auto scrubbed_data = GetScrubbedData(data);
		auto centroids = GetCentroids(temp_k, scrubbed_data);
		const size_t k = centroids.size();
		std::vector<Point>& ref_data = ChooseDataSet(data, scrubbed_data);

		// Process the data
		unsigned int iterations = 0;
		do {
			auto put_data_in_buckets = [&centroids = std::as_const(centroids), k](Point& point) {
				inline_put_data_in_buckets(centroids, k, point);
			};
			auto zero_centroids = [](Centroid& centroid) {
				centroid.Zero();
			};
			auto sum_up_values = [&centroids](const Point& point) {  // not thread safe
				int index = point.GetBucket();
				centroids[index].AddPoint(point);
			};
			auto set_centroid_values = [](Centroid& centroid) {
				centroid.SetToAveragePointValue();
			};


			std::vector<Centroid> previous_centroids = centroids;
			std::for_each(std::execution::par, ref_data.begin(), ref_data.end(), put_data_in_buckets);
			std::for_each(std::execution::par, centroids.begin(), centroids.end(), zero_centroids);
			std::for_each(std::execution::seq, ref_data.begin(), ref_data.end(), sum_up_values); // not thread safe
			std::for_each(std::execution::par, centroids.begin(), centroids.end(), set_centroid_values);

			bool centroids_stayed_the_same = std::equal(
				std::execution::par,
				previous_centroids.begin(),
				previous_centroids.end(),
				centroids.begin(),
				centroids.end()
			);

			if (centroids_stayed_the_same) {
				break;
			}

			iterations++;
		} while (iterations > default_max_iterations);

		// put original data set into the appropriate buckets
		if (!are_the_same_data_set(data, ref_data)) {
			auto put_data_in_buckets = [&centroids = std::as_const(centroids), k](Point& point) {
				inline_put_data_in_buckets(centroids, k, point);
			};
			std::for_each(std::execution::par, data.begin(), data.end(), put_data_in_buckets);
		}

		// remove unused centroids
		auto unused_centroids = std::remove_if(
			std::execution::par,
			centroids.begin(),
			centroids.end(),
			[](const Centroid& centroid) {
				return centroid.IsUnused();
			}
		);
		centroids.erase(unused_centroids, centroids.end());

		// make the pair and return the result
		auto result = std::make_pair(data, centroids);
		return result;
	}

	std::vector<Point> KMeans::GetScrubbedData(const std::vector<Point>& data) const {
		return return_unique_data(data);
	}

	std::vector<Centroid> KMeans::GetCentroids(const size_t _k, std::vector<Point>& unique_data) const {
		return get_naive_centroids(_k, unique_data);
	}

	std::vector<Point>& KMeans::ChooseDataSet(std::vector<Point>& original_data, std::vector<Point>& scrubbed_data) const {
		return original_data;
	}


	/* -- KMeansPP -- */
	std::vector<Point> KMeansPP::GetScrubbedData(const std::vector<Point>& data) const {
		//k++ algorithm wants weighted data points going into centroid calculation
		return return_full_data(data);
	}

	std::vector<Centroid> KMeansPP::GetCentroids(const size_t _k, std::vector<Point>& data) const {
		return get_plus_plus_centroids(_k, data);
	}


	/* -- KMeansUnweighted -- */
	std::vector<Point>& KMeansUnweighted::ChooseDataSet(std::vector<Point>& original_data, std::vector<Point>& scrubbed_data) const {
		// unweighted wants unique data points to process
		return scrubbed_data;
	}


	/* -- KMeansUnweightedPP -- */
	std::vector<Centroid> KMeansUnweightedPP::GetCentroids(const size_t k, std::vector<Point>& unique_data) const {
		return get_plus_plus_centroids(k, unique_data);
	}

	/* ------------ End Class Method Definitions ------------ */


	/* -- Helper Functions Definitions -- */
	inline size_t clamp_k(const size_t k) {
		if (k < default_min_k)
			return default_min_k;
		if (k > default_max_k)
			return default_max_k;
		return k;
	}

	inline void inline_put_data_in_buckets(const std::vector<Centroid>& centroids, size_t k, Point& point) {
		int sq_distance = Point::BeyondMaxSquaredDistance;
		size_t i = 0;
		do {
			int temp_sq_distance = centroids[i].SquaredDistance(point);  // <-- unweighted kmeansunweighted++ has access violation
			if (temp_sq_distance < sq_distance) {
				sq_distance = temp_sq_distance;
				point.SetBucket(i);
			}
			i++;
		} while (i < k);
		return;
	}

	inline bool are_the_same_data_set(const std::vector<Point>& a, const std::vector<Point>& b) {
		return (&a == &b);
	}

	inline std::vector<Point> return_unique_data(const std::vector<Point>& data) {
		std::vector<Point> copy = data;
		// move non-unique items to the end
		std::sort(copy.begin(), copy.end());
		auto end_of_unique_points = std::unique(copy.begin(), copy.end());
		// remove the non-unique items
		copy.erase(end_of_unique_points, copy.end());
		return copy;
	}

	inline std::vector<Point> return_full_data(const std::vector<Point>& data) {
		std::vector<Point> copy = data;
		return copy;
	}

	inline std::vector<Centroid> get_naive_centroids(const size_t _k, std::vector<Point>& unique_data) {
		// k cannot be larger than the set of unique colors.
		static std::mt19937 random_generator(static_cast<unsigned int>(time(nullptr)));

		const size_t num_points = unique_data.size();
		const size_t k = (_k < num_points) ? _k : num_points;

		std::vector<Centroid> result;
		result.reserve(k);

		std::shuffle(unique_data.begin(), unique_data.end(), random_generator);

		for (size_t i = 0; i < k; i++) {
			result.emplace_back(unique_data[i]);
		}

		return result;
	}

	inline std::vector<Centroid> get_plus_plus_centroids(const size_t _k, std::vector<Point>& data) {
		// k cannot be larger than the set of unique colors.
		static std::mt19937 random_generator(static_cast<unsigned int>(time(nullptr)));


		const size_t num_points = data.size();
		const size_t k = (_k < num_points) ? _k : num_points;

		std::vector<Centroid> centroids;
		centroids.reserve(k);

		std::shuffle(data.begin(), data.end(), random_generator);
		centroids.emplace_back(data.back());

		do {
			auto set_distances = [&centroids = std::as_const(centroids)](Point& point) {
				int sq_distance = Point::BeyondMaxSquaredDistance;
				for (const auto& centroid : centroids) {
					int temp_sq_distance = centroid.SquaredDistance(point);
					if (temp_sq_distance < sq_distance) {
						sq_distance = temp_sq_distance;
						point.SetBucket(sq_distance);
					}
				}
			};
			std::for_each(std::execution::par, data.begin(), data.end(), set_distances);

			int64_t sq_distance_sum = 0;
			for (const auto& point : data) {
				sq_distance_sum += point.GetBucket();
			};

			std::uniform_int_distribution<int64_t> rand_distance_generator(1, sq_distance_sum);
			int64_t random_number = rand_distance_generator(random_generator);

			for (const auto& point : data) {
				random_number -= point.GetBucket();
				if (random_number <= 0) {
					centroids.emplace_back(point);
					break;
				}
			}
		} while (centroids.size() < k);

		return centroids;
	}
}
