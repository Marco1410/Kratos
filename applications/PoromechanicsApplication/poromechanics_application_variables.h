
//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Ignasi de Pouplana
//


#if !defined(KRATOS_POROMECHANICS_APPLICATION_VARIABLES_H_INCLUDED )
#define  KRATOS_POROMECHANICS_APPLICATION_VARIABLES_H_INCLUDED

// Project includes
#include "includes/define.h"
#include "includes/kratos_application.h"
#include "includes/variables.h"
#include "includes/cfd_variables.h"
#include "includes/mat_variables.h"
#include "structural_mechanics_application_variables.h"

namespace Kratos
{
//Define Variables

//Warning: Note that the application variables must not be defined if they already exist in "includes/variables.h",
//         in "includes/cfd_variables.h" or in "structural_mechanics_application_variables.h"

KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, VELOCITY_COEFFICIENT )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, DT_LIQUID_PRESSURE_COEFFICIENT )

KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, LIQUID_PRESSURE )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, REACTION_LIQUID_PRESSURE )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, DT_LIQUID_PRESSURE )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, NORMAL_LIQUID_FLUX )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, LIQUID_DISCHARGE )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, CAPILLARY_PRESSURE )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, LIQUID_SATURATION_DEGREE )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, LIQUID_RELATIVE_PERMEABILITY )

KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, DENSITY_SOLID )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, DENSITY_LIQUID )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, DYNAMIC_VISCOSITY_LIQUID )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, std::string, SATURATION_LAW_NAME )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, RESIDUAL_LIQUID_SATURATION )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, PORE_SIZE_FACTOR )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, MINIMUM_RELATIVE_PERMEABILITY )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, BULK_MODULUS_SOLID )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, BULK_MODULUS_LIQUID )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, PERMEABILITY_XX )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, PERMEABILITY_YY )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, PERMEABILITY_ZZ )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, PERMEABILITY_XY )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, PERMEABILITY_YZ )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, PERMEABILITY_ZX )

KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, NORMAL_STIFFNESS )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, SHEAR_STIFFNESS )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, PENALTY_STIFFNESS )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, TENSILE_STRENGTH )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, BETA_EQSTRAIN_SHEAR_FACTOR )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, int, DAMAGE_EVOLUTION_LAW )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, FRICTION_ANGLE )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, DILATANCY_ANGLE )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, COHESION )

KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, INITIAL_JOINT_WIDTH )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, TRANSVERSAL_PERMEABILITY_COEFFICIENT )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, MID_PLANE_LIQUID_PRESSURE )
KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS( POROMECHANICS_APPLICATION, LIQUID_FLUX_VECTOR )
KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS( POROMECHANICS_APPLICATION, LOCAL_LIQUID_FLUX_VECTOR )
KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS( POROMECHANICS_APPLICATION, CONTACT_STRESS_VECTOR )
KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS( POROMECHANICS_APPLICATION, LOCAL_STRESS_VECTOR )
KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS( POROMECHANICS_APPLICATION, LOCAL_RELATIVE_DISPLACEMENT_VECTOR )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, Matrix, PERMEABILITY_MATRIX )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, Matrix, LOCAL_PERMEABILITY_MATRIX )

KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, CRITICAL_DISPLACEMENT )

KRATOS_DEFINE_APPLICATION_VARIABLE(POROMECHANICS_APPLICATION, bool, IS_CONVERGED)

KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, Matrix, TOTAL_STRESS_TENSOR )

KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, Matrix, INITIAL_STRESS_TENSOR )

KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, STATE_VARIABLE )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, ARC_LENGTH_LAMBDA )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, ARC_LENGTH_RADIUS_FACTOR )

KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, TIME_UNIT_CONVERTER )

KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, LOCAL_EQUIVALENT_STRAIN )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, NONLOCAL_EQUIVALENT_STRAIN )

KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, JOINT_WIDTH )

KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, bool, NODAL_SMOOTHING )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, Matrix, NODAL_CAUCHY_STRESS_TENSOR )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, Matrix, EFFECTIVE_STRESS_TENSOR )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, Matrix, NODAL_EFFECTIVE_STRESS_TENSOR )
KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS( POROMECHANICS_APPLICATION, LIQUID_PRESSURE_GRADIENT )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, NODAL_JOINT_AREA )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, NODAL_JOINT_WIDTH )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, NODAL_JOINT_DAMAGE )

KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, SHEAR_FRACTURE_ENERGY )

KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, BIOT_COEFFICIENT )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, CURVE_FITTING_ETA )

KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS( POROMECHANICS_APPLICATION, DAMPING_FORCE )
KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS( POROMECHANICS_APPLICATION, DISPLACEMENT_OLD )
KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS( POROMECHANICS_APPLICATION, DISPLACEMENT_OLDER )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, FLUX_RESIDUAL )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, G_COEFFICIENT )
KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, THETA_FACTOR )

KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS( POROMECHANICS_APPLICATION, TARGET_REACTION )
KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS( POROMECHANICS_APPLICATION, AVERAGE_REACTION )
KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS( POROMECHANICS_APPLICATION, LOADING_VELOCITY )

}

#endif	/* KRATOS_POROMECHANICS_APPLICATION_VARIABLES_H_INCLUDED */
