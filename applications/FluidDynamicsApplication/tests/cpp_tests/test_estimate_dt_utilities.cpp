//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Ruben Zorrilla
//
//

// System includes

// External includes

// Project includes
#include "testing/testing.h"
#include "containers/model.h"
#include "includes/cfd_variables.h"

// Application includes
#include "fluid_dynamics_application_variables.h"
#include "custom_utilities/estimate_dt_utilities.h"

namespace Kratos {
namespace Testing  {

namespace Internals {

void TestEstimateDtUtilitiesInitializeModelPart(
    ModelPart& rModelPart,
    const double DeltaTime)
{

    rModelPart.AddNodalSolutionStepVariable(VELOCITY);
    Properties::Pointer p_properties = rModelPart.CreateNewProperties(0);

    // Geometry creation
    rModelPart.CreateNewNode(1, 0.0, 0.0, 0.0);
    rModelPart.CreateNewNode(2, 2.0, 0.0, 0.0);
    rModelPart.CreateNewNode(3, 0.0, 1.0, 0.0);
    rModelPart.CreateNewNode(4, 1.0, 1.0, 0.0);
    std::vector<ModelPart::IndexType> element_nodes_1{1, 2, 3};
    std::vector<ModelPart::IndexType> element_nodes_2{3, 2, 4};
    rModelPart.CreateNewElement("QSVMS2D3N", 1, element_nodes_1, p_properties);
    rModelPart.CreateNewElement("QSVMS2D3N", 2, element_nodes_2, p_properties);

    // Set a fake current delta time
    rModelPart.GetProcessInfo().SetValue(DELTA_TIME, DeltaTime);

    // Set nodal data
    for (auto& rNode : rModelPart.Nodes()) {
        rNode.FastGetSolutionStepValue(VELOCITY_X) = rNode.Id() * rNode.X();
        rNode.FastGetSolutionStepValue(VELOCITY_Y) = rNode.Id() * rNode.Y();
    }
}

} // namespace internals

KRATOS_TEST_CASE_IN_SUITE(EstimateDtUtilitiesEstimateDt, FluidDynamicsApplicationFastSuite)
{
    // Set an extremely large current delta time to obtain a large CFL number
    const double current_dt = 1.0;

    // Create the test model part
    Model model;
    ModelPart& r_model_part = model.CreateModelPart("TestModelPart");
    Internals::TestEstimateDtUtilitiesInitializeModelPart(r_model_part, current_dt);

    // Estimate the delta time
    Parameters estimate_dt_settings = Parameters(R"({
        "automatic_time_step"   : true,
        "CFL_number"            : 1.0,
        "minimum_delta_time"    : 1e-4,
        "maximum_delta_time"    : 1e+1
    })");
    const auto estimate_dt_utility = EstimateDtUtility<2>(r_model_part, estimate_dt_settings);
    const double obtained_dt = estimate_dt_utility.EstimateDt();

    // Check results
    const double expected_dt = 0.238938;
    const double tolerance = 2.0e-6;
    KRATOS_CHECK_NEAR(expected_dt, obtained_dt, tolerance);
}

KRATOS_TEST_CASE_IN_SUITE(EstimateDtUtilitiesCalculateLocalCFL, FluidDynamicsApplicationFastSuite)
{
    // Set the current delta time to calculate the CFL number
    const double current_dt = 1.0e-1;

    // Create the test model part
    Model model;
    ModelPart& r_model_part = model.CreateModelPart("TestModelPart");
    Internals::TestEstimateDtUtilitiesInitializeModelPart(r_model_part, current_dt);

    // Calculate the CFL number for each element
    EstimateDtUtility<2>::CalculateLocalCFL(r_model_part);

    // Check results
    const double tolerance = 2.0e-6;
    KRATOS_CHECK_NEAR(r_model_part.GetElement(1).GetValue(CFL_NUMBER), 0.104167, tolerance);
    KRATOS_CHECK_NEAR(r_model_part.GetElement(2).GetValue(CFL_NUMBER), 0.418519, tolerance);
}

}
}