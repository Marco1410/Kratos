//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//

#if !defined(KRATOS_INTEGRATION_INFO_H_INCLUDED )
#define  KRATOS_INTEGRATION_INFO_H_INCLUDED

// System includes

// External includes

// Project includes
#include "includes/define.h"
#include "geometries/geometry_data.h"
#include "containers/data_value_container.h"

namespace Kratos
{

///@name Kratos Classes
///@{

/// Integration information for the creation of integration points.
/* Within this class distinct information of integration can be
 * stored and processed.
 */
class IntegrationInfo
{
public:
    ///@name Type Definitions
    ///@{

    /// Pointer definition of IntegrationPoint
    KRATOS_CLASS_POINTER_DEFINITION(IntegrationInfo);

    typedef typename Point::IndexType SizeType;
    typedef typename Point::IndexType IndexType;

    /// Integration methods implemented specified within enum.
    typedef GeometryData::IntegrationMethod IntegrationMethod;

    ///@}
    ///@name Type Definitions
    ///@{

    enum class QuadratureMethod
    {
        Default,
        GAUSS,
        EXTENDED_GAUSS
    };

    ///@}
    ///@name Life Cycle
    ///@{

    IntegrationInfo(SizeType LocalSpaceDimension,
        IntegrationMethod ThisIntegrationMethod)
    {
        mNumberOfIntegrationPointsPerSpanVector = std::vector<SizeType>(LocalSpaceDimension);
        mQuadratureMethodVector = std::vector<QuadratureMethod>(LocalSpaceDimension);

        for (IndexType i = 0; i < LocalSpaceDimension; ++i) {
            SetIntegrationMethod(i, ThisIntegrationMethod);
        }
    }

    IntegrationInfo(SizeType LocalSpaceDimension,
        SizeType NumberOfIntegrationPointsPerSpan,
        QuadratureMethod ThisQuadratureMethod = QuadratureMethod::GAUSS)
    {
        mNumberOfIntegrationPointsPerSpanVector = std::vector<SizeType>(LocalSpaceDimension);
        mQuadratureMethodVector = std::vector<QuadratureMethod>(LocalSpaceDimension);

        for (IndexType i = 0; i < LocalSpaceDimension; ++i) {
            mNumberOfIntegrationPointsPerSpanVector[i] = NumberOfIntegrationPointsPerSpan;
            mQuadratureMethodVector[i] = ThisQuadratureMethod;
        }
    }

    IntegrationInfo(
        std::vector<SizeType> NumberOfIntegrationPointsPerSpanVector,
        std::vector<QuadratureMethod> ThisQuadratureMethodVector)
        : mNumberOfIntegrationPointsPerSpanVector(NumberOfIntegrationPointsPerSpanVector)
        , mQuadratureMethodVector(ThisQuadratureMethodVector)
    {
        KRATOS_ERROR_IF(NumberOfIntegrationPointsPerSpanVector.size() != ThisQuadratureMethodVector.size())
            << "The sizes of the NumberOfIntegrationPointsPerSpanVector: " << NumberOfIntegrationPointsPerSpanVector.size()
            << " and the ThisQuadratureMethodVector: " << ThisQuadratureMethodVector.size() << " does not coincide." << std::endl;
    }

    ///@}
    ///@name Dimension
    ///@{

    SizeType LocalSpaceDimension()
    {
        return mNumberOfIntegrationPointsPerSpanVector.size();
    }

    ///@}
    ///@name Data Value Container
    ///@{

    /// Access Data Value Container
    DataValueContainer& GetData()
    {
        return mData;
    }

    /// Const Access Data Value Container
    DataValueContainer const& GetData() const
    {
        return mData;
    }

    /// Set Data Value Container
    void SetData(DataValueContainer const& rThisData)
    {
        mData = rThisData;
    }

    /// Check if the Data exists.
    template<class TDataType> bool Has(const Variable<TDataType>& rThisVariable) const
    {
        return mData.Has(rThisVariable);
    }

    /// Set Data with SetValue and the Variable.
    template<class TVariableType> void SetValue(
        const TVariableType& rThisVariable,
        typename TVariableType::Type const& rValue)
    {
        mData.SetValue(rThisVariable, rValue);
    }

    /// Get Data with GetValue and the Variable.
    template<class TVariableType> typename TVariableType::Type& GetValue(
        const TVariableType& rThisVariable)
    {
        return mData.GetValue(rThisVariable);
    }

