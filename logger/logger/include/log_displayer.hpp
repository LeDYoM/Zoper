#pragma once

#ifndef HAF_LOG_LOG_DISPLAYER_INCLUDE_HPP
#define HAF_LOG_LOG_DISPLAYER_INCLUDE_HPP

#include <utility>

namespace logger
{
/**
 * @brief Helper class to forward the log calls to a Logger. It makes some
 * assumptions about the @b SeverityType.
 *
 * @tparam LogClass Log class capable of displaying logs.
 */
template <typename LogClass, typename SeverityType>
struct LogDisplayer
{
private:
    template <typename SeverityType::severity_type_t severity_value,
              typename... Args>
    static constexpr void log_if_severity_under(Args&&... args) noexcept
    {
        LogClass::log(std::forward<Args>(args)...);
    }

public:
    template <typename... Args>
    static constexpr void debug(Args&&... args) noexcept
    {
        log_if_severity_under<SeverityType::severity_type_t::debug>(
            std::forward<Args>(args)...);
    }

    template <bool Condition, typename... Args>
    static constexpr void debug_if_ce([[maybe_unused]] Args&&... args) noexcept
    {
        if constexpr (Condition)
        {
            debug(std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    static constexpr void debug_if(bool const cond, Args&&... args) noexcept
    {
        if (cond)
        {
            debug(std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    static constexpr void verbose(Args&&... args) noexcept
    {
        log_if_severity_under<SeverityType::severity_type_t::verbose>(
            std::forward<Args>(args)...);
    }

    template <bool Condition, typename... Args>
    static constexpr void verbose_if_ce([[maybe_unused]] Args&&... args) noexcept
    {
        if constexpr (Condition)
        {
            verbose(std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    static constexpr void verbose_if(bool const cond, Args&&... args) noexcept
    {
        if (cond)
        {
            verbose(std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    static constexpr void info(Args&&... args) noexcept
    {
        log_if_severity_under<SeverityType::severity_type_t::info>(
            std::forward<Args>(args)...);
    }

    template <bool Condition, typename... Args>
    static constexpr void info_if_ce([[maybe_unused]] Args&&... args) noexcept
    {
        if constexpr (Condition)
        {
            info(std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    static constexpr void info_if(bool const cond, Args&&... args) noexcept
    {
        if (cond)
        {
            info(std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    static constexpr void warn(Args&&... args) noexcept
    {
        log_if_severity_under<SeverityType::severity_type_t::warn>(
            std::forward<Args>(args)...);
    }

    template <bool Condition, typename... Args>
    static constexpr void warn_if_ce([[maybe_unused]] Args&&... args) noexcept
    {
        if constexpr (Condition)
        {
            warn(std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    static constexpr void warn_if(bool const cond, Args&&... args) noexcept
    {
        if (cond)
        {
            warn(std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    static constexpr void error(Args&&... args) noexcept
    {
        log_if_severity_under<SeverityType::severity_type_t::error>(
            std::forward<Args>(args)...);
    }

    template <bool Condition, typename... Args>
    static constexpr void error_if_ce([[maybe_unused]] Args&&... args) noexcept
    {
        if constexpr (Condition)
        {
            error(std::forward<Args>(args)...);
        }
    }

    template <typename... Args>
    static constexpr void error_if(bool const cond, Args&&... args) noexcept
    {
        if (cond)
        {
            error(std::forward<Args>(args)...);
        }
    }
};

}  // namespace logger

#endif
