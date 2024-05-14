//
//   Project Name:        KratosPoromechanicsApplication $
//   Last Modified by:    $Author:    Ignasi de Pouplana $
//   Date:                $Date:            January 2016 $
//   Revision:            $Revision:                 1.0 $
//

#if defined(KRATOS_PYTHON)

// System includes
#include <pybind11/pybind11.h>

// Project includes
#include "includes/define.h"
#include "includes/define_python.h"

// Application includes
#include "custom_python/add_custom_strategies_to_python.h"
#include "custom_python/add_custom_constitutive_laws_to_python.h"
#include "custom_python/add_custom_processes_to_python.h"
#include "custom_python/add_custom_utilities_to_python.h"
#include "poromechanics_application.h"

namespace Kratos
{

namespace Python
{

namespace py = pybind11;

PYBIND11_MODULE(KratosPoromechanicsApplication, m)
{
    py::class_<KratosPoromechanicsApplication,
    KratosPoromechanicsApplication::Pointer,
    KratosApplication>(m, "KratosPoromechanicsApplication")
    .def( py::init<>());

    AddCustomStrategiesToPython(m);
    AddCustomConstitutiveLawsToPython(m);
    AddCustomProcessesToPython(m);
    AddCustomUtilitiesToPython(m);

    //Registering variables in python
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, VELOCITY_COEFFICIENT )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, DT_LIQUID_PRESSURE_COEFFICIENT )

    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, LIQUID_PRESSURE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, REACTION_LIQUID_PRESSURE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, DT_LIQUID_PRESSURE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, NORMAL_LIQUID_FLUX )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, LIQUID_DISCHARGE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, CAPILLARY_PRESSURE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, LIQUID_SATURATION_DEGREE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, LIQUID_RELATIVE_PERMEABILITY )

    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( m, LIQUID_FLUX_VECTOR )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( m, LOCAL_LIQUID_FLUX_VECTOR )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( m, CONTACT_STRESS_VECTOR )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( m, LOCAL_STRESS_VECTOR )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( m, LOCAL_RELATIVE_DISPLACEMENT_VECTOR )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, PERMEABILITY_MATRIX )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, LOCAL_PERMEABILITY_MATRIX )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, TOTAL_STRESS_TENSOR )

    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, INITIAL_STRESS_TENSOR )

    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, IS_CONVERGED )

    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, ARC_LENGTH_LAMBDA )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, ARC_LENGTH_RADIUS_FACTOR )

    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, TIME_UNIT_CONVERTER )

    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, JOINT_WIDTH )

    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, MID_PLANE_LIQUID_PRESSURE)
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, SLIP_TENDENCY )

    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, NODAL_SMOOTHING )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, NODAL_CAUCHY_STRESS_TENSOR )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, EFFECTIVE_STRESS_TENSOR )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, NODAL_EFFECTIVE_STRESS_TENSOR )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( m, LIQUID_PRESSURE_GRADIENT )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, NODAL_JOINT_AREA )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, NODAL_JOINT_WIDTH )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, NODAL_JOINT_DAMAGE )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, NODAL_MID_PLANE_LIQUID_PRESSURE )

    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, SHEAR_FRACTURE_ENERGY )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, BIOT_COEFFICIENT )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, CURVE_FITTING_ETA )

    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( m, DAMPING_FORCE )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( m, DISPLACEMENT_OLD )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( m, DISPLACEMENT_OLDER )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, FLUX_RESIDUAL )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, G_COEFFICIENT )
    KRATOS_REGISTER_IN_PYTHON_VARIABLE( m, THETA_FACTOR )

    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( m, TARGET_REACTION )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( m, AVERAGE_REACTION )
    KRATOS_REGISTER_IN_PYTHON_3D_VARIABLE_WITH_COMPONENTS( m, LOADING_VELOCITY )

}

}  // namespace Python.
}  // namespace Kratos.

#endif // KRATOS_PYTHON defined
