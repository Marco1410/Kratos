//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Alejandro Cornejo
//                   Riccardo Rossi
//                   Carlos Roig
//
//

// System includes

// External includes

// Project includes
#include "includes/table_accessor.h"
#include "includes/properties.h"

namespace Kratos
{

/***********************************************************************************/
/***********************************************************************************/

double TableAccessor::GetValue(
    const Variable<double>& rVariable,
    const Properties& rProperties,
    const GeometryType& rGeometry,
    const Vector& rShapeFunctionVector,
    const ProcessInfo& rProcessInfo
    ) const
{
    return GetValueFromTable(*mpInputVariable, rVariable, rProperties, rGeometry, rShapeFunctionVector, rProcessInfo);
}

/***********************************************************************************/
/***********************************************************************************/

double TableAccessor::GetValueFromTable(
        const Variable<double> &rIndependentVariable,
        const Variable<double> &rDependentVariable,
        const Properties& rProperties,
        const GeometryType& rGeometry,
        const Vector& rShapeFunctionVector,
        const ProcessInfo& rProcessInfo) const
{
    double independent_at_gauss = 0.0;
    if (mInputVariableType == Globals::DataLocation::NodeHistorical) {
        KRATOS_ERROR_IF_NOT(rGeometry[0].SolutionStepsDataHas(rIndependentVariable)) << "The Variable " << rIndependentVariable.Name() << " is not available at the nodes of the Geometry to retrieve Table values." << std::endl;
        for (SizeType i = 0; i < rShapeFunctionVector.size(); ++i) {
            KRATOS_DEBUG_ERROR_IF_NOT(rGeometry[i].SolutionStepsDataHas(rIndependentVariable)) << "The Variable " << rIndependentVariable.Name() << " is not available at the nodes of the Geometry to retrieve Table values." << std::endl;
            const double nodal_value = rGeometry[i].FastGetSolutionStepValue(rIndependentVariable);
            independent_at_gauss += nodal_value * rShapeFunctionVector[i];
        }
    } else if (mInputVariableType == Globals::DataLocation::NodeNonHistorical) {
        KRATOS_ERROR_IF_NOT(rGeometry[0].Has(rIndependentVariable)) << "The Variable " << rIndependentVariable.Name() << " is not available at the nodes of the Geometry to retrieve Table values." << std::endl;
        for (SizeType i = 0; i < rShapeFunctionVector.size(); ++i) {
            KRATOS_DEBUG_ERROR_IF_NOT(rGeometry[i].Has(rIndependentVariable)) << "The Variable " << rIndependentVariable.Name() << " is not available at the nodes of the Geometry to retrieve Table values." << std::endl;
            const double nodal_value = rGeometry[i].GetValue(rIndependentVariable);
            independent_at_gauss += nodal_value * rShapeFunctionVector[i];
        }
    } else if (mInputVariableType == Globals::DataLocation::Element) {
        KRATOS_ERROR_IF_NOT(rGeometry.Has(rIndependentVariable)) << "The Variable " << rIndependentVariable.Name() << " is not available at the Geometry to retrieve Table values." << std::endl;
        independent_at_gauss = rGeometry.GetValue(rIndependentVariable);
    } else {
        KRATOS_ERROR << "The table_input_variable_type is incorrect or not supported. Types available are : nodal_historical, nodal_non_historical and elemental_non_historical" << std::endl;
    }

    // Retrieve the dependent variable from the table
    const auto& r_table = rProperties.GetTable(rIndependentVariable, rDependentVariable);
    return r_table.GetValue(independent_at_gauss);
}

/***********************************************************************************/
/***********************************************************************************/

Accessor::UniquePointer TableAccessor::Clone() const
{
    return Kratos::make_unique<TableAccessor>(*this);
}

} // namespace Kratos
