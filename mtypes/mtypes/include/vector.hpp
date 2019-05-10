#pragma once

#ifndef MTYPES_VECTOR_INCLUDE_HPP
#define MTYPES_VECTOR_INCLUDE_HPP

#include <utility>
#include <initializer_list>
#include "function.hpp"

namespace lib
{
    template <typename T>
    class Allocator
    {
    public:
        T *allocate(const size_type size)
        {
            if (size)
            {
                return new T[size];
            }
            return nullptr;
        }

        void deallocate(T* element)
        {
            if (element)
            {
                delete[] element;
            }
        }
    };

	/** Vector class to store a sequence of elements
	* This class is a container to store sequences of Ts. It can be resized.
	* Other use cases include search, replacement, etc...
	*/
	template <typename T, typename Allocator_t = Allocator<T>>
	class  vector 
    {
	public:
		using iterator = T*;
		using const_iterator = const T*;
		using reference = T&;
		using const_reference = const T&;
        using value_type = T;
        using pointer = T *;
        using const_pointer = const T*;

		constexpr vector() noexcept : m_capacity{ 0 }, m_size{ 0 }, m_buffer{ nullptr } {}

        explicit constexpr vector(const size_type size) : m_capacity{ size }, m_size{ size }, 
            m_buffer{ m_allocator.allocate(size) }
        {}

		constexpr vector(std::initializer_list<value_type> ilist) noexcept : vector( ilist.size() ) 
        {
			_copyElements(ilist.begin(), m_size);
		}

		constexpr vector(const T*elements, const size_type size) : vector(size) 
        {
			_copyElements(elements, m_size);
		}

		constexpr vector(const vector&other) : m_capacity{ other.m_capacity }, m_size{ other.m_size }, 
            m_buffer{ m_allocator.allocate(m_capacity) }
        {
			_copyElements(other.m_buffer, other.m_size);
		}

		constexpr vector(const vector&other, const size_type size)
			: vector(other.m_buffer, size) {}

		constexpr vector(vector&&other) noexcept : m_capacity{ other.m_capacity }, m_size{ other.m_size }, m_buffer{ other.m_buffer } {
			other.m_capacity = 0U;
			other.m_size = 0U;
			other.m_buffer = nullptr;
		}

        constexpr vector(const_iterator _begin, const_iterator _end)
            : vector{ _begin, static_cast<size_type>(std::distance(_begin, _end)) } { }

		constexpr vector& operator=(const vector&other) {
			if (this != &other) {
				if (m_capacity < other.m_size) 
					_copyStructure(other);

				_copyElements(other.m_buffer, other.m_size);
				m_size = other.m_size;
			}
			return *this;
		}

		constexpr vector& copyExact(const vector&other) {
			if (this != &other) {
				_copyStructure(other);
				_copyElements(other.m_buffer, other.m_size);
			}
			return *this;
		}

		constexpr vector& operator=(vector&&other) noexcept {
			swap(other);
			return *this;
		}

		~vector() {
            _destroy();
		}

		constexpr reference operator[](const size_type index) noexcept { return m_buffer[index]; }
		constexpr const_reference operator[](const size_type index) const noexcept { return m_buffer[index]; }
		constexpr size_type capacity() const noexcept { return m_capacity; }
		constexpr size_type size() const noexcept { return m_size; }
		constexpr bool empty() const noexcept { return m_size == 0; }
		constexpr iterator begin() noexcept { return m_buffer; }
		constexpr const_iterator begin() const noexcept { return m_buffer; }
		constexpr const_iterator cbegin() const noexcept { return begin(); }
		constexpr iterator end() noexcept { return m_buffer + m_size; }
		constexpr const_iterator end() const noexcept { return m_buffer + m_size; }
		constexpr const_iterator cend() const noexcept { return end(); }
		constexpr T& front() noexcept { return m_buffer[0]; }
		constexpr T& back() noexcept { return m_buffer[m_size > 0 ? (m_size - 1) : 0]; }
        constexpr const T& cfront() const noexcept { return m_buffer[0]; }
        constexpr const T& cback() const noexcept { return m_buffer[m_size > 0 ? (m_size - 1) : 0]; }

