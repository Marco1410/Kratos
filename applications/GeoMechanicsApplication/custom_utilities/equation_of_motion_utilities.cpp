// KRATOS___
//     //   ) )
//    //         ___      ___
//   //  ____  //___) ) //   ) )
//  //    / / //       //   / /
// ((____/ / ((____   ((___/ /  MECHANICS
//
//  License:         geo_mechanics_application/license.txt
//
//  Main authors:    Gennady Markelov
//

// Project includes

// Application includes
#include "custom_utilities/equation_of_motion_utilities.h"

namespace Kratos
{

Matrix GeoEquationOfMotionUtilities::CalculateMassMatrix(SizeType      dimension,
                                                         SizeType      number_U_nodes,
                                                         SizeType      NumberIntegrationPoints,
                                                         const Matrix& Nu_container,
                                                         const Vector& rSolidDensities,
                                                         const std::vector<double>& rIntegrationCoefficients)
{
    const SizeType block_element_size = number_U_nodes * dimension;
    Matrix         Nu                 = ZeroMatrix(dimension, block_element_size);
    Matrix         aux_density_matrix = ZeroMatrix(dimension, block_element_size);
    Matrix         density_matrix     = ZeroMatrix(dimension, dimension);
    Matrix         mass_matrix        = ZeroMatrix(block_element_size, block_element_size);

    for (unsigned int g_point = 0; g_point < NumberIntegrationPoints; ++g_point) {
        GeoElementUtilities::AssembleDensityMatrix(density_matrix, rSolidDensities(g_point));
        GeoElementUtilities::CalculateNuMatrix(dimension, number_U_nodes, Nu, Nu_container, g_point);
        noalias(aux_density_matrix) = prod(density_matrix, Nu);
        mass_matrix += prod(trans(Nu), aux_density_matrix) * rIntegrationCoefficients[g_point];
    }
    return mass_matrix;
}

Vector GeoEquationOfMotionUtilities::CalculateDetJsInitialConfiguration(const Geometry<Node>& rGeom,
                                                                        const GeometryData::IntegrationMethod IntegrationMethod)
{
    const Geometry<Node>::IntegrationPointsArrayType& integration_points =
        rGeom.IntegrationPoints(IntegrationMethod);
    const std::size_t number_G_points = integration_points.size();

    Vector det_Js_initial_configuration(number_G_points);
    Matrix J0;
    Matrix inv_J0;
    for (unsigned int g_point = 0; g_point < number_G_points; ++g_point) {
        GeometryUtils::JacobianOnInitialConfiguration(rGeom, integration_points[g_point], J0);
        const Matrix& dN_De = rGeom.ShapeFunctionsLocalGradients(IntegrationMethod)[g_point];
        MathUtils<double>::InvertMatrix(J0, inv_J0, det_Js_initial_configuration(g_point));
    }
    return det_Js_initial_configuration;
}

} /* namespace Kratos.*/
