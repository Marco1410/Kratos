//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Philipp Bucher (https://github.com/philbucher)
//

#if !defined (KRATOS_PYRAMID_3D_5_H_INCLUDED)
#define KRATOS_PYRAMID_3D_5_H_INCLUDED

// System includes

// External includes

// Project includes
#include "includes/define.h"
#include "geometries/geometry.h"
#include "integration/pyramid_gauss_legendre_integration_points.h"


namespace Kratos {

///@name Kratos Classes
///@{

/**
 * @class Pyramid3D5
 * @ingroup KratosCore
 * @brief A five node pyramid geometry with linear shape functions
 * @details The node ordering corresponds with:
 *                     4
 *                   ,/|\
 *                 ,/ .'|\
 *               ,/   | | \
 *             ,/    .' | `.
 *           ,/      |  '.  \
 *         ,/       .' w |   \
 *       ,/         |  ^ |    \
 *      0----------.'--|-3    `.
 *       `\        |   |  `\    \
 *         `\     .'   +----`\ - \ -> v
 *           `\   |    `\     `\  \
 *             `\.'      `\     `\`
 *                1----------------2
 *                          `\
 *                             u
 * @author Philipp Bucher
 */
template<class TPointType>
class Pyramid3D5 : public Geometry<TPointType>
{
public:
    ///@}
    ///@name Type Definitions
    ///@{

    /// Geometry as base class.
    typedef Geometry<TPointType> BaseType;

    /// Pointer definition of Pyramid3D5
    KRATOS_CLASS_POINTER_DEFINITION(Pyramid3D5);

    /** Integration methods implemented in geometry.
     */
    typedef GeometryData::IntegrationMethod IntegrationMethod;

    /** Redefinition of template parameter TPointType.
     */
    typedef TPointType PointType;

    /** Type used for indexing in geometry class.std::size_t used for indexing
     point or integration point access methods and also all other
    methods which need point or integration point index.
    */
    typedef typename BaseType::IndexType IndexType;


    /** This typed used to return size or dimension in
     geometry. Dimension, WorkingDimension, PointsNumber and
    ... return this type as their results.
    */
    typedef typename BaseType::SizeType SizeType;

    /** Array of counted pointers to point. This type used to hold
     geometry's points.
    */
    typedef typename BaseType::PointsArrayType PointsArrayType;

    /** This type used for representing an integration point in
     geometry. This integration point is a point with an
    additional weight component.
    */
    typedef typename BaseType::IntegrationPointType IntegrationPointType;

    /** A Vector of IntegrationPointType which used to hold
     integration points related to an integration
    method. IntegrationPoints functions used this type to return
    their results.
    */
    typedef typename BaseType::IntegrationPointsArrayType IntegrationPointsArrayType;

    /** A Vector of IntegrationPointsArrayType which used to hold
     integration points related to different integration method
    implemented in geometry.
    */
    typedef typename BaseType::IntegrationPointsContainerType IntegrationPointsContainerType;

    /** A third order tensor used as shape functions' values
     container.
    */
    typedef typename BaseType::ShapeFunctionsValuesContainerType ShapeFunctionsValuesContainerType;

    /**
     * A third order tensor to hold shape functions' local
     * gradients. ShapefunctionsLocalGradients function return this
     * type as its result.
     */
    typedef typename BaseType::ShapeFunctionsGradientsType ShapeFunctionsGradientsType;

    /**
     * A fourth order tensor used as shape functions' local
     * gradients container in geometry.
     */
    typedef typename BaseType::ShapeFunctionsLocalGradientsContainerType
    ShapeFunctionsLocalGradientsContainerType;

    ///@}
    ///@name Life Cycle
    ///@{

    explicit Pyramid3D5(
        typename PointType::Pointer pPoint1,
        typename PointType::Pointer pPoint2,
        typename PointType::Pointer pPoint3,
        typename PointType::Pointer pPoint4,
        typename PointType::Pointer pPoint5)
        : BaseType( PointsArrayType(), &msGeometryData )
    {
        this->Points().reserve(5);
        this->Points().push_back(pPoint1);
        this->Points().push_back(pPoint2);
        this->Points().push_back(pPoint3);
        this->Points().push_back(pPoint4);
        this->Points().push_back(pPoint5);
    }

    explicit Pyramid3D5( const PointsArrayType& ThisPoints )
        : BaseType( ThisPoints, &msGeometryData )
    {
        KRATOS_ERROR_IF( this->PointsNumber() != 5 ) << "Invalid points number. Expected 5, given " << this->PointsNumber() << std::endl;
    }

