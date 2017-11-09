#pragma once

#ifndef LIB_STATES_CONTROLLER_INCLUDE_HPP__
#define LIB_STATES_CONTROLLER_INCLUDE_HPP__

#include <mtypes/include/types.hpp>
#include <mtypes/include/function.hpp>
#include <mtypes/include/connection.hpp>
#include <mtypes/include/stack.hpp>
#include <mtypes/include/log.hpp>
#include <lib/scene/components/icomponent.hpp>

namespace lib
{
	using Action = function<void()>;
	namespace scene
	{
		template <class T>
		class StatesController : public IComponent
		{
		public:
			constexpr StatesController() noexcept = default;
			constexpr void UseDeferred() noexcept { m_useDeferred = true; }
			constexpr void UseDirect() noexcept { m_useDeferred = false; }
			constexpr virtual void update() override final {
				if (!m_pendingActions[0].empty()) {
					auto oldSize = m_pendingActions[0].size();
					m_inLoop = true;
					for (auto&& action : m_pendingActions[0]) {
						action();
					}
					m_inLoop = false;
					for (auto&& action : m_pendingActions[1]) {
						action();
					}
					assert_debug(m_pendingActions[0].size() == oldSize, "Action vector varied while processing");
					m_pendingActions[0].clear();
					m_pendingActions[1].clear();
				}
			}

			constexpr void start(T firstState) noexcept {
				assert_debug(m_statesStack.size() == 0, "You cannot call start if the stack is not empty");
				BeforeStart(firstState);
				push_state(std::move(firstState));
			}

			constexpr void push_state(T firstState) noexcept {
				postAction([this, firstState = std::move(firstState)]() {
					if (m_statesStack.size() > 0) {
						StatePaused(m_statesStack.back());
					}
					StatePushed(firstState);
					m_statesStack.push_back(std::move(firstState));
				});
			}

			constexpr void pop_state() noexcept {
				postAction([this]() {
					assert_debug(m_statesStack.size() > 0, "m_statesStack.size() is 0");
					StateFinished(m_statesStack.back());
					if (m_statesStack.size() > 1) {
						m_statesStack.pop_back();
						StateResumed(m_statesStack.back());
					}
					else {
						BeforeFinish(m_statesStack.back());
						m_statesStack.pop_back();
					}
				});
			}

			constexpr void setState(T newState) {
				changeState(std::move(newState));
			}

			constexpr const T&currentState() const noexcept { return m_statesStack.back(); }

			emitter<const T&> StateFinished;
			emitter<const T&> StateStarted;
			emitter<const T&> StatePushed;
			emitter<const T&> StatePopped;
			emitter<const T&> StatePaused;
			emitter<const T&> StateResumed;
			emitter<const T&> BeforeStart;
			emitter<const T&> BeforeFinish;

		private:
			inline void changeState(T newState) {
				postAction([this, newState = std::move(newState)]() {
					assert_debug(m_statesStack.size() != 0, "States stack size is 0");
					const T&oldState{ m_statesStack.back() };
					m_statesStack.pop_back();
					StateFinished(oldState);
					StateStarted(newState);
					m_statesStack.push_back(std::move(newState));
				});
			}

			constexpr void postAction(Action action) {
				if (m_useDeferred) {
					const size_type index{ m_inLoop ? 1u : 0u };
					m_pendingActions[index].push_back(std::move(action));
				}
				else {
					action();
				}
			}

			bool m_useDeferred{ false };
			stack<T> m_statesStack;
			vector<Action> m_pendingActions[2];
			bool m_inLoop{ false };
		};
	}
}

#endif
