#pragma once

#ifndef LIB_PROPERTIES_HPP
#define LIB_PROPERTIES_HPP

#include "function.hpp"

namespace lib
{
    using callback_t = function<void()>;

    /**
     * This class provides a basic interface for all Properties of
     * the system.
     */
    template <typename T>
    class IProperty
    {
    public:
        /**
         * Get value of the property.
         * @return The content of the property.
        */
        virtual const T &get() const noexcept = 0;
        virtual void set(const T&v) noexcept = 0;
    };

    template <typename T>
    class BasicProperty : public IProperty<T>
    {
    public:
        constexpr BasicProperty() noexcept = default;
        constexpr BasicProperty(T&& iv) noexcept : m_value{ std::move(iv) } {}
        constexpr BasicProperty(const T& iv) noexcept : m_value{ iv } {}

        constexpr const T&operator()() const noexcept { return m_value; }
        constexpr void operator=(const T&v) noexcept { set(v); }
        const T &get() const noexcept override { return m_value; }
        void set(const T&v) noexcept override { m_value = v; }

    protected:
        T m_value{};
    };

    template <typename T>
    class PropertyState : public BasicProperty<T>
    {
        using BaseClass = BasicProperty<T>;
    public:
        constexpr PropertyState() noexcept : BaseClass{}, next_{} {}
        constexpr PropertyState(T iv) noexcept : BaseClass{ iv }, next_{ iv } {}

        constexpr void operator=(const T&v) noexcept { set(v); }

        constexpr bool hasChanged() const noexcept { return m_hasChanged; }
        constexpr void swapValues() noexcept { m_hasChanged = false; m_value = next_.get(); }
        constexpr void setChanged() noexcept { m_hasChanged = true; }
        constexpr bool readResetHasChanged() noexcept { const bool v{ m_hasChanged }; swapValues(); return v; }
        const T &get() const noexcept override { return m_value; }
        void set(const T&v) noexcept override { next_.set(v); setChanged(); }

    private:
        BasicProperty<T> next_;
        bool m_hasChanged{ true };
    };
/*
    template <typename T>
    class PropertyDoubleBuffered : public BasicProperty<T>
    {
        using BaseClass = BasicProperty<T>;
    public:
        constexpr PropertyDoubleBuffered() noexcept : BaseClass{T{}}, next_{T{}} {}
        constexpr PropertyDoubleBuffered(const T& iv) noexcept : BaseClass{ iv }, next_{iv} {}

        constexpr void operator=(const T&v) noexcept { set(v); }

        constexpr bool hasChanged() const noexcept { return m_hasChanged; }
        constexpr void resetHasChanged() noexcept { m_hasChanged = false; }
        constexpr void setChanged() noexcept { m_hasChanged = true; }
        constexpr bool readResetHasChanged() noexcept { const bool v{ m_hasChanged }; resetHasChanged(); return v; }
        const T &get() const noexcept override { return m_value; }
        void set(const T&v) noexcept override { m_value = v; setChanged(); }

    private:
        BasicProperty<T> next_;
        bool m_hasChanged{ true };
    };
*/
    template<typename T, typename ...Args>
    constexpr bool ps_hasChanged(const PropertyState<T> &arg, Args&&... args)
    {
        return arg.hasChanged() || ps_hasChanged(std::forward<Args>(args)...);
    }

    template<typename T>
    constexpr bool ps_hasChanged(PropertyState<T> &arg)
    {
        return arg.hasChanged();
    }

    template<typename T, typename ...Args>
    constexpr void ps_resetHasChanged(PropertyState<T> &arg, Args&&... args)
    {
        arg.resetHasChanged();
        ps_resetHasChanged(std::forward<Args>(args)...);
    }

    template<typename T>
    constexpr void ps_resetHasChanged(PropertyState<T> &arg)
    {
        return arg.resetHasChanged();
    }

    template<typename T, typename ...Args>
    constexpr bool ps_readResetHasChanged(PropertyState<T> &arg, Args&&... args)
    {
        const bool result_unary{arg.readResetHasChanged()};
        const bool result_rest{ps_readResetHasChanged(std::forward<Args>(args)...)};
        return result_unary || result_rest;
    }

    template<typename T>
    constexpr bool ps_readResetHasChanged(PropertyState<T> &arg)
    {
        return arg.readResetHasChanged();
    }
}

#endif