    /// Constructor with Geometry Id
    explicit Pyramid3D5(
        const IndexType GeometryId,
        const PointsArrayType& rThisPoints
    ) : BaseType( GeometryId, rThisPoints, &msGeometryData)
    {
        KRATOS_ERROR_IF( this->PointsNumber() != 5 ) << "Invalid points number. Expected 5, given " << this->PointsNumber() << std::endl;
    }

    /// Constructor with Geometry Name
    explicit Pyramid3D5(
        const std::string& rGeometryName,
        const PointsArrayType& rThisPoints
    ) : BaseType(rGeometryName, rThisPoints, &msGeometryData)
    {
        KRATOS_ERROR_IF(this->PointsNumber() != 5) << "Invalid points number. Expected 5, given " << this->PointsNumber() << std::endl;
    }

    /** Copy constructor.
     Construct this geometry as a copy of given geometry.

    @note This copy constructor don't copy the points and new
    geometry shares points with given source geometry. It's
    obvious that any change to this new geometry's point affect
    source geometry's points too.
    */
    // Pyramid3D5(Pyramid3D5 const& rOther)
    // : BaseType(rOther)
    // {
    // }

    /** Copy constructor from a geometry with other point type.
     Construct this geometry as a copy of given geometry which
    has different type of points. The given goemetry's
    TOtherPointType* must be implicity convertible to this
    geometry PointType.

    @note This copy constructor don't copy the points and new
    geometry shares points with given source geometry. It's
    obvious that any change to this new geometry's point affect
    source geometry's points too.
    */
    // template<class TOtherPointType> Pyramid3D5(Pyramid3D5<TOtherPointType> const& rOther)
    // : BaseType(rOther)
    // {
    // }

    ///@}
    ///@name Operators
    ///@{

    /** Assignment operator.

    @note This operator don't copy the points and this
    geometry shares points with given source geometry. It's
    obvious that any change to this geometry's point affect
    source geometry's points too.

    @see Clone
    @see ClonePoints
    */
    Pyramid3D5& operator=(const Pyramid3D5& rOther)
    {
        BaseType::operator=(rOther);

        return *this;
    }

    /** Assignment operator for geometries with different point type.

    @note This operator don't copy the points and this
    geometry shares points with given source geometry. It's
    obvious that any change to this geometry's point affect
    source geometry's points too.

    @see Clone
    @see ClonePoints
    */
    template<class TOtherPointType>
    Pyramid3D5& operator=(Pyramid3D5<TOtherPointType> const & rOther)
    {
        BaseType::operator=(rOther);

        return *this;
    }

    ///@}
    ///@name Informations
    ///@{

    /** This method calculate and return volume of this
     geometry. For one and two dimensional geometry it returns
    zero and for three dimensional it gives volume of geometry.

    @return double value contains volume.
    @see Length()
    @see Area()
    @see DomainSize()
    */
    double Volume() const override
    {
        Vector temp;
        this->DeterminantOfJacobian(temp, msGeometryData.DefaultIntegrationMethod());
        const IntegrationPointsArrayType& integration_points = this->IntegrationPoints(msGeometryData.DefaultIntegrationMethod());
        double vol = 0.00;

        for (std::size_t i=0; i<integration_points.size(); ++i) {
            vol += temp[i] * integration_points[i].Weight();
        }

        return vol;
    }

    /**
     * Calculates the values of all shape function in all integration points.
     * Integration points are expected to be given in local coordinates
     *
     * @param ThisMethod the current integration method
     * @return the matrix of values of every shape function in each integration point
     *
     */

    static Matrix CalculateShapeFunctionsIntegrationPointsValues(typename BaseType::IntegrationMethod ThisMethod)
    {
        IntegrationPointsContainerType all_integration_points = AllIntegrationPoints();
        IntegrationPointsArrayType integration_points = all_integration_points[ThisMethod];
        //number of integration points
        const int integration_points_number = integration_points.size();
        //number of nodes in current geometry
        const int points_number = 6;
        //setting up return matrix
        Matrix shape_function_values( integration_points_number, points_number );
        //loop over all integration points

        // for ( int pnt = 0; pnt < integration_points_number; pnt++ ) {
        //     shape_function_values( pnt, 0 ) = ( 1.0
        //                                         - integration_points[pnt].X()
        //                                         - integration_points[pnt].Y()
        //                                         - integration_points[pnt].Z()
        //                                         + ( integration_points[pnt].X() * integration_points[pnt].Z() )
        //                                         + ( integration_points[pnt].Y() * integration_points[pnt].Z() ) );
        //     shape_function_values( pnt, 1 ) = integration_points[pnt].X()
        //                                       - ( integration_points[pnt].X() * integration_points[pnt].Z() );
        //     shape_function_values( pnt, 2 ) = integration_points[pnt].Y()
        //                                       - ( integration_points[pnt].Y() * integration_points[pnt].Z() );
        //     shape_function_values( pnt, 3 ) = integration_points[pnt].Z()
        //                                       - ( integration_points[pnt].X() * integration_points[pnt].Z() )
        //                                       - ( integration_points[pnt].Y() * integration_points[pnt].Z() );
        //     shape_function_values( pnt, 4 ) = ( integration_points[pnt].X() * integration_points[pnt].Z() );
        //     shape_function_values( pnt, 5 ) = ( integration_points[pnt].Y() * integration_points[pnt].Z() );
        // }

        return shape_function_values;
    }