    /// Const Get Data with GetValue and the Variable.
    template<class TVariableType> typename TVariableType::Type const& GetValue(
        const TVariableType& rThisVariable) const
    {
        return mData.GetValue(rThisVariable);
    }

    ///@}
    ///@name integration rules
    ///@{

    void SetIntegrationMethod(
        IndexType DimensionIndex,
        IntegrationMethod ThisIntegrationMethod)
    {
        switch (ThisIntegrationMethod) {
        case IntegrationMethod::GI_GAUSS_1:
            mNumberOfIntegrationPointsPerSpanVector[DimensionIndex] = 1;
            mQuadratureMethodVector[DimensionIndex] = QuadratureMethod::GAUSS;
            break;
        case IntegrationMethod::GI_GAUSS_2:
            mNumberOfIntegrationPointsPerSpanVector[DimensionIndex] = 2;
            mQuadratureMethodVector[DimensionIndex] = QuadratureMethod::GAUSS;
            break;
        case IntegrationMethod::GI_GAUSS_3:
            mNumberOfIntegrationPointsPerSpanVector[DimensionIndex] = 3;
            mQuadratureMethodVector[DimensionIndex] = QuadratureMethod::GAUSS;
            break;
        case IntegrationMethod::GI_GAUSS_4:
            mNumberOfIntegrationPointsPerSpanVector[DimensionIndex] = 4;
            mQuadratureMethodVector[DimensionIndex] = QuadratureMethod::GAUSS;
            break;
        case IntegrationMethod::GI_GAUSS_5:
            mNumberOfIntegrationPointsPerSpanVector[DimensionIndex] = 5;
            mQuadratureMethodVector[DimensionIndex] = QuadratureMethod::GAUSS;
            break;
        case IntegrationMethod::GI_EXTENDED_GAUSS_1:
            mNumberOfIntegrationPointsPerSpanVector[DimensionIndex] = 1;
            mQuadratureMethodVector[DimensionIndex] = QuadratureMethod::EXTENDED_GAUSS;
            break;
        case IntegrationMethod::GI_EXTENDED_GAUSS_2:
            mNumberOfIntegrationPointsPerSpanVector[DimensionIndex] = 2;
            mQuadratureMethodVector[DimensionIndex] = QuadratureMethod::EXTENDED_GAUSS;
            break;
        case IntegrationMethod::GI_EXTENDED_GAUSS_3:
            mNumberOfIntegrationPointsPerSpanVector[DimensionIndex] = 3;
            mQuadratureMethodVector[DimensionIndex] = QuadratureMethod::EXTENDED_GAUSS;
            break;
        case IntegrationMethod::GI_EXTENDED_GAUSS_4:
            mNumberOfIntegrationPointsPerSpanVector[DimensionIndex] = 4;
            mQuadratureMethodVector[DimensionIndex] = QuadratureMethod::EXTENDED_GAUSS;
            break;
        case IntegrationMethod::GI_EXTENDED_GAUSS_5:
            mNumberOfIntegrationPointsPerSpanVector[DimensionIndex] = 5;
            mQuadratureMethodVector[DimensionIndex] = QuadratureMethod::EXTENDED_GAUSS;
            break;
        case IntegrationMethod::NumberOfIntegrationMethods:
            mNumberOfIntegrationPointsPerSpanVector[DimensionIndex] = 0;
            mQuadratureMethodVector[DimensionIndex] = QuadratureMethod::Default;
            break;
        }
    }

    SizeType GetNumberOfIntegrationPointsPerSpan(IndexType DimensionIndex) const
    {
        return mNumberOfIntegrationPointsPerSpanVector[DimensionIndex];
    }

    void SetNumberOfIntegrationPointsPerSpan(IndexType DimensionIndex,
        SizeType NumberOfIntegrationPointsPerSpan)
    {
        mNumberOfIntegrationPointsPerSpanVector[DimensionIndex] = NumberOfIntegrationPointsPerSpan;
    }

    QuadratureMethod GetQuadratureMethodVector(IndexType DimensionIndex) const
    {
        return mQuadratureMethodVector[DimensionIndex];
    }

    void SetQuadratureMethodVector(IndexType DimensionIndex,
        QuadratureMethod ThisQuadratureMethod)
    {
        mQuadratureMethodVector[DimensionIndex] = ThisQuadratureMethod;
    }

