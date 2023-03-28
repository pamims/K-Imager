#ifndef CC_MAPPER_H
#define CC_MAPPER_H

#include <utility>
#include <vector>
#include "CC_Point.h"



namespace cc {
	class DataMapper {
	private:
		DataMapper();
	public:
		static std::vector<Point> BuildData(const std::pair<std::vector<Point>, std::vector<Centroid>>& input);
	};
}

#endif