    /**
     * Calculates the local gradients of all shape functions in all integration points.
     * Integration points are expected to be given in local coordinates
     *
     * @param ThisMethod the current integration method
     * @return the vector of the gradients of all shape functions
     * in each integration point
     *
     */
    static ShapeFunctionsGradientsType CalculateShapeFunctionsIntegrationPointsLocalGradients(typename BaseType::IntegrationMethod ThisMethod)
    {
        IntegrationPointsContainerType all_integration_points =
            AllIntegrationPoints();
        IntegrationPointsArrayType integration_points =
            all_integration_points[ThisMethod];
        //number of integration points
        const int integration_points_number = integration_points.size();
        ShapeFunctionsGradientsType d_shape_f_values( integration_points_number );
        //initialising container
        //loop over all integration points

        // for ( int pnt = 0; pnt < integration_points_number; pnt++ )
        // {
        //     Matrix result = ZeroMatrix( 6, 3 );
        //     result( 0, 0 ) = -1.0 + integration_points[pnt].Z();
        //     result( 0, 1 ) = -1.0 + integration_points[pnt].Z();
        //     result( 0, 2 ) = -1.0 + integration_points[pnt].X() + integration_points[pnt].Y();
        //     result( 1, 0 ) =  1.0 - integration_points[pnt].Z();
        //     result( 1, 1 ) =  0.0;
        //     result( 1, 2 ) =  -integration_points[pnt].X();
        //     result( 2, 0 ) =  0.0;
        //     result( 2, 1 ) =  1.0 - integration_points[pnt].Z();
        //     result( 2, 2 ) =  -integration_points[pnt].Y();
        //     result( 3, 0 ) =  -integration_points[pnt].Z();
        //     result( 3, 1 ) =  -integration_points[pnt].Z();
        //     result( 3, 2 ) =  1.0 - integration_points[pnt].X() - integration_points[pnt].Y();
        //     result( 4, 0 ) =  integration_points[pnt].Z();
        //     result( 4, 1 ) =  0.0;
        //     result( 4, 2 ) =  integration_points[pnt].X();
        //     result( 5, 0 ) =  0.0;
        //     result( 5, 1 ) =  integration_points[pnt].Z();
        //     result( 5, 2 ) =  integration_points[pnt].Y();
        //     d_shape_f_values[pnt] = result;
        // }

        return d_shape_f_values;
    }

    ///@}
    ///@name Input and output
    ///@{

    /** Turn back information as a string.

    @return String contains information about this geometry.
    @see PrintData()
    @see PrintInfo()
    */
    std::string Info() const override
    {
        return "3 dimensional pyramid with four nodes in 3D space";
    }

    /** Print information about this object.

    @param rOStream Stream to print into it.
    @see PrintData()
    @see Info()
    */
    void PrintInfo(std::ostream& rOStream) const override
    {
        rOStream << Info();
    }

    /** Print geometry's data into given stream. Prints it's points
     by the order they stored in the geometry and then center
    point of geometry.

    @param rOStream Stream to print into it.
    @see PrintInfo()
    @see Info()
    */
    void PrintData(std::ostream& rOStream) const override
    {
        BaseType::PrintData(rOStream);
    }

    ///@}

private:
    ///@name Static Member Variables
    ///@{

    static const GeometryData msGeometryData;

    static const GeometryDimension msGeometryDimension;

    ///@}
    ///@name Serialization
    ///@{

    friend class Serializer;

    void save( Serializer& rSerializer ) const override
    {
        KRATOS_SERIALIZE_SAVE_BASE_CLASS( rSerializer, BaseType );
    }

    void load( Serializer& rSerializer ) override
    {
        KRATOS_SERIALIZE_LOAD_BASE_CLASS( rSerializer, BaseType );
    }

