#ifndef REX_MPI
#define REX_MPI

// This header provides a very thin wrapper around the MPI C API.

#include <string>
#include <system_error>
#include <utility>

#include <mpi.h>


namespace rex
{
    inline namespace mpi
    {
        // Class: mpi_category
        //
        // Error category class for OpenMPI error codes.
        //
        struct mpi_category : std::error_category
        {
            char const* name() const noexcept override
            {
                return "mpi";
            }

            std::string message(int code) const override
            {
                char msg[MPI_MAX_ERROR_STRING];
                int len = 0;
                if (MPI_Error_string(code, msg, &len)) {
                    return std::to_string(code);
                }
                return std::string(msg, msg + len);
            }
        };

        // Class: this_process
        //
        // RAII class for maintaining the MPI library. Instantiate this class
        // in the main function.
        //
        struct this_process
        {
            this_process(this_process&&) = delete;
            this_process(this_process const&) = delete;

            // Constructor
            //
            // Initializes the MPI library.
            //
            this_process(int& argc, char**& argv)
            {
                if (int err = MPI_Init(&argc, &argv)) {
                    throw std::system_error(err, mpi_category{});
                }
            }

            // Destructor
            //
            // Finalizes the MPI library.
            //
            ~this_process()
            {
                MPI_Finalize();
            }

            // Function: world
            //
            // Returns the world communicator.
            //
            MPI_Comm world() const
            {
                return MPI_COMM_WORLD;
            }
        };

        // Function: size
        //
        // Returns the number of processes in the given communicator.
        //
        inline int size(MPI_Comm comm)
        {
            int size = 0;
            if (int err = MPI_Comm_size(comm, &size)) {
                throw std::system_error(err, mpi_category{});
            }
            return size;
        }

        // Function: rank
        //
        // Returns the rank of this process in the given communicator.
        //
        inline int rank(MPI_Comm comm)
        {
            int rank = 0;
            if (int err = MPI_Comm_rank(comm, &rank)) {
                throw std::system_error(err, mpi_category{});
            }
            return rank;
        }

        // Function: send
        //
        // Sends a (double, int) pair.
        //
        // Parameters:
        //
        //   comm  - Communicator to use.
        //   rank  - Rank of the process to send a message to.
        //   tag   - Message tag.
        //   value - The pair of double-int pair to send.
        //
        inline void send(MPI_Comm comm, int rank, int tag, std::pair<double, int> const& value)
        {
            if (int err = MPI_Send(&value, 1, MPI_DOUBLE_INT, rank, tag, comm)) {
                throw std::system_error(err, mpi_category{});
            }
        }

        // Function: send
        //
        // Receives a (double, int) pair.
        //
        // Parameters:
        //
        //   comm  - Communicator to use.
        //   rank  - Rank of the process to receive a message from.
        //   tag   - Message tag.
        //   value - The variable to which a double-int pair is assgiend.
        //
        inline void recv(MPI_Comm comm, int rank, int tag, std::pair<double, int>& value)
        {
            MPI_Status status;
            if (int err = MPI_Recv(&value, 1, MPI_DOUBLE_INT, rank, tag, comm, &status)) {
                throw std::system_error(err, mpi_category{});
            }
        }
    }
}

#endif
