#pragma once

#ifndef LIB_TIMER_INCLUDE_HPP__
#define LIB_TIMER_INCLUDE_HPP__

#include <mtypes/include/types.hpp>
#include <mtypes/include/connection.hpp>
#include "mtypes_export.hpp"
#include <chrono>

namespace lib
{
    namespace time
    {
        class MTYPES_EXPORT TimePoint
        {
        public:
            using time_rep_t = s64;
            using millis_rep_t = s64;
            using seconds_rep_t = f32;

            constexpr TimePoint(time_rep_t representation)
                : m_representation{ representation } {}
            
            constexpr time_rep_t micro() const noexcept {
                return m_representation;
            }

            constexpr millis_rep_t millis() const noexcept {
                return static_cast<millis_rep_t>(m_representation / 1000);
            }

            constexpr seconds_rep_t seconds() const noexcept {
                return static_cast<seconds_rep_t>(millis() / 1000);
            }

            friend constexpr TimePoint operator-(const TimePoint &lhs, const TimePoint &rhs) noexcept;
            friend constexpr bool operator>(const TimePoint &lhs, const TimePoint &rhs) noexcept;
            friend constexpr bool operator<(const TimePoint &lhs, const TimePoint &rhs) noexcept;
            friend constexpr bool operator==(const TimePoint &lhs, const TimePoint &rhs) noexcept;
            friend constexpr bool operator!=(const TimePoint &lhs, const TimePoint &rhs) noexcept;

        private:
            time_rep_t m_representation;
        };

        constexpr TimePoint operator-(const TimePoint &lhs, const TimePoint &rhs) noexcept {
            return TimePoint(lhs.m_representation - rhs.m_representation);
        }

        constexpr bool operator>(const TimePoint &lhs, const TimePoint &rhs) noexcept {
            return lhs.m_representation > rhs.m_representation;
        }

        constexpr bool operator<(const TimePoint &lhs, const TimePoint &rhs) noexcept {
            return lhs.m_representation < rhs.m_representation;
        }

        constexpr bool operator==(const TimePoint &lhs, const TimePoint &rhs) noexcept {
            return lhs.m_representation == rhs.m_representation;
        }

        constexpr bool operator!=(const TimePoint &lhs, const TimePoint &rhs) noexcept {
            return lhs.m_representation != rhs.m_representation;
        }

        class IClock
        {
        public:
            virtual TimePoint now() const = 0;
        };
        class MTYPES_EXPORT Clock : public IClock
        {
        public:
            TimePoint now() const override;
        };

         class MTYPES_EXPORT Timer
        {
        public:
            Timer(Clock cl) : m_clock{ std::move(cl) }, m_start { m_clock.now() } {}
            TimePoint ellapsed() const { return m_clock.now() - m_start; }

        private:
            Clock m_clock;
            TimePoint m_start;
        };
    }

	struct TimerPrivate;
	struct TimePrivate;

    enum class TimeInitializationTag
    {
        Microseconds,
        Milliseconds,
        Seconds
    };
	class MTYPES_EXPORT Time
	{
	public:
        Time(const u64 quantity = 0, TimeInitializationTag initTag = TimeInitializationTag::Microseconds);
		Time(Time &&rh);
        Time(const Time&rhs);
        Time&operator=(Time&&rhs);
		Time&operator=(const Time&rhs);
		~Time();

		Time &operator+=(const Time &rh);
		Time &operator-=(const Time &rh);
		Time operator-(const Time &rh) const;
        bool operator>(const Time&rhs) const noexcept;
        bool operator<(const Time&rhs) const noexcept;
        bool operator>=(const Time&rhs) const noexcept;
        bool operator<=(const Time&rhs) const noexcept;
        bool operator==(const Time&rhs) const noexcept;
        bool operator!=(const Time&rhs) const noexcept;

        u64 asMicroSeconds() const noexcept;
        u64 asMilliSeconds() const noexcept;
        u64 asSeconds() const noexcept;
        void setZero() noexcept;

	private:
#ifdef _MSC_VER
        #pragma warning(push)
        #pragma warning(disable:4251)
#endif
		uptr<TimePrivate> m_timePrivate;
#ifdef _MSC_VER
        #pragma warning(pop)
#endif
		friend class Timer;
	};
	class MTYPES_EXPORT Timer
	{
	public:
		Timer();
		virtual ~Timer();

        Timer(Timer&&) = default;
        Timer& operator=(Timer&&) = default;

		virtual const Time getElapsedTime() const;
		virtual void restart();
	private:
#ifdef _MSC_VER
        #pragma warning(push)
        #pragma warning(disable:4251)
#endif
        uptr<TimerPrivate> m_timerPrivate;
#ifdef _MSC_VER
        #pragma warning(pop)
#endif
	};

	class MTYPES_EXPORT PausableTimer : public Timer
	{
	public:
		void pause();
		void resume();
		bool switchPause();
		virtual const Time getElapsedTime() const override;
		virtual void restart() override;

	private:
		Timer m_pausedTimer;
		Time m_pausedTime;
		bool m_paused{ false };
	};
}

#endif