#ifndef REX_REPLICA_AGENT_HPP
#define REX_REPLICA_AGENT_HPP

// This header provides a replica-exchange agent class.

#include <iterator>
#include <random>
#include <utility>
#include <vector>

#include <cassert>
#include <cmath>
#include <cstdlib>

#include "mpi.hpp"


namespace rex
{
    // Class: replica_agent
    //
    // General-purpose replica exchange algorithm.
    //
    struct replica_agent
    {
        replica_agent(replica_agent&&) = default;
        replica_agent(replica_agent const&) = delete;
        replica_agent& operator=(replica_agent&&) = delete;
        replica_agent& operator=(replica_agent const&) = delete;

        // Constructor
        //
        // Parameters:
        //   communicator - MPI communicator to use.
        //   message_tag  - Arbitrary message tag to use. Juse pass zero if you
        //                  are unsure.
        //   temperatures - List of replica temperatures. The length must be the
        //                  same as the number of MPI processes.
        //
        replica_agent(
            MPI_Comm communicator, int message_tag, std::vector<double> const& temperatures
        )
            : communicator_{communicator}
            , temperature_table_{temperatures}
            , notify_replica_info_{message_tag}
        {
            assert(static_cast<int>(temperature_table_.size()) == mpi::size(communicator));
            assert(0 <= id_ && id_ < mpi::size(communicator));
        }

        // Function: id
        //
        // Returns the ID number of the replica currently this process holds.
        // This value changes on replica exchange.
        //
        int id() const noexcept
        {
            return id_;
        }

        // Function: temperature
        //
        // Returns the temperature of the replica currently this process holds.
        //
        double temperature() const noexcept
        {
            return temperature_of(id_);
        }

        // Function: exchange
        //
        // Parameters:
        //   energy - Energy of the replica this process holds.
        //   random - Random number generator to use.
        //
        // Returns:
        //   true if an exchange is done on this process.
        //
        template<class RNG>
        bool exchange(double energy, RNG& random)
        {
            auto const last_id = id_;

            // Each process tries to exchange replica with its adjacent pair.
            // The pairing pattern is 1/2, 3/4, ... on 2n-th trials and 0/1,
            // 2/3, ... on (2n+1)-th trials.
            auto const size = mpi::size(communicator_);
            auto const rank = mpi::rank(communicator_);
            auto const pair_rank =
                exchange_trials_ % 2 ?
                    (rank % 2 ? rank + 1 : rank - 1) :
                    (rank % 2 ? rank - 1 : rank + 1);
            assert(std::abs(rank - pair_rank) == 1);

            if (0 <= pair_rank && pair_rank < size) {
                // Master determines whether the exchange should be done.
                if (rank > pair_rank) {
                    id_ = exchange_as_master(energy, pair_rank, random);
                } else {
                    id_ = exchange_as_slave(energy, pair_rank);
                }
                assert(0 <= id_ && id_ < size);
            }
            ++exchange_trials_;

            return id_ != last_id;
        }

      private:

        // Protocol:
        //
        // 1. Slave sends its energy and ID to Master
        // 2. Master determines if an exchange should be done
        // 3. Master sends Slave's new energy and ID to Slave

        template<typename RNG>
        int exchange_as_master(double energy, int pair_rank, RNG& random) const
        {
            assert(pair_rank != mpi::rank(communicator_));

            // Protocol (1)
            std::pair<double, int> pair_info;
            mpi::recv(communicator_, pair_rank, notify_replica_info_, pair_info);
            auto const pair_energy = pair_info.first;
            auto const pair_id = pair_info.second;

            // Protocol (2)
            auto new_id = id_;
            auto const temp = temperature_of(id_);
            auto const pair_temp = temperature_of(pair_id);
            auto const log_random = std::exponential_distribution<double>{}(random);
            if (pair_temp * temp * log_random >= (pair_temp - temp) * (pair_energy - energy)) {
                std::swap(new_id, pair_info.second);
            }

            // Protocol (3)
            mpi::send(communicator_, pair_rank, notify_replica_info_, pair_info);

            return new_id;
        }

        int exchange_as_slave(double energy, int pair_rank) const
        {
            assert(pair_rank != mpi::rank(communicator_));

            // Protocol (1)
            mpi::send(communicator_, pair_rank, notify_replica_info_, std::make_pair(energy, id_));

            // Protocol (3)
            std::pair<double, int> new_info;
            mpi::recv(communicator_, pair_rank, notify_replica_info_, new_info);

            return new_info.second;
        }

        // Function: temperature_of
        //
        // Returns the temperature of the replica of the given ID.
        //
        double temperature_of(int id) const noexcept
        {
            assert(0 <= id && id < mpi::size(communicator_));
            return temperature_table_[static_cast<unsigned>(id)];
        }

      private:
        // MPI communicator to use.
        MPI_Comm const communicator_;

        // Temperature of each replica.
        std::vector<double> const temperature_table_;

        // Message tag used for replica exchange.
        int const notify_replica_info_;

        // ID of the replica currently owned by this process.
        int id_ = mpi::rank(communicator_);

        // Number of exchange trials so far.
        unsigned exchange_trials_ = 0;
    };
}

#endif