        constexpr void for_each(const function<void(const T&)> f) {
			if (!empty()) {
				iterator current{ begin() };
                do {
					f(*current);
				} while (++current != end());
			}
		}

		constexpr void swap(vector& other) noexcept
		{
			std::swap(m_buffer, other.m_buffer);
			std::swap(m_size, other.m_size);
			std::swap(m_capacity, other.m_capacity);
		}

        constexpr size_type index_from_iterator(const const_iterator it) const noexcept
        {
            for (size_type i{ 0 }; i < m_size; ++i) 
            {
                if (m_buffer + i == it) {
                    return i;
                }
            }
			return m_size;
        }

		constexpr iterator remove_value(const T &value, iterator start)
        {
			return remove_if([&value](const T p) { return p == value; }, start);
		}

		constexpr iterator remove_value(const T &value)
        {
			return remove_value(value, begin());
		}

        //TO DO: Optimize
		constexpr size_type remove_values(const T&value, iterator start) 
        {
			return remove_all_if([&value](const T p) { return p == value; } , start);
		}

		constexpr size_type remove_values(const T&value) {
			return remove_values(value, begin());
		}

		constexpr size_type remove_all_from(const vector &other) {
			for (auto&& node : other) {
				remove_value(std::forward<T>(node));
			}
            return m_size;
		}

		constexpr iterator remove_if(function<bool(const T&)> condition, iterator start) 
        {
			bool moving{ false };
			iterator where_it_was{ end() };
            auto old_size(m_size);
			for (size_type i{ index_from_iterator(start) }; i < old_size; ++i) 
            {
				if (!moving) 
                {
					if (condition(m_buffer[i])) 
                    {
						moving = true;
						--m_size;
						where_it_was = m_buffer + i;
					}
				}
				else 
                {
					m_buffer[i - 1] = std::move(m_buffer[i]);
				}
			}
			return where_it_was;
		}

		constexpr iterator remove_if(function<bool(const T&)> condition) 
        {
            return remove_if(std::move(condition), begin());
		}

		constexpr size_type remove_all_if(function<bool(const T&)> condition, iterator start) 
        {
			do 
            {
				start = remove_if(condition, start);
			} while (start != end());

			return m_size;
		}

		constexpr size_type remove_all_if(function<bool(const T&)> condition) 
        {
			return remove_all_if(condition, begin());
		}

		constexpr decltype(auto) find_first_of(const vector& other) const noexcept
		{
			auto it(cbegin());
			while (it != cend())
			{
				if (other.cfind(*it) != other.cend())
				{
					return it;
				}
				++it;
			}
			return it;
		}

		constexpr iterator find(const T&element) noexcept 
		{
			auto finder( begin() );
			while (finder != end() && !(*finder == element)) ++finder;
			return finder;
		}

		constexpr const_iterator find(const T&element) const noexcept {
			return cfind(element);
		}

		constexpr const_iterator cfind(const T&element) const noexcept
		{
			auto finder{ cbegin() };
			while (finder != cend() && !(*finder == element)) ++finder;
			return finder;
		}

		template<typename ...Args>
		constexpr void emplace_back(Args&&... args) {
			if (m_size == m_capacity) {
				reserve(m_size + 1);
			}

			m_buffer[m_size++] = T(std::forward<Args>(args)...);
		}

		constexpr void shrink_to_fit() 
        {
			if (m_size < m_capacity) 
            {
				T*oldBuffer{ m_buffer };
				m_buffer = m_allocator.allocate(m_size);
				m_capacity = m_size;
				_moveElements(oldBuffer,m_size);

				m_allocator.deallocate(oldBuffer);
			}
		}

		constexpr void push_back(const T& value) 
        {
			reserve(m_size + 1);
			m_buffer[m_size++] = value;
		}

