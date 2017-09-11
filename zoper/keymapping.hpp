#ifndef __KEYMAPPING_HPP__
#define __KEYMAPPING_HPP__

#include <mtypes/include/streams.hpp>
#include <lib/include/key.hpp>
#include "direction.hpp"
#include <array>

namespace zoper
{
	class KeyMapping
	{
	public:
		KeyMapping();
		virtual ~KeyMapping();

		void reset();
		static constexpr lib::u32 TotalKeys = Direction::Total + 2;

		lib::input::Key getKey(const Direction d) const noexcept;
		Direction getDirectionFromKey(const lib::input::Key k) const noexcept;
		lib::input::Key getLaunchKey() const noexcept;
		bool isLaunchKey(const lib::input::Key key) const noexcept;
		lib::input::Key getPauseKey() const noexcept;
		bool isPauseKey(const lib::input::Key key) const noexcept;

		bool setKey(const lib::u32 index, const lib::input::Key key);
		void apply();

		friend lib::SerializationStreamIn& operator>>(lib::SerializationStreamIn&ssi, KeyMapping &data);
		friend lib::SerializationStreamOut& operator<<(lib::SerializationStreamOut&sso, const KeyMapping&data);
	private:
		std::array<lib::input::Key, KeyMapping::TotalKeys> m_keys;

	};


	inline lib::SerializationStreamIn & operator>>(lib::SerializationStreamIn & ssi, KeyMapping & data)
	{
		ssi >> data.m_keys;
		return ssi;
	}

	inline lib::SerializationStreamOut& operator<<(lib::SerializationStreamOut&sso, const KeyMapping&data)
	{
		sso << data.m_keys;
		return sso;
	}
}

#endif