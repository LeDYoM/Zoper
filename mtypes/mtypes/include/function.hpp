#ifndef MTYPES_FUNCTION_INCLUDE_HPP
#define MTYPES_FUNCTION_INCLUDE_HPP

#include <cassert>
#include "types.hpp"

namespace mtps
{
/**
 * @brief Unused general template
 * @tparam typename 
 */
template <typename>
class function;

/**
 * @brief Wrapper class to store a pointer to a function. It can store any
 * pointer to function, lambda or pointer to member with object
 * 
 * @tparam ReturnValue Return type of the stored function
 * @tparam Args Arguments type for the stored function
 */
template <typename ReturnValue, typename... Args>
class function<ReturnValue(Args...)> final
{
public:
    /**
     * @brief Default constructor.
     * Construct an empty object
     */
    constexpr function() noexcept : m_callable{nullptr} {}

    /**
     * @brief Construct from a callable object. Normally, a lambda or a pointer
     * to function.
     * 
     * @tparam T Type of the object passed
     * @param t Object to construct from
     */
    template <typename T>
    constexpr function(T t) : m_callable{msptr<CallableT<T>>(std::move(t))}
    {}

    /**
     * @brief Construct from a pointer to member and object.
     * 
     * @tparam T Type of the object
     * @tparam V Type of the member object pointer
     * @param t Object of the type t
     * @param f Pointer to the member function
     */
    template <typename T, typename V>
    constexpr function(T* const t, V f) :
        m_callable{msptr<CallableMethodPointerT<T>>(t, f)}
    {}

    /**
     * @brief Implicit conversion to bool
     * 
     * @return true The object contains data.
     * @return false The object is empty 
     */
    constexpr operator bool() const noexcept { return m_callable != nullptr; }

    /**
     * @brief Operator invoke to perform the actual call.
     * 
     * @param args Arguments to be forwarded to the function.
     * @return ReturnValue Return value of the function.
     */
    constexpr ReturnValue operator()(Args&&... args) const
    {
        assert(m_callable);
        return m_callable->Invoke(std::forward<Args>(args)...);
    }

    /**
     * @brief Equality comparison operator. Compares if two objects points
     * to the same internal object.
     * @note Returns false if they are not copy constructed or copy assigned.
     * 
     * @param other Other object to compare to.
     * @return true The two objects point to the same internal object.
     * @return false The two object point to different internal objects.
     */
    constexpr bool operator==(const function& other) const noexcept
    {
        return m_callable == other.m_callable;
    }

    /**
     * @brief Compare two objects if they point to the same callable object.
     * @note Returns true in case of pointers to members to the same object
     * and method.
     * 
     * @param other Other object to compare to.
     * @return true If the two objects are equal.
     * @return false If the two objects are different.
     */
    constexpr bool equals(function const& other) const noexcept
    {
        return m_callable->equals(*(other.m_callable));
    }

private:
    class ICallable
    {
    public:
        virtual ~ICallable() {}
        virtual ReturnValue Invoke(Args&&...)                = 0;
        virtual bool equals(ICallable const&) const noexcept = 0;
    };

    template <typename T>
    class CallableT final : public ICallable
    {
    public:
        template <typename Y>
        constexpr CallableT(Y&& t) noexcept : m_t{std::forward<Y>(t)}
        {}

        inline ReturnValue Invoke(Args&&... args) override
        {
            return m_t(std::forward<Args>(args)...);
        }

        inline bool equals(CallableT const& other) const noexcept
        {
            return &m_t == &other.m_t;
        }

        inline bool equals(ICallable const& other) const noexcept override
        {
            CallableT const* const tmp = dynamic_cast<CallableT const*>(&other);
            if (tmp == nullptr)
            {
                return false;
            }
            return equals(*tmp);
        }

    private:
        T m_t;
    };

    template <class T>
    class CallableMethodPointerT final : public ICallable
    {
    public:
        typedef ReturnValue (T::*HandlerFunctionPtr)(Args...);

        constexpr CallableMethodPointerT(T* const receiver,
                                         const HandlerFunctionPtr function) :
            obj{receiver}, function_{function}
        {}

        inline ReturnValue Invoke(Args&&... args) override
        {
            return (obj->*function_)(std::forward<Args>(args)...);
        }

        inline bool equals(CallableMethodPointerT const& other) const noexcept
        {
            return obj == other.obj && function_ == other.function_;
        }

        inline bool equals(ICallable const& other) const noexcept override
        {
            function::CallableMethodPointerT<T> const* const tmp =
                dynamic_cast<function::CallableMethodPointerT<T> const*>(
                    &other);
            if (tmp == nullptr)
            {
                return false;
            }
            return equals(*tmp);
        }

    private:
        T* const obj;
        const HandlerFunctionPtr function_;
    };

    sptr<ICallable> m_callable;
};

/**
 * @brief Helper function to create a @b function object deducing the type
 * automatically. The template parameters will be autodeduced.
 * 
 * @tparam ReturnType Return type of the function
 * @tparam Args Parameter types of the arguments
 * @param p Callable object
 * @return function with the same return type and arguments that the object
 * passed.
 */
template<typename ReturnType, typename... Args>
constexpr auto make_function(ReturnType(*p)(Args...))
{
  return function<ReturnType(Args...)>(p);
}

/**
 * @brief Helper function to create a @b function object deducing the type
 * automatically. Overload for objects and member pointers.
 * 
 * @tparam ReturnType Return type of the function
 * @tparam T Type of the class with the member function
 * @tparam Args Parameter types of the arguments
 * @param obj Objcect of type T
 * @param p Member pointer
 * @return function with the same return type and arguments that the object
 * and member function passed.
 */
template<typename ReturnType, typename T, typename... Args>
constexpr auto make_function(T *obj, ReturnType(T::*p)(Args...))
{
  return function<ReturnType(Args...)>(obj, p);
}

}  // namespace mtps

#endif
