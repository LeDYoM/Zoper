#ifndef HAF_HOST_IHOST_CONNECTOR_INCLUDE_HPP
#define HAF_HOST_IHOST_CONNECTOR_INCLUDE_HPP

namespace haf::host
{
class IHostConnector
{
public:
    /**
     * @brief Empty defaulted constexpr constructor.
     */
    constexpr IHostConnector() noexcept {}

    /**
     * @brief Destroy the HostConnector object. This destructor is intended
     * to be empty in this class
     */
    virtual ~IHostConnector() {}
};

}  // namespace haf::host

#endif
