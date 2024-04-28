// KRATOS  ___|  |                   |                   |
//       \___ \  __|  __| |   |  __| __| |   |  __| _` | |
//             | |   |    |   | (    |   |   | |   (   | |
//       _____/ \__|_|   \__,_|\___|\__|\__,_|_|  \__,_|_| MECHANICS
//
//  License:         BSD License
//                   license: StructuralMechanicsApplication/license.txt
//
//  Main authors:    Riccardo Rossi
//
#include "structural_mechanics_application_variables.h"

namespace Kratos
{
typedef array_1d<double, 3> Vector3;
typedef std::size_t IndexType;

// General pourpose
KRATOS_CREATE_VARIABLE(int, INTEGRATION_ORDER); // The integration order considered on the element
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(LOCAL_MATERIAL_AXIS_1)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(LOCAL_MATERIAL_AXIS_2)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(LOCAL_MATERIAL_AXIS_3)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(LOCAL_PRESTRESS_AXIS_1)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(LOCAL_PRESTRESS_AXIS_2)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(CENTER_OF_GRAVITY)
KRATOS_CREATE_VARIABLE(double, MASS_MOMENT_OF_INERTIA)
KRATOS_CREATE_VARIABLE(Matrix, ELASTICITY_TENSOR)

// Generalized eigenvalue problem
KRATOS_CREATE_VARIABLE(int, BUILD_LEVEL)
KRATOS_CREATE_VARIABLE(Vector, EIGENVALUE_VECTOR)
KRATOS_CREATE_VARIABLE(Matrix, EIGENVECTOR_MATRIX)

KRATOS_CREATE_VARIABLE(Matrix, MODAL_MASS_MATRIX)
KRATOS_CREATE_VARIABLE(Matrix, MODAL_STIFFNESS_MATRIX)

// Geometrical
KRATOS_CREATE_VARIABLE(double, AXIAL_FORCE)
KRATOS_CREATE_VARIABLE(double, SHEAR_FORCE)
KRATOS_CREATE_VARIABLE(double, BENDING_MOMENT)
KRATOS_CREATE_VARIABLE(double, AREA)
KRATOS_CREATE_VARIABLE(double, IT)
KRATOS_CREATE_VARIABLE(double, IY)
KRATOS_CREATE_VARIABLE(double, IZ)
KRATOS_CREATE_VARIABLE(double, CROSS_AREA)
KRATOS_CREATE_VARIABLE(double, MEAN_RADIUS)
KRATOS_CREATE_VARIABLE(int, SECTION_SIDES)
KRATOS_CREATE_VARIABLE(Matrix, GEOMETRIC_STIFFNESS)
KRATOS_CREATE_VARIABLE(Matrix, LOCAL_ELEMENT_ORIENTATION)
KRATOS_CREATE_VARIABLE(double, MATERIAL_ORIENTATION_ANGLE)
KRATOS_CREATE_VARIABLE(Vector, CONDENSED_DOF_LIST)

// Truss generalized variables
KRATOS_CREATE_VARIABLE(double, TRUSS_PRESTRESS_PK2)
KRATOS_CREATE_VARIABLE(double, HARDENING_MODULUS_1D)
KRATOS_CREATE_VARIABLE(double, TANGENT_MODULUS)
KRATOS_CREATE_VARIABLE(double, PLASTIC_ALPHA)

// Beam generalized variables
KRATOS_CREATE_VARIABLE(double, AREA_EFFECTIVE_Y)
KRATOS_CREATE_VARIABLE(double, AREA_EFFECTIVE_Z)
KRATOS_CREATE_VARIABLE(double, INERTIA_ROT_Y)
KRATOS_CREATE_VARIABLE(double, INERTIA_ROT_Z)
KRATOS_CREATE_VARIABLE(Vector, LOCAL_AXES_VECTOR)
KRATOS_CREATE_VARIABLE(double, TORSIONAL_INERTIA)
KRATOS_CREATE_VARIABLE(double, I22)
KRATOS_CREATE_VARIABLE(double, I33)
KRATOS_CREATE_VARIABLE(double, LUMPED_MASS_ROTATION_COEFFICIENT)
KRATOS_CREATE_VARIABLE(Vector, BEAM_INITIAL_STRAIN_VECTOR)

// Shell generalized variables
KRATOS_CREATE_VARIABLE(bool, STENBERG_SHEAR_STABILIZATION_SUITABLE)
KRATOS_CREATE_VARIABLE(double, SHELL_OFFSET)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_STRAIN)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_STRAIN_GLOBAL)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_CURVATURE)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_CURVATURE_GLOBAL)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_FORCE)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_FORCE_GLOBAL)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_MOMENT)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_MOMENT_GLOBAL)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_STRESS_TOP_SURFACE)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_STRESS_TOP_SURFACE_GLOBAL)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_STRESS_MIDDLE_SURFACE)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_STRESS_MIDDLE_SURFACE_GLOBAL)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_STRESS_BOTTOM_SURFACE)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_STRESS_BOTTOM_SURFACE_GLOBAL)
KRATOS_CREATE_VARIABLE(double, VON_MISES_STRESS_TOP_SURFACE)
KRATOS_CREATE_VARIABLE(double, VON_MISES_STRESS_MIDDLE_SURFACE)
KRATOS_CREATE_VARIABLE(double, VON_MISES_STRESS_BOTTOM_SURFACE)
KRATOS_CREATE_VARIABLE(double, SHEAR_ANGLE)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_ORTHOTROPIC_STRESS_BOTTOM_SURFACE)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_ORTHOTROPIC_STRESS_TOP_SURFACE)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_ORTHOTROPIC_STRESS_BOTTOM_SURFACE_GLOBAL)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_ORTHOTROPIC_STRESS_TOP_SURFACE_GLOBAL)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_ORTHOTROPIC_4PLY_THROUGH_THICKNESS)
KRATOS_CREATE_VARIABLE(double, TSAI_WU_RESERVE_FACTOR)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_ORTHOTROPIC_LAMINA_STRENGTHS)

