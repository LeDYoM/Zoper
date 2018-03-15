#include "../mtypes/include/str.hpp"
#include <sstream>
#include <string>

namespace lib
{
    str::str(const u64  n) : str{ std::to_string(n).c_str() } {}
	str::str(const s64  n) : str{ std::to_string(n).c_str() } {}
	str::str(const u32  n) : str{ std::to_string(n).c_str() } {}
    str::str(const s32 n) : str{ std::to_string(n).c_str() } {}
    str::str(const f32 n) : str{ std::to_string(n).c_str() } {}
    str::str(const f64 n) : str{ std::to_string(n).c_str() } {}

    namespace detail
    {
        constexpr size_t _str_len(const char_type *const p_str) noexcept
        {
            const char_type *p_str_copy{ p_str };
            while (*p_str_copy) ++p_str_copy;
            return p_str_copy - p_str;
        }
    }
    str::str(const char_type * n) noexcept : m_data(n, detail::_str_len(n) + 1) {}

    vector<str> str::split(const char_type separator) const
    {
        vector<str> result;
        std::stringstream ss((*this).c_str());
        std::string tok;

        while (std::getline(ss, tok, separator)) {
            result.push_back(str(tok.c_str()));
        }
        return result;
    }

    str str::substr(size_type start, size_type len) const
    {
        if (start >= size() || len < 1)
            return "";

        str temp;
        while (start < size() && len > 0) {
            if (m_data[start] != 0) {
                temp.m_data.push_back(m_data[start]);
            }
            ++start;
            --len;
        }

        temp.m_data.push_back(0);
        return temp;
    }

	bool str::convert(u64 & n) const
	{
		return static_cast<bool>(std::istringstream(c_str()) >> n);
	}

	bool str::convert(s64 & n) const
	{
		return static_cast<bool>(std::istringstream(c_str()) >> n);
	}

	bool str::convert(u32 & n) const
    {
		return static_cast<bool>(std::istringstream(c_str()) >> n);
	}

	bool str::convert(s32 & n) const
    {
		return static_cast<bool>(std::istringstream(c_str()) >> n);
	}

	bool str::convert(u16 & n) const
	{
		return static_cast<bool>(std::istringstream(c_str()) >> n);
	}

	bool str::convert(s16 & n) const
	{
		return static_cast<bool>(std::istringstream(c_str()) >> n);
	}

	bool str::convert(f32 & n) const
    {
		return static_cast<bool>(std::istringstream(c_str()) >> n);
	}

	bool str::convert(f64 & n) const
    {
		return static_cast<bool>(std::istringstream(c_str()) >> n);
	}
}
