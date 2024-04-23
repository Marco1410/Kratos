//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Vicente Mataix Ferrandiz
//

// System includes

// External includes

// Project includes
#include "containers/model.h"
#include "testing/testing.h"
#include "includes/kratos_flags.h"
#include "utilities/cpp_tests_utilities.h"

/* Processes */
#include "processes/find_global_nodal_neighbours_for_entities_process.h"
#include "processes/find_nodal_neighbours_process.h"

namespace Kratos::Testing
{
/**
* This test case verifies the functionality of the FindNodalNeighboursProcess for a 2D triangular mesh.
* It checks that the process correctly identifies the number of nodal neighbours and neighbouring elements for each node.
*/
KRATOS_TEST_CASE_IN_SUITE(FindNodalNeighboursProcess1, KratosCoreFastSuite)
{
    Model current_model;
    ModelPart& r_model_part = current_model.CreateModelPart("Main",2);

    CppTestsUtilities::Create2DGeometry(r_model_part, "Element2D3N");

    FindNodalNeighboursProcess process(r_model_part);
    process.Execute();

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(1)->GetValue(NEIGHBOUR_NODES).size(), 3);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(1)->GetValue(NEIGHBOUR_ELEMENTS).size(), 2);

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(2)->GetValue(NEIGHBOUR_NODES).size(), 3);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(2)->GetValue(NEIGHBOUR_ELEMENTS).size(), 2);

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(3)->GetValue(NEIGHBOUR_NODES).size(), 5);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(3)->GetValue(NEIGHBOUR_ELEMENTS).size(), 4);

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(4)->GetValue(NEIGHBOUR_NODES).size(), 2);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(4)->GetValue(NEIGHBOUR_ELEMENTS).size(), 1);

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(5)->GetValue(NEIGHBOUR_NODES).size(), 3);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(5)->GetValue(NEIGHBOUR_ELEMENTS).size(), 2);

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(6)->GetValue(NEIGHBOUR_NODES).size(), 2);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(6)->GetValue(NEIGHBOUR_ELEMENTS).size(), 1);
}

/**
* This test case verifies the functionality of the FindNodalNeighboursProcess for a 3D tetrahedral mesh.
* It ensures that the process correctly computes the number of nodal neighbours and neighbouring elements for each node.
*/
KRATOS_TEST_CASE_IN_SUITE(FindNodalNeighboursProcess2, KratosCoreFastSuite)
{
    Model current_model;
    ModelPart& r_model_part = current_model.CreateModelPart("Main",2);

    CppTestsUtilities::Create3DGeometry(r_model_part, "Element3D4N");

    FindNodalNeighboursProcess process(r_model_part);
    process.Execute();

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(1)->GetValue(NEIGHBOUR_NODES).size(), 4);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(1)->GetValue(NEIGHBOUR_ELEMENTS).size(), 2);

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(2)->GetValue(NEIGHBOUR_NODES).size(), 4);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(2)->GetValue(NEIGHBOUR_ELEMENTS).size(), 2);

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(3)->GetValue(NEIGHBOUR_NODES).size(), 7);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(3)->GetValue(NEIGHBOUR_ELEMENTS).size(), 6);

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(4)->GetValue(NEIGHBOUR_NODES).size(), 5);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(4)->GetValue(NEIGHBOUR_ELEMENTS).size(), 3);

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(5)->GetValue(NEIGHBOUR_NODES).size(), 4);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(5)->GetValue(NEIGHBOUR_ELEMENTS).size(), 2);

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(6)->GetValue(NEIGHBOUR_NODES).size(), 9);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(6)->GetValue(NEIGHBOUR_ELEMENTS).size(), 9);

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(7)->GetValue(NEIGHBOUR_NODES).size(), 6);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(7)->GetValue(NEIGHBOUR_ELEMENTS).size(), 5);

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(8)->GetValue(NEIGHBOUR_NODES).size(), 8);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(8)->GetValue(NEIGHBOUR_ELEMENTS).size(), 7);

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(9)->GetValue(NEIGHBOUR_NODES).size(), 7);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(9)->GetValue(NEIGHBOUR_ELEMENTS).size(), 6);

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(10)->GetValue(NEIGHBOUR_NODES).size(), 4);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(10)->GetValue(NEIGHBOUR_ELEMENTS).size(), 2);

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(11)->GetValue(NEIGHBOUR_NODES).size(), 4);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(11)->GetValue(NEIGHBOUR_ELEMENTS).size(), 2);

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(12)->GetValue(NEIGHBOUR_NODES).size(), 4);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(12)->GetValue(NEIGHBOUR_ELEMENTS).size(), 2);
}