// ENERGY
KRATOS_CREATE_VARIABLE(double, ENERGY_DAMPING_DISSIPATION)

// Shell energies
KRATOS_CREATE_VARIABLE(double, SHELL_ELEMENT_MEMBRANE_ENERGY)
KRATOS_CREATE_VARIABLE(double, SHELL_ELEMENT_BENDING_ENERGY)
KRATOS_CREATE_VARIABLE(double, SHELL_ELEMENT_SHEAR_ENERGY)
KRATOS_CREATE_VARIABLE(double, SHELL_ELEMENT_MEMBRANE_ENERGY_FRACTION)
KRATOS_CREATE_VARIABLE(double, SHELL_ELEMENT_BENDING_ENERGY_FRACTION)
KRATOS_CREATE_VARIABLE(double, SHELL_ELEMENT_SHEAR_ENERGY_FRACTION)

// Membrane variables
KRATOS_CREATE_VARIABLE(Matrix, MEMBRANE_PRESTRESS)
KRATOS_CREATE_VARIABLE(Vector, PRESTRESS_VECTOR)
KRATOS_CREATE_VARIABLE(Vector, PRESTRESS_AXIS_1_GLOBAL)
KRATOS_CREATE_VARIABLE(Vector, PRESTRESS_AXIS_2_GLOBAL)
KRATOS_CREATE_VARIABLE(Matrix, PRESTRESS_AXIS_1)
KRATOS_CREATE_VARIABLE(Matrix, PRESTRESS_AXIS_2)
KRATOS_CREATE_VARIABLE(std::string, PROJECTION_TYPE_COMBO)
KRATOS_CREATE_VARIABLE(Vector, PRINCIPAL_CAUCHY_STRESS_VECTOR)
KRATOS_CREATE_VARIABLE(Vector, PRINCIPAL_PK2_STRESS_VECTOR)