    /* returns the IntegrationMethod to
     * corresponding to the direction index.
     */
    IntegrationMethod GetIntegrationMethod(
        IndexType DimensionIndex) const
    {
        return GetIntegrationMethod(
            GetNumberOfIntegrationPointsPerSpan(DimensionIndex),
            GetQuadratureMethodVector(DimensionIndex));
    }

    /* Evaluates the corresponding IntegrationMethod to 
     * the number of points and the quadrature method.
     */
    static IntegrationMethod GetIntegrationMethod(
        SizeType NumberOfIntegrationPointsPerSpan,
        QuadratureMethod ThisQuadratureMethod)
    {
        switch (NumberOfIntegrationPointsPerSpan) {
        case 1:
            if (ThisQuadratureMethod == QuadratureMethod::GAUSS) {
                return IntegrationMethod::GI_GAUSS_1;
            } else {
                return IntegrationMethod::GI_EXTENDED_GAUSS_1;
            }
            break;
        case 2:
            if (ThisQuadratureMethod == QuadratureMethod::GAUSS) {
                return IntegrationMethod::GI_GAUSS_2;
            } else {
                return IntegrationMethod::GI_EXTENDED_GAUSS_2;
            }
            break;
        case 3:
            if (ThisQuadratureMethod == QuadratureMethod::GAUSS) {
                return IntegrationMethod::GI_GAUSS_3;
            } else {
                return IntegrationMethod::GI_EXTENDED_GAUSS_3;
            }
            break;
        case 4:
            if (ThisQuadratureMethod == QuadratureMethod::GAUSS) {
                return IntegrationMethod::GI_GAUSS_4;
            } else {
                return IntegrationMethod::GI_EXTENDED_GAUSS_4;
            }
            break;
        case 5:
            if (ThisQuadratureMethod == QuadratureMethod::GAUSS) {
                return IntegrationMethod::NumberOfIntegrationMethods;
            } else {
                return IntegrationMethod::GI_EXTENDED_GAUSS_5;
            }
            break;
        case 0:
            return IntegrationMethod::NumberOfIntegrationMethods;
            break;
        }
        KRATOS_WARNING("Evaluation of Integration Method")
            << "Chosen combination of number of points per span and quadrature method does not has a corresponding IntegrationMethod in the KRATOS core."
            << "NumberOfIntegrationPointsPerSpan: " << NumberOfIntegrationPointsPerSpan << std::endl;
        return IntegrationMethod::NumberOfIntegrationMethods;
    }

    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.
    std::string Info() const
    {
        std::stringstream buffer;
        buffer << " Integration info with local space dimension: " << mNumberOfIntegrationPointsPerSpanVector.size()
            << " and number of integration points per spans: " << mNumberOfIntegrationPointsPerSpanVector;
        return buffer.str();
    }

    /// Print information about this object.
    void PrintInfo(std::ostream& rOStream) const
    {
        rOStream << " Integration info with local space dimension: " << mNumberOfIntegrationPointsPerSpanVector.size()
            << " and number of integration points per spans: " << mNumberOfIntegrationPointsPerSpanVector;
    }

    /// Print object's data.
    void PrintData(std::ostream& rOStream) const
    {
        rOStream << " Integration info with local space dimension: " << mNumberOfIntegrationPointsPerSpanVector.size()
            << " and number of integration points per spans: " << mNumberOfIntegrationPointsPerSpanVector;
    }

    ///@}

private:
    ///@name Member Variables
    ///@{

    std::vector<SizeType> mNumberOfIntegrationPointsPerSpanVector;

    std::vector<QuadratureMethod> mQuadratureMethodVector;

    DataValueContainer mData;

    ///@}

}; // Class IntegrationPoint

///@}
///@name Input and output
///@{


/// input stream function
template<std::size_t TDimension, class TDataType, class TWeightType>
inline std::istream& operator >> (std::istream& rIStream, IntegrationInfo& rThis);

/// output stream function
template<std::size_t TDimension, class TDataType, class TWeightType>
inline std::ostream& operator << (std::ostream& rOStream, const IntegrationInfo& rThis)
{
    rThis.PrintInfo(rOStream);
    rThis.PrintData(rOStream);

    return rOStream;
}
///@}

}  // namespace Kratos.

#endif // KRATOS_INTEGRATION_INFO_H_INCLUDED  defined


