#pragma once

#ifndef LIB_CORE_SYSTEM_SIMULATIONSYSTEM_PRIVATE_INCLUDE_HPP
#define LIB_CORE_SYSTEM_SIMULATIONSYSTEM_PRIVATE_INCLUDE_HPP

#include "simulationsystem.hpp"
#include "simulationaction.hpp"

#include "timesystem.hpp"
#include "inputsystem.hpp"

#include <lib/include/key.hpp>
#include <lib/include/liblog.hpp>
#include <lib/include/core/timepoint.hpp>
#include <lib/system/systemprovider.hpp>
#include <lib/system/simulabledataprovider.hpp>
#include <lib/system/randomsystem.hpp>

#include <mtypes/include/types.hpp>
#include <mtypes/include/object.hpp>
#include <mtypes/include/object_utils.hpp>

namespace lib::core
{
    struct ReplayData
    {
        SimulableDataBuffer data_buffer_;
        SimulationActionContainer simulation_actions_;  ///< Container containing the simulation actions to execute.
        str save_replay_file;
        str load_replay_file;
    };

    const Object& operator>>(const Object&obj, ReplayData& replay_data)
    {
        obj["replay_data"].getObject() >> replay_data.data_buffer_;
//        obj["input_data"].getObject() >> replay_data.simulation_actions_;
        return obj;
    }

    void setObjectProperty(Object& obj, const ReplayData& replay_data)
    {
        obj.set("replay_data", replay_data.data_buffer_);
//        obj.set("input_data", replay_data.simulation_actions_);
    }

    struct SimulationSystem::SimulationSystemPrivate final
    {
        CurrentSimulationActionIterator current_simulation_action_iterator_;    ///< Iterator pointing to the current simulation.
        SimulableDataBuffer simulable_data_buffer_;
        SimulableDataBuffer::const_iterator current_simulable_data_buffer_iterator;
        TimePoint last_checked_point_;

        ReplayData replay_data_;

        void setSimulationActions(const TimePoint &current, SimulationActionContainer sim_act_container)
        {
            replay_data_.simulation_actions_.swap(sim_act_container);
            current_simulation_action_iterator_ = replay_data_.simulation_actions_.cbegin();
            last_checked_point_ = current;
            log_debug_info("Simulation System started ");
        }

        void addSimulationAction(SimulationAction simulation_action)
        {
            replay_data_.simulation_actions_.push_back(std::move(simulation_action));
        }

        void setSimulatedDataBuffer(SimulableDataBuffer simulated_data_buffer)
        {
            simulable_data_buffer_ = std::move(simulated_data_buffer);
            current_simulable_data_buffer_iterator = simulable_data_buffer_.begin();
        }
    };
}

#endif