		constexpr void push_back(T&& value) {
			reserve(m_size + 1);
			m_buffer[m_size++] = std::move(value);
		}

		constexpr void insert(const vector &other) {
			//TO DO: Optimize
            if (!other.empty())
            {
                reserve(m_size + other.m_size);
                for (const auto& element : other) {
                    push_back(element);
                }
            }
		}

		constexpr void insert(vector &&other) {
			//TO DO: Optimize
			reserve(m_size + other.m_size);
			for (auto&& element : other) {
				push_back(std::move(element));
			}
		}

		constexpr vector& operator+=(const vector &other) {
			insert(other);
			return *this;
		}

		constexpr vector& operator+=(vector &&other) {
			insert(std::move(other));
			return *this;
		}

		constexpr void pop_back() noexcept { if (m_size > 0) --m_size; }

		constexpr void reserve(const size_type capacity) noexcept 
        {
			if (m_capacity < capacity) 
            {
				T *old_buffer{ m_buffer };
				m_capacity = capacity;

				m_buffer = m_allocator.allocate(m_capacity);
				_moveElements(old_buffer, m_size);
                m_allocator.deallocate(old_buffer);
			}
		}

		constexpr void resize(const size_type size) {
			if (size != m_size) {
				// Delete to shrink
				while (m_size > size) {
					pop_back();
				}

				// Append the necessary
				while (m_size < size) {
					emplace_back();
				}
			}
		}

		constexpr void clear() noexcept {
			m_size = 0;
		}

	private:

		constexpr void _copyStructure(const vector&other)
        {
			_destroy();
			m_buffer = m_allocator.allocate(other.m_size);
			m_capacity = other.m_size;
			m_size = other.m_size;
		}

		constexpr void _copyElements(const T*const source, const size_type s)
        {
			for (size_type i{ 0U }; i < s; ++i) 
            {
				m_buffer[i] = source[i];
			}
		}

		constexpr void _moveElements(T*source, const size_type s) noexcept 
        {
			for (size_type i{ 0U }; i < s; ++i) 
            {
				m_buffer[i] = std::move(source[i]);
			}
		}

		constexpr void _destroy() 
        {
			if (m_buffer) 
            {
                m_allocator.deallocate(m_buffer);
				m_buffer = nullptr;
				m_size = m_capacity = 0;
			}
		}

		size_type m_capacity;
		size_type m_size;
		T* m_buffer;
        Allocator_t m_allocator;

		template <class A>
		friend constexpr bool operator==(const vector<A>& lhs, const vector<A>& rhs) noexcept;

		template <class A>
		friend constexpr bool operator!=(const vector<A>& lhs, const vector<A>& rhs) noexcept;
	};

	template <class A>
	constexpr bool operator==(const vector<A>& lhs, const vector<A>& rhs) noexcept 
    {
		if (lhs.m_size != rhs.m_size) 
        {
			return false;
		}
        else 
        {
			for (decltype(lhs.m_size) i{ 0 }; i < lhs.m_size;++i) 
            {
				if (lhs.m_buffer[i] != rhs.m_buffer[i]) 
                {
					return false;
				}
			}
			return true;
		}
	}

    class str;
    template <typename T> str &operator<<(str &str_in, const vector<T>& data)
    {
        str_in << "[";
        for (size_type index = 0U; index < data.size(); ++index)
        {
            str_in << data[index];
            if (index != data.size() - 1)
            {
                str_in << ",";
            }
        }
        str_in << "]";
        return str_in;
    }


	template <class A>
	constexpr bool operator!=(const vector<A>& lhs, const vector<A>& rhs) noexcept {
		return !(lhs == rhs);
	}

	template <typename T>
	using vector_shared_pointers = vector<sptr<T>>;

	template <typename T>
	using vector_unique_pointers = vector<uptr<T>>;

    template <typename T>
    using vector_weak_pointers = vector<wptr<T>>;
}

#endif
