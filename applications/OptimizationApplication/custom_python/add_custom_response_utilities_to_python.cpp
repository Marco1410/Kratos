//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   license: OptimizationApplication/license.txt
//
//  Main author:     Suneth Warnakulasuriya
//

// System includes

// External includes

// Project includes

// Application includes
#include "custom_utilities/response/mass_response_utils.h"
#include "custom_utilities/response/linear_strain_energy_response_utils.h"

// Include base h
#include "add_custom_response_utilities_to_python.h"

namespace Kratos {
namespace Python {

void  AddCustomResponseUtilitiesToPython(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<MassResponseUtils >(m, "MassResponseUtils")
        .def_static("Check", &MassResponseUtils::Check)
        .def_static("CalculateValue", &MassResponseUtils::CalculateValue)
        .def_static("CalculateSensitivity", &MassResponseUtils::CalculateSensitivity<double>)
        .def_static("CalculateSensitivity", &MassResponseUtils::CalculateSensitivity<array_1d<double, 3>>)
        ;

    py::class_<LinearStrainEnergyResponseUtils >(m, "LinearStrainEnergyResponseUtils")
        .def_static("CalculateValue", &LinearStrainEnergyResponseUtils::CalculateValue)
        .def_static("CalculateSensitivity", &LinearStrainEnergyResponseUtils::CalculateSensitivity<double>)
        .def_static("CalculateSensitivity", &LinearStrainEnergyResponseUtils::CalculateSensitivity<array_1d<double, 3>>)
        ;

}

}  // namespace Python.
} // Namespace Kratos

