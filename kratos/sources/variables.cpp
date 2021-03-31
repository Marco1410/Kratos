//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Pooyan Dadvand
//

// System includes

// External includes

// Project includes
#include "includes/define.h"
#include "includes/variables.h"
#include "includes/kernel.h"
#include "includes/convection_diffusion_settings.h"
#include "includes/radiation_settings.h"

namespace Kratos {
typedef Node<3> NodeType;
typedef array_1d<double, 3> Vector3;

//Create Variables by type:

//bools
KRATOS_CREATE_VARIABLE(bool, IS_RESTARTED)
KRATOS_CREATE_VARIABLE(bool, COMPUTE_DYNAMIC_TANGENT)
KRATOS_CREATE_VARIABLE(bool, COMPUTE_LUMPED_MASS_MATRIX)

//for Structural application:

//for Level Set application:

//ints
KRATOS_CREATE_VARIABLE(int, DOMAIN_SIZE)
KRATOS_CREATE_VARIABLE(int, SPACE_DIMENSION)

//for General kratos application:
KRATOS_CREATE_VARIABLE(int, LOAD_RESTART)
KRATOS_CREATE_VARIABLE(int, TIME_STEPS)
KRATOS_CREATE_VARIABLE(int, RIGID_BODY_ID)

//for Structural application
KRATOS_CREATE_VARIABLE(int, FIRST_TIME_STEP)
KRATOS_CREATE_VARIABLE(int, QUASI_STATIC_ANALYSIS)

KRATOS_CREATE_VARIABLE(int, NL_ITERATION_NUMBER)
KRATOS_CREATE_VARIABLE(int, PERIODIC_PAIR_INDEX)
KRATOS_CREATE_VARIABLE(int, STATIONARY)
KRATOS_CREATE_VARIABLE(int, ACTIVATION_LEVEL)

//for PFEM fluids application:
KRATOS_CREATE_VARIABLE(int, SCALE)

//for Level Set application:
KRATOS_CREATE_VARIABLE(int, REFINEMENT_LEVEL)

KRATOS_CREATE_VARIABLE(int, STEP)
KRATOS_CREATE_VARIABLE(int, PRINTED_STEP)
KRATOS_CREATE_VARIABLE(int, PRINTED_RESTART_STEP)
KRATOS_CREATE_VARIABLE(int, RUNGE_KUTTA_STEP)

//doubles

//for General kratos application:
KRATOS_CREATE_VARIABLE(double, TIME)
KRATOS_CREATE_VARIABLE(double, START_TIME)
KRATOS_CREATE_VARIABLE(double, END_TIME)
KRATOS_CREATE_VARIABLE(double, DELTA_TIME)
KRATOS_CREATE_VARIABLE(double, PREVIOUS_DELTA_TIME)
KRATOS_CREATE_VARIABLE(double, INTERVAL_END_TIME)
KRATOS_CREATE_VARIABLE(double, DELTA_TIME_FACTOR)

KRATOS_CREATE_VARIABLE(double, RESIDUAL_NORM)
KRATOS_CREATE_VARIABLE(double, CONVERGENCE_RATIO)
KRATOS_CREATE_VARIABLE(double, BUILD_SCALE_FACTOR)
KRATOS_CREATE_VARIABLE(double, CONSTRAINT_SCALE_FACTOR)
KRATOS_CREATE_VARIABLE(double, AUXILIAR_CONSTRAINT_SCALE_FACTOR)

KRATOS_CREATE_VARIABLE(double, TEMPERATURE)
KRATOS_CREATE_VARIABLE(double, PRESSURE)

KRATOS_CREATE_VARIABLE(double, NEGATIVE_FACE_PRESSURE)
KRATOS_CREATE_VARIABLE(double, POSITIVE_FACE_PRESSURE)
KRATOS_CREATE_VARIABLE(double, FACE_HEAT_FLUX)

KRATOS_CREATE_VARIABLE(double, DENSITY)
KRATOS_CREATE_VARIABLE(double, YOUNG_MODULUS)
KRATOS_CREATE_VARIABLE(double, POISSON_RATIO)
KRATOS_CREATE_VARIABLE(double, SHEAR_MODULUS_GAMMA12)
KRATOS_CREATE_VARIABLE(double, SHEAR_MODULUS_GAMMA12_2)
KRATOS_CREATE_VARIABLE(double, SHEAR_MODULUS_GAMMA12_3)
KRATOS_CREATE_VARIABLE(double, SHEAR_MODULUS_GAMMA12_4)
KRATOS_CREATE_VARIABLE(double, THICKNESS)
KRATOS_CREATE_VARIABLE(double, EQUIVALENT_YOUNG_MODULUS)

KRATOS_CREATE_VARIABLE(double, NODAL_H)

KRATOS_CREATE_VARIABLE(double, KINETIC_ENERGY)
KRATOS_CREATE_VARIABLE(double, INTERNAL_ENERGY)
KRATOS_CREATE_VARIABLE(double, STRAIN_ENERGY)
KRATOS_CREATE_VARIABLE(double, EXTERNAL_ENERGY)
KRATOS_CREATE_VARIABLE(double, TOTAL_ENERGY)

KRATOS_CREATE_VARIABLE(double, VOLUMETRIC_STRAIN)

KRATOS_CREATE_VARIABLE(double, THERMAL_EXPANSION_COEFFICIENT)
KRATOS_CREATE_VARIABLE(double, STABILIZATION_FACTOR)
KRATOS_CREATE_VARIABLE(double, DETERMINANT_F)

KRATOS_CREATE_VARIABLE(double, GRADIENT_PENALTY_COEFFICIENT)

KRATOS_CREATE_VARIABLE(double, TIME_INTEGRATION_THETA)

//sheme info :: pass to elements
KRATOS_CREATE_VARIABLE(double, NEWMARK_BETA)
KRATOS_CREATE_VARIABLE(double, NEWMARK_GAMMA)
KRATOS_CREATE_VARIABLE(double, BOSSAK_ALPHA)
KRATOS_CREATE_VARIABLE(double, EQUILIBRIUM_POINT)

// Lagrange multiplier for general purpose
KRATOS_CREATE_VARIABLE(double, SCALAR_LAGRANGE_MULTIPLIER)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(VECTOR_LAGRANGE_MULTIPLIER)

//for Structural application
KRATOS_CREATE_VARIABLE(double, FRICTION_COEFFICIENT)
KRATOS_CREATE_VARIABLE(double, LAMBDA)
KRATOS_CREATE_VARIABLE(double, MIU)
KRATOS_CREATE_VARIABLE(double, SCALE_FACTOR)
KRATOS_CREATE_VARIABLE(double, NORMAL_CONTACT_STRESS)
KRATOS_CREATE_VARIABLE(double, TANGENTIAL_CONTACT_STRESS)

KRATOS_CREATE_VARIABLE(int,    PARTITION_INDEX)
KRATOS_CREATE_VARIABLE(double, TEMPERATURE_OLD_IT)
KRATOS_CREATE_VARIABLE(double, VISCOSITY)
KRATOS_CREATE_VARIABLE(double, ERROR_RATIO)
KRATOS_CREATE_VARIABLE(double, ENERGY_NORM_OVERALL )
KRATOS_CREATE_VARIABLE(double, ERROR_OVERALL )
KRATOS_CREATE_VARIABLE(double, RHS_WATER)
KRATOS_CREATE_VARIABLE(double, RHS_AIR)
KRATOS_CREATE_VARIABLE(double, WEIGHT_FATHER_NODES)
KRATOS_CREATE_VARIABLE(double, INITIAL_PENALTY)
KRATOS_CREATE_VARIABLE(double, DP_EPSILON)
KRATOS_CREATE_VARIABLE(double, DP_ALPHA1)
KRATOS_CREATE_VARIABLE(double, DP_K)
KRATOS_CREATE_VARIABLE(double, INTERNAL_FRICTION_ANGLE)
KRATOS_CREATE_VARIABLE(double, K0)
KRATOS_CREATE_VARIABLE(double, NODAL_VOLUME)

KRATOS_CREATE_VARIABLE(double, WATER_PRESSURE)
KRATOS_CREATE_VARIABLE(double, REACTION_WATER_PRESSURE)
KRATOS_CREATE_VARIABLE(double, WATER_PRESSURE_ACCELERATION )

KRATOS_CREATE_VARIABLE(double, AIR_PRESSURE)
KRATOS_CREATE_VARIABLE(double, REACTION_AIR_PRESSURE)
KRATOS_CREATE_VARIABLE(double, FLAG_VARIABLE)
KRATOS_CREATE_VARIABLE(double, DISTANCE)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(DISTANCE_GRADIENT)

KRATOS_CREATE_VARIABLE(double, LAGRANGE_AIR_PRESSURE)
KRATOS_CREATE_VARIABLE(double, LAGRANGE_WATER_PRESSURE)
KRATOS_CREATE_VARIABLE(double, LAGRANGE_TEMPERATURE)

// for MultiScale application
KRATOS_CREATE_VARIABLE(Vector, INITIAL_STRAIN)
KRATOS_CREATE_VARIABLE(double, COEFFICIENT_THERMAL_EXPANSION)
KRATOS_CREATE_VARIABLE(double, CHARACTERISTIC_LENGTH_MULTIPLIER)
//MeshMovingApplication
KRATOS_CREATE_VARIABLE(double, AUX_MESH_VAR)

//Adjoint
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( ADJOINT_VECTOR_1 )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( ADJOINT_VECTOR_2 )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( ADJOINT_VECTOR_3 )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS( AUX_ADJOINT_VECTOR_1 )
KRATOS_CREATE_VARIABLE(double, ADJOINT_SCALAR_1 )
KRATOS_CREATE_VARIABLE(double, SCALAR_SENSITIVITY )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(SHAPE_SENSITIVITY)
KRATOS_CREATE_VARIABLE(double, NORMAL_SENSITIVITY)
KRATOS_CREATE_VARIABLE(double, NUMBER_OF_NEIGHBOUR_ELEMENTS)
KRATOS_CREATE_VARIABLE(bool, UPDATE_SENSITIVITIES)
KRATOS_CREATE_VARIABLE(AdjointExtensions::Pointer, ADJOINT_EXTENSIONS )
KRATOS_CREATE_VARIABLE(Matrix, NORMAL_SHAPE_DERIVATIVE )

//for Electric application

// For MeshingApplication
KRATOS_CREATE_VARIABLE(double, NODAL_ERROR )
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(NODAL_ERROR_COMPONENTS)
KRATOS_CREATE_VARIABLE(double, ELEMENT_ERROR )
KRATOS_CREATE_VARIABLE(double, ELEMENT_H )
KRATOS_CREATE_VARIABLE(Vector, RECOVERED_STRESS )
KRATOS_CREATE_VARIABLE(double, ERROR_INTEGRATION_POINT )
KRATOS_CREATE_VARIABLE(double, CONTACT_PRESSURE )

//for PFEM fluids application:
KRATOS_CREATE_VARIABLE(double, NODAL_AREA)

KRATOS_CREATE_VARIABLE(double, BULK_MODULUS)
KRATOS_CREATE_VARIABLE(double, SATURATION)
KRATOS_CREATE_VARIABLE(double, DENSITY_WATER)
KRATOS_CREATE_VARIABLE(double, VISCOSITY_WATER)
KRATOS_CREATE_VARIABLE(double, DENSITY_AIR)
KRATOS_CREATE_VARIABLE(double, VISCOSITY_AIR)
KRATOS_CREATE_VARIABLE(double, POROSITY)
KRATOS_CREATE_VARIABLE(double, DIAMETER)
KRATOS_CREATE_VARIABLE(double, LIN_DARCY_COEF)
KRATOS_CREATE_VARIABLE(double, NONLIN_DARCY_COEF)

KRATOS_CREATE_VARIABLE(double, AIR_ENTRY_VALUE)
KRATOS_CREATE_VARIABLE(double, FIRST_SATURATION_PARAM)
KRATOS_CREATE_VARIABLE(double, SECOND_SATURATION_PARAM)
KRATOS_CREATE_VARIABLE(double, PERMEABILITY_WATER)
KRATOS_CREATE_VARIABLE(double, PERMEABILITY_AIR)
KRATOS_CREATE_VARIABLE(double, BULK_AIR)

KRATOS_CREATE_VARIABLE(double, TEMP_CONV_PROJ)
KRATOS_CREATE_VARIABLE(double, CONVECTION_COEFFICIENT)

KRATOS_CREATE_VARIABLE(double, SOUND_VELOCITY)
KRATOS_CREATE_VARIABLE(double, AIR_SOUND_VELOCITY)
KRATOS_CREATE_VARIABLE(double, WATER_SOUND_VELOCITY)
KRATOS_CREATE_VARIABLE(double, NODAL_MASS)
KRATOS_CREATE_SYMMETRIC_3D_TENSOR_VARIABLE_WITH_COMPONENTS(NODAL_INERTIA_TENSOR)
KRATOS_CREATE_VARIABLE(double, AUX_INDEX)
KRATOS_CREATE_VARIABLE(double, EXTERNAL_PRESSURE)
KRATOS_CREATE_VARIABLE(double, VELOCITY_PERIOD)
KRATOS_CREATE_VARIABLE(double, ANGULAR_VELOCITY_PERIOD)
KRATOS_CREATE_VARIABLE(std::string, IDENTIFIER)

//for Other applications:
KRATOS_CREATE_VARIABLE(double, ARRHENIUS)
KRATOS_CREATE_VARIABLE(double, ARRHENIUSAUX)
KRATOS_CREATE_VARIABLE(double, ARRHENIUSAUX_)
KRATOS_CREATE_VARIABLE(double, PRESSUREAUX)
KRATOS_CREATE_VARIABLE(double, NODAL_MAUX)
KRATOS_CREATE_VARIABLE(double, NODAL_PAUX)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(NODAL_VAUX)
KRATOS_CREATE_VARIABLE(double, HEAT_FLUX)
KRATOS_CREATE_VARIABLE(double, REACTION_FLUX)
KRATOS_CREATE_VARIABLE(double, TC)
KRATOS_CREATE_VARIABLE(double, CONDUCTIVITY)
KRATOS_CREATE_VARIABLE(double, SPECIFIC_HEAT)
KRATOS_CREATE_VARIABLE(double, MATERIAL_VARIABLE)
KRATOS_CREATE_VARIABLE(double, FUEL)
KRATOS_CREATE_VARIABLE(double, YO)
KRATOS_CREATE_VARIABLE(double, YF)
KRATOS_CREATE_VARIABLE(double, YI)
KRATOS_CREATE_VARIABLE(double, Y1)
KRATOS_CREATE_VARIABLE(double, Y2)
KRATOS_CREATE_VARIABLE(double, YP)

KRATOS_CREATE_VARIABLE(double, ABSORPTION_COEFFICIENT)
KRATOS_CREATE_VARIABLE(double, STEFAN_BOLTZMANN_CONSTANT)

KRATOS_CREATE_VARIABLE(double, EMISSIVITY)
KRATOS_CREATE_VARIABLE(double, ENTHALPY)
KRATOS_CREATE_VARIABLE(double, MIXTURE_FRACTION)

KRATOS_CREATE_VARIABLE(double, YCH4)
KRATOS_CREATE_VARIABLE(double, YO2)
KRATOS_CREATE_VARIABLE(double, YCO2)
KRATOS_CREATE_VARIABLE(double, YH2O)
KRATOS_CREATE_VARIABLE(double, YN2)

KRATOS_CREATE_VARIABLE(double, WET_VOLUME)
KRATOS_CREATE_VARIABLE(double, CUTTED_AREA)
KRATOS_CREATE_VARIABLE(double, NET_INPUT_MATERIAL)

KRATOS_CREATE_VARIABLE(double, INCIDENT_RADIATION_FUNCTION)

KRATOS_CREATE_VARIABLE(double, SWITCH_TEMPERATURE)
KRATOS_CREATE_VARIABLE(double, NODAL_SWITCH)

//for Xfem application:
KRATOS_CREATE_VARIABLE(double, CRACK_OPENING)
KRATOS_CREATE_VARIABLE(double, CRACK_TRANSLATION)

//for Level Set application:
KRATOS_CREATE_VARIABLE(double, MIN_DT)
KRATOS_CREATE_VARIABLE(double, MAX_DT)
KRATOS_CREATE_VARIABLE(double, VEL_ART_VISC)
KRATOS_CREATE_VARIABLE(double, PR_ART_VISC)

//for Vulcan application
KRATOS_CREATE_VARIABLE(double, LATENT_HEAT)
KRATOS_CREATE_VARIABLE(double, AMBIENT_TEMPERATURE)

//vectors

//for General kratos application:
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(ANGULAR_ACCELERATION)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS_WITH_TIME_DERIVATIVE(ANGULAR_VELOCITY, ANGULAR_ACCELERATION)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS_WITH_TIME_DERIVATIVE(ROTATION, ANGULAR_VELOCITY)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(DELTA_ROTATION)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(REACTION_MOMENT)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(VELOCITY_LAPLACIAN)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(VELOCITY_LAPLACIAN_RATE)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(VELOCITY_COMPONENT_GRADIENT)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(VELOCITY_X_GRADIENT)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(VELOCITY_Y_GRADIENT)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(VELOCITY_Z_GRADIENT)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(REACTION)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(ACCELERATION)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS_WITH_TIME_DERIVATIVE(VELOCITY, ACCELERATION)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS_WITH_TIME_DERIVATIVE(DISPLACEMENT,VELOCITY)

KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(STEP_ROTATION)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(STEP_DISPLACEMENT)

KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(VOLUME_ACCELERATION)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(FORCE)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(TORQUE)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MOMENT)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(FACE_LOAD)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(NORMAL)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(TANGENT_XI)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(TANGENT_ETA)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(LOCAL_TANGENT)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(BODY_FORCE)

KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(FORCE_RESIDUAL)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MOMENT_RESIDUAL)

KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(EXTERNAL_FORCE)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(INTERNAL_FORCE)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(CONTACT_FORCE)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(CONTACT_NORMAL)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(TEMPERATURE_GRADIENT)

KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(LINEAR_MOMENTUM)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(ANGULAR_MOMENTUM)

KRATOS_CREATE_VARIABLE(Vector, EXTERNAL_FORCES_VECTOR)
KRATOS_CREATE_VARIABLE(Vector, INTERNAL_FORCES_VECTOR)
KRATOS_CREATE_VARIABLE(Vector, CONTACT_FORCES_VECTOR)

KRATOS_CREATE_VARIABLE(Vector, CAUCHY_STRESS_VECTOR)
KRATOS_CREATE_VARIABLE(Vector, PK2_STRESS_VECTOR)

KRATOS_CREATE_VARIABLE(Vector, RESIDUAL_VECTOR)

KRATOS_CREATE_VARIABLE(Vector, MARKER_LABELS)
KRATOS_CREATE_VARIABLE(Vector, MARKER_MESHES)

KRATOS_CREATE_VARIABLE(Vector, CONSTRAINT_LABELS)
KRATOS_CREATE_VARIABLE(Vector, CONSTRAINT_MESHES)

