#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace vrlib
{
	namespace util
	{

		std::vector<std::string> split(std::string toSplit, const std::string &seperator);

		std::vector<std::string> scandir(const std::string &path);

		glm::vec3 randomHsv();
		glm::vec3 hsv2rgb(glm::vec3 hsv);
		glm::vec3 rgb2hsv(glm::vec3 rgb);

		float random(float min, float max);

		std::string getGuid();

	}
}