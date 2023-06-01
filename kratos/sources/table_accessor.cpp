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
    // KRATOS_ERROR << "You are calling the GetValue of the virtual Accessor class..." << std::endl;
    return 0.0;
}

/***********************************************************************************/
/***********************************************************************************/

Accessor::UniquePointer TableAccessor::Clone() const
{
    return Kratos::make_unique<TableAccessor>(*this);
}

} // namespace Kratos