KRATOS_CREATE_VARIABLE(Vector, LOAD_LABELS)
KRATOS_CREATE_VARIABLE(Vector, LOAD_MESHES)

//for Structural application:
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MOMENTUM)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(LAGRANGE_DISPLACEMENT)

KRATOS_CREATE_VARIABLE(Vector, ELEMENTAL_DISTANCES)
KRATOS_CREATE_VARIABLE(Vector, ELEMENTAL_EDGE_DISTANCES)
KRATOS_CREATE_VARIABLE(Vector, ELEMENTAL_EDGE_DISTANCES_EXTRAPOLATED)
KRATOS_CREATE_VARIABLE(Vector, MATERIAL_PARAMETERS)
KRATOS_CREATE_VARIABLE(Vector, INTERNAL_VARIABLES)
KRATOS_CREATE_VARIABLE(Vector, INSITU_STRESS)

KRATOS_CREATE_VARIABLE(Vector, PENALTY)
KRATOS_CREATE_VARIABLE(Vector, NORMAL_STRESS)
KRATOS_CREATE_VARIABLE(Vector, TANGENTIAL_STRESS)
KRATOS_CREATE_VARIABLE(Vector, STRESSES)
KRATOS_CREATE_VARIABLE(Vector, STRAIN)

KRATOS_CREATE_VARIABLE(DenseVector<int>, NEIGHBOURS_INDICES)

