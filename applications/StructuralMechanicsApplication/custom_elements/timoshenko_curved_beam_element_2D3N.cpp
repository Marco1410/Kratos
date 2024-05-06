// KRATOS  ___|  |                   |                   |
//       \___ \  __|  __| |   |  __| __| |   |  __| _` | |
//             | |   |    |   | (    |   |   | |   (   | |
//       _____/ \__|_|   \__,_|\___|\__|\__,_|_|  \__,_|_| MECHANICS
//
//  License:         BSD License
//                   license: StructuralMechanicsApplication/license.txt
//
//  Main authors:  Alejandro Cornejo
//
//

// System includes

// External includes

// Project includes

// Application includes
#include "custom_elements/timoshenko_curved_beam_element_2D3N.h"
#include "custom_utilities/constitutive_law_utilities.h"
#include "structural_mechanics_application_variables.h"

namespace Kratos
{

void LinearTimoshenkoCurvedBeamElement2D3N::Initialize(const ProcessInfo& rCurrentProcessInfo)
{
    KRATOS_TRY

    // Initialization should not be done again in a restart!
    if (!rCurrentProcessInfo[IS_RESTARTED]) {
        if (this->UseGeometryIntegrationMethod()) {
            if (GetProperties().Has(INTEGRATION_ORDER) ) {
                mThisIntegrationMethod = static_cast<GeometryData::IntegrationMethod>(GetProperties()[INTEGRATION_ORDER] - 1);
            } else {
                mThisIntegrationMethod = GeometryData::IntegrationMethod::GI_GAUSS_5;
            }
        }

        const auto& r_integration_points = this->IntegrationPoints(mThisIntegrationMethod);

        // Constitutive Law initialisation
        if (mConstitutiveLawVector.size() != r_integration_points.size())
            mConstitutiveLawVector.resize(r_integration_points.size());
        InitializeMaterial();
    }
    KRATOS_CATCH("")
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::InitializeMaterial()
{
    KRATOS_TRY

    if (GetProperties()[CONSTITUTIVE_LAW] != nullptr) {
        const auto& r_geometry   = GetGeometry();
        const auto& r_properties = GetProperties();
        auto N_values            = Vector();
        for (IndexType point_number = 0; point_number < mConstitutiveLawVector.size(); ++point_number) {
            mConstitutiveLawVector[point_number] = GetProperties()[CONSTITUTIVE_LAW]->Clone();
            mConstitutiveLawVector[point_number]->InitializeMaterial(r_properties, r_geometry, N_values);
        }
    } else
        KRATOS_ERROR << "A constitutive law needs to be specified for the element with ID " << this->Id() << std::endl;

    KRATOS_CATCH("");
}

/***********************************************************************************/
/***********************************************************************************/

Element::Pointer LinearTimoshenkoCurvedBeamElement2D3N::Clone(
    IndexType NewId,
    NodesArrayType const& rThisNodes
    ) const
{
    KRATOS_TRY

    LinearTimoshenkoCurvedBeamElement2D3N::Pointer p_new_elem = Kratos::make_intrusive<LinearTimoshenkoCurvedBeamElement2D3N>
        (NewId, GetGeometry().Create(rThisNodes), pGetProperties());
    p_new_elem->SetData(this->GetData());
    p_new_elem->Set(Flags(*this));

    // Currently selected integration methods
    p_new_elem->SetIntegrationMethod(mThisIntegrationMethod);

    // The vector containing the constitutive laws
    p_new_elem->SetConstitutiveLawVector(mConstitutiveLawVector);

    return p_new_elem;

    KRATOS_CATCH("");
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::EquationIdVector(
    EquationIdVectorType& rResult,
    const ProcessInfo& rCurrentProcessInfo
    ) const
{
    const auto& r_geometry = this->GetGeometry();
    const SizeType number_of_nodes = r_geometry.size();
    const SizeType dofs_per_node = DoFperNode; // u, v, theta

    IndexType local_index = 0;

    if (rResult.size() != dofs_per_node * number_of_nodes)
        rResult.resize(dofs_per_node * number_of_nodes, false);

    const IndexType xpos    = this->GetGeometry()[0].GetDofPosition(DISPLACEMENT_X);
    const IndexType rot_pos = this->GetGeometry()[0].GetDofPosition(ROTATION_Z);

    for (IndexType i = 0; i < number_of_nodes; ++i) {
        rResult[local_index++] = r_geometry[i].GetDof(DISPLACEMENT_X, xpos    ).EquationId();
        rResult[local_index++] = r_geometry[i].GetDof(DISPLACEMENT_Y, xpos + 1).EquationId();
        rResult[local_index++] = r_geometry[i].GetDof(ROTATION_Z    , rot_pos ).EquationId();
    }
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::GetDofList(
    DofsVectorType& rElementalDofList,
    const ProcessInfo& rCurrentProcessInfo
    ) const
{
    KRATOS_TRY;

    const auto& r_geom = GetGeometry();
    const SizeType number_of_nodes = r_geom.size();
    const SizeType dofs_per_node = DoFperNode; // u, v, theta
    rElementalDofList.resize(dofs_per_node * number_of_nodes);

    for (IndexType i = 0; i < number_of_nodes; ++i) {
        const SizeType index = i * dofs_per_node;
        rElementalDofList[index]     = r_geom[i].pGetDof(DISPLACEMENT_X);
        rElementalDofList[index + 1] = r_geom[i].pGetDof(DISPLACEMENT_Y);
        rElementalDofList[index + 2] = r_geom[i].pGetDof(ROTATION_Z    );
    }
    KRATOS_CATCH("")
}

/***********************************************************************************/
/***********************************************************************************/

const double LinearTimoshenkoCurvedBeamElement2D3N::GetJacobian(const double xi)
{
    GlobalSizeVector r_dN_dxi;
    GetLocalFirstDerivativesNu0ShapeFunctionsValues(r_dN_dxi, xi);
    const auto r_geom = GetGeometry();

    double dx_dxi = 0.0;
    double dy_dxi = 0.0;

    for (IndexType i = 0; i < NumberOfNodes; ++i) {
        const IndexType u_coord = DoFperNode * i;
        const auto &r_coords_node = r_geom[i].Coordinates();
        dx_dxi += r_coords_node[0] * r_dN_dxi[u_coord];
        dy_dxi += r_coords_node[1] * r_dN_dxi[u_coord];
    }
    return std::sqrt(std::pow(dx_dxi, 2) + std::pow(dy_dxi, 2));
}

/***********************************************************************************/
/***********************************************************************************/

const double LinearTimoshenkoCurvedBeamElement2D3N::GetGeometryCurvature(
    const double J,
    const double xi
    )
{
    GlobalSizeVector r_dN_dxi, r_d2N_dxi2;
    GetLocalFirstDerivativesNu0ShapeFunctionsValues (r_dN_dxi,   xi);
    GetLocalSecondDerivativesNu0ShapeFunctionsValues(r_d2N_dxi2, xi);
    const auto r_geom = GetGeometry();

    double dx_dxi = 0.0;
    double dy_dxi = 0.0;

    double d2x_dxi2 = 0.0;
    double d2y_dxi2 = 0.0;

    for (IndexType i = 0; i < NumberOfNodes; ++i) {
        const IndexType u_coord = DoFperNode * i;
        const auto &r_coords_node = r_geom[i].Coordinates();
        dx_dxi += r_coords_node[0] * r_dN_dxi[u_coord];
        dy_dxi += r_coords_node[1] * r_dN_dxi[u_coord];

        d2x_dxi2 += r_coords_node[0] * r_d2N_dxi2[u_coord];
        d2y_dxi2 += r_coords_node[1] * r_d2N_dxi2[u_coord];
    }
    return (dx_dxi * d2y_dxi2 - dy_dxi * d2x_dxi2) / std::pow(J, 3);
}

/***********************************************************************************/
/***********************************************************************************/

const double LinearTimoshenkoCurvedBeamElement2D3N::GetBendingShearStiffnessRatio()
{
    const auto &r_props = GetProperties();
    const double E  = r_props[YOUNG_MODULUS];
    const double I  = r_props[I33];
    const double As = r_props[AREA_EFFECTIVE_Y];
    const double G  = ConstitutiveLawUtilities<3>::CalculateShearModulus(r_props);

    if (As == 0.0) // If effective area is null -> Euler Bernouilli case
        return 0.0;
    else
        return E * I / (G * As);
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::GetShapeFunctionsValues(
    GlobalSizeVector& rN,
    const double J,
    const double xi
    )
{
    // Nodal values of the Jacobian
    const double J1 = GetJacobian(-1.0);
    const double J2 = GetJacobian( 1.0);
    const double J3 = GetJacobian( 0.0);

    // Nodal values of the geometric curvatures
    const double k01 = GetGeometryCurvature(J, -1.0);
    const double k02 = GetGeometryCurvature(J,  1.0);
    const double k03 = GetGeometryCurvature(J,  0.0);

    const double k_s = GetBendingShearStiffnessRatio();

    const double xi_pow_2 = std::pow(xi, 2);
    const double xi_pow_3 = std::pow(xi, 3);
    const double xi_pow_4 = std::pow(xi, 4);
    const double xi_pow_5 = std::pow(xi, 5);

    const double J1_pow_2 = std::pow(J1, 2);
    const double J1_pow_3 = std::pow(J1, 3);

    const double J2_pow_2 = std::pow(J2, 2);
    const double J2_pow_3 = std::pow(J2, 3);

    const double J3_pow_2 = std::pow(J3, 2);
    const double J3_pow_3 = std::pow(J3, 3);

    const double k_s_pow_2 = std::pow(k_s, 2);
    const double k_s_pow_3 = std::pow(k_s, 3);

    const double Nu1 = (-J1_pow_3 * J2_pow_2 * J3_pow_2 * k01 * xi_pow_5 + J1_pow_3 * J2_pow_2 * J3_pow_2 * k01 * xi_pow_4 + J1_pow_3 * J2_pow_2 * J3_pow_2 * k01 * xi_pow_3 - J1_pow_3 * J2_pow_2 * J3_pow_2 * k01 * xi_pow_2 + 6.0 * J1_pow_3 * J2_pow_2 * k01 * k_s * xi_pow_5 - 12.0 * J1_pow_3 * J2_pow_2 * k01 * k_s * xi_pow_4 + 12.0 * J1_pow_3 * J2_pow_2 * k01 * k_s * xi_pow_2 - 6.0 * J1_pow_3 * J2_pow_2 * k01 * k_s * xi - 12.0 * J1_pow_3 * J3_pow_2 * k01 * k_s * xi_pow_5 + 27.0 * J1_pow_3 * J3_pow_2 * k01 * k_s * xi_pow_4 + 12.0 * J1_pow_3 * J3_pow_2 * k01 * k_s * xi_pow_3 - 27.0 * J1_pow_3 * J3_pow_2 * k01 * k_s * xi_pow_2 + 72.0 * J1_pow_3 * k01 * k_s_pow_2 * xi_pow_5 - 180.0 * J1_pow_3 * k01 * k_s_pow_2 * xi_pow_4 + 180.0 * J1_pow_3 * k01 * k_s_pow_2 * xi_pow_2 - 72.0 * J1_pow_3 * k01 * k_s_pow_2 * xi) / (4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 48.0 * J1_pow_2 * J2_pow_2 * k_s + 78.0 * J1_pow_2 * J3_pow_2 * k_s - 648.0 * J1_pow_2 * k_s_pow_2 + 78.0 * J2_pow_2 * J3_pow_2 * k_s - 648.0 * J2_pow_2 * k_s_pow_2 + 1296.0 * J3_pow_2 * k_s_pow_2 - 8640.0 * k_s_pow_2 * k_s);
    const double Nv1 = (6.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_5 - 4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_4 - 10.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_3 + 8.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_2 - 12.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_5 + 48.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_4 - 96.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_2 + 60.0 * J1_pow_2 * J2_pow_2 * k_s * xi + 66.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_5 - 129.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_4 - 144.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_3 + 207.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_2 - 216.0 * J1_pow_2 * k_s_pow_2 * xi_pow_5 + 540.0 * J1_pow_2 * k_s_pow_2 * xi_pow_4 - 1188.0 * J1_pow_2 * k_s_pow_2 * xi_pow_2 + 864.0 * J1_pow_2 * k_s_pow_2 * xi + 18.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_5 + 21.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_4 - 96.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_3 + 57.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_2 + 72.0 * J2_pow_2 * k_s_pow_2 * xi_pow_5 + 180.0 * J2_pow_2 * k_s_pow_2 * xi_pow_4 - 828.0 * J2_pow_2 * k_s_pow_2 * xi_pow_2 + 576.0 * J2_pow_2 * k_s_pow_2 * xi + 144.0 * J3_pow_2 * k_s_pow_2 * xi_pow_5 - 1440.0 * J3_pow_2 * k_s_pow_2 * xi_pow_3 + 1296.0 * J3_pow_2 * k_s_pow_2 * xi_pow_2 - 8640.0 * k_s_pow_2 * xi_pow_2 + 8640.0 * k_s_pow_3 * xi) / (8.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 96.0 * J1_pow_2 * J2_pow_2 * k_s + 156.0 * J1_pow_2 * J3_pow_2 * k_s - 1296.0 * J1_pow_2 * k_s_pow_2 + 156.0 * J2_pow_2 * J3_pow_2 * k_s - 1296.0 * J2_pow_2 * k_s_pow_2 + 2592.0 * J3_pow_2 * k_s_pow_2 - 17280.0 * k_s_pow_3);
    const double Nt1 = (J1_pow_3 * J2_pow_2 * J3_pow_2 * xi_pow_5 - J1_pow_3 * J2_pow_2 * J3_pow_2 * xi_pow_4 - J1_pow_3 * J2_pow_2 * J3_pow_2 * xi_pow_3 + J1_pow_3 * J2_pow_2 * J3_pow_2 * xi_pow_2 - 6.0 * J1_pow_3 * J2_pow_2 * k_s * xi_pow_5 + 12.0 * J1_pow_3 * J2_pow_2 * k_s * xi_pow_4 - 12.0 * J1_pow_3 * J2_pow_2 * k_s * xi_pow_2 + 6.0 * J1_pow_3 * J2_pow_2 * k_s * xi + 12.0 * J1_pow_3 * J3_pow_2 * k_s * xi_pow_5 - 27.0 * J1_pow_3 * J3_pow_2 * k_s * xi_pow_4 - 12.0 * J1_pow_3 * J3_pow_2 * k_s * xi_pow_3 + 27.0 * J1_pow_3 * J3_pow_2 * k_s * xi_pow_2 - 72.0 * J1_pow_3 * k_s_pow_2 * xi_pow_5 + 180.0 * J1_pow_3 * k_s_pow_2 * xi_pow_4 - 180.0 * J1_pow_3 * k_s_pow_2 * xi_pow_2 + 72.0 * J1_pow_3 * k_s_pow_2 * xi) / (4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 48.0 * J1_pow_2 * J2_pow_2 * k_s + 78.0 * J1_pow_2 * J3_pow_2 * k_s - 648.0 * J1_pow_2 * k_s_pow_2 + 78.0 * J2_pow_2 * J3_pow_2 * k_s - 648.0 * J2_pow_2 * k_s_pow_2 + 1296.0 * J3_pow_2 * k_s_pow_2 - 8640.0 * k_s_pow_2);
    const double Nu2 = (-J1_pow_2 * J2_pow_3 * J3_pow_2 * k02 * xi_pow_5 - J1_pow_2 * J2_pow_3 * J3_pow_2 * k02 * xi_pow_4 + J1_pow_2 * J2_pow_3 * J3_pow_2 * k02 * xi_pow_3 + J1_pow_2 * J2_pow_3 * J3_pow_2 * k02 * xi_pow_2 + 6.0 * J1_pow_2 * J2_pow_3 * k02 * k_s * xi_pow_5 + 12.0 * J1_pow_2 * J2_pow_3 * k02 * k_s * xi_pow_4 - 12.0 * J1_pow_2 * J2_pow_3 * k02 * k_s * xi_pow_2 - 6.0 * J1_pow_2 * J2_pow_3 * k02 * k_s * xi - 12.0 * J2_pow_3 * J3_pow_2 * k02 * k_s * xi_pow_5 - 27.0 * J2_pow_3 * J3_pow_2 * k02 * k_s * xi_pow_4 + 12.0 * J2_pow_3 * J3_pow_2 * k02 * k_s * xi_pow_3 + 27.0 * J2_pow_3 * J3_pow_2 * k02 * k_s * xi_pow_2 + 72.0 * J2_pow_3 * k02 * k_s_pow_2 * xi_pow_5 + 180.0 * J2_pow_3 * k02 * k_s_pow_2 * xi_pow_4 - 180.0 * J2_pow_3 * k02 * k_s_pow_2 * xi_pow_2 - 72.0 * J2_pow_3 * k02 * k_s_pow_2 * xi) / (4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 48.0 * J1_pow_2 * J2_pow_2 * k_s + 78.0 * J1_pow_2 * J3_pow_2 * k_s - 648.0 * J1_pow_2 * k_s_pow_2 + 78.0 * J2_pow_2 * J3_pow_2 * k_s - 648.0 * J2_pow_2 * k_s_pow_2 + 1296.0 * J3_pow_2 * k_s_pow_2 - 8640.0 * k_s_pow_3);
    const double Nv2 = (-6.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_5 - 4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_4 + 10.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_3 + 8.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_2 + 12.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_5 + 48.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_4 - 96.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_2 - 60.0 * J1_pow_2 * J2_pow_2 * k_s * xi - 18.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_5 + 21.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_4 + 96.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_3 + 57.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_2 - 72.0 * J1_pow_2 * k_s_pow_2 * xi_pow_5 + 180.0 * J1_pow_2 * k_s_pow_2 * xi_pow_4 - 828.0 * J1_pow_2 * k_s_pow_2 * xi_pow_2 - 576.0 * J1_pow_2 * k_s_pow_2 * xi - 66.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_5 - 129.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_4 + 144.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_3 + 207.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_2 + 216.0 * J2_pow_2 * k_s_pow_2 * xi_pow_5 + 540.0 * J2_pow_2 * k_s_pow_2 * xi_pow_4 - 1188.0 * J2_pow_2 * k_s_pow_2 * xi_pow_2 - 864.0 * J2_pow_2 * k_s_pow_2 * xi - 144.0 * J3_pow_2 * k_s_pow_2 * xi_pow_5 + 1440.0 * J3_pow_2 * k_s_pow_2 * xi_pow_3 + 1296.0 * J3_pow_2 * k_s_pow_2 * xi_pow_2 - 8640.0 * k_s_pow_3 * xi_pow_2 - 8640.0 * k_s_pow_3 * xi) / (8.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 96.0 * J1_pow_2 * J2_pow_2 * k_s + 156.0 * J1_pow_2 * J3_pow_2 * k_s - 1296.0 * J1_pow_2 * k_s_pow_2 + 156.0 * J2_pow_2 * J3_pow_2 * k_s - 1296.0 * J2_pow_2 * k_s_pow_2 + 2592.0 * J3_pow_2 * k_s_pow_2 - 17280.0 * k_s_pow_3);
    const double Nt2 = (J1_pow_2 * J2_pow_3 * J3_pow_2 * xi_pow_5 + J1_pow_2 * J2_pow_3 * J3_pow_2 * xi_pow_4 - J1_pow_2 * J2_pow_3 * J3_pow_2 * xi_pow_3 - J1_pow_2 * J2_pow_3 * J3_pow_2 * xi_pow_2 - 6.0 * J1_pow_2 * J2_pow_3 * k_s * xi_pow_5 - 12.0 * J1_pow_2 * J2_pow_3 * k_s * xi_pow_4 + 12.0 * J1_pow_2 * J2_pow_3 * k_s * xi_pow_2 + 6.0 * J1_pow_2 * J2_pow_3 * k_s * xi + 12.0 * J2_pow_3 * J3_pow_2 * k_s * xi_pow_5 + 27.0 * J2_pow_3 * J3_pow_2 * k_s * xi_pow_4 - 12.0 * J2_pow_3 * J3_pow_2 * k_s * xi_pow_3 - 27.0 * J2_pow_3 * J3_pow_2 * k_s * xi_pow_2 - 72.0 * J2_pow_3 * k_s_pow_2 * xi_pow_5 - 180.0 * J2_pow_3 * k_s_pow_2 * xi_pow_4 + 180.0 * J2_pow_3 * k_s_pow_2 * xi_pow_2 + 72.0 * J2_pow_3 * k_s_pow_2 * xi) / (4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 48.0 * J1_pow_2 * J2_pow_2 * k_s + 78.0 * J1_pow_2 * J3_pow_2 * k_s - 648.0 * J1_pow_2 * k_s_pow_2 + 78.0 * J2_pow_2 * J3_pow_2 * k_s - 648.0 * J2_pow_2 * k_s_pow_2 + 1296.0 * J3_pow_2 * k_s_pow_2 - 8640.0 * k_s_pow_3);
    const double Nu3 = (-2.0 * J1_pow_2 * J2_pow_2 * J3_pow_3 * k03 * xi_pow_5 + 4.0 * J1_pow_2 * J2_pow_2 * J3_pow_3 * k03 * xi_pow_3 - 2.0 * J1_pow_2 * J2_pow_2 * J3_pow_3 * k03 * xi - 15.0 * J1_pow_2 * J3_pow_3 * k03 * k_s * xi_pow_5 + 24.0 * J1_pow_2 * J3_pow_3 * k03 * k_s * xi_pow_4 + 54.0 * J1_pow_2 * J3_pow_3 * k03 * k_s * xi_pow_3 - 24.0 * J1_pow_2 * J3_pow_3 * k03 * k_s * xi_pow_2 - 39.0 * J1_pow_2 * J3_pow_3 * k03 * k_s * xi - 15.0 * J2_pow_2 * J3_pow_3 * k03 * k_s * xi_pow_5 - 24.0 * J2_pow_2 * J3_pow_3 * k03 * k_s * xi_pow_4 + 54.0 * J2_pow_2 * J3_pow_3 * k03 * k_s * xi_pow_3 + 24.0 * J2_pow_2 * J3_pow_3 * k03 * k_s * xi_pow_2 - 39.0 * J2_pow_2 * J3_pow_3 * k03 * k_s * xi - 72.0 * J3_pow_3 * k03 * k_s_pow_2 * xi_pow_5 + 720.0 * J3_pow_3 * k03 * k_s_pow_2 * xi_pow_3 - 648.0 * J3_pow_3 * k03 * k_s_pow_2 * xi) / (2.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 24.0 * J1_pow_2 * J2_pow_2 * k_s + 39.0 * J1_pow_2 * J3_pow_2 * k_s - 324.0 * J1_pow_2 * k_s_pow_2 + 39.0 * J2_pow_2 * J3_pow_2 * k_s - 324.0 * J2_pow_2 * k_s_pow_2 + 648.0 * J3_pow_2 * k_s_pow_2 - 4320.0 * k_s_pow_3);
    const double Nv3 = (2.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_4 - 4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_2 + 2.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 24.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_4 + 48.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_2 - 24.0 * J1_pow_2 * J2_pow_2 * k_s - 12.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_5 + 27.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_4 + 12.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_3 - 66.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_2 + 39.0 * J1_pow_2 * J3_pow_2 * k_s + 72.0 * J1_pow_2 * k_s_pow_2 * xi_pow_5 - 180.0 * J1_pow_2 * k_s_pow_2 * xi_pow_4 + 504.0 * J1_pow_2 * k_s_pow_2 * xi_pow_2 - 72.0 * J1_pow_2 * k_s_pow_2 * xi - 324.0 * J1_pow_2 * k_s_pow_2 + 12.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_5 + 27.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_4 - 12.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_3 - 66.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_2 + 39.0 * J2_pow_2 * J3_pow_2 * k_s - 72.0 * J2_pow_2 * k_s_pow_2 * xi_pow_5 - 180.0 * J2_pow_2 * k_s_pow_2 * xi_pow_4 + 504.0 * J2_pow_2 * k_s_pow_2 * xi_pow_2 + 72.0 * J2_pow_2 * k_s_pow_2 * xi - 324.0 * J2_pow_2 * k_s_pow_2 - 648.0 * J3_pow_2 * k_s_pow_2 * xi_pow_2 + 648.0 * J3_pow_2 * k_s_pow_2 + 4320.0 * k_s_pow_3 * xi_pow_2 - 4320.0 * k_s_pow_3) / (2.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 24.0 * J1_pow_2 * J2_pow_2 * k_s + 39.0 * J1_pow_2 * J3_pow_2 * k_s - 324.0 * J1_pow_2 * k_s_pow_2 + 39.0 * J2_pow_2 * J3_pow_2 * k_s - 324.0 * J2_pow_2 * k_s_pow_2 + 648.0 * J3_pow_2 * k_s_pow_2 - 4320.0 * k_s_pow_3);
    const double Nt3 = (2.0 * J1_pow_2 * J2_pow_2 * J3_pow_3 * xi_pow_5 - 4.0 * J1_pow_2 * J2_pow_2 * J3_pow_3 * xi_pow_3 + 2.0 * J1_pow_2 * J2_pow_2 * J3_pow_3 * xi + 15.0 * J1_pow_2 * J3_pow_3 * k_s * xi_pow_5 - 24.0 * J1_pow_2 * J3_pow_3 * k_s * xi_pow_4 - 54.0 * J1_pow_2 * J3_pow_3 * k_s * xi_pow_3 + 24.0 * J1_pow_2 * J3_pow_3 * k_s * xi_pow_2 + 39.0 * J1_pow_2 * J3_pow_3 * k_s * xi + 15.0 * J2_pow_2 * J3_pow_3 * k_s * xi_pow_5 + 24.0 * J2_pow_2 * J3_pow_3 * k_s * xi_pow_4 - 54.0 * J2_pow_2 * J3_pow_3 * k_s * xi_pow_3 - 24.0 * J2_pow_2 * J3_pow_3 * k_s * xi_pow_2 + 39.0 * J2_pow_2 * J3_pow_3 * k_s * xi + 72.0 * J3_pow_3 * k_s_pow_2 * xi_pow_5 - 720.0 * J3_pow_3 * k_s_pow_2 * xi_pow_3 + 648.0 * J3_pow_3 * k_s_pow_2 * xi) / (2.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 24.0 * J1_pow_2 * J2_pow_2 * k_s + 39.0 * J1_pow_2 * J3_pow_2 * k_s - 324.0 * J1_pow_2 * k_s_pow_2 + 39.0 * J2_pow_2 * J3_pow_2 * k_s - 324.0 * J2_pow_2 * k_s_pow_2 + 648.0 * J3_pow_2 * k_s_pow_2 - 4320.0 * k_s_pow_3);

    rN[0] = Nu1;
    rN[1] = Nv1;
    rN[2] = Nt1;

    rN[3] = Nu2;
    rN[4] = Nv2;
    rN[5] = Nt2;

    rN[6] = Nu3;
    rN[7] = Nv3;
    rN[8] = Nt3;
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::GetFirstDerivativesShapeFunctionsValues(
    GlobalSizeVector& rdN,
    const double J,
    const double xi
    )
{
    // Nodal values of the Jacobian
    const double J1 = GetJacobian(-1.0);
    const double J2 = GetJacobian( 1.0);
    const double J3 = GetJacobian( 0.0);

    // Nodal values of the geometric curvatures
    const double k01 = GetGeometryCurvature(J, -1.0);
    const double k02 = GetGeometryCurvature(J,  1.0);
    const double k03 = GetGeometryCurvature(J,  0.0);

    const double k_s = GetBendingShearStiffnessRatio();

    const double xi_pow_2 = std::pow(xi, 2);
    const double xi_pow_3 = std::pow(xi, 3);
    const double xi_pow_4 = std::pow(xi, 4);

    const double J1_pow_2 = std::pow(J1, 2);
    const double J1_pow_3 = std::pow(J1, 3);

    const double J2_pow_2 = std::pow(J2, 2);
    const double J2_pow_3 = std::pow(J2, 3);

    const double J3_pow_2 = std::pow(J3, 2);
    const double J3_pow_3 = std::pow(J3, 3);

    const double k_s_pow_2 = std::pow(k_s, 2);
    const double k_s_pow_3 = std::pow(k_s, 3);

    rdN[0] = (-5.0 * J1_pow_3 * J2_pow_2 * J3_pow_2 * k01 * xi_pow_4 + 4.0 * J1_pow_3 * J2_pow_2 * J3_pow_2 * k01 * xi_pow_3 + 3 * J1_pow_3 * J2_pow_2 * J3_pow_2 * k01 * xi_pow_2 - 2 * J1_pow_3 * J2_pow_2 * J3_pow_2 * k01 * xi + 30.0 * J1_pow_3 * J2_pow_2 * k01 * k_s * xi_pow_4 - 48.0 * J1_pow_3 * J2_pow_2 * k01 * k_s * xi_pow_3 + 24.0 * J1_pow_3 * J2_pow_2 * k01 * k_s * xi - 6.0 * J1_pow_3 * J2_pow_2 * k01 * k_s - 60.0 * J1_pow_3 * J3_pow_2 * k01 * k_s * xi_pow_4 + 108.0 * J1_pow_3 * J3_pow_2 * k01 * k_s * xi_pow_3 + 36.0 * J1_pow_3 * J3_pow_2 * k01 * k_s * xi_pow_2 - 54.0 * J1_pow_3 * J3_pow_2 * k01 * k_s * xi + 360.0 * J1_pow_3 * k01 * k_s_pow_2 * xi_pow_4 - 720.0 * J1_pow_3 * k01 * k_s_pow_2 * xi_pow_3 + 360.0 * J1_pow_3 * k01 * k_s_pow_2 * xi - 72.0 * J1_pow_3 * k01 * k_s_pow_2) / (4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 48.0 * J1_pow_2 * J2_pow_2 * k_s + 78.0 * J1_pow_2 * J3_pow_2 * k_s - 648.0 * J1_pow_2 * k_s_pow_2 + 78.0 * J2_pow_2 * J3_pow_2 * k_s - 648.0 * J2_pow_2 * k_s_pow_2 + 1296.0 * J3_pow_2 * k_s_pow_2 - 8640.0 * k_s_pow_2);
    rdN[1] = (30.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_4 - 16.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_3 - 30.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_2 + 16.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi - 60.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_4 + 192.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_3 - 192.0 * J1_pow_2 * J2_pow_2 * k_s * xi + 60.0 * J1_pow_2 * J2_pow_2 * k_s + 330.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_4 - 516.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_3 - 432.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_2 + 414.0 * J1_pow_2 * J3_pow_2 * k_s * xi - 1080.0 * J1_pow_2 * k_s_pow_2 * xi_pow_4 + 2160.0 * J1_pow_2 * k_s_pow_2 * xi_pow_3 - 2376.0 * J1_pow_2 * k_s_pow_2 * xi + 864.0 * J1_pow_2 * k_s_pow_2 + 90.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_4 + 84.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_3 - 288.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_2 + 114.0 * J2_pow_2 * J3_pow_2 * k_s * xi + 360.0 * J2_pow_2 * k_s_pow_2 * xi_pow_4 + 720.0 * J2_pow_2 * k_s_pow_2 * xi_pow_3 - 1656.0 * J2_pow_2 * k_s_pow_2 * xi + 576.0 * J2_pow_2 * k_s_pow_2 + 720.0 * J3_pow_2 * k_s_pow_2 * xi_pow_4 - 4320.0 * J3_pow_2 * k_s_pow_2 * xi_pow_2 + 2592.0 * J3_pow_2 * k_s_pow_2 * xi - 17280.0 * k_s_pow_3 * xi + 8640.0 * k_s_pow_3) / (8.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 96.0 * J1_pow_2 * J2_pow_2 * k_s + 156.0 * J1_pow_2 * J3_pow_2 * k_s - 1296.0 * J1_pow_2 * k_s_pow_2 + 156.0 * J2_pow_2 * J3_pow_2 * k_s - 1296.0 * J2_pow_2 * k_s_pow_2 + 2592.0 * J3_pow_2 * k_s_pow_2 - 17280.0 * k_s_pow_3);
    rdN[2] = (5.0 * J1_pow_3 * J2_pow_2 * J3_pow_2 * xi_pow_4 - 4 * J1_pow_3 * J2_pow_2 * J3_pow_2 * xi_pow_3 - 3.0 * J1_pow_3 * J2_pow_2 * J3_pow_2 * xi_pow_2 + 2 * J1_pow_3 * J2_pow_2 * J3_pow_2 * xi - 30.0 * J1_pow_3 * J2_pow_2 * k_s * xi_pow_4 + 48.0 * J1_pow_3 * J2_pow_2 * k_s * xi_pow_3 - 24.0 * J1_pow_3 * J2_pow_2 * k_s * xi + 6.0 * J1_pow_3 * J2_pow_2 * k_s + 60.0 * J1_pow_3 * J3_pow_2 * k_s * xi_pow_4 - 108.0 * J1_pow_3 * J3_pow_2 * k_s * xi_pow_3 - 36.0 * J1_pow_3 * J3_pow_2 * k_s * xi_pow_2 + 54.0 * J1_pow_3 * J3_pow_2 * k_s * xi - 360.0 * J1_pow_3 * k_s_pow_2 * xi_pow_4 + 720.0 * J1_pow_3 * k_s_pow_2 * xi_pow_3 - 360.0 * J1_pow_3 * k_s_pow_2 * xi + 72.0 * J1_pow_3 * k_s_pow_2) / (4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 48.0 * J1_pow_2 * J2_pow_2 * k_s + 78.0 * J1_pow_2 * J3_pow_2 * k_s - 648.0 * J1_pow_2 * k_s_pow_2 + 78.0 * J2_pow_2 * J3_pow_2 * k_s - 648.0 * J2_pow_2 * k_s_pow_2 + 1296.0 * J3_pow_2 * k_s_pow_2 - 8640.0 * k_s_pow_3);

    rdN[3] = (-5.0 * J1_pow_2 * J2_pow_3 * J3_pow_2 * k02 * xi_pow_4 - 4.0 * J1_pow_2 * J2_pow_3 * J3_pow_2 * k02 * xi_pow_3 + 3 * J1_pow_2 * J2_pow_3 * J3_pow_2 * k02 * xi_pow_2 + 2 * J1_pow_2 * J2_pow_3 * J3_pow_2 * k02 * xi + 30.0 * J1_pow_2 * J2_pow_3 * k02 * k_s * xi_pow_4 + 48.0 * J1_pow_2 * J2_pow_3 * k02 * k_s * xi_pow_3 - 24.0 * J1_pow_2 * J2_pow_3 * k02 * k_s * xi - 6.0 * J1_pow_2 * J2_pow_3 * k02 * k_s - 60.0 * J2_pow_3 * J3_pow_2 * k02 * k_s * xi_pow_4 - 108.0 * J2_pow_3 * J3_pow_2 * k02 * k_s * xi_pow_3 + 36.0 * J2_pow_3 * J3_pow_2 * k02 * k_s * xi_pow_2 + 54.0 * J2_pow_3 * J3_pow_2 * k02 * k_s * xi + 360.0 * J2_pow_3 * k02 * k_s_pow_2 * xi_pow_4 + 720.0 * J2_pow_3 * k02 * k_s_pow_2 * xi_pow_3 - 360.0 * J2_pow_3 * k02 * k_s_pow_2 * xi - 72.0 * J2_pow_3 * k02 * k_s_pow_2) / (4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 48.0 * J1_pow_2 * J2_pow_2 * k_s + 78.0 * J1_pow_2 * J3_pow_2 * k_s - 648.0 * J1_pow_2 * k_s_pow_2 + 78.0 * J2_pow_2 * J3_pow_2 * k_s - 648.0 * J2_pow_2 * k_s_pow_2 + 1296.0 * J3_pow_2 * k_s_pow_2 - 8640.0 * k_s_pow_3);
    rdN[4] = (-30.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_4 - 16.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_3 + 30.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_2 + 16.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi + 60.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_4 + 192.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_3 - 192.0 * J1_pow_2 * J2_pow_2 * k_s * xi - 60.0 * J1_pow_2 * J2_pow_2 * k_s - 90.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_4 + 84.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_3 + 288.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_2 + 114.0 * J1_pow_2 * J3_pow_2 * k_s * xi - 360.0 * J1_pow_2 * k_s_pow_2 * xi_pow_4 + 720.0 * J1_pow_2 * k_s_pow_2 * xi_pow_3 - 1656.0 * J1_pow_2 * k_s_pow_2 * xi - 576.0 * J1_pow_2 * k_s_pow_2 - 330.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_4 - 516.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_3 + 432.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_2 + 414.0 * J2_pow_2 * J3_pow_2 * k_s * xi + 1080.0 * J2_pow_2 * k_s_pow_2 * xi_pow_4 + 2160.0 * J2_pow_2 * k_s_pow_2 * xi_pow_3 - 2376.0 * J2_pow_2 * k_s_pow_2 * xi - 864.0 * J2_pow_2 * k_s_pow_2 - 720.0 * J3_pow_2 * k_s_pow_2 * xi_pow_4 + 4320.0 * J3_pow_2 * k_s_pow_2 * xi_pow_2 + 2592.0 * J3_pow_2 * k_s_pow_2 * xi - 17280.0 * k_s_pow_2 * xi - 8640.0 * k_s_pow_2) / (8.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 96.0 * J1_pow_2 * J2_pow_2 * k_s + 156.0 * J1_pow_2 * J3_pow_2 * k_s - 1296.0 * J1_pow_2 * k_s_pow_2 + 156.0 * J2_pow_2 * J3_pow_2 * k_s - 1296.0 * J2_pow_2 * k_s_pow_2 + 2592.0 * J3_pow_2 * k_s_pow_2 - 17280.0 * k_s_pow_3);
    rdN[5] = (5.0 * J1_pow_2 * J2_pow_3 * J3_pow_2 * xi_pow_4 + 4.0 * J1_pow_2 * J2_pow_3 * J3_pow_2 * xi_pow_3 - 3.0 * J1_pow_2 * J2_pow_3 * J3_pow_2 * xi_pow_2 - 2 * J1_pow_2 * J2_pow_3 * J3_pow_2 * xi - 30.0 * J1_pow_2 * J2_pow_3 * k_s * xi_pow_4 - 48.0 * J1_pow_2 * J2_pow_3 * k_s * xi_pow_3 + 24.0 * J1_pow_2 * J2_pow_3 * k_s * xi + 6.0 * J1_pow_2 * J2_pow_3 * k_s + 60.0 * J2_pow_3 * J3_pow_2 * k_s * xi_pow_4 + 108.0 * J2_pow_3 * J3_pow_2 * k_s * xi_pow_3 - 36.0 * J2_pow_3 * J3_pow_2 * k_s * xi_pow_2 - 54.0 * J2_pow_3 * J3_pow_2 * k_s * xi - 360.0 * J2_pow_3 * k_s_pow_2 * xi_pow_4 - 720.0 * J2_pow_3 * k_s_pow_2 * xi_pow_3 + 360.0 * J2_pow_3 * k_s_pow_2 * xi + 72.0 * J2_pow_3 * k_s_pow_2) / (4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 48.0 * J1_pow_2 * J2_pow_2 * k_s + 78.0 * J1_pow_2 * J3_pow_2 * k_s - 648.0 * J1_pow_2 * k_s_pow_2 + 78.0 * J2_pow_2 * J3_pow_2 * k_s - 648.0 * J2_pow_2 * k_s_pow_2 + 1296.0 * J3_pow_2 * k_s_pow_2 - 8640.0 * k_s_pow_3);

    rdN[6] = (-10.0 * J1_pow_2 * J2_pow_2 * J3_pow_3 * k03 * xi_pow_4 + 12.0 * J1_pow_2 * J2_pow_2 * J3_pow_3 * k03 * xi_pow_2 - 2.0 * J1_pow_2 * J2_pow_2 * J3_pow_3 * k03 - 75.0 * J1_pow_2 * J3_pow_3 * k03 * k_s * xi_pow_4 + 96.0 * J1_pow_2 * J3_pow_3 * k03 * k_s * xi_pow_3 + 162.0 * J1_pow_2 * J3_pow_3 * k03 * k_s * xi_pow_2 - 48.0 * J1_pow_2 * J3_pow_3 * k03 * k_s * xi - 39.0 * J1_pow_2 * J3_pow_3 * k03 * k_s - 75.0 * J2_pow_2 * J3_pow_3 * k03 * k_s * xi_pow_4 - 96.0 * J2_pow_2 * J3_pow_3 * k03 * k_s * xi_pow_3 + 162.0 * J2_pow_2 * J3_pow_3 * k03 * k_s * xi_pow_2 + 48.0 * J2_pow_2 * J3_pow_3 * k03 * k_s * xi - 39.0 * J2_pow_2 * J3_pow_3 * k03 * k_s - 360.0 * J3_pow_3 * k03 * k_s_pow_2 * xi_pow_4 + 2160.0 * J3_pow_3 * k03 * k_s_pow_2 * xi_pow_2 - 648.0 * J3_pow_3 * k03 * k_s_pow_2) / (2.0 * J1_pow_2 * J2_pow_2 * J3_pow_3 - 24.0 * J1_pow_2 * J2_pow_2 * k_s + 39.0 * J1_pow_2 * J3_pow_3 * k_s - 324.0 * J1_pow_2 * k_s_pow_2 + 39.0 * J2_pow_2 * J3_pow_3 * k_s - 324.0 * J2_pow_2 * k_s_pow_2 + 648.0 * J3_pow_3 * k_s_pow_2 - 4320.0 * k_s_pow_3);
    rdN[7] = (8.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_3 - 8.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi - 96.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_3 + 96.0 * J1_pow_2 * J2_pow_2 * k_s * xi - 60.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_4 + 108.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_3 + 36.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_2 - 132.0 * J1_pow_2 * J3_pow_2 * k_s * xi + 360.0 * J1_pow_2 * k_s_pow_2 * xi_pow_4 - 720.0 * J1_pow_2 * k_s_pow_2 * xi_pow_3 + 1008.0 * J1_pow_2 * k_s_pow_2 * xi - 72.0 * J1_pow_2 * k_s_pow_2 + 60.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_4 + 108.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_3 - 36.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_2 - 132.0 * J2_pow_2 * J3_pow_2 * k_s * xi - 360.0 * J2_pow_2 * k_s_pow_2 * xi_pow_4 - 720.0 * J2_pow_2 * k_s_pow_2 * xi_pow_3 + 1008.0 * J2_pow_2 * k_s_pow_2 * xi + 72.0 * J2_pow_2 * k_s_pow_2 - 1296.0 * J3_pow_2 * k_s_pow_2 * xi + 8640.0 * k_s_pow_2 * xi) / (2.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 24.0 * J1_pow_2 * J2_pow_2 * k_s + 39.0 * J1_pow_2 * J3_pow_2 * k_s - 324.0 * J1_pow_2 * k_s_pow_2 + 39.0 * J2_pow_2 * J3_pow_2 * k_s - 324.0 * J2_pow_2 * k_s_pow_2 + 648.0 * J3_pow_2 * k_s_pow_2 - 4320.0 * k_s_pow_3);
    rdN[8] = (10.0 * J1_pow_2 * J2_pow_2 * J3_pow_3 * xi_pow_4 - 12.0 * J1_pow_2 * J2_pow_2 * J3_pow_3 * xi_pow_2 + 2.0 * J1_pow_2 * J2_pow_2 * J3_pow_3 + 75.0 * J1_pow_2 * J3_pow_3 * k_s * xi_pow_4 - 96.0 * J1_pow_2 * J3_pow_3 * k_s * xi_pow_3 - 162.0 * J1_pow_2 * J3_pow_3 * k_s * xi_pow_2 + 48.0 * J1_pow_2 * J3_pow_3 * k_s * xi + 39.0 * J1_pow_2 * J3_pow_3 * k_s + 75.0 * J2_pow_2 * J3_pow_3 * k_s * xi_pow_4 + 96.0 * J2_pow_2 * J3_pow_3 * k_s * xi_pow_3 - 162.0 * J2_pow_2 * J3_pow_3 * k_s * xi_pow_2 - 48.0 * J2_pow_2 * J3_pow_3 * k_s * xi + 39.0 * J2_pow_2 * J3_pow_3 * k_s + 360.0 * J3_pow_3 * k_s_pow_2 * xi_pow_4 - 2160.0 * J3_pow_3 * k_s_pow_2 * xi_pow_2 + 648.0 * J3_pow_3 * k_s_pow_2) / (2.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 24.0 * J1_pow_2 * J2_pow_2 * k_s + 39.0 * J1_pow_2 * J3_pow_3 * k_s - 324.0 * J1_pow_2 * k_s_pow_2 + 39.0 * J2_pow_2 * J3_pow_2 * k_s - 324.0 * J2_pow_2 * k_s_pow_2 + 648.0 * J3_pow_2 * k_s_pow_2 - 4320.0 * k_s_pow_3);

    rdN /= J;
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::GetSecondDerivativesShapeFunctionsValues(
    GlobalSizeVector& rd2N,
    const double J,
    const double xi
    )
{
    // Nodal values of the Jacobian
    const double J1 = GetJacobian(-1.0);
    const double J2 = GetJacobian( 1.0);
    const double J3 = GetJacobian( 0.0);

    // Nodal values of the geometric curvatures
    const double k01 = GetGeometryCurvature(J, -1.0);
    const double k02 = GetGeometryCurvature(J,  1.0);
    const double k03 = GetGeometryCurvature(J,  0.0);

    const double k_s = GetBendingShearStiffnessRatio();

    const double xi_pow_2 = std::pow(xi, 2);
    const double xi_pow_3 = std::pow(xi, 3);

    const double J1_pow_2 = std::pow(J1, 2);
    const double J1_pow_3 = std::pow(J1, 3);

    const double J2_pow_2 = std::pow(J2, 2);
    const double J2_pow_3 = std::pow(J2, 3);

    const double J3_pow_2 = std::pow(J3, 2);
    const double J3_pow_3 = std::pow(J3, 3);

    const double k_s_pow_2 = std::pow(k_s, 2);
    const double k_s_pow_3 = std::pow(k_s, 3);

    rd2N[0] = (-20.0 * J1_pow_3 * J2_pow_2 * J3_pow_2 * k01 * xi_pow_3 + 12 * J1_pow_3 * J2_pow_2 * J3_pow_2 * k01 * xi_pow_2 + 6 * J1_pow_3 * J2_pow_2 * J3_pow_2 * k01 * xi - 2 * J1_pow_3 * J2_pow_2 * J3_pow_2 * k01 + 120.0 * J1_pow_3 * J2_pow_2 * k01 * k_s * xi_pow_3 - 144.0 * J1_pow_3 * J2_pow_2 * k01 * k_s * xi_pow_2 + 24.0 * J1_pow_3 * J2_pow_2 * k01 * k_s - 240.0 * J1_pow_3 * J3_pow_2 * k01 * k_s * xi_pow_3 + 324.0 * J1_pow_3 * J3_pow_2 * k01 * k_s * xi_pow_2 + 72.0 * J1_pow_3 * J3_pow_2 * k01 * k_s * xi - 54.0 * J1_pow_3 * J3_pow_2 * k01 * k_s + 1440.0 * J1_pow_3 * k01 * k_s_pow_2 * xi_pow_3 - 2160.0 * J1_pow_3 * k01 * k_s_pow_2 * xi_pow_2 + 360.0 * J1_pow_3 * k01 * k_s_pow_2) / (4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 48.0 * J1_pow_2 * J2_pow_2 * k_s + 78.0 * J1_pow_2 * J3_pow_2 * k_s - 648.0 * J1_pow_2 * k_s_pow_2 + 78.0 * J2_pow_2 * J3_pow_2 * k_s - 648.0 * J2_pow_2 * k_s_pow_2 + 1296.0 * J3_pow_2 * k_s_pow_2 - 8640.0 * k_s_pow_3);
    rd2N[1] = (120.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_3 - 48.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_2 - 60.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi + 16.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 240.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_3 + 576.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_2 - 192.0 * J1_pow_2 * J2_pow_2 * k_s + 1320.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_3 - 1548.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_2 - 864.0 * J1_pow_2 * J3_pow_2 * k_s * xi + 414.0 * J1_pow_2 * J3_pow_2 * k_s - 4320.0 * J1_pow_2 * k_s_pow_2 * xi_pow_3 + 6480.0 * J1_pow_2 * k_s_pow_2 * xi_pow_2 - 2376.0 * J1_pow_2 * k_s_pow_2 + 360.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_3 + 252.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_2 - 576.0 * J2_pow_2 * J3_pow_2 * k_s * xi + 114.0 * J2_pow_2 * J3_pow_2 * k_s + 1440.0 * J2_pow_2 * k_s_pow_2 * xi_pow_3 + 2160.0 * J2_pow_2 * k_s_pow_2 * xi_pow_2 - 1656.0 * J2_pow_2 * k_s_pow_2 + 2880.0 * J3_pow_2 * k_s_pow_2 * xi_pow_3 - 8640.0 * J3_pow_2 * k_s_pow_2 * xi + 2592.0 * J3_pow_2 * k_s_pow_2 - 17280.0 * k_s_pow_3);
    rd2N[2] = (20.0 * J1_pow_3 * J2_pow_2 * J3_pow_2 * xi_pow_3 - 12.0 * J1_pow_3 * J2_pow_2 * J3_pow_2 * xi_pow_2 - 6.0 * J1_pow_3 * J2_pow_2 * J3_pow_2 * xi + 2.0 * J1_pow_3 * J2_pow_2 * J3_pow_2 - 120.0 * J1_pow_3 * J2_pow_2 * k_s * xi_pow_3 + 144.0 * J1_pow_3 * J2_pow_2 * k_s * xi_pow_2 - 24.0 * J1_pow_3 * J2_pow_2 * k_s + 240.0 * J1_pow_3 * J3_pow_2 * k_s * xi_pow_3 - 324.0 * J1_pow_3 * J3_pow_2 * k_s * xi_pow_2 - 72.0 * J1_pow_3 * J3_pow_2 * k_s * xi + 54.0 * J1_pow_3 * J3_pow_2 * k_s - 1440.0 * J1_pow_3 * k_s_pow_2 * xi_pow_3 + 2160.0 * J1_pow_3 * k_s_pow_2 * xi_pow_2 - 360.0 * J1_pow_3 * k_s_pow_2) / (4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 48.0 * J1_pow_2 * J2_pow_2 * k_s + 78.0 * J1_pow_2 * J3_pow_2 * k_s - 648.0 * J1_pow_2 * k_s_pow_2 + 78.0 * J2_pow_2 * J3_pow_2 * k_s - 648.0 * J2_pow_2 * k_s_pow_2 + 1296.0 * J3_pow_2 * k_s_pow_2 - 8640.0 * k_s_pow_3);

    rd2N[3] = (-20.0 * J1_pow_2 * J2_pow_3 * J3_pow_2 * k02 * xi_pow_3 - 12.0 * J1_pow_2 * J2_pow_3 * J3_pow_2 * k02 * xi_pow_2 + 6.0 * J1_pow_2 * J2_pow_3 * J3_pow_2 * k02 * xi + 2.0 * J1_pow_2 * J2_pow_3 * J3_pow_2 * k02 + 120.0 * J1_pow_2 * J2_pow_3 * k02 * k_s * xi_pow_3 + 144.0 * J1_pow_2 * J2_pow_3 * k02 * k_s * xi_pow_2 - 24.0 * J1_pow_2 * J2_pow_3 * k02 * k_s - 240.0 * J2_pow_3 * J3_pow_2 * k02 * k_s * xi_pow_3 - 324.0 * J2_pow_3 * J3_pow_2 * k02 * k_s * xi_pow_2 + 72.0 * J2_pow_3 * J3_pow_2 * k02 * k_s * xi + 54.0 * J2_pow_3 * J3_pow_2 * k02 * k_s + 1440.0 * J2_pow_3 * k02 * k_s_pow_2 * xi_pow_3 + 2160.0 * J2_pow_3 * k02 * k_s_pow_2 * xi_pow_2 - 360.0 * J2_pow_3 * k02 * k_s_pow_2) / (4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 48.0 * J1_pow_2 * J2_pow_2 * k_s + 78.0 * J1_pow_2 * J3_pow_2 * k_s - 648.0 * J1_pow_2 * k_s_pow_2 + 78.0 * J2_pow_2 * J3_pow_2 * k_s - 648.0 * J2_pow_2 * k_s_pow_2 + 1296.0 * J3_pow_2 * k_s_pow_2 - 8640.0 * k_s_pow_3);
    rd2N[4] = (-120.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_3 - 48.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_2 + 60.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi + 16.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 + 240.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_3 + 576.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_2 - 192.0 * J1_pow_2 * J2_pow_2 * k_s - 360.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_3 + 252.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_2 + 576.0 * J1_pow_2 * J3_pow_2 * k_s * xi + 114.0 * J1_pow_2 * J3_pow_2 * k_s - 1440.0 * J1_pow_2 * k_s_pow_2 * xi_pow_3 + 2160.0 * J1_pow_2 * k_s_pow_2 * xi_pow_2 - 1656.0 * J1_pow_2 * k_s_pow_2 - 1320.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_3 - 1548.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_2 + 864.0 * J2_pow_2 * J3_pow_2 * k_s * xi + 414.0 * J2_pow_2 * J3_pow_2 * k_s + 4320.0 * J2_pow_2 * k_s_pow_2 * xi_pow_3 + 6480.0 * J2_pow_2 * k_s_pow_2 * xi_pow_2 - 2376.0 * J2_pow_2 * k_s_pow_2 - 2880.0 * J3_pow_2 * k_s_pow_2 * xi_pow_3 + 8640.0 * J3_pow_2 * k_s_pow_2 * xi + 2592.0 * J3_pow_2 * k_s_pow_2 - 17280.0 * k_s_pow_3) / (8.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 96.0 * J1_pow_2 * J2_pow_2 * k_s + 156.0 * J1_pow_2 * J3_pow_2 * k_s - 1296.0 * J1_pow_2 * k_s_pow_2 + 156.0 * J2_pow_2 * J3_pow_2 * k_s - 1296.0 * J2_pow_2 * k_s_pow_2 + 2592.0 * J3_pow_2 * k_s_pow_2 - 17280.0 * k_s_pow_3);
    rd2N[5] = (20 * J1_pow_2 * J2_pow_3 * J3_pow_2 * xi_pow_3 + 12 * J1_pow_2 * J2_pow_3 * J3_pow_2 * xi_pow_2 - 6 * J1_pow_2 * J2_pow_3 * J3_pow_2 * xi - 2 * J1_pow_2 * J2_pow_3 * J3_pow_2 - 120.0 * J1_pow_2 * J2_pow_3 * k_s * xi_pow_3 - 144.0 * J1_pow_2 * J2_pow_3 * k_s * xi_pow_2 + 24.0 * J1_pow_2 * J2_pow_3 * k_s + 240.0 * J2_pow_3 * J3_pow_2 * k_s * xi_pow_3 + 324.0 * J2_pow_3 * J3_pow_2 * k_s * xi_pow_2 - 72.0 * J2_pow_3 * J3_pow_2 * k_s * xi - 54.0 * J2_pow_3 * J3_pow_2 * k_s - 1440.0 * J2_pow_3 * k_s_pow_2 * xi_pow_3 - 2160.0 * J2_pow_3 * k_s_pow_2 * xi_pow_2 + 360.0 * J2_pow_3 * k_s_pow_2) / (4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 48.0 * J1_pow_2 * J2_pow_2 * k_s + 78.0 * J1_pow_2 * J3_pow_2 * k_s - 648.0 * J1_pow_2 * k_s_pow_2 + 78.0 * J2_pow_2 * J3_pow_2 * k_s - 648.0 * J2_pow_2 * k_s_pow_2 + 1296.0 * J3_pow_2 * k_s_pow_2 - 8640.0 * k_s_pow_3);

    rd2N[6] = (-40.0 * J1_pow_2 * J2_pow_2 * J3_pow_3 * k03 * xi_pow_3 + 24.0 * J1_pow_2 * J2_pow_2 * J3_pow_3 * k03 * xi - 300.0 * J1_pow_2 * J3_pow_3 * k03 * k_s * xi_pow_3 + 288.0 * J1_pow_2 * J3_pow_3 * k03 * k_s * xi_pow_2 + 324.0 * J1_pow_2 * J3_pow_3 * k03 * k_s * xi - 48.0 * J1_pow_2 * J3_pow_3 * k03 * k_s - 300.0 * J2_pow_2 * J3_pow_3 * k03 * k_s * xi_pow_3 - 288.0 * J2_pow_2 * J3_pow_3 * k03 * k_s * xi_pow_2 + 324.0 * J2_pow_2 * J3_pow_3 * k03 * k_s * xi + 48.0 * J2_pow_2 * J3_pow_3 * k03 * k_s - 1440.0 * J3_pow_3 * k03 * k_s_pow_2 * xi_pow_3 + 4320.0 * J3_pow_3 * k03 * k_s_pow_2 * xi) / (2.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 24.0 * J1_pow_2 * J2_pow_2 * k_s + 39.0 * J1_pow_2 * J3_pow_2 * k_s - 324.0 * J1_pow_2 * k_s_pow_2 + 39.0 * J2_pow_2 * J3_pow_2 * k_s - 324.0 * J2_pow_2 * k_s_pow_2 + 648.0 * J3_pow_2 * k_s_pow_2 - 4320.0 * k_s_pow_3);
    rd2N[7] = (24.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_2 - 8.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 288.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_2 + 96.0 * J1_pow_2 * J2_pow_2 * k_s - 240.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_3 + 324.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_2 + 72.0 * J1_pow_2 * J3_pow_2 * k_s * xi - 132.0 * J1_pow_2 * J3_pow_2 * k_s + 1440.0 * J1_pow_2 * k_s_pow_2 * xi_pow_3 - 2160.0 * J1_pow_2 * k_s_pow_2 * xi_pow_2 + 1008.0 * J1_pow_2 * k_s_pow_2 + 240.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_3 + 324.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_2 - 72.0 * J2_pow_2 * J3_pow_2 * k_s * xi - 132.0 * J2_pow_2 * J3_pow_2 * k_s - 1440.0 * J2_pow_2 * k_s_pow_2 * xi_pow_3 - 2160.0 * J2_pow_2 * k_s_pow_2 * xi_pow_2 + 1008.0 * J2_pow_2 * k_s_pow_2 - 1296.0 * J3_pow_2 * k_s_pow_2 + 8640.0 * k_s_pow_3) / (2.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 24.0 * J1_pow_2 * J2_pow_2 * k_s + 39.0 * J1_pow_2 * J3_pow_2 * k_s - 324.0 * J1_pow_2 * k_s_pow_2 + 39.0 * J2_pow_2 * J3_pow_2 * k_s - 324.0 * J2_pow_2 * k_s_pow_2 + 648.0 * J3_pow_2 * k_s_pow_2 - 4320.0 * k_s_pow_3);
    rd2N[8] = (40.0 * J1_pow_2 * J2_pow_2 * J3_pow_3 * xi_pow_3 - 24.0 * J1_pow_2 * J2_pow_2 * J3_pow_3 * xi + 300.0 * J1_pow_2 * J3_pow_3 * k_s * xi_pow_3 - 288.0 * J1_pow_2 * J3_pow_3 * k_s * xi_pow_2 - 324.0 * J1_pow_2 * J3_pow_3 * k_s * xi + 48.0 * J1_pow_2 * J3_pow_3 * k_s + 300.0 * J2_pow_2 * J3_pow_3 * k_s * xi_pow_3 + 288.0 * J2_pow_2 * J3_pow_3 * k_s * xi_pow_2 - 324.0 * J2_pow_2 * J3_pow_3 * k_s * xi - 48.0 * J2_pow_2 * J3_pow_3 * k_s + 1440.0 * J3_pow_3 * k_s_pow_2 * xi_pow_3 - 4320.0 * J3_pow_3 * k_s_pow_2 * xi) / (2.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 24.0 * J1_pow_2 * J2_pow_2 * k_s + 39.0 * J1_pow_2 * J3_pow_2 * k_s - 324.0 * J1_pow_2 * k_s_pow_2 + 39.0 * J2_pow_2 * J3_pow_2 * k_s - 324.0 * J2_pow_2 * k_s_pow_2 + 648.0 * J3_pow_2 * k_s_pow_2 - 4320.0 * k_s_pow_3);

    rd2N /= std::pow(J, 2);
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::GetThirdDerivativesShapeFunctionsValues(
    GlobalSizeVector& rd3N,
    const double J,
    const double xi
    )
{
    // Nodal values of the Jacobian
    const double J1 = GetJacobian(-1.0);
    const double J2 = GetJacobian( 1.0);
    const double J3 = GetJacobian( 0.0);

    // Nodal values of the geometric curvatures
    const double k01 = GetGeometryCurvature(J, -1.0);
    const double k02 = GetGeometryCurvature(J,  1.0);
    const double k03 = GetGeometryCurvature(J,  0.0);

    const double k_s = GetBendingShearStiffnessRatio();

    const double xi_pow_2 = std::pow(xi, 2);

    const double J1_pow_2 = std::pow(J1, 2);
    const double J1_pow_3 = std::pow(J1, 3);

    const double J2_pow_2 = std::pow(J2, 2);
    const double J2_pow_3 = std::pow(J2, 3);

    const double J3_pow_2 = std::pow(J3, 2);
    const double J3_pow_3 = std::pow(J3, 3);

    const double k_s_pow_2 = std::pow(k_s, 2);
    const double k_s_pow_3 = std::pow(k_s, 3);

    rd3N[0] = (-60.0 * J1_pow_3 * J2_pow_2 * J3_pow_2 * k01 * xi_pow_2 + 24 * J1_pow_3 * J2_pow_2 * J3_pow_2 * k01 * xi + 6 * J1_pow_3 * J2_pow_2 * J3_pow_2 * k01 + 360.0 * J1_pow_3 * J2_pow_2 * k01 * k_s * xi_pow_2 - 288.0 * J1_pow_3 * J2_pow_2 * k01 * k_s * xi - 720.0 * J1_pow_3 * J3_pow_2 * k01 * k_s * xi_pow_2 + 648.0 * J1_pow_3 * J3_pow_2 * k01 * k_s * xi + 72.0 * J1_pow_3 * J3_pow_2 * k01 * k_s + 4320.0 * J1_pow_3 * k01 * k_s_pow_2 * xi_pow_2 - 4320.0 * J1_pow_3 * k01 * k_s_pow_2 * xi) / (4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 48.0 * J1_pow_2 * J2_pow_2 * k_s + 78.0 * J1_pow_2 * J3_pow_2 * k_s - 648.0 * J1_pow_2 * k_s_pow_2 + 78.0 * J2_pow_2 * J3_pow_2 * k_s - 648.0 * J2_pow_2 * k_s_pow_2 + 1296.0 * J3_pow_2 * k_s_pow_2 - 8640.0 * k_s_pow_3);
    rd3N[1] = (360.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi_pow_2 - 96.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi - 60.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 720.0 * J1_pow_2 * J2_pow_2 * k_s * xi_pow_2 + 1152.0 * J1_pow_2 * J2_pow_2 * k_s * xi + 3960.0 * J1_pow_2 * J3_pow_2 * k_s * xi_pow_2 - 3096.0 * J1_pow_2 * J3_pow_2 * k_s * xi - 864.0 * J1_pow_2 * J3_pow_2 * k_s - 12960.0 * J1_pow_2 * k_s_pow_2 * xi_pow_2 + 12960.0 * J1_pow_2 * k_s_pow_2 * xi + 1080.0 * J2_pow_2 * J3_pow_2 * k_s * xi_pow_2 + 504.0 * J2_pow_2 * J3_pow_2 * k_s * xi - 576.0 * J2_pow_2 * J3_pow_2 * k_s + 4320.0 * J2_pow_2 * k_s_pow_2 * xi_pow_2 + 4320.0 * J2_pow_2 * k_s_pow_2 * xi + 8640.0 * J3_pow_2 * k_s_pow_2 * xi_pow_2 - 8640.0 * J3_pow_2 * k_s_pow_2) / (8.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 96.0 * J1_pow_2 * J2_pow_2 * k_s + 156.0 * J1_pow_2 * J3_pow_2 * k_s - 1296.0 * J1_pow_2 * k_s_pow_2 + 156.0 * J2_pow_2 * J3_pow_2 * k_s - 1296.0 * J2_pow_2 * k_s_pow_2 + 2592.0 * J3_pow_2 * k_s_pow_2 - 17280.0 * k_s_pow_3);
    rd3N[2] = (60 * J1_pow_3 * J2_pow_2 * J3_pow_2 * xi_pow_2 - 24 * J1_pow_3 * J2_pow_2 * J3_pow_2 * xi - 6 * J1_pow_3 * J2_pow_2 * J3_pow_2 - 360.0 * J1_pow_3 * J2_pow_2 * k_s * xi_pow_2 + 288.0 * J1_pow_3 * J2_pow_2 * k_s * xi + 720.0 * J1_pow_3 * J3_pow_2 * k_s * xi_pow_2 - 648.0 * J1_pow_3 * J3_pow_2 * k_s * xi - 72.0 * J1_pow_3 * J3_pow_2 * k_s - 4320.0 * J1_pow_3 * k_s_pow_2 * xi_pow_2 + 4320.0 * J1_pow_3 * k_s_pow_2 * xi) / (4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 48.0 * J1_pow_2 * J2_pow_2 * k_s + 78.0 * J1_pow_2 * J3_pow_2 * k_s - 648.0 * J1_pow_2 * k_s_pow_2 + 78.0 * J2_pow_2 * J3_pow_2 * k_s - 648.0 * J2_pow_2 * k_s_pow_2 + 1296.0 * J3_pow_2 * k_s_pow_2 - 8640.0 * k_s_pow_3);

    rd3N[3] = (-60 * J1_pow_2 *J2_pow_3 *J3_pow_2 * k02 * xi_pow_2 - 24 * J1_pow_2 *J2_pow_3 *J3_pow_2 * k02 * xi + 6 * J1_pow_2 *J2_pow_3 *J3_pow_2 * k02 + 360.0 * J1_pow_2 *J2_pow_3 * k02 * k_s * xi_pow_2 + 288.0 * J1_pow_2 *J2_pow_3 * k02 * k_s * xi - 720.0 *J2_pow_3 *J3_pow_2 * k02 * k_s * xi_pow_2 - 648.0 *J2_pow_3 *J3_pow_2 * k02 * k_s * xi + 72.0 *J2_pow_3 *J3_pow_2 * k02 * k_s + 4320.0 *J2_pow_3 * k02 * k_s_pow_2 * xi_pow_2 + 4320.0 *J2_pow_3 * k02 * k_s_pow_2 * xi) / (4.0 * J1_pow_2 *J2_pow_2 *J3_pow_2 - 48.0 * J1_pow_2 *J2_pow_2 * k_s + 78.0 * J1_pow_2 *J3_pow_2 * k_s - 648.0 * J1_pow_2 * k_s_pow_2 + 78.0 *J2_pow_2 *J3_pow_2 * k_s - 648.0 *J2_pow_2 * k_s_pow_2 + 1296.0 *J3_pow_2 * k_s_pow_2 - 8640.0 * k_s_pow_3);
    // rd3N[4] = 
    // rd3N[5] = 

    // rd3N[6] = 
    // rd3N[7] = 
    // rd3N[8] = 

    rd3N /= std::pow(J, 3);
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::GetFourthDerivativesShapeFunctionsValues(
    GlobalSizeVector& rd4N,
    const double J,
    const double xi
    )
{
    // Nodal values of the Jacobian
    const double J1 = GetJacobian(-1.0);
    const double J2 = GetJacobian( 1.0);
    const double J3 = GetJacobian( 0.0);

    // Nodal values of the geometric curvatures
    const double k01 = GetGeometryCurvature(J, -1.0);
    const double k02 = GetGeometryCurvature(J,  1.0);
    const double k03 = GetGeometryCurvature(J,  0.0);

    const double k_s = GetBendingShearStiffnessRatio();

    const double xi_pow_2 = std::pow(xi, 2);

    const double J1_pow_2 = std::pow(J1, 2);
    const double J1_pow_3 = std::pow(J1, 3);

    const double J2_pow_2 = std::pow(J2, 2);
    const double J2_pow_3 = std::pow(J2, 3);

    const double J3_pow_2 = std::pow(J3, 2);
    const double J3_pow_3 = std::pow(J3, 3);

    const double k_s_pow_2 = std::pow(k_s, 2);
    const double k_s_pow_3 = std::pow(k_s, 3);

    rd4N[0] = (-120.0 * J1_pow_3 * J2_pow_2 * J3_pow_2 * k01 * xi + 24 * J1_pow_3 * J2_pow_2 * J3_pow_2 * k01 + 720.0 * J1_pow_3 * J2_pow_2 * k01 * k_s * xi - 288.0 * J1_pow_3 * J2_pow_2 * k01 * k_s - 1440.0 * J1_pow_3 * J3_pow_2 * k01 * k_s * xi + 648.0 * J1_pow_3 * J3_pow_2 * k01 * k_s + 8640.0 * J1_pow_3 * k01 * k_s_pow_2 * xi - 4320.0 * J1_pow_3 * k01 * k_s_pow_2) / (4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 48.0 * J1_pow_2 * J2_pow_2 * k_s + 78.0 * J1_pow_2 * J3_pow_2 * k_s - 648.0 * J1_pow_2 * k_s_pow_2 + 78.0 * J2_pow_2 * J3_pow_2 * k_s - 648.0 * J2_pow_2 * k_s_pow_2 + 1296.0 * J3_pow_2 * k_s_pow_2 - 8640.0 * k_s_pow_3);
    rd4N[1] = (720.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 * xi - 96.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 1440.0 * J1_pow_2 * J2_pow_2 * k_s * xi + 1152.0 * J1_pow_2 * J2_pow_2 * k_s + 7920.0 * J1_pow_2 * J3_pow_2 * k_s * xi - 3096.0 * J1_pow_2 * J3_pow_2 * k_s - 25920.0 * J1_pow_2 * k_s_pow_2 * xi + 12960.0 * J1_pow_2 * k_s_pow_2 + 2160.0 * J2_pow_2 * J3_pow_2 * k_s * xi + 504.0 * J2_pow_2 * J3_pow_2 * k_s + 8640.0 * J2_pow_2 * k_s_pow_2 * xi + 4320.0 * J2_pow_2 * k_s_pow_2 + 17280.0 * J3_pow_2 * k_s_pow_2 * xi) / (8.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 96.0 * J1_pow_2 * J2_pow_2 * k_s + 156.0 * J1_pow_2 * J3_pow_2 * k_s - 1296.0 * J1_pow_2 * k_s_pow_2 + 156.0 * J2_pow_2 * J3_pow_2 * k_s - 1296.0 * J2_pow_2 * k_s_pow_2 + 2592.0 * J3_pow_2 * k_s_pow_2 - 17280.0 * k_s_pow_3);
    rd4N[2] = (120.0 * J1_pow_3 * J2_pow_2 * J3_pow_2 * xi - 24 * J1_pow_3 * J2_pow_2 * J3_pow_2 - 720.0 * J1_pow_3 * J2_pow_2 * k_s * xi + 288.0 * J1_pow_3 * J2_pow_2 * k_s + 1440.0 * J1_pow_3 * J3_pow_2 * k_s * xi - 648.0 * J1_pow_3 * J3_pow_2 * k_s - 8640.0 * J1_pow_3 * k_s_pow_2 * xi + 4320.0 * J1_pow_3 * k_s_pow_2) / (4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 48.0 * J1_pow_2 * J2_pow_2 * k_s + 78.0 * J1_pow_2 * J3_pow_2 * k_s - 648.0 * J1_pow_2 * k_s_pow_2 + 78.0 * J2_pow_2 * J3_pow_2 * k_s - 648.0 * J2_pow_2 * k_s_pow_2 + 1296.0 * J3_pow_2 * k_s_pow_2 - 8640.0 * k_s_pow_3);

    rd4N[3] = (-120.0 * J1_pow_3 * J2_pow_2 * J3_pow_2 * k02 * xi - 24 * J1_pow_3 * J2_pow_2 * J3_pow_2 * k02 + 720.0 * J1_pow_3 * J2_pow_2 * k02 * k_s * xi + 288.0 * J1_pow_3 * J2_pow_2 * k02 * k_s - 1440.0 * J2_pow_3 * J3_pow_2 * k02 * k_s * xi - 648.0 * J2_pow_3 * J3_pow_2 * k02 * k_s + 8640.0 * J2_pow_3 * k02 * k_s_pow_2 * xi + 4320.0 * J2_pow_3 * k02 * k_s_pow_2) / (4.0 * J1_pow_2 * J2_pow_2 * J3_pow_2 - 48.0 * J1_pow_2 * J2_pow_2 * k_s + 78.0 * J1_pow_2 * J3_pow_2 * k_s - 648.0 * J1_pow_2 * k_s_pow_2 + 78.0 * J2_pow_2 * J3_pow_2 * k_s - 648.0 * J2_pow_2 * k_s_pow_2 + 1296.0 * J3_pow_2 * k_s_pow_2 - 8640.0 * k_s_pow_3);
    // rd4N[4] = 
    // rd4N[5] = 

    // rd4N[6] = 
    // rd4N[7] = 
    // rd4N[8] = 

    rd4N /= std::pow(J, 4);
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::GetNThetaShapeFunctionsValues(
    GlobalSizeVector& rd4N,
    const double J,
    const double ShearFactor,
    const double k0,
    const double xi
    )
{

}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::GetFirstDerivativesNThetaShapeFunctionsValues(
    GlobalSizeVector& rN,
    const double J,
    const double ShearFactor,
    const double k0,
    const double xi
    )
{
    // todo
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::GetNu0ShapeFunctionsValues(
    GlobalSizeVector& rNu,
    const double xi
    )
{
    rNu.clear();
    rNu[0] = 0.5 * xi * (xi - 1.0);
    rNu[3] = 0.5 * xi * (xi + 1.0);
    rNu[6] = 1.0 - std::pow(xi, 2);
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::GetFirstDerivativesNu0ShapeFunctionsValues(
    GlobalSizeVector& rNu,
    const double J,
    const double xi
    )
{
    GetLocalFirstDerivativesNu0ShapeFunctionsValues(rNu, xi);
    rNu /= J;
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::GetLocalFirstDerivativesNu0ShapeFunctionsValues(
    GlobalSizeVector& rNu,
    const double xi
    )
{
    rNu.clear();
    rNu[0] = xi - 0.5;
    rNu[3] = xi + 0.5;
    rNu[6] = -2.0 * xi;
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::GetLocalSecondDerivativesNu0ShapeFunctionsValues(
    GlobalSizeVector& rNu,
    const double xi
    )
{
    rNu.clear();
    rNu[0] = 1.0;
    rNu[3] = 1.0;
    rNu[6] = -2.0;
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::GetSecondDerivativesNu0ShapeFunctionsValues(
    GlobalSizeVector& rNu,
    const double J,
    const double xi
    )
{
    GetLocalSecondDerivativesNu0ShapeFunctionsValues(rNu, xi);
    rNu /= std::pow(J, 2);
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::GetNodalValuesVector(GlobalSizeVector& rNodalValues)
{
    const auto &r_geom = GetGeometry();
    const auto& r_displ_0 = r_geom[0].FastGetSolutionStepValue(DISPLACEMENT);
    const auto& r_displ_1 = r_geom[1].FastGetSolutionStepValue(DISPLACEMENT);
    const auto& r_displ_2 = r_geom[2].FastGetSolutionStepValue(DISPLACEMENT);

    rNodalValues[0] = r_displ_0[0];
    rNodalValues[1] = r_displ_0[1];
    rNodalValues[2] = r_geom[0].FastGetSolutionStepValue(ROTATION_Z);
    rNodalValues[3] = r_displ_1[0];
    rNodalValues[4] = r_displ_1[1];
    rNodalValues[5] = r_geom[1].FastGetSolutionStepValue(ROTATION_Z);
    rNodalValues[6] = r_displ_2[0];
    rNodalValues[7] = r_displ_2[1];
    rNodalValues[8] = r_geom[2].FastGetSolutionStepValue(ROTATION_Z);
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::CalculateGeneralizedStrainsVector(
    VectorType& rStrain,
    const double J,
    const double ShearFactor,
    const double xi,
    const GlobalSizeVector &rNodalValues
    )
{
    // todo
}

/***********************************************************************************/
/***********************************************************************************/

double LinearTimoshenkoCurvedBeamElement2D3N::CalculateAxialStrain(
    const double J,
    const double ShearFactor,
    const double xi,
    const GlobalSizeVector& rNodalValues
    )
{
    return 0.0;
}

/***********************************************************************************/
/***********************************************************************************/

double LinearTimoshenkoCurvedBeamElement2D3N::CalculateShearStrain(
    const double J,
    const double ShearFactor,
    const double xi,
    const GlobalSizeVector& rNodalValues
    )
{
    return 0.0;
}

/***********************************************************************************/
/***********************************************************************************/

double LinearTimoshenkoCurvedBeamElement2D3N::CalculateBendingCurvature(
    const double J,
    const double ShearFactor,
    const double xi,
    const GlobalSizeVector& rNodalValues
    )
{
    return 0.0;
}

/***********************************************************************************/
/***********************************************************************************/

array_1d<double, 3> LinearTimoshenkoCurvedBeamElement2D3N::GetLocalAxesBodyForce(
    const Element &rElement,
    const GeometryType::IntegrationPointsArrayType &rIntegrationPoints,
    const IndexType PointNumber
    )
{
    return array_1d<double, 3>();
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::CalculateLocalSystem(
    MatrixType& rLHS,
    VectorType& rRHS,
    const ProcessInfo& rProcessInfo
    )
{
    rLHS.clear();
    const double xi = -1.0;
    const double J = GetJacobian(xi);
    const double k0 = GetGeometryCurvature(J, xi);

    GlobalSizeVector N;
    GetShapeFunctionsValues(N, J,  xi);

    KRATOS_WATCH(N)
    KRATOS_WATCH(J)
    KRATOS_WATCH(k0)
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::CalculateLeftHandSide(
    MatrixType& rLHS,
    const ProcessInfo& rProcessInfo
    )
{

}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::CalculateRightHandSide(
    VectorType& rRHS,
    const ProcessInfo& rProcessInfo
    )
{

}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::CalculateOnIntegrationPoints(
    const Variable<double>& rVariable,
    std::vector<double>& rOutput,
    const ProcessInfo& rProcessInfo
    )
{

}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::CalculateOnIntegrationPoints(
    const Variable<ConstitutiveLaw::Pointer>& rVariable,
    std::vector<ConstitutiveLaw::Pointer>& rValues,
    const ProcessInfo& rCurrentProcessInfo
    )
{
    if (rVariable == CONSTITUTIVE_LAW) {
        const SizeType integration_points_number = mConstitutiveLawVector.size();
        if (rValues.size() != integration_points_number) {
            rValues.resize(integration_points_number);
        }
        for (IndexType point_number = 0; point_number < integration_points_number; ++point_number) {
            rValues[point_number] = mConstitutiveLawVector[point_number];
        }
    }
}

/***********************************************************************************/
/***********************************************************************************/

int LinearTimoshenkoCurvedBeamElement2D3N::Check(const ProcessInfo& rCurrentProcessInfo) const
{
    KRATOS_TRY;

    return mConstitutiveLawVector[0]->Check(GetProperties(), GetGeometry(), rCurrentProcessInfo);

    KRATOS_CATCH( "" );
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::save(Serializer& rSerializer) const
{
    KRATOS_SERIALIZE_SAVE_BASE_CLASS(rSerializer, Element);
    int IntMethod = int(this->GetIntegrationMethod());
    rSerializer.save("IntegrationMethod",IntMethod);
    rSerializer.save("ConstitutiveLawVector", mConstitutiveLawVector);
}

/***********************************************************************************/
/***********************************************************************************/

void LinearTimoshenkoCurvedBeamElement2D3N::load(Serializer& rSerializer)
{
    KRATOS_SERIALIZE_LOAD_BASE_CLASS(rSerializer, Element);
    int IntMethod;
    rSerializer.load("IntegrationMethod",IntMethod);
    mThisIntegrationMethod = IntegrationMethod(IntMethod);
    rSerializer.load("ConstitutiveLawVector", mConstitutiveLawVector);
}

/***********************************************************************************/
/***********************************************************************************/

} // Namespace Kratos
