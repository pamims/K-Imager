#ifndef CC_ALGORITHM_H
#define CC_ALGORITHM_H

#include <vector>   // for std::vector<>
#include <utility>  // for std::pair<>
#include "CC_Point.h"  // for Point and Centroid


namespace cc {

	class IClusterAlgorithm {
	public:
		virtual std::pair<std::vector<Point>, std::vector<Centroid>>
			Process(size_t k, std::vector<Point> data) const = 0;
	};




	class KMeans : public IClusterAlgorithm {  //unique data, naive centroids, original data
	protected:
		virtual std::vector<Point> GetScrubbedData(const std::vector<Point>& data) const;  
		virtual std::vector<Centroid> GetCentroids(const size_t k, std::vector<Point>& unique_data) const;
		virtual std::vector<Point>& ChooseDataSet(std::vector<Point>& original_data, std::vector<Point>& scrubbed_data) const;
	public:
		KMeans() {};
		std::pair<std::vector<Point>, std::vector<Centroid>>
			Process(size_t k, std::vector<Point> data) const override;
	};




	class KMeansPP : public KMeans {  //original data, ++ centroids, original data
	protected:
		std::vector<Point> GetScrubbedData(const std::vector<Point>& data) const override;
		std::vector<Centroid> GetCentroids(const size_t k, std::vector<Point>& unique_data) const override;
		//std::vector<Point>& ChooseDataSet(std::vector<Point>& original_data, std::vector<Point>& scrubbed_data) const override; //temp
	};




	class KMeansUnweighted : public KMeans {  //unique data, naive centroids, scrubbed data
	protected:
		std::vector<Point>& ChooseDataSet(std::vector<Point>& original_data, std::vector<Point>& scrubbed_data) const override;
	};




	class KMeansUnweightedPP : public KMeansUnweighted {  //unique data, ++ centroids, scrubbed data
	protected:
		std::vector<Centroid> GetCentroids(const size_t k, std::vector<Point>& unique_data) const override;
	};
}

#endif
