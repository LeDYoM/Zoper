#ifndef MTPS_PROPERTIES_INCLUDE_HPP
#define MTPS_PROPERTIES_INCLUDE_HPP

namespace mtps
{
struct DummyTag {};

/**
 * @brief Class provides a basic interface for properties. This is the base
 * class for any Property class.
 * @tparam T Inner type of the property.
 */
template <typename T, typename Tag = DummyTag>
class IProperty
{
public:
    using tag = Tag;
    using type = T;
    using const_type = T const;
    using reference = T&;
    using const_reference = T const&;
    using pointer = T*;
    using const_pointer = T const *;

    /**
     * @brief Get value of the property.
     * @return const T& The content of the property.
     */
    virtual const T &get() const noexcept = 0;

    /**
     * @brief Sets the vañue of the inner type of the property.
     * 
     * @param v New value to set.
     * @return true The value has changed
     * @return false v was equal to the content, so no changes made.
     */
    virtual bool set(const T &v) = 0;
    virtual bool set(T &&v) = 0;
};

template <typename T, typename Tag = DummyTag>
class BasicProperty : public IProperty<T, Tag>
{
public:
    using Base = IProperty<T, Tag>;
    using tag = typename Base::tag;
    using type = typename Base::type;
    using const_type = typename Base::const_type;
    using reference = typename Base::reference;
    using const_reference = typename Base::const_reference;
    using pointer = typename Base::pointer;
    using const_pointer = typename Base::const_pointer;

    constexpr BasicProperty() noexcept = default;
    constexpr BasicProperty(BasicProperty &&) noexcept = default;
    BasicProperty(const BasicProperty &) = default;
    constexpr BasicProperty &operator=(BasicProperty &&) noexcept = default;
    constexpr BasicProperty &operator=(const BasicProperty &) noexcept = default;

    constexpr BasicProperty(T &&iv) noexcept : m_value{std::move(iv)} {}
    constexpr BasicProperty(const T &iv) noexcept : m_value{iv} {}

    constexpr const T &operator()() const noexcept { return m_value; }
    constexpr const T &operator=(const T &v) noexcept
    {
        set(v);
        return v;
    }
    inline const T &get() const noexcept override final { return m_value; }
    inline bool set(const T &v) override
    {
        if (!(m_value == v))
        {
            m_value = v;
            return true;
        }
        return false;
    }

    inline bool set(T &&v) override
    {
        if (!(m_value == std::forward<T>(v)))
        {
            m_value = std::move(v);
            return true;
        }
        return false;
    }

protected:
    T m_value{};
};
} // namespace mtps

#endif