// Mixed formulations variables
KRATOS_CREATE_VARIABLE(double, REACTION_STRAIN)

// Formfinding
KRATOS_CREATE_VARIABLE(double, LAMBDA_MAX)
KRATOS_CREATE_VARIABLE(bool, IS_FORMFINDING)
KRATOS_CREATE_VARIABLE(Matrix, BASE_REF_1)
KRATOS_CREATE_VARIABLE(Matrix, BASE_REF_2)

// Cross section
KRATOS_CREATE_VARIABLE(int, SHELL_CROSS_SECTION_OUTPUT_PLY_ID)
KRATOS_CREATE_VARIABLE(double, SHELL_CROSS_SECTION_OUTPUT_PLY_LOCATION)
KRATOS_CREATE_VARIABLE(Matrix, SHELL_ORTHOTROPIC_LAYERS)

// Nodal stiffness for the nodal concentrated element
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(NODAL_INITIAL_DISPLACEMENT)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(NODAL_DISPLACEMENT_STIFFNESS)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(NODAL_INITIAL_ROTATION)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(NODAL_ROTATIONAL_STIFFNESS)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(NODAL_DAMPING_RATIO)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(NODAL_ROTATIONAL_DAMPING_RATIO)

// For explicit central difference scheme
KRATOS_CREATE_VARIABLE(double, MASS_FACTOR)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MIDDLE_VELOCITY)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(FRACTIONAL_ACCELERATION)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(FRACTIONAL_ANGULAR_ACCELERATION)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MIDDLE_ANGULAR_VELOCITY)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(NODAL_INERTIA)
KRATOS_CREATE_VARIABLE(double, NODAL_DISPLACEMENT_DAMPING)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(NODAL_ROTATION_DAMPING)

// CONDITIONS
/* Beam conditions */
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(POINT_MOMENT)

// Adding the SPRISM EAS variables
KRATOS_CREATE_VARIABLE(double, ALPHA_EAS);
KRATOS_CREATE_VARIABLE(bool, CONSIDER_IMPLICIT_EAS_SPRISM_ELEMENT);
KRATOS_CREATE_VARIABLE(bool, CONSIDER_TOTAL_LAGRANGIAN_SPRISM_ELEMENT);
KRATOS_CREATE_VARIABLE(bool, PURE_EXPLICIT_RHS_COMPUTATION);

// Reset equations ids "flag"
KRATOS_CREATE_VARIABLE(bool, RESET_EQUATION_IDS);

// Adding the SPRISM additional variables
KRATOS_CREATE_VARIABLE(double, ANG_ROT);

// Adding the SPRISM variable to deactivate the quadratic interpolation
KRATOS_CREATE_VARIABLE(bool, CONSIDER_QUADRATIC_SPRISM_ELEMENT);

// Additional strain measures
KRATOS_CREATE_VARIABLE(Vector, HENCKY_STRAIN_VECTOR);
KRATOS_CREATE_VARIABLE(Matrix, HENCKY_STRAIN_TENSOR);

KRATOS_CREATE_VARIABLE(double, VON_MISES_STRESS)

KRATOS_CREATE_VARIABLE(Matrix, REFERENCE_DEFORMATION_GRADIENT);
KRATOS_CREATE_VARIABLE(double, REFERENCE_DEFORMATION_GRADIENT_DETERMINANT);

// Rayleigh variables
KRATOS_CREATE_VARIABLE(double, RAYLEIGH_ALPHA)
KRATOS_CREATE_VARIABLE(double, RAYLEIGH_BETA)

// System damping
KRATOS_CREATE_VARIABLE(double,  SYSTEM_DAMPING_RATIO )
KRATOS_CREATE_VARIABLE(double,  SECOND_SYSTEM_DAMPING_RATIO )