//ALE Application
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(DETERMINANT)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(ELEMENTSHAPE)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY)

//for PFEM fluids application:
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(GRAVITY)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(NORMAL_TO_WALL)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(STRUCTURE_VELOCITY)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(DRAG_FORCE)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(EMBEDDED_VELOCITY)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(FRACT_VEL)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(ROTATION_CENTER)

//for Other applications:
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(SEEPAGE_DRAG)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MASS)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(RHS)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(FORCE_CM)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(MOMENTUM_CM)

KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(DIRECTION)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(Y)

KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(LOCAL_AXIS_1)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(LOCAL_AXIS_2)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(LOCAL_AXIS_3)

KRATOS_CREATE_VARIABLE(Vector, BDF_COEFFICIENTS)

//for Vulcan application
KRATOS_CREATE_VARIABLE(Vector, ENRICHED_PRESSURES)

//matrices

//for General kratos application:
KRATOS_CREATE_VARIABLE(Matrix, GREEN_LAGRANGE_STRAIN_TENSOR)
KRATOS_CREATE_VARIABLE(Matrix, PK2_STRESS_TENSOR)
KRATOS_CREATE_VARIABLE(Matrix, CAUCHY_STRESS_TENSOR)
KRATOS_CREATE_VARIABLE(Matrix, LOCAL_INERTIA_TENSOR)
KRATOS_CREATE_VARIABLE(Matrix, LOCAL_AXES_MATRIX)
KRATOS_CREATE_VARIABLE(Matrix, LOCAL_CONSTITUTIVE_MATRIX)
KRATOS_CREATE_VARIABLE(Matrix, CONSTITUTIVE_MATRIX)
KRATOS_CREATE_VARIABLE(Matrix, DEFORMATION_GRADIENT)
KRATOS_CREATE_VARIABLE(Matrix, MATERIAL_STIFFNESS_MATRIX)
KRATOS_CREATE_VARIABLE(Matrix, GEOMETRIC_STIFFNESS_MATRIX)
KRATOS_CREATE_VARIABLE(Vector, DETERMINANTS_OF_JACOBIAN_PARENT)
KRATOS_CREATE_VARIABLE(Quaternion<double>, ORIENTATION)

//for Structural application
KRATOS_CREATE_VARIABLE(Matrix, INERTIA)

//for General kratos application:
KRATOS_CREATE_VARIABLE(ConstitutiveLaw::Pointer, CONSTITUTIVE_LAW)

//NEIGHBOR_ELEMENTS defined in element.h
KRATOS_CREATE_VARIABLE(GlobalPointersVector<Element>, NEIGHBOUR_ELEMENTS)
//NEIGHBOR_CONDITIONS defined in condition.h
KRATOS_CREATE_VARIABLE(GlobalPointersVector<Condition>, NEIGHBOUR_CONDITIONS)

//for Structural application:
KRATOS_CREATE_VARIABLE(GlobalPointersVector<GeometricalObject>, NEIGHBOUR_EMBEDDED_FACES)
KRATOS_CREATE_VARIABLE(ConvectionDiffusionSettings::Pointer, CONVECTION_DIFFUSION_SETTINGS)
KRATOS_CREATE_VARIABLE(RadiationSettings::Pointer, RADIATION_SETTINGS)

KRATOS_CREATE_VARIABLE(PeriodicVariablesContainer, PERIODIC_VARIABLES)

// Variables that should be moved to applications (but have too many dependencies)
KRATOS_CREATE_VARIABLE(int, FRACTIONAL_STEP)
KRATOS_CREATE_VARIABLE(double, POWER_LAW_N)
KRATOS_CREATE_VARIABLE(double, POWER_LAW_K)
KRATOS_CREATE_VARIABLE(double, EQ_STRAIN_RATE)
KRATOS_CREATE_VARIABLE(double, YIELD_STRESS)
KRATOS_CREATE_VARIABLE(double, MU)
KRATOS_CREATE_VARIABLE(double, TAU)

KRATOS_CREATE_VARIABLE(double, SEARCH_RADIUS)

KRATOS_CREATE_VARIABLE(double, INTEGRATION_WEIGHT)
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(INTEGRATION_COORDINATES)
KRATOS_CREATE_VARIABLE(TableStreamUtility::Pointer, TABLE_UTILITY )

//for Geometry Variables
KRATOS_CREATE_3D_VARIABLE_WITH_COMPONENTS(PARAMETER_2D_COORDINATES)

KRATOS_CREATE_VARIABLE(std::vector<double>, SPANS_LOCAL_SPACE)

//------------------------------------------------------------------------------//
//------------------------------------------------------------------------------//
//------------------------------------------------------------------------------//

