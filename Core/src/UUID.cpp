#include "UUID.h"

#include <sstream>
#include <random>

namespace Core
{
	UUID::UUID()
	{
		static thread_local std::random_device rd;
		static thread_local std::mt19937_64 gen(rd());
		static thread_local std::uniform_int_distribution<uint64_t> dis;

		m_UUID = dis(gen);
	}
}