/**
* This test case verifies the functionality of the FindNodalNeighboursProcess for a 3D triangles mesh.
* It ensures that the process correctly computes the number of nodal neighbours and neighbouring elements for each node.
*/
KRATOS_TEST_CASE_IN_SUITE(FindNodalNeighboursProcess3, KratosCoreFastSuite)
{
    Model current_model;
    ModelPart& r_model_part = current_model.CreateModelPart("Main",2);

    CppTestsUtilities::CreateSphereTriangularMesh(r_model_part, "SurfaceCondition3D3N", 1.0);

    FindNodalNeighboursProcess process(r_model_part);
    process.Execute();

    // With conditions 0 is expected
    for (auto& r_node : r_model_part.Nodes()) {
        KRATOS_EXPECT_EQ(r_node.GetValue(NEIGHBOUR_NODES).size(), 0);
        KRATOS_EXPECT_EQ(r_node.GetValue(NEIGHBOUR_ELEMENTS).size(), 0);
    }

    // Create elements with the same geometry as conditions
    for (auto& r_cond : r_model_part.Conditions()) {
        r_model_part.CreateNewElement("Element3D3N", r_cond.Id(), r_cond.GetGeometry(), r_cond.pGetProperties());
    }

    // Re-execute the process
    process.Execute();

    // Check the neighbours greater than 0
    for (auto& r_node : r_model_part.Nodes()) {
        KRATOS_EXPECT_GT(r_node.GetValue(NEIGHBOUR_NODES).size(), 0);
        KRATOS_EXPECT_GT(r_node.GetValue(NEIGHBOUR_ELEMENTS).size(), 0);
    }
}

/**
* This test case examines the functionality of the FindNodalNeighboursProcess specifically for nodal condition neighbours in a 2D triangular mesh.
* It validates that the process accurately computes the number of neighbouring nodes for each condition node.
*/
KRATOS_TEST_CASE_IN_SUITE(FindNodalNeighboursProcess2_Conditions, KratosCoreFastSuite)
{
    Model current_model;
    ModelPart& r_model_part = current_model.CreateModelPart("Main",2);

    CppTestsUtilities::Create2DGeometry(r_model_part, "SurfaceCondition3D3N", true, false);

    FindNodalNeighboursForEntitiesProcess<ModelPart::ConditionsContainerType> process(
        r_model_part,
        NEIGHBOUR_CONDITION_NODES);
    process.Execute();

    KRATOS_EXPECT_EQ(r_model_part.pGetNode(1)->GetValue(NEIGHBOUR_CONDITION_NODES).size(), 3);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(2)->GetValue(NEIGHBOUR_CONDITION_NODES).size(), 3);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(3)->GetValue(NEIGHBOUR_CONDITION_NODES).size(), 5);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(4)->GetValue(NEIGHBOUR_CONDITION_NODES).size(), 2);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(5)->GetValue(NEIGHBOUR_CONDITION_NODES).size(), 3);
    KRATOS_EXPECT_EQ(r_model_part.pGetNode(6)->GetValue(NEIGHBOUR_CONDITION_NODES).size(), 2);
}

}  // namespace Kratos::Testing.