void KratosApplication::RegisterVariables() {
    KratosApplication::RegisterDeprecatedVariables();
    KratosApplication::RegisterCFDVariables();      //TODO: move to application
    KratosApplication::RegisterALEVariables();      //TODO: move to application
    KratosApplication::RegisterMappingVariables();  //TODO: move to application
    KratosApplication::RegisterDEMVariables();      //TODO: move to application
    KratosApplication::RegisterFSIVariables();      //TODO: move to application
    KratosApplication::RegisterMATVariables();      //TODO: move to application
    KratosApplication::RegisterGlobalPointerVariables();

    // Variables that should be moved to applications (but have too many dependencies)
    KRATOS_REGISTER_VARIABLE(FRACTIONAL_STEP)
    KRATOS_REGISTER_VARIABLE(POWER_LAW_N)
    KRATOS_REGISTER_VARIABLE(POWER_LAW_K)
    KRATOS_REGISTER_VARIABLE(EQ_STRAIN_RATE)
    KRATOS_REGISTER_VARIABLE(YIELD_STRESS)
    KRATOS_REGISTER_VARIABLE(MU)
    KRATOS_REGISTER_VARIABLE(TAU)

    //--------------- GENERAL VARIABLES FOR MULTIPLE APPLICATIONS -------------------//

    KRATOS_REGISTER_VARIABLE(DOMAIN_SIZE)
    KRATOS_REGISTER_VARIABLE(SPACE_DIMENSION)

    //STRATEGIES
    KRATOS_REGISTER_VARIABLE(LOAD_RESTART)

    KRATOS_REGISTER_VARIABLE(TIME_STEPS)
    KRATOS_REGISTER_VARIABLE(RIGID_BODY_ID)

    KRATOS_REGISTER_VARIABLE(STEP)
    KRATOS_REGISTER_VARIABLE(PRINTED_STEP)
    KRATOS_REGISTER_VARIABLE(PRINTED_RESTART_STEP)
    KRATOS_REGISTER_VARIABLE(RUNGE_KUTTA_STEP)

    KRATOS_REGISTER_VARIABLE(TIME)
    KRATOS_REGISTER_VARIABLE(START_TIME)
    KRATOS_REGISTER_VARIABLE(END_TIME)
    KRATOS_REGISTER_VARIABLE(DELTA_TIME)
    KRATOS_REGISTER_VARIABLE(PREVIOUS_DELTA_TIME)
    KRATOS_REGISTER_VARIABLE(INTERVAL_END_TIME)
    KRATOS_REGISTER_VARIABLE(DELTA_TIME_FACTOR)

    KRATOS_REGISTER_VARIABLE(RESIDUAL_NORM)
    KRATOS_REGISTER_VARIABLE(CONVERGENCE_RATIO)
    KRATOS_REGISTER_VARIABLE(BUILD_SCALE_FACTOR)
    KRATOS_REGISTER_VARIABLE(CONSTRAINT_SCALE_FACTOR)
    KRATOS_REGISTER_VARIABLE(AUXILIAR_CONSTRAINT_SCALE_FACTOR)

    //SCHEMES
    KRATOS_REGISTER_VARIABLE(IS_RESTARTED)
    KRATOS_REGISTER_VARIABLE(COMPUTE_DYNAMIC_TANGENT)
    KRATOS_REGISTER_VARIABLE(COMPUTE_LUMPED_MASS_MATRIX)

    KRATOS_REGISTER_VARIABLE(NEWMARK_BETA)
    KRATOS_REGISTER_VARIABLE(NEWMARK_GAMMA)
    KRATOS_REGISTER_VARIABLE(BOSSAK_ALPHA)
    KRATOS_REGISTER_VARIABLE(EQUILIBRIUM_POINT)

    //ROTATION
    //movement
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(ROTATION)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(DELTA_ROTATION)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(STEP_ROTATION)
    //reaction
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(REACTION_MOMENT)
    //movement time derivatives
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(ANGULAR_VELOCITY)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(ANGULAR_ACCELERATION)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(VELOCITY_LAPLACIAN)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(VELOCITY_LAPLACIAN_RATE)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(VELOCITY_COMPONENT_GRADIENT)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(VELOCITY_X_GRADIENT)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(VELOCITY_Y_GRADIENT)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(VELOCITY_Z_GRADIENT)
    //DISPLACEMENT
    // Movement time derivatives
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(ACCELERATION)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(VELOCITY)
    // Movement
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(STEP_DISPLACEMENT)
    //reaction
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(REACTION)

    //THERMAL DOFS
    KRATOS_REGISTER_VARIABLE(TEMPERATURE)

    //PRESSURE DOFS
    KRATOS_REGISTER_VARIABLE(PRESSURE)

    //EXTERNAL CONDITIONS
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(VOLUME_ACCELERATION)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(FORCE)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MOMENT)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(TORQUE)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(FACE_LOAD)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(NORMAL)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(TANGENT_XI)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(TANGENT_ETA)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(LOCAL_TANGENT)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(BODY_FORCE)  //to be deleted ?

    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(FORCE_RESIDUAL)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MOMENT_RESIDUAL)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(INTERNAL_FORCE)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(EXTERNAL_FORCE)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(CONTACT_FORCE)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(CONTACT_NORMAL)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(TEMPERATURE_GRADIENT)

    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(LINEAR_MOMENTUM)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(ANGULAR_MOMENTUM)

    KRATOS_REGISTER_VARIABLE(EXTERNAL_FORCES_VECTOR)
    KRATOS_REGISTER_VARIABLE(INTERNAL_FORCES_VECTOR)
    KRATOS_REGISTER_VARIABLE(CONTACT_FORCES_VECTOR)

    KRATOS_REGISTER_VARIABLE(RESIDUAL_VECTOR)

    KRATOS_REGISTER_VARIABLE(MARKER_LABELS)
    KRATOS_REGISTER_VARIABLE(MARKER_MESHES)

    KRATOS_REGISTER_VARIABLE(CONSTRAINT_LABELS)
    KRATOS_REGISTER_VARIABLE(CONSTRAINT_MESHES)

    KRATOS_REGISTER_VARIABLE(LOAD_LABELS)
    KRATOS_REGISTER_VARIABLE(LOAD_MESHES)

    KRATOS_REGISTER_VARIABLE(NEGATIVE_FACE_PRESSURE)
    KRATOS_REGISTER_VARIABLE(POSITIVE_FACE_PRESSURE)

    KRATOS_REGISTER_VARIABLE(FACE_HEAT_FLUX)

    //CONSTITUTIVE LAW AND PROPERTIES
    KRATOS_REGISTER_VARIABLE(CONSTITUTIVE_LAW)
    KRATOS_REGISTER_VARIABLE(DENSITY)
    KRATOS_REGISTER_VARIABLE(YOUNG_MODULUS)
    KRATOS_REGISTER_VARIABLE(POISSON_RATIO)
    KRATOS_REGISTER_VARIABLE(SHEAR_MODULUS_GAMMA12)
    KRATOS_REGISTER_VARIABLE(SHEAR_MODULUS_GAMMA12_2)
    KRATOS_REGISTER_VARIABLE(SHEAR_MODULUS_GAMMA12_3)
    KRATOS_REGISTER_VARIABLE(SHEAR_MODULUS_GAMMA12_4)
    KRATOS_REGISTER_VARIABLE(THICKNESS)
    KRATOS_REGISTER_VARIABLE(EQUIVALENT_YOUNG_MODULUS)
    KRATOS_REGISTER_VARIABLE(THERMAL_EXPANSION_COEFFICIENT)
    KRATOS_REGISTER_VARIABLE(STABILIZATION_FACTOR)
    KRATOS_REGISTER_VARIABLE(LOCAL_INERTIA_TENSOR)
    KRATOS_REGISTER_VARIABLE(LOCAL_AXES_MATRIX)
    KRATOS_REGISTER_VARIABLE(LOCAL_CONSTITUTIVE_MATRIX)
    KRATOS_REGISTER_VARIABLE(CONSTITUTIVE_MATRIX)
    KRATOS_REGISTER_VARIABLE(DEFORMATION_GRADIENT)
    KRATOS_REGISTER_VARIABLE(MATERIAL_STIFFNESS_MATRIX)
    KRATOS_REGISTER_VARIABLE(GEOMETRIC_STIFFNESS_MATRIX)
    KRATOS_REGISTER_VARIABLE(DETERMINANTS_OF_JACOBIAN_PARENT)
    KRATOS_REGISTER_VARIABLE(DETERMINANT_F)
    KRATOS_REGISTER_VARIABLE(GRADIENT_PENALTY_COEFFICIENT)
    KRATOS_REGISTER_VARIABLE(TIME_INTEGRATION_THETA)


    //STRAIN MEASURES
    KRATOS_REGISTER_VARIABLE(VOLUMETRIC_STRAIN)
    KRATOS_REGISTER_VARIABLE(GREEN_LAGRANGE_STRAIN_TENSOR)

    //STRESS MEASURES
    KRATOS_REGISTER_VARIABLE(CAUCHY_STRESS_TENSOR)
    KRATOS_REGISTER_VARIABLE(PK2_STRESS_TENSOR)

    KRATOS_REGISTER_VARIABLE(CAUCHY_STRESS_VECTOR)
    KRATOS_REGISTER_VARIABLE(PK2_STRESS_VECTOR)

    //GEOMETRICAL
    KRATOS_REGISTER_VARIABLE(NODAL_H)

    KRATOS_REGISTER_VARIABLE(NEIGHBOUR_ELEMENTS)
    KRATOS_REGISTER_VARIABLE(NEIGHBOUR_CONDITIONS)

    //ENERGIES
    KRATOS_REGISTER_VARIABLE(KINETIC_ENERGY)
    KRATOS_REGISTER_VARIABLE(INTERNAL_ENERGY)
    KRATOS_REGISTER_VARIABLE(STRAIN_ENERGY)
    KRATOS_REGISTER_VARIABLE(EXTERNAL_ENERGY)
    KRATOS_REGISTER_VARIABLE(TOTAL_ENERGY)

    // LAGRANGE MULTIPLIER
    KRATOS_REGISTER_VARIABLE(SCALAR_LAGRANGE_MULTIPLIER)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(VECTOR_LAGRANGE_MULTIPLIER)

    //--------------- GENERAL VARIABLES FOR MULTIPLE APPLICATIONS -------------------//

    //--------------- STRUCTURAL Application -------------------//

    KRATOS_REGISTER_VARIABLE(FIRST_TIME_STEP)
    KRATOS_REGISTER_VARIABLE(QUASI_STATIC_ANALYSIS)

    KRATOS_REGISTER_VARIABLE(NL_ITERATION_NUMBER)
    KRATOS_REGISTER_VARIABLE(PERIODIC_PAIR_INDEX)
    KRATOS_REGISTER_VARIABLE(STATIONARY)
    KRATOS_REGISTER_VARIABLE(ACTIVATION_LEVEL)

    KRATOS_REGISTER_VARIABLE(FRICTION_COEFFICIENT)
    KRATOS_REGISTER_VARIABLE(LAMBDA)
    KRATOS_REGISTER_VARIABLE(MIU)
    KRATOS_REGISTER_VARIABLE(SCALE_FACTOR)
    KRATOS_REGISTER_VARIABLE(NORMAL_CONTACT_STRESS)
    KRATOS_REGISTER_VARIABLE(TANGENTIAL_CONTACT_STRESS)

    KRATOS_REGISTER_VARIABLE(PARTITION_INDEX)
    KRATOS_REGISTER_VARIABLE(TEMPERATURE_OLD_IT)
    KRATOS_REGISTER_VARIABLE(VISCOSITY)
    KRATOS_REGISTER_VARIABLE(ERROR_RATIO)
    KRATOS_REGISTER_VARIABLE(ENERGY_NORM_OVERALL)
    KRATOS_REGISTER_VARIABLE(ERROR_OVERALL)
    KRATOS_REGISTER_VARIABLE(RHS_WATER)
    KRATOS_REGISTER_VARIABLE(RHS_AIR)
    KRATOS_REGISTER_VARIABLE(WEIGHT_FATHER_NODES)
    KRATOS_REGISTER_VARIABLE(INITIAL_PENALTY)
    KRATOS_REGISTER_VARIABLE(DP_EPSILON)
    KRATOS_REGISTER_VARIABLE(DP_ALPHA1)
    KRATOS_REGISTER_VARIABLE(DP_K)
    KRATOS_REGISTER_VARIABLE(INTERNAL_FRICTION_ANGLE)
    KRATOS_REGISTER_VARIABLE(K0)
    KRATOS_REGISTER_VARIABLE(NODAL_VOLUME)

    KRATOS_REGISTER_VARIABLE(WATER_PRESSURE)
    KRATOS_REGISTER_VARIABLE(REACTION_WATER_PRESSURE)
    KRATOS_REGISTER_VARIABLE(WATER_PRESSURE_ACCELERATION)

    KRATOS_REGISTER_VARIABLE(AIR_PRESSURE)
    KRATOS_REGISTER_VARIABLE(REACTION_AIR_PRESSURE)
    KRATOS_REGISTER_VARIABLE(FLAG_VARIABLE)
    KRATOS_REGISTER_VARIABLE(DISTANCE)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(DISTANCE_GRADIENT)

    KRATOS_REGISTER_VARIABLE(LAGRANGE_AIR_PRESSURE)
    KRATOS_REGISTER_VARIABLE(LAGRANGE_WATER_PRESSURE)
    KRATOS_REGISTER_VARIABLE(LAGRANGE_TEMPERATURE)

    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MOMENTUM)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(LAGRANGE_DISPLACEMENT)

    KRATOS_REGISTER_VARIABLE(ELEMENTAL_DISTANCES)
    KRATOS_REGISTER_VARIABLE(ELEMENTAL_EDGE_DISTANCES)
    KRATOS_REGISTER_VARIABLE(ELEMENTAL_EDGE_DISTANCES_EXTRAPOLATED)
    KRATOS_REGISTER_VARIABLE(MATERIAL_PARAMETERS)
    KRATOS_REGISTER_VARIABLE(INTERNAL_VARIABLES)
    KRATOS_REGISTER_VARIABLE(INSITU_STRESS)

    KRATOS_REGISTER_VARIABLE(PENALTY)
    KRATOS_REGISTER_VARIABLE(NORMAL_STRESS)
    KRATOS_REGISTER_VARIABLE(TANGENTIAL_STRESS)
    KRATOS_REGISTER_VARIABLE(STRESSES)
    KRATOS_REGISTER_VARIABLE(STRAIN)

    KRATOS_REGISTER_VARIABLE(NEIGHBOURS_INDICES)

    KRATOS_REGISTER_VARIABLE(INERTIA)

    KRATOS_REGISTER_VARIABLE(NEIGHBOUR_EMBEDDED_FACES)
    KRATOS_REGISTER_VARIABLE(CONVECTION_DIFFUSION_SETTINGS)
    KRATOS_REGISTER_VARIABLE(RADIATION_SETTINGS)

    //--------------- STRUCTURAL Application -------------------//

    //--------------- MULTISCALE Application -------------------//

    KRATOS_REGISTER_VARIABLE(INITIAL_STRAIN)
    KRATOS_REGISTER_VARIABLE(COEFFICIENT_THERMAL_EXPANSION)
    KRATOS_REGISTER_VARIABLE(CHARACTERISTIC_LENGTH_MULTIPLIER)

    //--------------- Incompressible Fluid Application ---------//

    //--------------- ALE Application -------------------//

    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(DETERMINANT)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(ELEMENTSHAPE)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MESH_VELOCITY)
    KRATOS_REGISTER_VARIABLE(AUX_MESH_VAR)

    //--------------- ALE Application -------------------//

    //--------------- Adjoint Fluid Application -------------------//
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS( ADJOINT_VECTOR_1 )
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS( ADJOINT_VECTOR_2 )
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS( ADJOINT_VECTOR_3 )
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS( AUX_ADJOINT_VECTOR_1 )
    KRATOS_REGISTER_VARIABLE(ADJOINT_SCALAR_1 )
    KRATOS_REGISTER_VARIABLE(SCALAR_SENSITIVITY )
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(SHAPE_SENSITIVITY)
    KRATOS_REGISTER_VARIABLE(NORMAL_SENSITIVITY)
    KRATOS_REGISTER_VARIABLE(NUMBER_OF_NEIGHBOUR_ELEMENTS)
    KRATOS_REGISTER_VARIABLE(UPDATE_SENSITIVITIES)
    KRATOS_REGISTER_VARIABLE(ADJOINT_EXTENSIONS)
    KRATOS_REGISTER_VARIABLE(NORMAL_SHAPE_DERIVATIVE)


    //--------------- Meshing ApplicationApplication -------------------//

    KRATOS_REGISTER_VARIABLE(NODAL_ERROR)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(NODAL_ERROR_COMPONENTS)
    KRATOS_REGISTER_VARIABLE(ELEMENT_ERROR )
    KRATOS_REGISTER_VARIABLE(ELEMENT_H )
    KRATOS_REGISTER_VARIABLE(RECOVERED_STRESS )
    KRATOS_REGISTER_VARIABLE(ERROR_INTEGRATION_POINT )
    KRATOS_REGISTER_VARIABLE(CONTACT_PRESSURE )

    //--------------- PFEM fluids Application -------------------//

    KRATOS_REGISTER_VARIABLE(NODAL_AREA)

    KRATOS_REGISTER_VARIABLE(BULK_MODULUS)
    KRATOS_REGISTER_VARIABLE(SATURATION)
    KRATOS_REGISTER_VARIABLE(DENSITY_WATER)
    KRATOS_REGISTER_VARIABLE(VISCOSITY_WATER)
    KRATOS_REGISTER_VARIABLE(DENSITY_AIR)
    KRATOS_REGISTER_VARIABLE(VISCOSITY_AIR)
    KRATOS_REGISTER_VARIABLE(POROSITY)
    KRATOS_REGISTER_VARIABLE(DIAMETER)
    KRATOS_REGISTER_VARIABLE(LIN_DARCY_COEF)
    KRATOS_REGISTER_VARIABLE(NONLIN_DARCY_COEF)

    KRATOS_REGISTER_VARIABLE(AIR_ENTRY_VALUE)
    KRATOS_REGISTER_VARIABLE(FIRST_SATURATION_PARAM)
    KRATOS_REGISTER_VARIABLE(SECOND_SATURATION_PARAM)
    KRATOS_REGISTER_VARIABLE(PERMEABILITY_WATER)
    KRATOS_REGISTER_VARIABLE(PERMEABILITY_AIR)
    KRATOS_REGISTER_VARIABLE(BULK_AIR)

    KRATOS_REGISTER_VARIABLE(TEMP_CONV_PROJ)
    KRATOS_REGISTER_VARIABLE(CONVECTION_COEFFICIENT)

    KRATOS_REGISTER_VARIABLE(SCALE)

    KRATOS_REGISTER_VARIABLE(SOUND_VELOCITY)
    KRATOS_REGISTER_VARIABLE(AIR_SOUND_VELOCITY)
    KRATOS_REGISTER_VARIABLE(WATER_SOUND_VELOCITY)

    KRATOS_REGISTER_VARIABLE(NODAL_MASS)
    KRATOS_REGISTER_VARIABLE(NODAL_INERTIA_TENSOR)
    KRATOS_REGISTER_VARIABLE(AUX_INDEX)
    KRATOS_REGISTER_VARIABLE(EXTERNAL_PRESSURE)
    KRATOS_REGISTER_VARIABLE(BDF_COEFFICIENTS)
    KRATOS_REGISTER_VARIABLE(VELOCITY_PERIOD)
    KRATOS_REGISTER_VARIABLE(ANGULAR_VELOCITY_PERIOD)
    KRATOS_REGISTER_VARIABLE(IDENTIFIER)

    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(GRAVITY)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(NORMAL_TO_WALL)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(STRUCTURE_VELOCITY)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(DRAG_FORCE)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(EMBEDDED_VELOCITY)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(FRACT_VEL)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(ROTATION_CENTER)

    //--------------- PFEM fluids Application -------------------//

    //--------------- Other Applications -------------------//

    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(SEEPAGE_DRAG)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MASS)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(RHS)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(FORCE_CM)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(MOMENTUM_CM)

    //       KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS( xi_c )
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(DIRECTION)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(Y)

    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(LOCAL_AXIS_1)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(LOCAL_AXIS_2)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(LOCAL_AXIS_3)

    KRATOS_REGISTER_VARIABLE(ARRHENIUS)
    KRATOS_REGISTER_VARIABLE(ARRHENIUSAUX)
    KRATOS_REGISTER_VARIABLE(ARRHENIUSAUX_)
    KRATOS_REGISTER_VARIABLE(PRESSUREAUX)
    KRATOS_REGISTER_VARIABLE(NODAL_MAUX)
    KRATOS_REGISTER_VARIABLE(NODAL_PAUX)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(NODAL_VAUX)
    KRATOS_REGISTER_VARIABLE(HEAT_FLUX)
    KRATOS_REGISTER_VARIABLE(REACTION_FLUX)
    KRATOS_REGISTER_VARIABLE(TC)
    KRATOS_REGISTER_VARIABLE(CONDUCTIVITY)
    KRATOS_REGISTER_VARIABLE(SPECIFIC_HEAT)
    KRATOS_REGISTER_VARIABLE(MATERIAL_VARIABLE)
    KRATOS_REGISTER_VARIABLE(FUEL)
    KRATOS_REGISTER_VARIABLE(YO)
    KRATOS_REGISTER_VARIABLE(YF)
    KRATOS_REGISTER_VARIABLE(YI)
    KRATOS_REGISTER_VARIABLE(Y1)
    KRATOS_REGISTER_VARIABLE(Y2)
    KRATOS_REGISTER_VARIABLE(YP)

    KRATOS_REGISTER_VARIABLE(ABSORPTION_COEFFICIENT)
    KRATOS_REGISTER_VARIABLE(STEFAN_BOLTZMANN_CONSTANT)

    KRATOS_REGISTER_VARIABLE(EMISSIVITY)
    KRATOS_REGISTER_VARIABLE(ENTHALPY)
    KRATOS_REGISTER_VARIABLE(MIXTURE_FRACTION)

    KRATOS_REGISTER_VARIABLE(YCH4)
    KRATOS_REGISTER_VARIABLE(YO2)
    KRATOS_REGISTER_VARIABLE(YCO2)
    KRATOS_REGISTER_VARIABLE(YH2O)
    KRATOS_REGISTER_VARIABLE(YN2)

    KRATOS_REGISTER_VARIABLE(WET_VOLUME)
    KRATOS_REGISTER_VARIABLE(CUTTED_AREA)
    KRATOS_REGISTER_VARIABLE(NET_INPUT_MATERIAL)

    KRATOS_REGISTER_VARIABLE(INCIDENT_RADIATION_FUNCTION)

    KRATOS_REGISTER_VARIABLE(SWITCH_TEMPERATURE)
    KRATOS_REGISTER_VARIABLE(NODAL_SWITCH)

    //--------------- OTHER Applications -------------------//

    //--------------- XFEM Set Application -------------------//

    KRATOS_REGISTER_VARIABLE(CRACK_OPENING)
    KRATOS_REGISTER_VARIABLE(CRACK_TRANSLATION)

    //--------------- XFEM Application -------------------//

    //--------------- Level Set Application -------------------//

    KRATOS_REGISTER_VARIABLE(REFINEMENT_LEVEL)
    KRATOS_REGISTER_VARIABLE(MIN_DT)
    KRATOS_REGISTER_VARIABLE(MAX_DT)
    KRATOS_REGISTER_VARIABLE(VEL_ART_VISC)
    KRATOS_REGISTER_VARIABLE(PR_ART_VISC)

    //--------------- Level Set Application -------------------//

    KRATOS_REGISTER_VARIABLE(PERIODIC_VARIABLES)

    //--------------- Vulcan Application -------------------//
    KRATOS_REGISTER_VARIABLE(LATENT_HEAT)
    KRATOS_REGISTER_VARIABLE(AMBIENT_TEMPERATURE)

    KRATOS_REGISTER_VARIABLE(ENRICHED_PRESSURES)

    KRATOS_REGISTER_VARIABLE(SEARCH_RADIUS)
    KRATOS_REGISTER_VARIABLE(ORIENTATION)

    KRATOS_REGISTER_VARIABLE(INTEGRATION_WEIGHT)
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(INTEGRATION_COORDINATES)

    KRATOS_REGISTER_VARIABLE(TABLE_UTILITY)


    //--------------- Geometry Variables -------------------//
    KRATOS_REGISTER_3D_VARIABLE_WITH_COMPONENTS(PARAMETER_2D_COORDINATES)

    KRATOS_REGISTER_VARIABLE(SPANS_LOCAL_SPACE)
}
}  // namespace Kratos.