    Pyramid3D5() : BaseType( PointsArrayType(), &msGeometryData ) {}

    ///@}
    ///@name Private Operations
    ///@{

    static const IntegrationPointsContainerType AllIntegrationPoints()
    {
        IntegrationPointsContainerType integration_points =
        {
            {
                Quadrature < PyramidGaussLegendreIntegrationPoints1,
                3, IntegrationPoint<3> >::GenerateIntegrationPoints(),
                Quadrature < PyramidGaussLegendreIntegrationPoints2,
                3, IntegrationPoint<3> >::GenerateIntegrationPoints(),
                Quadrature < PyramidGaussLegendreIntegrationPoints3,
                3, IntegrationPoint<3> >::GenerateIntegrationPoints(),
                Quadrature < PyramidGaussLegendreIntegrationPoints4,
                3, IntegrationPoint<3> >::GenerateIntegrationPoints(),
                Quadrature < PyramidGaussLegendreIntegrationPoints5,
                3, IntegrationPoint<3> >::GenerateIntegrationPoints()
            }
        };
        return integration_points;
    }

    static const ShapeFunctionsValuesContainerType AllShapeFunctionsValues()
    {
        ShapeFunctionsValuesContainerType shape_functions_values =
        {
            {
                Pyramid3D5<TPointType>::CalculateShapeFunctionsIntegrationPointsValues(
                    GeometryData::GI_GAUSS_1 ),
                Pyramid3D5<TPointType>::CalculateShapeFunctionsIntegrationPointsValues(
                    GeometryData::GI_GAUSS_2 ),
                Pyramid3D5<TPointType>::CalculateShapeFunctionsIntegrationPointsValues(
                    GeometryData::GI_GAUSS_3 ),
                Pyramid3D5<TPointType>::CalculateShapeFunctionsIntegrationPointsValues(
                    GeometryData::GI_GAUSS_4 ),
                Pyramid3D5<TPointType>::CalculateShapeFunctionsIntegrationPointsValues(
                    GeometryData::GI_GAUSS_5 )
            }
        };
        return shape_functions_values;
    }

    /**
     * TODO: TO BE VERIFIED
     */
    static const ShapeFunctionsLocalGradientsContainerType AllShapeFunctionsLocalGradients()
    {
        ShapeFunctionsLocalGradientsContainerType shape_functions_local_gradients =
        {
            {
                Pyramid3D5<TPointType>::CalculateShapeFunctionsIntegrationPointsLocalGradients(
                    GeometryData::GI_GAUSS_1 ),
                Pyramid3D5<TPointType>::CalculateShapeFunctionsIntegrationPointsLocalGradients(
                    GeometryData::GI_GAUSS_2 ),
                Pyramid3D5<TPointType>::CalculateShapeFunctionsIntegrationPointsLocalGradients(
                    GeometryData::GI_GAUSS_3 ),
                Pyramid3D5<TPointType>::CalculateShapeFunctionsIntegrationPointsLocalGradients(
                    GeometryData::GI_GAUSS_4 ),
                Pyramid3D5<TPointType>::CalculateShapeFunctionsIntegrationPointsLocalGradients(
                    GeometryData::GI_GAUSS_5 )
            }
        };
        return shape_functions_local_gradients;
    }

    ///@}
    ///@name Private  Access
    ///@{


    ///@}
    ///@name Private Inquiry
    ///@{


    ///@}
    ///@name Private Friends
    ///@{

    template<class TOtherPointType> friend class Pyramid3D5;

    ///@}

}; // Class Geometry

///@}

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{

/// input stream function
template<class TPointType>
inline std::istream& operator >> (std::istream& rIStream,
                    Pyramid3D5<TPointType>& rThis);

/// output stream function
template<class TPointType>
inline std::ostream& operator << (std::ostream& rOStream,
                    const Pyramid3D5<TPointType>& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
}
///@}

template<class TPointType> const
GeometryData Pyramid3D5<TPointType>::msGeometryData(
    &msGeometryDimension,
    GeometryData::GI_GAUSS_2,
    Pyramid3D5<TPointType>::AllIntegrationPoints(),
    Pyramid3D5<TPointType>::AllShapeFunctionsValues(),
    AllShapeFunctionsLocalGradients()
);

template<class TPointType> const
GeometryDimension Pyramid3D5<TPointType>::msGeometryDimension(
    3, 3, 3);

}  // namespace Kratos.

#endif // KRATOS_PYRAMID_3D_5_H_INCLUDED defined
