#include "UUID.h"
#include <sstream>

namespace UUID
{
	std::string GenerateUUID()
	{
		static thread_local std::random_device rd;
		static thread_local std::mt19937 gen(rd());
		static thread_local std::uniform_int_distribution<int> dis(0, 15);
		static thread_local std::uniform_int_distribution<int> dis2(8, 11);

		std::stringstream ss;
		int i = 0;
		ss << std::hex;

		for (i = 0; i < 8; i++)
			ss << dis(gen);

		ss << "-";

		for (i = 0; i < 4; i++)
			ss << dis(gen);

		ss << "-4";

		for (i = 0; i < 3; i++)
			ss << dis(gen);

		ss << "-";
		ss << dis2(gen);

		for (i = 0; i < 3; i++)
			ss << dis(gen);

		ss << "-";

		for (i = 0; i < 12; i++)
			ss << dis(gen);

		return ss.str();
	}
}