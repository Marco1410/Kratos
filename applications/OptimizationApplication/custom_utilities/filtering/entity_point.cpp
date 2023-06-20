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

// Project includes
#include "includes/define.h"
#include "geometries/point.h"
#include "includes/model_part.h"

// Application includes

// Include base h
#include "entity_point.h"

namespace Kratos {

template<class TEntityType>
EntityPoint<TEntityType>::EntityPoint(
    const TEntityType& rEntity,
    const IndexType Id)
    : Point(GetPoint(rEntity)),
      mId(Id)
{
}

template<class TEntityType>
IndexType EntityPoint<TEntityType>::Id() const
{
    return mId;
}

template<class TEntityType>
constexpr Point EntityPoint<TEntityType>::GetPoint(const TEntityType& rEntity)
{
    if constexpr(std::is_same_v<TEntityType, ModelPart::NodeType>) {
        return rEntity;
    } else {
        return rEntity.GetGeometry().Center();
    }
}

//template instantiations
template KRATOS_API(OPTIMIZATION_APPLICATION) class EntityPoint<ModelPart::NodeType>;
template KRATOS_API(OPTIMIZATION_APPLICATION) class EntityPoint<ModelPart::ConditionType>;
template KRATOS_API(OPTIMIZATION_APPLICATION) class EntityPoint<ModelPart::ElementType>;

} // namespace Kratos