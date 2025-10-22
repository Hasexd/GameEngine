#pragma once

#include <string>


namespace Core
{
	class UUID
	{
	public:

		UUID();

		std::string ToString() const { return std::to_string(m_UUID); }

		[[nodiscard]] operator uint64_t() const { return m_UUID; }
		[[nodiscard]] operator std::string() const { return ToString(); }
	private:
		uint64_t m_UUID;
	};
}

namespace std
{
	template<>
	struct hash<Core::UUID>
	{
		std::size_t operator()(const Core::UUID& uuid) const noexcept
		{
			return std::hash<uint64_t>()(uuid);
		}
	};
}