// Nodal load variables
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(POINT_LOAD)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(LINE_LOAD)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(SURFACE_LOAD)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MOVING_LOAD)
KRATOS_CREATE_VARIABLE(double, MOVING_LOAD_LOCAL_DISTANCE)

// Condition load variables
KRATOS_CREATE_VARIABLE(Vector, POINT_LOADS_VECTOR)
KRATOS_CREATE_VARIABLE(Vector, LINE_LOADS_VECTOR)
KRATOS_CREATE_VARIABLE(Vector, SURFACE_LOADS_VECTOR)
KRATOS_CREATE_VARIABLE(Vector, MOVING_LOADS_VECTOR)
KRATOS_CREATE_VARIABLE(Vector, POSITIVE_FACE_PRESSURES_VECTOR)
KRATOS_CREATE_VARIABLE(Vector, NEGATIVE_FACE_PRESSURES_VECTOR)

// Displacement-Control variables
KRATOS_CREATE_VARIABLE(double, LOAD_FACTOR)
KRATOS_CREATE_VARIABLE(double, PRESCRIBED_DISPLACEMENT)

// Response function variables
KRATOS_CREATE_VARIABLE(double, RESPONSE_VALUE)

// Adjoint Variables
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(ADJOINT_DISPLACEMENT)
KRATOS_CREATE_VARIABLE(Vector, ADJOINT_PARTICULAR_DISPLACEMENT)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(ADJOINT_ROTATION)
KRATOS_CREATE_VARIABLE(double, PERTURBATION_SIZE)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( ADJOINT_CURVATURE );
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( ADJOINT_STRAIN );
KRATOS_CREATE_VARIABLE(bool, ADAPT_PERTURBATION_SIZE)

// Variables for output of sensitivities
KRATOS_CREATE_VARIABLE( double, CROSS_AREA_SENSITIVITY );
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( POINT_LOAD_SENSITIVITY );
KRATOS_CREATE_VARIABLE(double, I22_SENSITIVITY );
KRATOS_CREATE_VARIABLE(double, I33_SENSITIVITY );
KRATOS_CREATE_VARIABLE(double, THICKNESS_SENSITIVITY );
KRATOS_CREATE_VARIABLE(double, YOUNG_MODULUS_SENSITIVITY );
KRATOS_CREATE_VARIABLE(double, AREA_EFFECTIVE_Y_SENSITIVITY );
KRATOS_CREATE_VARIABLE(double, AREA_EFFECTIVE_Z_SENSITIVITY );
KRATOS_CREATE_VARIABLE(bool, IS_ADJOINT );
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(NODAL_DISPLACEMENT_STIFFNESS_SENSITIVITY);
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(NODAL_ROTATIONAL_STIFFNESS_SENSITIVITY);

// Variables to for computing parts of sensitivity analysis
KRATOS_CREATE_VARIABLE( int, TRACED_STRESS_TYPE );
KRATOS_CREATE_VARIABLE( Matrix, STRESS_DISP_DERIV_ON_NODE);
KRATOS_CREATE_VARIABLE( Matrix, STRESS_DISP_DERIV_ON_GP );
KRATOS_CREATE_VARIABLE( Matrix, STRESS_DESIGN_DERIVATIVE_ON_NODE);
KRATOS_CREATE_VARIABLE( Matrix, STRESS_DESIGN_DERIVATIVE_ON_GP );
KRATOS_CREATE_VARIABLE( Vector, STRESS_ON_GP );
KRATOS_CREATE_VARIABLE( Vector, STRESS_ON_NODE );
KRATOS_CREATE_VARIABLE( std::string, DESIGN_VARIABLE_NAME );

// for DEM-FEM 2D
KRATOS_CREATE_VARIABLE(double, IMPOSED_Z_STRAIN_VALUE)
KRATOS_CREATE_VARIABLE(bool, IMPOSED_Z_STRAIN_OPTION)


}
