#pragma once

#ifndef MTYPES_FUNCTION_INCLUDE_HPP
#define MTYPES_FUNCTION_INCLUDE_HPP

#include <cassert>
#include "types.hpp"

namespace mtps
{
template <typename>
class function;

template <typename ReturnValue, typename... Args>
class function<ReturnValue(Args...)> final
{
public:
    constexpr function() noexcept : m_callable{nullptr} {}
    constexpr function(std::nullptr_t) noexcept : m_callable{nullptr} {}

    constexpr operator bool() const noexcept { return m_callable != nullptr; }

    template <typename T, typename V>
    constexpr function(T *const t, V function) : m_callable{msptr<CallableMethodPointerT<T>>(t, function)} {}

    template <typename T>
    constexpr function(T t) : m_callable{msptr<CallableT<T>>(std::move(t))} {}

    constexpr ReturnValue operator()(Args... args) const
    {
        assert(m_callable);
        return m_callable->Invoke(std::forward<Args>(args)...);
    }

    constexpr bool operator==(const function &other) const noexcept
    {
        return m_callable == other.m_callable;
    }

private:
    class ICallable
    {
    public:
        virtual ~ICallable() {}
        virtual ReturnValue Invoke(Args...) = 0;
    };

    template <typename T>
    class CallableT final : public ICallable
    {
    public:
        template <typename Y>
        constexpr CallableT(Y &&t) noexcept : m_t{std::forward<Y>(t)} {}

        inline ReturnValue Invoke(Args... args) override
        {
            return m_t(std::forward<Args>(args)...);
        }

    private:
        T m_t;
    };

    template <class T>
    class CallableMethodPointerT final : public ICallable
    {
    public:
        typedef ReturnValue (T::*HandlerFunctionPtr)(Args...);

        constexpr CallableMethodPointerT(T *const receiver,
                                         const HandlerFunctionPtr function)
            : obj{receiver}, function_{function} {}

        inline ReturnValue Invoke(Args... args) override
        {
            return (obj->*function_)(std::forward<Args>(args)...);
        }

    private:
        T *const obj;
        const HandlerFunctionPtr function_;
    };

    sptr<ICallable> m_callable;
};
} // namespace mtps

#endif
