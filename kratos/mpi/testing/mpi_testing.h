//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Carlos Roig
//
//

#pragma once

// System includes

// External includes
#include <mpi.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Project includes
#include "testing/testing.h"
#include "includes/parallel_environment.h"

// Parallel Extension
#include "mpi/includes/mpi_expect.h"
#include "mpi/includes/mpi_communicator.h"
#include "mpi/includes/mpi_data_communicator.h"
#include "mpi/utilities/parallel_fill_communicator.h"

namespace Kratos::Testing 
{

/*
 * This Fixture creates a new kernel instance for kratos, so the test is able to interact with the database.
 * Its called this way to that all tests belong to a existing kernel fixture
*/
class KratosMpiTestEnv : public ::testing::Environment 
{
    public:
        ~KratosMpiTestEnv() override {}

        void SetUp() override;
        void TearDown() override;
};

/*
 * Initializes the parallel testing environment. This is usefull for other tests depending on a parallel environment.
*/
class MPIGTestMain {
    public:
        static int InitializeMPITesting(int argc, char* argv[]) {
            // Initialize MPI
            // int err = MPI_Init(&argc, &argv);
            MPI_Init(&argc, &argv);

            // Initialize the tests
            ::testing::InitGoogleTest(&argc, argv);

            // Get the size and rank 
            // TODO: we should ask this from the KratosMpiTestEnv, not from MPI directly, but for now it will have to suffice.
            int rank = 0;
            int size = 0;

            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            MPI_Comm_size(MPI_COMM_WORLD, &size);

            // Remove the default listener
            testing::TestEventListeners& listeners = testing::UnitTest::GetInstance()->listeners();
            auto default_printer = listeners.Release(listeners.default_result_printer());

            // Create a configurable listener
            Kratos::Testing::ConfigurableEventListener *listener = new Kratos::Testing::ConfigurableEventListener(default_printer);

            // Set the listener configuration (by default false to all if rank != 0)
            if (rank != 0) {
                listener->showStart = false;
                listener->showIterations = false;
                listener->showEnvironment = false;
                listener->showTestCases = false;
                listener->showTestNames = false;
                listener->showSuccesses = false;
                listener->showInlineFailures = false;
                listener->showResult = false;
                listener->showEnd = false;
            }

            // Add the MPI environment to the test 
            ::testing::AddGlobalTestEnvironment(new Kratos::Testing::KratosMpiTestEnv);

            // Add our listener
            listeners.Append(listener);

            // Run the tests ( we cannt use RUN_ALL_TESTS() in the return clause because we need to finalize MPI first but after having had the tests run)
            auto testing_resut = RUN_ALL_TESTS();

            // Finalize MPI
            MPI_Finalize();

            // Run the tests
            return testing_resut;
        }
};

} // namespace Kratos::Testing
