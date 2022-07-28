#pragma once
//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Mohamed Nabi
//                   
//                   
//  contributors:    
//                   
//


#if !defined(KRATOS_TRIANGLE_2D_15_H_INCLUDED )
#define  KRATOS_TRIANGLE_2D_15_H_INCLUDED

// System includes

// External includes

// Project includes
#include "custom_elements/line_2d_5.hpp"
#include "integration/triangle_gauss_legendre_integration_points.h"

namespace Kratos
{
    ///@name Kratos Globals
    ///@{

    ///@}
    ///@name Type Definitions
    ///@{

    ///@}
    ///@name  Enum's
    ///@{

    ///@}
    ///@name  Functions
    ///@{

    ///@}
    ///@name Kratos Classes
    ///@{

    /**
     * @class Triangle2D15
     * @ingroup KratosCore
     * @brief A fifteen node 2D triangular geometry with quadratic shape functions
     * @details While the shape functions are only defined in 2D it is possible to define an arbitrary orientation in space. Thus it can be used for defining surfaces on 3D elements.
     * The node ordering corresponds with:
     *          2
     *          |`\
     *          9  8
     *          |   `\
     *          10 14 7
     *          |      '\
     *          11 12 13 6
     *          |         `\
     *          0--3--4--5--1
     * @author Mohamed Nabi
     * @author 
     * @author 
     */
    template<class TPointType> class Triangle2D15
        : public Geometry<TPointType>
    {
    public:
        ///@}
        ///@name Type Definitions
        ///@{

        /**
         * Geometry as base class.
         */
        typedef Geometry<TPointType> BaseType;

        /**
         * Type of edge geometry
         */
        typedef Line2D5<TPointType> EdgeType;

        /**
         * Pointer definition of Triangle2D15
         */
        KRATOS_CLASS_POINTER_DEFINITION(Triangle2D15);

        /**
         * Integration methods implemented in geometry.
         */
        typedef GeometryData::IntegrationMethod IntegrationMethod;

        /**
         * A Vector of counted pointers to Geometries.
         * Used for returning edges of the geometry.
         */
        typedef typename BaseType::GeometriesArrayType GeometriesArrayType;

        /**
         * Redefinition of template parameter TPointType.
         */
        typedef TPointType PointType;

        /**
         * Type used for indexing in geometry class.
         * std::size_t used for indexing
         * point or integration point access methods and also all other
         * methods which need point or integration point index.
         */
        typedef typename BaseType::IndexType IndexType;

        /**
         * This type is used to return size or dimension in
         * geometry. Dimension, WorkingDimension, PointsNumber and
         * ... return this type as their results.
         */
        typedef typename BaseType::SizeType SizeType;

        /**
         * Array of counted pointers to point.
         * This type used to hold geometry's points.
         */
        typedef  typename BaseType::PointsArrayType PointsArrayType;

        /**
         * Array of coordinates. Can be Nodes, Points or IntegrationPoints
         */
        typedef typename BaseType::CoordinatesArrayType CoordinatesArrayType;

        /**
         * This type used for representing an integration point in geometry.
         * This integration point is a point with an additional weight component.
         */
        typedef typename BaseType::IntegrationPointType IntegrationPointType;

        /**
         * A Vector of IntegrationPointType which used to hold
         * integration points related to an integration
         * method.
         * IntegrationPoints functions used this type to return
         * their results.
         */
        typedef typename BaseType::IntegrationPointsArrayType IntegrationPointsArrayType;

        /**
         * A Vector of IntegrationPointsArrayType which used to hold
         * integration points related to different integration method
         * implemented in geometry.
         */
        typedef typename BaseType::IntegrationPointsContainerType IntegrationPointsContainerType;

        /**
         * A third order tensor used as shape functions' values
         * container.
         */
        typedef typename BaseType::ShapeFunctionsValuesContainerType ShapeFunctionsValuesContainerType;

        /**
         * A fourth order tensor used as shape functions' local
         * gradients container in geometry.
         */
        typedef typename BaseType::ShapeFunctionsLocalGradientsContainerType ShapeFunctionsLocalGradientsContainerType;

        /**
         * A third order tensor to hold jacobian matrices evaluated at
         * integration points. Jacobian and InverseOfJacobian functions
         * return this type as their result.
         */
        typedef typename BaseType::JacobiansType JacobiansType;

        /**
         * A third order tensor to hold shape functions' local
         * gradients. ShapefunctionsLocalGradients function return this
         * type as its result.
         */
        typedef typename BaseType::ShapeFunctionsGradientsType ShapeFunctionsGradientsType;

        /**
         * A third order tensor to hold shape functions' local second derivatives.
         * ShapefunctionsLocalGradients function return this
         * type as its result.
         */
        typedef typename BaseType::ShapeFunctionsSecondDerivativesType
            ShapeFunctionsSecondDerivativesType;

        /**
        * A third order tensor to hold shape functions' local third derivatives.
        * ShapefunctionsLocalGradients function return this
        * type as its result.
        */
        typedef typename BaseType::ShapeFunctionsThirdDerivativesType
            ShapeFunctionsThirdDerivativesType;

        /**
         * Type of the normal vector used for normal to edges in geometry.
         */
        typedef typename BaseType::NormalType NormalType;

        ///@}
        ///@name Life Cycle
        ///@{
        
        // ========================================================================================
        // ========================================================================================
        Triangle2D15(const PointType& Point01, const PointType& Point02, const PointType& Point03,
            const PointType& Point04, const PointType& Point05, const PointType& Point06,
            const PointType& Point07, const PointType& Point08, const PointType& Point09,
            const PointType& Point10, const PointType& Point11, const PointType& Point12,
            const PointType& Point13, const PointType& Point14, const PointType& Point15)
            : BaseType(PointsArrayType(), &msGeometryData)
        {
            this->Points().push_back(typename PointType::Pointer(new PointType(Point01)));
            this->Points().push_back(typename PointType::Pointer(new PointType(Point02)));
            this->Points().push_back(typename PointType::Pointer(new PointType(Point03)));
            this->Points().push_back(typename PointType::Pointer(new PointType(Point04)));
            this->Points().push_back(typename PointType::Pointer(new PointType(Point05)));
            this->Points().push_back(typename PointType::Pointer(new PointType(Point06)));
            this->Points().push_back(typename PointType::Pointer(new PointType(Point07)));
            this->Points().push_back(typename PointType::Pointer(new PointType(Point08)));
            this->Points().push_back(typename PointType::Pointer(new PointType(Point09)));
            this->Points().push_back(typename PointType::Pointer(new PointType(Point10)));
            this->Points().push_back(typename PointType::Pointer(new PointType(Point11)));
            this->Points().push_back(typename PointType::Pointer(new PointType(Point12)));
            this->Points().push_back(typename PointType::Pointer(new PointType(Point13)));
            this->Points().push_back(typename PointType::Pointer(new PointType(Point14)));
            this->Points().push_back(typename PointType::Pointer(new PointType(Point15)));
        }

        // ========================================================================================
        // ========================================================================================
        Triangle2D15(typename PointType::Pointer pPoint01, typename PointType::Pointer pPoint02,
                     typename PointType::Pointer pPoint03, typename PointType::Pointer pPoint04,
                     typename PointType::Pointer pPoint05, typename PointType::Pointer pPoint06,
                     typename PointType::Pointer pPoint07, typename PointType::Pointer pPoint08,
                     typename PointType::Pointer pPoint09, typename PointType::Pointer pPoint10,
                     typename PointType::Pointer pPoint11, typename PointType::Pointer pPoint12,
                     typename PointType::Pointer pPoint13, typename PointType::Pointer pPoint14,
                     typename PointType::Pointer pPoint15) : BaseType(PointsArrayType(), &msGeometryData)
        {
            this->Points().push_back(pPoint01);
            this->Points().push_back(pPoint02);
            this->Points().push_back(pPoint03);
            this->Points().push_back(pPoint04);
            this->Points().push_back(pPoint05);
            this->Points().push_back(pPoint06);
            this->Points().push_back(pPoint07);
            this->Points().push_back(pPoint08);
            this->Points().push_back(pPoint09);
            this->Points().push_back(pPoint10);
            this->Points().push_back(pPoint11);
            this->Points().push_back(pPoint12);
            this->Points().push_back(pPoint13);
            this->Points().push_back(pPoint14);
            this->Points().push_back(pPoint15);
        }

        // ========================================================================================
        // ========================================================================================
        Triangle2D15(const PointsArrayType& ThisPoints) : BaseType(ThisPoints, &msGeometryData)
        {
            if (this->PointsNumber() != 15) {
                KRATOS_ERROR << "Invalid points number. Expected 15, given " << this->PointsNumber()
                    << std::endl;
            }
        }

        // ========================================================================================
        // ========================================================================================
        /// Constructor with Geometry Id
        explicit Triangle2D15(const IndexType GeometryId, const PointsArrayType& rThisPoints)
            : BaseType(GeometryId, rThisPoints, &msGeometryData)
        {
            KRATOS_ERROR_IF(this->PointsNumber() != 15) << "Invalid points number. Expected 15, given "
                << this->PointsNumber() << std::endl;
        }

        // ========================================================================================
        // ========================================================================================
        /// Constructor with Geometry Name
        explicit Triangle2D15(const std::string& rGeometryName, const PointsArrayType& rThisPoints)
            : BaseType(rGeometryName, rThisPoints, &msGeometryData)
        {
            KRATOS_ERROR_IF(this->PointsNumber() != 15) << "Invalid points number. Expected 15, given " 
                << this->PointsNumber() << std::endl;
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * Copy constructor. 
         * Construct this geometry as a copy of given geometry.
         *
         * @note This copy constructor does not copy the points and new
         * geometry shares points with given source geometry. It is
         * obvious that any change to this new geometry's point affect
         * source geometry's points too.
         */
        Triangle2D15(Triangle2D15 const& rOther) : BaseType(rOther)
        {
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * Copy constructor from a geometry with other point type. 
         * Construct this geometry as a copy of given geometry which 
         * has different type of points. The given goemetry's 
         * TOtherPointType* must be implicity convertible to this 
         * geometry PointType.
         *
         * @note This copy constructor does not copy the points and new
         * geometry shares points with given source geometry. It is
         * obvious that any change to this new geometry's point affect
         * source geometry's points too.
         */
        template<class TOtherPointType> Triangle2D15(Triangle2D15<TOtherPointType> const& rOther)
            : BaseType(rOther)
        {
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * Destructor. Does nothing!!!
         */
        ~Triangle2D15() override {}

        // ========================================================================================
        // ========================================================================================
        GeometryData::KratosGeometryFamily GetGeometryFamily() const override
        {
            return GeometryData::KratosGeometryFamily::Kratos_Triangle;
        }

        // ========================================================================================
        // ========================================================================================
//        GeometryData::KratosGeometryType GetGeometryType() const override
//        {
//            return GeometryData::KratosGeometryType::Kratos_Triangle2D15;
//        }

        // ========================================================================================
        // ========================================================================================
        ///@}
        ///@name Operators
        ///@{

        /**
         * Assignment operator.
         *
         * @note This operator don't copy the points and this
         * geometry shares points with given source geometry. It's
         * obvious that any change to this geometry's point affect
         * source geometry's points too.
         *
         * @see Clone
         * @see ClonePoints
         */
        Triangle2D15& operator=(const Triangle2D15& rOther)
        {
            BaseType::operator=(rOther);
            return *this;
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * Assignment operator for geometries with different point type.
         *
         * @note This operator don't copy the points and this
         * geometry shares points with given source geometry. It's
         * obvious that any change to this geometry's point affect
         * source geometry's points too.
         *
         * @see Clone
         * @see ClonePoints
         */
        template<class TOtherPointType>
        Triangle2D15& operator=(Triangle2D15<TOtherPointType> const& rOther)
        {
            BaseType::operator=(rOther);
            return *this;
        }

        // ========================================================================================
        // ========================================================================================
        ///@}
        ///@name Operations
        ///@{

        /**
         * @brief Creates a new geometry pointer
         * @param rThisPoints the nodes of the new geometry
         * @return Pointer to the new geometry
         */
        typename BaseType::Pointer Create(PointsArrayType const& rThisPoints) const override
        {
            return typename BaseType::Pointer(new Triangle2D15(rThisPoints));
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * @brief Creates a new geometry pointer
         * @param NewGeometryId the ID of the new geometry
         * @param rThisPoints the nodes of the new geometry
         * @return Pointer to the new geometry
         */
        typename BaseType::Pointer Create(const IndexType NewGeometryId,
            PointsArrayType const& rThisPoints) const override
        {
            return typename BaseType::Pointer(new Triangle2D15(NewGeometryId, rThisPoints));
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * @brief Creates a new geometry pointer
         * @param rGeometry reference to an existing geometry
         * @return Pointer to the new geometry
         */
        typename BaseType::Pointer Create(const BaseType& rGeometry) const override
        {
            auto p_geometry = typename BaseType::Pointer(new Triangle2D15(rGeometry.Points()));
            p_geometry->SetData(rGeometry.GetData());
            return p_geometry;
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * @brief Creates a new geometry pointer
         * @param NewGeometryId the ID of the new geometry
         * @param rGeometry reference to an existing geometry
         * @return Pointer to the new geometry
         */
        typename BaseType::Pointer Create(const IndexType NewGeometryId,
            const BaseType& rGeometry) const override
        {
            auto p_geometry = typename BaseType::Pointer(new Triangle2D15(NewGeometryId, rGeometry.Points()));
            p_geometry->SetData(rGeometry.GetData());
            return p_geometry;
        }

        // ========================================================================================
        // ========================================================================================
        /// Returns number of points per direction.
        SizeType PointsNumberInDirection(IndexType LocalDirectionIndex) const override
        {
            if ((LocalDirectionIndex == 0) || (LocalDirectionIndex == 1)) {
                return 5;
            }
            KRATOS_ERROR << "Possible direction index reaches from 0-1. Given direction index: "
                << LocalDirectionIndex << std::endl;
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * returns the local coordinates of all nodes of the current geometry
         * @param rResult a Matrix object that will be overwritten by the result
         * @return the local coordinates of all nodes
         */
        Matrix& PointsLocalCoordinates(Matrix& rResult) const override
        {
            rResult.resize(15, 2, false);
            noalias(rResult) = ZeroMatrix(15, 2);

            rResult(0, 0) = 0.00;
            rResult(0, 1) = 0.00;
            //
            rResult(1, 0) = 1.00;
            rResult(1, 1) = 0.00;
            //
            rResult(2, 0) = 0.00;
            rResult(2, 1) = 1.00;
            //
            rResult(3, 0) = 0.25;
            rResult(3, 1) = 0.00;
            //
            rResult(4, 0) = 0.50;
            rResult(4, 1) = 0.00;
            //
            rResult(5, 0) = 0.75;
            rResult(5, 1) = 0.00;
            //
            rResult(6, 0) = 0.75;
            rResult(6, 1) = 0.25;
            //
            rResult(7, 0) = 0.50;
            rResult(7, 1) = 0.50;
            //
            rResult(8, 0) = 0.25;
            rResult(8, 1) = 0.75;
            //
            rResult(9, 0) = 0.00;
            rResult(9, 1) = 0.75;
            //
            rResult(10, 0) = 0.00;
            rResult(10, 1) = 0.50;
            //
            rResult(11, 0) = 0.00;
            rResult(11, 1) = 0.25;
            //
            rResult(12, 0) = 0.25;
            rResult(12, 1) = 0.25;
            //
            rResult(13, 0) = 0.50;
            rResult(13, 1) = 0.25;
            //
            rResult(14, 0) = 0.25;
            rResult(14, 1) = 0.50;
            //
            return rResult;
        }

        // ========================================================================================
        // ========================================================================================
        ///@}
        ///@name Information
        ///@{

        /**
         * This method calculates and returns Length or charactereistic
         * length of this geometry depending on it's dimension.
         * For one dimensional geometry for example Line it returns
         * length of it and for the other geometries it gives Characteristic
         * length otherwise.
         * In the current geometry this function returns the determinant of
         * jacobian
         *
         * @return double value contains length or Characteristic
         * length
         * @see Area()
         * @see Volume()
         * @see DomainSize()
         */
        double Length() const override
        {
            return sqrt(fabs(Area()));
        }

        // ========================================================================================
        // ========================================================================================
        /** This method calculates and returns area or surface area of
         * this geometry depending to it's dimension. For one dimensional
         * geometry it returns zero, for two dimensional it gives area
         * and for three dimensional geometries it gives surface area.
         *
         * @return double value contains area or surfacede
         * area.
         * @see Length()
         * @see Volume()
         * @see DomainSize()
         */
        double Area() const override
        {
            Vector temp;
            this->DeterminantOfJacobian(temp, msGeometryData.DefaultIntegrationMethod());
            const IntegrationPointsArrayType& integration_points = this->IntegrationPoints(msGeometryData.DefaultIntegrationMethod());
            double area = 0.00;
            for (unsigned int i = 0; i < integration_points.size(); i++)
            {
                area += temp[i] * integration_points[i].Weight();
            }
            return area;
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * This method calculates and returns the volume of this geometry.
         * This method calculates and returns the volume of this geometry.
         *
         * This method uses the V = (A x B) * C / 6 formula.
         *
         * @return double value contains length, area or volume.
         *
         * @see Length()
         * @see Area()
         * @see Volume()
         *
         * @todo might be necessary to reimplement
         */
        double Volume() const override
        {
            return Area();
        }

        // ========================================================================================
        // ========================================================================================
        /** This method calculates and returns length, area or volume of
         * this geometry depending to it's dimension. For one dimensional
         * geometry it returns its length, for two dimensional it gives area
         * and for three dimensional geometries it gives its volume.
         *
         * @return double value contains length, area or volume.
         * @see Length()
         * @see Area()
         * @see Volume()
         */
        double DomainSize() const override
        {
            return Area();
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * @brief Returns whether given arbitrary point is inside the Geometry and the respective
         * local point for the given global point
         * @param rPoint The point to be checked if is inside o note in global coordinates
         * @param rResult The local coordinates of the point
         * @param Tolerance The  tolerance that will be considered to check if the point is inside or not
         * @return True if the point is inside, false otherwise
         */
        bool IsInside(const CoordinatesArrayType& rPoint, CoordinatesArrayType& rResult,
            const double Tolerance = std::numeric_limits<double>::epsilon()) const override
        {
            this->PointLocalCoordinates(rResult, rPoint);
            if ((rResult[0] >= (0.0 - Tolerance)) && (rResult[0] <= (1.0 + Tolerance))) {
                if ((rResult[1] >= (0.0 - Tolerance)) && (rResult[1] <= (1.0 + Tolerance))) {
                    if ((rResult[0] + rResult[1]) <= (1.0 + Tolerance)) {
                        return true;
                    }
                }
            }
            return false;
        }

        // ========================================================================================
        // ========================================================================================
        ///@}
        ///@name Shape Function
        ///@{

        /**
         * @brief Returns vector of shape function values at local coordinate.
         *
         * For a definition of the shape functions see, e.g.,
         * https://www.colorado.edu/engineering/CAS/courses.d/IFEM.d/IFEM.Ch18.d/IFEM.Ch18.pdf.
         */
        Vector& ShapeFunctionsValues(Vector& rResult, const CoordinatesArrayType& rCoordinates) const override
        {
            if (rResult.size() != 15) rResult.resize(15, false);
            //
            double cof1 = 128.0 / 3.0;
            double cof2 = 32.0 / 3.0;
            double xai = rCoordinates[0];
            double eta = rCoordinates[1];
            double zta = 1.0 - xai - eta;
            //
            rResult[0]  = zta * (zta - 0.25) * (zta - 0.5) * (zta - 0.75) * cof2;
            rResult[1]  = xai * (xai - 0.25) * (xai - 0.5) * (xai - 0.75) * cof2;
            rResult[2]  = eta * (eta - 0.25) * (eta - 0.5) * (eta - 0.75) * cof2;
            rResult[3]  = xai * zta * (zta - 0.25) * (zta - 0.5) * cof1;
            rResult[4]  = xai * zta * (zta - 0.25) * (xai - 0.25) * 64.0;
            rResult[5]  = xai * zta * (xai - 0.25) * (xai - 0.5) * cof1;
            rResult[6]  = xai * eta * (xai - 0.25) * (xai - 0.5) * cof1;
            rResult[7]  = xai * eta * (xai - 0.25) * (eta - 0.25) * 64.0;
            rResult[8]  = xai * eta * (eta - 0.25) * (eta - 0.5) * cof1;
            rResult[9]  = eta * zta * (eta - 0.25) * (eta - 0.5) * cof1;
            rResult[10] = eta * zta * (eta - 0.25) * (zta - 0.25) * 64.0;
            rResult[11] = eta * zta * (zta - 0.25) * (zta - 0.5) * cof1;
            rResult[12] = xai * eta * zta * (zta - 0.25) * 128.0;
            rResult[13] = xai * eta * zta * (xai - 0.25) * 128.0;
            rResult[14] = xai * eta * zta * (eta - 0.25) * 128.0;
            //
            return rResult;
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * Calculates the value of a given shape function at a given point.
         *
         * @param ShapeFunctionIndex The number of the desired shape function
         * @param rPoint the given point in local coordinates at which the value of the shape
         * function is calculated
         *
         * @return the value of the shape function at the given point
         */
        double ShapeFunctionValue(IndexType ShapeFunctionIndex,
            const CoordinatesArrayType& rPoint) const override
        {
            double cof1 = 128.0 / 3.0;
            double cof2 = 32.0 / 3.0;
            double xai = rPoint[0];
            double eta = rPoint[1];
            double zta = 1.0 - xai - eta;
            double shape = 0.0;
            //
            switch (ShapeFunctionIndex) {
            case 0:
                shape = zta * (zta - 0.25) * (zta - 0.5) * (zta - 0.75) * cof2;
                return(shape);
            case 1:
                shape = xai * (xai - 0.25) * (xai - 0.5) * (xai - 0.75) * cof2;
                return(shape);
            case 2:
                shape = eta * (eta - 0.25) * (eta - 0.5) * (eta - 0.75) * cof2;
                return(shape);
            case 3:
                shape = xai * zta * (zta - 0.25) * (zta - 0.5) * cof1;
                return(shape);
            case 4:
                shape = xai * zta * (zta - 0.25) * (xai - 0.25) * 64.0;
                return(shape);
            case 5:
                shape = xai * zta * (xai - 0.25) * (xai - 0.5) * cof1;
                return(shape);
            case 6:
                shape = xai * eta * (xai - 0.25) * (xai - 0.5) * cof1;
                return(shape);
            case 7:
                shape = xai * eta * (xai - 0.25) * (eta - 0.25) * 64.0;
                return(shape);
            case 8:
                shape = xai * eta * (eta - 0.25) * (eta - 0.5) * cof1;
                return(shape);
            case 9:
                shape = eta * zta * (eta - 0.25) * (eta - 0.5) * cof1;
                return(shape);
            case 10:
                shape = eta * zta * (eta - 0.25) * (zta - 0.25) * 64.0;
                return(shape);
            case 11:
                shape = eta * zta * (zta - 0.25) * (zta - 0.5) * cof1;
                return(shape);
            case 12:
                shape = xai * eta * zta * (zta - 0.25) * 128.0;
                return(shape);
            case 13:
                shape = xai * eta * zta * (xai - 0.25) * 128.0;
                return(shape);
            case 14:
                shape = xai * eta * zta * (eta - 0.25) * 128.0;
                return(shape);
            default:
                KRATOS_ERROR << "Wrong index of shape function!" << *this << std::endl;
            }
            //
            return 0;
        }

        // ========================================================================================
        // ========================================================================================
        ///@}
        ///@name Input and output
        ///@{

        /**
         * Turn back information as a string.
         *
         * @return String contains information about this geometry.
         * @see PrintData()
         * @see PrintInfo()
         */
        std::string Info() const override
        {
            return "2 dimensional triangle with fifteen nodes in 2D space";
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * Print information about this object.
         * @param rOStream Stream to print into it.
         * @see PrintData()
         * @see Info()
         */
        void PrintInfo(std::ostream& rOStream)     const override
        {
            rOStream << "2 dimensional triangle with fifteen nodes in 2D space";
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * Print geometry's data into given stream.
         * Prints it's points
         * by the order they stored in the geometry and then center
         * point of geometry.
         *
         * @param rOStream Stream to print into it.
         * @see PrintInfo()
         * @see Info()
         */
        void PrintData(std::ostream& rOStream) const override
        {
            PrintInfo(rOStream);
            BaseType::PrintData(rOStream);
            std::cout << std::endl;
            Matrix jacobian;
            this->Jacobian(jacobian, PointType());
            rOStream << "    Jacobian in the origin\t : " << jacobian;
        }

        // ========================================================================================
        // ========================================================================================
        ///@}
        ///@name Edge
        ///@{

        /**
         * @brief This method gives you number of all edges of this geometry.
         * @details For example, for a hexahedron, this would be 12
         * @return SizeType containes number of this geometry edges.
         * @see EdgesNumber()
         * @see Edges()
         * @see GenerateEdges()
         * @see FacesNumber()
         * @see Faces()
         * @see GenerateFaces()
         */
        SizeType EdgesNumber() const override
        {
            return 3;
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * @brief This method gives you all edges of this geometry.
         * @details This method will gives you all the edges with one dimension less than this geometry.
         * For example a triangle would return three lines as its edges or a tetrahedral would return four triangle as its edges but won't return its six edge lines by this method.
         * @return GeometriesArrayType containes this geometry edges.
         * @see EdgesNumber()
         * @see Edge()
         */
        GeometriesArrayType GenerateEdges() const override
        {
            GeometriesArrayType edges = GeometriesArrayType();
            edges.push_back(Kratos::make_shared<EdgeType>(this->pGetPoint(0), this->pGetPoint(3), this->pGetPoint(4), this->pGetPoint(5), this->pGetPoint(1)));
            edges.push_back(Kratos::make_shared<EdgeType>(this->pGetPoint(1), this->pGetPoint(6), this->pGetPoint(7), this->pGetPoint(8), this->pGetPoint(2)));
            edges.push_back(Kratos::make_shared<EdgeType>(this->pGetPoint(2), this->pGetPoint(9), this->pGetPoint(10), this->pGetPoint(11), this->pGetPoint(0)));
            return edges;
        }

        // ========================================================================================
        // ========================================================================================
        SizeType FacesNumber() const override
        {
            return 3;
        }

        // ========================================================================================
        // ========================================================================================
        //Connectivities of faces required
        void NumberNodesInFaces(DenseVector<unsigned int>& NumberNodesInFaces) const override
        {
            if (NumberNodesInFaces.size() != 3) NumberNodesInFaces.resize(3, false);
            NumberNodesInFaces[0] = 5;
            NumberNodesInFaces[1] = 5;
            NumberNodesInFaces[2] = 5;
        }

        // ========================================================================================
        // ========================================================================================
        void NodesInFaces(DenseMatrix<unsigned int>& NodesInFaces) const override
        {
            // faces in columns
            if (NodesInFaces.size1() != 6 || NodesInFaces.size2() != 3)
                NodesInFaces.resize(6, 3, false);

            //compatible to Edges() function. Triangle library considers a different order
            NodesInFaces(0, 0) = 0;//face or master node
            NodesInFaces(1, 0) = 1;
            NodesInFaces(2, 0) = 6;
            NodesInFaces(3, 0) = 7;
            NodesInFaces(4, 0) = 8;
            NodesInFaces(5, 0) = 2;
            //
            NodesInFaces(0, 1) = 1;//face or master node
            NodesInFaces(1, 1) = 2;
            NodesInFaces(2, 1) = 9;
            NodesInFaces(3, 1) = 10;
            NodesInFaces(4, 1) = 11;
            NodesInFaces(5, 1) = 0;
            //
            NodesInFaces(0, 2) = 2;//face or master node
            NodesInFaces(1, 2) = 0;
            NodesInFaces(2, 2) = 3;
            NodesInFaces(3, 2) = 4;
            NodesInFaces(4, 2) = 5;
            NodesInFaces(5, 2) = 1;
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * Calculates the local gradients for all integration points for
         * given integration method
         */
        virtual ShapeFunctionsGradientsType ShapeFunctionsLocalGradients(IntegrationMethod ThisMethod)
        {
            ShapeFunctionsGradientsType localGradients 
                = CalculateShapeFunctionsIntegrationPointsLocalGradients(ThisMethod);
            const int integration_points_number = msGeometryData.IntegrationPointsNumber(ThisMethod);
            ShapeFunctionsGradientsType Result(integration_points_number);
            //
            for (int pnt = 0; pnt < integration_points_number; pnt++)
            {
                Result[pnt] = localGradients[pnt];
            }
            return Result;
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * Calculates the local gradients for all integration points for the
         * default integration method
         */
        virtual ShapeFunctionsGradientsType ShapeFunctionsLocalGradients()
        {
            IntegrationMethod ThisMethod = msGeometryData.DefaultIntegrationMethod();
            ShapeFunctionsGradientsType localGradients
                = CalculateShapeFunctionsIntegrationPointsLocalGradients(ThisMethod);
            const int integration_points_number = msGeometryData.IntegrationPointsNumber(ThisMethod);
            ShapeFunctionsGradientsType Result(integration_points_number);
            //
            for (int pnt = 0; pnt < integration_points_number; pnt++)
            {
                Result[pnt] = localGradients[pnt];
            }
            return Result;
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * Calculates the gradients in terms of local coordinates
         * of all shape functions in a given point.
         *
         * @param rPoint the current point at which the gradients are calculated in local
         * coordinates
         * @return the gradients of all shape functions
         * \f$ \frac{\partial N^i}{\partial \xi_j} \f$
         */
        Matrix& ShapeFunctionsLocalGradients(Matrix& rResult, const CoordinatesArrayType& rPoint) const override
        {
            rResult.resize(15, 2, false);
            double x1 = rPoint[0];
            double x2 = x1 * x1;
            double x3 = x2 * x1;
            double y1 = rPoint[1];
            double y2 = y1 * y1;
            double y3 = y2 * y1;
            double tC1 = 1.0 - x1 - y1;
            double tC2 = tC1 * tC1;
            double tC3 = tC2 * tC1;

            //// noalias(rResult) = ZeroMatrix(15, 2);
            rResult = ZeroMatrix(15, 2);
            //
            rResult(0, 0) = -(128.0 * tC3 - 144.0 * tC2 + 44.0 * tC1 - 3.0) / 3.0;
            rResult(0, 1) = -(128.0 * tC3 - 144.0 * tC2 + 44.0 * tC1 - 3.0) / 3.0;

            rResult(1, 0) = (128.0 * x3 - 144.0 * x2 + 44.0 * x1 - 3.0) / 3.0;
            rResult(1, 1) = 0.0;

            rResult(2, 0) = 0.0;
            rResult(2, 1) = (128.0 * y3 - 144.0 * y2 + 44.0 * y1 - 3.0) / 3.0;

            rResult(3, 0) = -128.0 * (tC2 - 0.5 * tC1 + 1.0 / 24.0) * x1 + (128.0 * tC3 - 96.0 * tC2 + 16.0 * tC1) / 3.0;
            rResult(3, 1) = -16.0 * x1 * (24.0 * tC2 - 12.0 * tC1 + 1.0) / 3.0;

            rResult(4, 0) = -128.0 * (x1 - 0.25) * (tC1 - 0.125) * x1 + 128.0 * (x1 - 0.125) * (tC1 - 0.25) * tC1;
            rResult(4, 1) = -4.0 * x1 * (4.0 * x1 - 1.0) * (8.0 * tC1 - 1.0);

            rResult(5, 0) = -(128.0 * x3 - 96.0 * x2 + 16.0 * x1) / 3.0 + 128.0 * (x2 - 0.5 * x1 + 1.0 / 24.0) * tC1;
            rResult(5, 1) = -16.0 * x1 * (8.0 * x2 - 6.0 * x1 + 1.0) / 3.0;

            rResult(6, 0) = 16.0 * y1 * (24.0 * x2 - 12.0 * x1 + 1.0) / 3.0;
            rResult(6, 1) = (128.0 * x3 - 96.0 * x2 + 16.0 * x1) / 3.0;

            rResult(7, 0) = 4.0 * (8.0 * x1 - 1.0) * (4.0 * y1 - 1.0) * y1;
            rResult(7, 1) = 4.0 * (4.0 * x1 - 1.0) * (8.0 * y1 - 1.0) * x1;

            rResult(8, 0) = (128.0 * y3 - 96.0 * y2 + 16.0 * y1) / 3.0;
            rResult(8, 1) = 16.0 * x1 * (24.0 * y2 - 12.0 * y1 + 1.0) / 3.0;

            rResult(9, 0) = -16.0 * y1 * (8.0 * y2 - 6.0 * y1 + 1.0) / 3.0;
            rResult(9, 1) = -(128.0 * y3 - 96.0 * y2 + 16.0 * y1) / 3.0 + 128.0 * (y2 - 0.5 * y1 + 1.0 / 24.0) * tC1;

            rResult(10, 0) = -4.0 * y1 * (4.0 * y1 - 1.0) * (8.0 * tC1 - 1.0);
            rResult(10, 1) = -128.0 * (y1 - 0.25) * (tC1 - 0.125) * y1 + 128.0 * (y1 - 0.125) * tC1 * (tC1 - 0.25);

            rResult(11, 0) = -16.0 * y1 * (24.0 * tC2 - 12.0 * tC1 + 1.0) / 3.0;
            rResult(11, 1) = -128.0 * (tC2 - 0.5 * tC1 + 1.0 / 24.0) * y1 + (128.0 * tC3 - 96.0 * tC2 + 16.0 * tC1) / 3.0;

            rResult(12, 0) = 256.0 * y1 * (-x1 * (tC1 - 0.125) + 0.5 * tC2 - 0.125 * tC1);
            rResult(12, 1) = 256.0 * x1 * (-y1 * (tC1 - 0.125) + 0.5 * tC2 - 0.125 * tC1);

            rResult(13, 0) = -32.0 * y1 * (4.0 * x2 - x1) + 256.0 * (x1 - 0.125) * y1 * tC1;
            rResult(13, 1) = 128.0 * (x1 - 0.25) * (-y1 + tC1) * x1;

            rResult(14, 0) = 128.0 * (y1 - 0.25) * y1 * (-x1 + tC1);
            rResult(14, 1) = -32.0 * x1 * (4.0 * y2 - y1) + 256.0 * (y1 - 0.125) * tC1 * x1;
            //
            return rResult;
        }


        // ========================================================================================
        // ========================================================================================
        /**
         * returns the shape function gradients in an arbitrary point,
         * given in local coordinates
         *
         * @param rResult the matrix of gradients,
         * will be overwritten with the gradients for all
         * shape functions in given point
         * @param rPoint the given point the gradients are calculated in
         */
        virtual Matrix& ShapeFunctionsGradients(Matrix& rResult, CoordinatesArrayType& rPoint)
        {
            rResult.resize(15, 2, false);
            double x1 = rPoint[0];
            double x2 = x1 * x1;
            double x3 = x2 * x1;
            double y1 = rPoint[1];
            double y2 = y1 * y1;
            double y3 = y2 * y1;
            double cor3 = 1.0 - x1 - y1;
            double tC1 = cor3;
            double tC2 = tC1 * tC1;
            double tC3 = tC2 * tC1;

            //// noalias(rResult) = ZeroMatrix(15, 2);
            rResult = ZeroMatrix(15, 2);


            rResult(0, 0) = -(128.0 * tC3 - 144.0 * tC2 + 44.0 * tC1 - 3.0) / 3.0;
            rResult(0, 1) = -(128.0 * tC3 - 144.0 * tC2 + 44.0 * tC1 - 3.0) / 3.0;

            rResult(1, 0) = (128.0 * x3 - 144.0 * x2 + 44.0 * x1 - 3.0) / 3.0;
            rResult(1, 1) = 0.0;

            rResult(2, 0) = 0.0;
            rResult(2, 1) = (128.0 * y3 - 144.0 * y2 + 44.0 * y1 - 3.0) / 3.0;

            rResult(3, 0) = -128.0 * (tC2 - 0.5 * tC1 + 1.0 / 24.0) * x1 + (128.0 * tC3 - 96.0 * tC2 + 16.0 * tC1) / 3.0;
            rResult(3, 1) = -16.0 * x1 * (24.0 * tC2 - 12.0 * tC1 + 1.0) / 3.0;

            rResult(4, 0) = -128.0 * (x1 - 0.25) * (tC1 - 0.125) * x1 + 128.0 * (x1 - 0.125) * (tC1 - 0.25) * tC1;
            rResult(4, 1) = -4.0 * x1 * (4.0 * x1 - 1.0) * (8.0 * tC1 - 1.0);

            rResult(5, 0) = -(128.0 * x3 - 96.0 * x2 + 16.0 * x1) / 3.0 + 128.0 * (x2 - 0.5 * x1 + 1.0 / 24.0) * tC1;
            rResult(5, 1) = -16.0 * x1 * (8.0 * x2 - 6.0 * x1 + 1.0) / 3.0;

            rResult(6, 0) = 16.0 * y1 * (24.0 * x2 - 12.0 * x1 + 1.0) / 3.0;
            rResult(6, 1) = (128.0 * x3 - 96.0 * x2 + 16.0 * x1) / 3.0;

            rResult(7, 0) = 4.0 * (8.0 * x1 - 1.0) * (4.0 * y1 - 1.0) * y1;
            rResult(7, 1) = 4.0 * (4.0 * x1 - 1.0) * (8.0 * y1 - 1.0) * x1;

            rResult(8, 0) = (128.0 * y3 - 96.0 * y2 + 16.0 * y1) / 3.0;
            rResult(8, 1) = 16.0 * x1 * (24.0 * y2 - 12.0 * y1 + 1.0) / 3.0;

            rResult(9, 0) = -16.0 * y1 * (8.0 * y2 - 6.0 * y1 + 1.0) / 3.0;
            rResult(9, 1) = -(128.0 * y3 - 96.0 * y2 + 16.0 * y1) / 3.0 + 128.0 * (y2 - 0.5 * y1 + 1.0 / 24.0) * tC1;

            rResult(10, 0) = -4.0 * y1 * (4.0 * y1 - 1.0) * (8.0 * tC1 - 1.0);
            rResult(10, 1) = -128.0 * (y1 - 0.25) * (tC1 - 0.125) * y1 + 128.0 * (y1 - 0.125) * tC1 * (tC1 - 0.25);

            rResult(11, 0) = -16.0 * y1 * (24.0 * tC2 - 12.0 * tC1 + 1.0) / 3.0;
            rResult(11, 1) = -128.0 * (tC2 - 0.5 * tC1 + 1.0 / 24.0) * y1 + (128.0 * tC3 - 96.0 * tC2 + 16.0 * tC1) / 3.0;

            rResult(12, 0) = 256.0 * y1 * (-x1 * (tC1 - 0.125) + 0.5 * tC2 - 0.125 * tC1);
            rResult(12, 1) = 256.0 * x1 * (-y1 * (tC1 - 0.125) + 0.5 * tC2 - 0.125 * tC1);

            rResult(13, 0) = -32.0 * y1 * (4.0 * x2 - x1) + 256.0 * (x1 - 0.125) * y1 * tC1;
            rResult(13, 1) = 128.0 * (x1 - 0.25) * (-y1 + tC1) * x1;

            rResult(14, 0) = 128.0 * (y1 - 0.25) * y1 * (-x1 + tC1);
            rResult(14, 1) = -32.0 * x1 * (4.0 * y2 - y1) + 256.0 * (y1 - 0.125) * tC1 * x1;

            return rResult;
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * returns the second order derivatives of all shape functions
         * in given arbitrary pointers
         * @param rResult a third order tensor which contains the second derivatives
         * @param rPoint the given point the second order derivatives are calculated in
         */
        ShapeFunctionsSecondDerivativesType& ShapeFunctionsSecondDerivatives(ShapeFunctionsSecondDerivativesType& rResult, const CoordinatesArrayType& rPoint) const override
        {
            if (rResult.size() != this->PointsNumber())
            {
                ShapeFunctionsGradientsType temp(this->PointsNumber());
                rResult.swap(temp);
            }
            double x1 = rPoint[0];
            double x2 = x1 * x1;
            double x3 = x2 * x1;
            double y1 = rPoint[1];
            double y2 = y1 * y1;
            double y3 = y2 * y1;
            //
            rResult[0].resize(2, 2, false);
            rResult[1].resize(2, 2, false);
            rResult[2].resize(2, 2, false);
            rResult[3].resize(2, 2, false);
            rResult[4].resize(2, 2, false);
            rResult[5].resize(2, 2, false);
            rResult[6].resize(2, 2, false);
            rResult[7].resize(2, 2, false);
            rResult[8].resize(2, 2, false);
            rResult[9].resize(2, 2, false);
            rResult[10].resize(2, 2, false);
            rResult[11].resize(2, 2, false);
            rResult[12].resize(2, 2, false);
            rResult[13].resize(2, 2, false);
            rResult[14].resize(2, 2, false);
            //
            rResult[0](0, 0) = 128.0 * (x2 + y2) - 160.0 * (x1 + y1) + 256.0 * x1 * y1 + 140.0 / 3.0;
            rResult[0](0, 1) = 128.0 * (x2 + y2) - 160.0 * (x1 + y1) + 256.0 * x1 * y1 + 140.0 / 3.0;
            rResult[0](1, 0) = 128.0 * (x2 + y2) - 160.0 * (x1 + y1) + 256.0 * x1 * y1 + 140.0 / 3.0;
            rResult[0](1, 1) = 128.0 * (x2 + y2) - 160.0 * (x1 + y1) + 256.0 * x1 * y1 + 140.0 / 3.0;
            //
            rResult[1](0, 0) = 128.0 * x2 - 96.0 * x1 + 44.0 / 3.0;
            rResult[1](0, 1) = 0.0;
            rResult[1](1, 0) = 0.0;
            rResult[1](1, 1) = 0.0;
            //
            rResult[2](0, 0) = 0.0;
            rResult[2](0, 1) = 0.0;
            rResult[2](1, 0) = 0.0;
            rResult[2](1, 1) = 128.0 * y2 - 96.0 * y1 + 44.0 / 3.0;
            //
            rResult[3](0, 0) = -512.0 * x2 - 768.0 * x1 * y1 - 256.0 * y2 + 576.0 * x1 + 384.0 * y1 - 416.0 / 3.0;
            rResult[3](0, 1) = -384.0 * x2 - 512.0 * x1 * y1 - 128.0 * y2 + 384.0 * x1 + 192.0 * y1 - 208.0 / 3.0;
            rResult[3](1, 0) = -384.0 * x2 - 512.0 * x1 * y1 - 128.0 * y2 + 384.0 * x1 + 192.0 * y1 - 208.0 / 3.0;
            rResult[3](1, 1) = 64.0 * x1 * (3.0 - 4.0 * x1 - 4.0 * y1);
            //
            rResult[4](0, 0) = 768.0 * x2 + 768.0 * (y1 - 1.0) * x1 + 128.0 * y2 - 288.0 * y1 + 152.0;
            rResult[4](0, 1) = 384.0 * x2 + (256.0 * y1 - 288.0) * x1 - 32.0 * y1 + 28.0;
            rResult[4](1, 0) = 384.0 * x2 + (256.0 * y1 - 288.0) * x1 - 32.0 * y1 + 28.0;
            rResult[4](1, 1) = 32.0 * x1 * (4.0 * x1 - 1.0);
            //
            rResult[5](0, 0) = -512.0 * x2 + 448.0 * x1 - 256.0 * x1 * y1 + 64.0 * y1 - 224.0 / 3.0;
            rResult[5](0, 1) = -128.0 * x2 + 64.0 * x1 - 16.0 / 3.0;
            rResult[5](1, 0) = -128.0 * x2 + 64.0 * x1 - 16.0 / 3.0;
            rResult[5](1, 1) = 0.0;
            //
            rResult[6](0, 0) = 64.0 * y1 * (4.0 * x1 - 1.0);
            rResult[6](0, 1) = 128.0 * x2 - 64.0 * x1 + 16.0 / 3.0;
            rResult[6](1, 0) = 128.0 * x2 - 64.0 * x1 + 16.0 / 3.0;
            rResult[6](1, 1) = 0.0;
            //
            rResult[7](0, 0) = 32.0 * y1 * (4.0 * y1 - 1.0);
            rResult[7](0, 1) = 32.0 * x1 * (8.0 * y1 - 1.0) - 32.0 * y1 + 4.0;
            rResult[7](1, 0) = 32.0 * x1 * (8.0 * y1 - 1.0) - 32.0 * y1 + 4.0;
            rResult[7](1, 1) = 32.0 * x1 * (4.0 * x1 - 1.0);
            //
            rResult[8](0, 0) = 0.0;
            rResult[8](0, 1) = 128.0 * y2 - 64.0 * y1 + 16.0 / 3.0;
            rResult[8](1, 0) = 128.0 * y2 - 64.0 * y1 + 16.0 / 3.0;
            rResult[8](1, 1) = (256.0 * y1 - 64.0) * x1;
            //
            rResult[9](0, 0) = 0.0;
            rResult[9](0, 1) = -128.0 * y2 + 64.0 * y1 - 16.0 / 3.0;
            rResult[9](1, 0) = -128.0 * y2 + 64.0 * y1 - 16.0 / 3.0;;
            rResult[9](1, 1) = 448.0 * y1 - 224.0 / 3.0 - 256.0 * x1 * y1 + 64.0 * x1 - 512.0 * y2;
            //
            rResult[10](0, 0) = 128.0 * y2 - 32.0 * y1;
            rResult[10](0, 1) = 384.0 * y2 + (256.0 * x1 - 288.0) * y1 - 32.0 * x1 + 28;
            rResult[10](1, 0) = 384.0 * y2 + (256.0 * x1 - 288.0) * y1 - 32.0 * x1 + 28;
            rResult[10](1, 1) = 128.0 * x2 + (768.0 * y1 - 288.0) * x1 + 768.0 * y2 - 768.0 * y1 + 152.0;
            //
            rResult[11](0, 0) = -64.0 * y1 * (-3.0 + 4.0 * x1 + 4.0 * y1);
            rResult[11](0, 1) = -384.0 * y2 + (-512 * x1 + 384.0) * y1 - 128.0 * x2 + 192.0 * x1 - 208.0 / 3.0;
            rResult[11](1, 0) = -384.0 * y2 + (-512 * x1 + 384.0) * y1 - 128.0 * x2 + 192.0 * x1 - 208.0 / 3.0;
            rResult[11](1, 1) = -512.0 * y2 + (-768.0 * x1 + 576.0) * y1 - 256.0 * x2 + 384.0 * x1 - 416.0 / 3.0;
            //
            rResult[12](0, 0) = 64.0 * y1 * (12.0 * x1 + 8.0 * y1 - 7.0);
            rResult[12](0, 1) = 384.0 * x2 + (1024.0 * y1 - 448.0) * x1 + 384.0 * y2 - 448.0 * y1 + 96.0;
            rResult[12](1, 0) = 384.0 * x2 + (1024.0 * y1 - 448.0) * x1 + 384.0 * y2 - 448.0 * y1 + 96.0;
            rResult[12](1, 1) = 64.0 * x1 * (8.0 * x1 + 12.0 * y1 - 7.0);
            //
            rResult[13](0, 0) = -64.0 * y1 * (12.0 * x1 + 4.0 * y1 - 5.0);
            rResult[13](0, 1) = -384.0 * x2 + (-512.0 * y1 + 320.0) * x1 + 64.0 * y1 - 32.0;
            rResult[13](1, 0) = -384.0 * x2 + (-512.0 * y1 + 320.0) * x1 + 64.0 * y1 - 32.0;
            rResult[13](1, 1) = -256.0 * x2 + 64.0 * x1;
            //
            rResult[14](0, 0) = -256.0 * y2 + 64.0 * y1;
            rResult[14](0, 1) = -384.0 * y2 + (-512.0 * x1 + 320.0) * y1 + 64.0 * x1 - 32.0;
            rResult[14](1, 0) = -384.0 * y2 + (-512.0 * x1 + 320.0) * y1 + 64.0 * x1 - 32.0;
            rResult[14](1, 1) = -64.0 * x1 * (4.0 * x1 + 12.0 * y1 - 5.0);
            //
            return rResult;
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * returns the third order derivatives of all shape functions
         * in given arbitrary pointers
         * @param rResult a fourth order tensor which contains the third derivatives
         * @param rPoint the given point the third order derivatives are calculated in
         */
        ShapeFunctionsThirdDerivativesType& ShapeFunctionsThirdDerivatives(ShapeFunctionsThirdDerivativesType& rResult, const CoordinatesArrayType& rPoint) const override
        {
            if (rResult.size() != this->PointsNumber())
            {
                ShapeFunctionsThirdDerivativesType temp(this->PointsNumber());
                rResult.swap(temp);
            }
            for (IndexType i = 0; i < rResult.size(); i++)
            {
                DenseVector<Matrix> temp(this->PointsNumber());
                rResult[i].swap(temp);
            }
            //
            rResult[0][0].resize(2, 2, false);
            rResult[0][1].resize(2, 2, false);
            rResult[1][0].resize(2, 2, false);
            rResult[1][1].resize(2, 2, false);
            rResult[2][0].resize(2, 2, false);
            rResult[2][1].resize(2, 2, false);
            rResult[3][0].resize(2, 2, false);
            rResult[3][1].resize(2, 2, false);
            rResult[4][0].resize(2, 2, false);
            rResult[4][1].resize(2, 2, false);
            rResult[5][0].resize(2, 2, false);
            rResult[5][1].resize(2, 2, false);
            rResult[6][0].resize(2, 2, false);
            rResult[6][1].resize(2, 2, false);
            rResult[7][0].resize(2, 2, false);
            rResult[7][1].resize(2, 2, false);
            rResult[8][0].resize(2, 2, false);
            rResult[8][1].resize(2, 2, false);
            rResult[9][0].resize(2, 2, false);
            rResult[9][1].resize(2, 2, false);
            rResult[10][0].resize(2, 2, false);
            rResult[10][1].resize(2, 2, false);
            rResult[11][0].resize(2, 2, false);
            rResult[11][1].resize(2, 2, false);
            rResult[12][0].resize(2, 2, false);
            rResult[12][1].resize(2, 2, false);
            rResult[13][0].resize(2, 2, false);
            rResult[13][1].resize(2, 2, false);
            rResult[14][0].resize(2, 2, false);
            rResult[14][1].resize(2, 2, false);
            //
            double fx3 = 256.0 * rPoint[0] + 256.0 * rPoint[1] - 160.0;
            double fx2y = fx3;
            double fxy2 = fx3;
            double fy3 = fx3;
            rResult[0][0](0, 0) = fx3;  // dx3
            rResult[0][0](0, 1) = fx2y; // dx2 dy
            rResult[0][0](1, 0) = fx2y; // dx2 dy
            rResult[0][0](1, 1) = fxy2; // dx  dy2
            rResult[0][1](0, 0) = fx2y; // dx2 dy
            rResult[0][1](0, 1) = fxy2; // dx  dy2
            rResult[0][1](1, 0) = fxy2; // dx  dy2
            rResult[0][1](1, 1) = fy3;  // dy3
            //
            fx3 = 256.0 * rPoint[0] - 96.0;
            rResult[1][0](0, 0) = fx3;  // dx3
            rResult[1][0](0, 1) = 0.0;  // dx2 dy
            rResult[1][0](1, 0) = 0.0;  // dx2 dy
            rResult[1][0](1, 1) = 0.0;  // dx  dy2
            rResult[1][1](0, 0) = 0.0;  // dx2 dy
            rResult[1][1](0, 1) = 0.0;  // dx  dy2
            rResult[1][1](1, 0) = 0.0;  // dx  dy2
            rResult[1][1](1, 1) = 0.0;  // dy3
            //
            fy3 = 256.0 * rPoint[1] - 96.0;
            rResult[2][0](0, 0) = 0.0;  // dx3
            rResult[2][0](0, 1) = 0.0;  // dx2 dy
            rResult[2][0](1, 0) = 0.0;  // dx2 dy
            rResult[2][0](1, 1) = 0.0;  // dx  dy2
            rResult[2][1](0, 0) = 0.0;  // dx2 dy
            rResult[2][1](0, 1) = 0.0;  // dx  dy2
            rResult[2][1](1, 0) = 0.0;  // dx  dy2
            rResult[2][1](1, 1) = fy3;  // dy3
            //
            fx3  = -768.0 * rPoint[1] - 1024.0 * rPoint[0] + 576.0;
            fx2y = -512.0 * rPoint[1] - 768.0  * rPoint[0] + 384.0;
            fxy2 = -256.0 * rPoint[1] - 512.0  * rPoint[0] + 192.0;
            fy3  = -256.0 * rPoint[0];
            rResult[3][0](0, 0) = fx3;  // dx3
            rResult[3][0](0, 1) = fx2y; // dx2 dy
            rResult[3][0](1, 0) = fx2y; // dx2 dy
            rResult[3][0](1, 1) = fxy2; // dx  dy2
            rResult[3][1](0, 0) = fx2y; // dx2 dy
            rResult[3][1](0, 1) = fxy2; // dx  dy2
            rResult[3][1](1, 0) = fxy2; // dx  dy2
            rResult[3][1](1, 1) = fy3;  // dy3
            //
            fx3 = 1536.0 * rPoint[0] + 768.0 * rPoint[1] - 768.0;
            fx2y = 768.0 * rPoint[0] + 256.0 * rPoint[1] - 288.0;
            fxy2 = 256.0 * rPoint[0] - 32.0;
            rResult[4][0](0, 0) = fx3;  // dx3
            rResult[4][0](0, 1) = fx2y; // dx2 dy
            rResult[4][0](1, 0) = fx2y; // dx2 dy
            rResult[4][0](1, 1) = fxy2; // dx  dy2
            rResult[4][1](0, 0) = fx2y; // dx2 dy
            rResult[4][1](0, 1) = fxy2; // dx  dy2
            rResult[4][1](1, 0) = fxy2; // dx  dy2
            rResult[4][1](1, 1) = 0.0;  // dy3
            //
            fx3 = 448.0 - 1024.0 * rPoint[0] - 256.0 * rPoint[1];
            fx2y = -256.0 * rPoint[0] + 64.0;
            rResult[5][0](0, 0) = fx3;  // dx3
            rResult[5][0](0, 1) = fx2y; // dx2 dy
            rResult[5][0](1, 0) = fx2y; // dx2 dy
            rResult[5][0](1, 1) = 0.0;  // dx  dy2
            rResult[5][1](0, 0) = fx2y; // dx2 dy
            rResult[5][1](0, 1) = 0.0;  // dx  dy2
            rResult[5][1](1, 0) = 0.0;  // dx  dy2
            rResult[5][1](1, 1) = 0.0;  // dy3
            //
            fx3 = 256.0 * rPoint[1];
            fx2y = 256.0 * rPoint[0] - 64.0;
            rResult[6][0](0, 0) = fx3;  // dx3
            rResult[6][0](0, 1) = fx2y; // dx2 dy
            rResult[6][0](1, 0) = fx2y; // dx2 dy
            rResult[6][0](1, 1) = 0.0;  // dx  dy2
            rResult[6][1](0, 0) = fx2y; // dx2 dy
            rResult[6][1](0, 1) = 0.0;  // dx  dy2
            rResult[6][1](1, 0) = 0.0;  // dx  dy2
            rResult[6][1](1, 1) = 0.0;  // dy3
            //
            fx2y = 256.0 * rPoint[1] - 32.0;
            fxy2 = 256.0 * rPoint[0] - 32.0;
            rResult[7][0](0, 0) = 0.0;  // dx3
            rResult[7][0](0, 1) = fx2y; // dx2 dy
            rResult[7][0](1, 0) = fx2y; // dx2 dy
            rResult[7][0](1, 1) = fxy2; // dx  dy2
            rResult[7][1](0, 0) = fx2y; // dx2 dy
            rResult[7][1](0, 1) = fxy2; // dx  dy2
            rResult[7][1](1, 0) = fxy2; // dx  dy2
            rResult[7][1](1, 1) = 0.0;  // dy3
            //
            fxy2 = 256.0 * rPoint[1] - 64.0;
            fy3 = 256.0 * rPoint[0];
            rResult[8][0](0, 0) = 0.0;  // dx3
            rResult[8][0](0, 1) = 0.0;  // dx2 dy
            rResult[8][0](1, 0) = 0.0;  // dx2 dy
            rResult[8][0](1, 1) = fxy2; // dx  dy2
            rResult[8][1](0, 0) = 0.0;  // dx2 dy
            rResult[8][1](0, 1) = fxy2; // dx  dy2
            rResult[8][1](1, 0) = fxy2; // dx  dy2
            rResult[8][1](1, 1) = fy3;  // dy3
            //
            fxy2 = -256.0 * rPoint[1] + 64.0;
            fy3 = 448.0 - 256.0 * rPoint[0] - 1024.0 * rPoint[1];
            rResult[9][0](0, 0) = 0.0;  // dx3
            rResult[9][0](0, 1) = 0.0;  // dx2 dy
            rResult[9][0](1, 0) = 0.0;  // dx2 dy
            rResult[9][0](1, 1) = fxy2; // dx  dy2
            rResult[9][1](0, 0) = 0.0;  // dx2 dy
            rResult[9][1](0, 1) = fxy2; // dx  dy2
            rResult[9][1](1, 0) = fxy2; // dx  dy2
            rResult[9][1](1, 1) = fy3;  // dy3
            //
            fx2y = 256.0 * rPoint[1] - 32.0;
            fxy2 = 768.0 * rPoint[1] + 256.0 * rPoint[0] - 288.0;
            fy3 = 768.0 * rPoint[0] + 1536.0 * rPoint[1] - 768.0;
            rResult[10][0](0, 0) = 0.0;  // dx3
            rResult[10][0](0, 1) = fx2y; // dx2 dy
            rResult[10][0](1, 0) = fx2y; // dx2 dy
            rResult[10][0](1, 1) = fxy2; // dx  dy2
            rResult[10][1](0, 0) = fx2y; // dx2 dy
            rResult[10][1](0, 1) = fxy2; // dx  dy2
            rResult[10][1](1, 0) = fxy2; // dx  dy2
            rResult[10][1](1, 1) = fy3;  // dy3
            //
            fx3 = -256.0 * rPoint[1];
            fx2y = -256.0 * rPoint[0] - 512.0 * rPoint[1] + 192.0;
            fxy2 = -768.0 * rPoint[1] - 512.0 * rPoint[0] + 384.0;
            fy3 = -1024.0 * rPoint[1] - 768.0 * rPoint[0] + 576.0;
            rResult[11][0](0, 0) = fx3;  // dx3
            rResult[11][0](0, 1) = fx2y; // dx2 dy
            rResult[11][0](1, 0) = fx2y; // dx2 dy
            rResult[11][0](1, 1) = fxy2; // dx  dy2
            rResult[11][1](0, 0) = fx2y; // dx2 dy
            rResult[11][1](0, 1) = fxy2; // dx  dy2
            rResult[11][1](1, 0) = fxy2; // dx  dy2
            rResult[11][1](1, 1) = fy3;  // dy3
            //
            fx3 = 768.0 * rPoint[1];
            fx2y = 768.0 * rPoint[0] + 1024.0 * rPoint[1] - 448.0;
            fxy2 = 1024.0 * rPoint[0] + 768.0 * rPoint[1] - 448.0;
            fy3 = 768.0 * rPoint[0];
            rResult[12][0](0, 0) = fx3;  // dx3
            rResult[12][0](0, 1) = fx2y; // dx2 dy
            rResult[12][0](1, 0) = fx2y; // dx2 dy
            rResult[12][0](1, 1) = fxy2; // dx  dy2
            rResult[12][1](0, 0) = fx2y; // dx2 dy
            rResult[12][1](0, 1) = fxy2; // dx  dy2
            rResult[12][1](1, 0) = fxy2; // dx  dy2
            rResult[12][1](1, 1) = fy3;  // dy3
            //
            fx3 = -768.0 * rPoint[1];
            fx2y = -768.0 * rPoint[0] - 512.0 * rPoint[1] + 320.0;
            fxy2 = -512.0 * rPoint[0] + 64.0;
            rResult[13][0](0, 0) = fx3;  // dx3
            rResult[13][0](0, 1) = fx2y; // dx2 dy
            rResult[13][0](1, 0) = fx2y; // dx2 dy
            rResult[13][0](1, 1) = fxy2; // dx  dy2
            rResult[13][1](0, 0) = fx2y; // dx2 dy
            rResult[13][1](0, 1) = fxy2; // dx  dy2
            rResult[13][1](1, 0) = fxy2; // dx  dy2
            rResult[13][1](1, 1) = 0.0;  // dy3
            //
            fx2y = -512.0 * rPoint[1] + 64.0;
            fxy2 = -768.0 * rPoint[1] - 512.0 * rPoint[0] + 320.0;
            fy3 = -768.0 * rPoint[0];
            rResult[14][0](0, 0) = 0.0;  // dx3
            rResult[14][0](0, 1) = fx2y; // dx2 dy
            rResult[14][0](1, 0) = fx2y; // dx2 dy
            rResult[14][0](1, 1) = fxy2; // dx  dy2
            rResult[14][1](0, 0) = fx2y; // dx2 dy
            rResult[14][1](0, 1) = fxy2; // dx  dy2
            rResult[14][1](1, 0) = fxy2; // dx  dy2
            rResult[14][1](1, 1) = fy3;  // dy3
            //
            return rResult;
        }

        ///@}
        ///@name Friends
        ///@{

        ///@}

    protected:
        /**
         * There are no protected members in class Triangle2D15
         */

    private:
        ///@name Static Member Variables
        ///@{

        static const GeometryData msGeometryData;

        static const GeometryDimension msGeometryDimension;

        ///@}
        ///@name Serialization
        ///@{

        friend class Serializer;
        // ========================================================================================
        // ========================================================================================
        void save(Serializer& rSerializer) const override
        {
            KRATOS_SERIALIZE_SAVE_BASE_CLASS(rSerializer, BaseType);
        }

        // ========================================================================================
        // ========================================================================================
        void load(Serializer& rSerializer) override
        {
            KRATOS_SERIALIZE_LOAD_BASE_CLASS(rSerializer, BaseType);
        }

        // ========================================================================================
        // ========================================================================================
        Triangle2D15() : BaseType(PointsArrayType(), &msGeometryData) {}

        ///@}
        ///@name Member Variables
        ///@{


        ///@}
        ///@name Private Operators
        ///@{


        ///@}
        ///@name Private Operations
        ///@{

        // ========================================================================================
        // ========================================================================================
        /**
         * Calculates the values of all shape function in all integration points.
         * Integration points are expected to be given in local coordinates
         * @param ThisMethod the current integration method
         * @return the matrix of values of every shape function in each integration point
         * :KLUDGE: number of points is hard-coded -> be careful if you want to copy and paste!
         */
        static Matrix CalculateShapeFunctionsIntegrationPointsValues(
            typename BaseType::IntegrationMethod ThisMethod)
        {
            IntegrationPointsContainerType all_integration_points = AllIntegrationPoints();
            IntegrationPointsArrayType integration_points = all_integration_points[static_cast<int>(ThisMethod)];
            //number of integration points
            const int integration_points_number = integration_points.size();
            //number of nodes in current geometry
            const int points_number = 15;
            //setting up return matrix
            Matrix shape_function_values(integration_points_number, points_number);
            //loop over all integration points
            for (int pnt = 0; pnt < integration_points_number; pnt++)
            {
                double cof1 = 128.0 / 3.0;
                double cof2 = 32.0  / 3.0;
                double xcor = integration_points[pnt].X();
                double ycor = integration_points[pnt].Y();
                double cor3 = 1.0 - xcor - ycor;
                //
                shape_function_values(pnt, 0) = cor3 * (cor3 - 0.25) * (cor3 - 0.5) * (cor3 - 0.75) * cof2;
                shape_function_values(pnt, 1) = xcor * (xcor - 0.25) * (xcor - 0.5) * (xcor - 0.75) * cof2;
                shape_function_values(pnt, 2) = ycor * (ycor - 0.25) * (ycor - 0.5) * (ycor - 0.75) * cof2;
                shape_function_values(pnt, 3) = xcor * cor3 * (cor3 - 0.25) * (cor3 - 0.5) * cof1;
                shape_function_values(pnt, 4) = xcor * cor3 * (cor3 - 0.25) * (xcor - 0.25) * 64.0;
                shape_function_values(pnt, 5) = xcor * cor3 * (xcor - 0.25) * (xcor - 0.5) * cof1;
                shape_function_values(pnt, 6) = xcor * ycor * (xcor - 0.25) * (xcor - 0.5) * cof1;
                shape_function_values(pnt, 7) = xcor * ycor * (xcor - 0.25) * (ycor - 0.25) * 64.0;
                shape_function_values(pnt, 8) = xcor * ycor * (ycor - 0.25) * (ycor - 0.5) * cof1;
                shape_function_values(pnt, 9) = ycor * cor3 * (ycor - 0.25) * (ycor - 0.5) * cof1;
                shape_function_values(pnt, 10) = ycor * cor3 * (ycor - 0.25) * (cor3 - 0.25) * 64.0;
                shape_function_values(pnt, 11) = ycor * cor3 * (cor3 - 0.25) * (cor3 - 0.5) * cof1;
                shape_function_values(pnt, 12) = xcor * ycor * cor3 * (cor3 - 0.25) * 128.0;
                shape_function_values(pnt, 13) = xcor * ycor * cor3 * (xcor - 0.25) * 128.0;
                shape_function_values(pnt, 14) = xcor * ycor * cor3 * (ycor - 0.25) * 128.0;
            }
            return shape_function_values;
        }

        // ========================================================================================
        // ========================================================================================
        /**
         * Calculates the local gradients of all shape functions
         * in all integration points.
         * Integration points are expected to be given in local coordinates
         *
         * @param ThisMethod the current integration method
         * @return the vector of the gradients of all shape functions
         * in each integration point
         */
        static ShapeFunctionsGradientsType
            CalculateShapeFunctionsIntegrationPointsLocalGradients(
                typename BaseType::IntegrationMethod ThisMethod)
        {
            IntegrationPointsContainerType all_integration_points = AllIntegrationPoints();
            IntegrationPointsArrayType integration_points = all_integration_points[static_cast<int>(ThisMethod)];
            //number of integration points
            const int integration_points_number = integration_points.size();
            ShapeFunctionsGradientsType d_shape_f_values(integration_points_number);
            //initialising container
            //std::fill(d_shape_f_values.begin(), d_shape_f_values.end(), Matrix(4,2));
            //loop over all integration points

            for (int pnt = 0; pnt < integration_points_number; pnt++)
            {
                Matrix result(15, 2);
                double x1 = integration_points[pnt].X();
                double x2 = x1 * x1;
                double x3 = x2 * x1;
                double y1 = integration_points[pnt].Y();
                double y2 = y1 * y1;
                double y3 = y2 * y1;
                double tC1 = 1.0 - x1 - y1;
                double tC2 = tC1 * tC1;
                double tC3 = tC2 * tC1;

                //// noalias(result) = ZeroMatrix(15, 2);
                result = ZeroMatrix(15, 2);
                //
                result(0, 0) = -(128.0 * tC3 - 144.0 * tC2 + 44.0 * tC1 - 3.0) / 3.0;
                result(0, 1) = -(128.0 * tC3 - 144.0 * tC2 + 44.0 * tC1 - 3.0) / 3.0;
                //
                result(1, 0) = (128.0 * x3 - 144.0 * x2 + 44.0 * x1 - 3.0) / 3.0;
                result(1, 1) = 0.0;
                //
                result(2, 0) = 0.0;
                result(2, 1) = (128.0 * y3 - 144.0 * y2 + 44.0 * y1 - 3.0) / 3.0;
                //
                result(3, 0) = -128.0 * (tC2 - 0.5 * tC1 + 1.0 / 24.0) * x1 + (128.0 * tC3 - 96.0 * tC2 + 16.0 * tC1) / 3.0;
                result(3, 1) = -16.0 * x1 * (24.0 * tC2 - 12.0 * tC1 + 1.0) / 3.0;
                //
                result(4, 0) = -128.0 * (x1 - 0.25) * (tC1 - 0.125) * x1 + 128.0 * (x1 - 0.125) * (tC1 - 0.25) * tC1;
                result(4, 1) = -4.0 * x1 * (4.0 * x1 - 1.0) * (8.0 * tC1 - 1.0);
                //
                result(5, 0) = -(128.0 * x3 - 96.0 * x2 + 16.0 * x1) / 3.0 + 128.0 * (x2 - 0.5 * x1 + 1.0 / 24.0) * tC1;
                result(5, 1) = -16.0 * x1 * (8.0 * x2 - 6.0 * x1 + 1.0) / 3.0;
                //
                result(6, 0) = 16.0 * y1 * (24.0 * x2 - 12.0 * x1 + 1.0) / 3.0;
                result(6, 1) = (128.0 * x3 - 96.0 * x2 + 16.0 * x1) / 3.0;
                //
                result(7, 0) = 4.0 * (8.0 * x1 - 1.0) * (4.0 * y1 - 1.0) * y1;
                result(7, 1) = 4.0 * (4.0 * x1 - 1.0) * (8.0 * y1 - 1.0) * x1;
                //
                result(8, 0) = (128.0 * y3 - 96.0 * y2 + 16.0 * y1) / 3.0;
                result(8, 1) = 16.0 * x1 * (24.0 * y2 - 12.0 * y1 + 1.0) / 3.0;
                //
                result(9, 0) = -16.0 * y1 * (8.0 * y2 - 6.0 * y1 + 1.0) / 3.0;
                result(9, 1) = -(128.0 * y3 - 96.0 * y2 + 16.0 * y1) / 3.0 + 128.0 * (y2 - 0.5 * y1 + 1.0 / 24.0) * tC1;
                //
                result(10, 0) = -4.0 * y1 * (4.0 * y1 - 1.0) * (8.0 * tC1 - 1.0);
                result(10, 1) = -128.0 * (y1 - 0.25) * (tC1 - 0.125) * y1 + 128.0 * (y1 - 0.125) * tC1 * (tC1 - 0.25);
                //
                result(11, 0) = -16.0 * y1 * (24.0 * tC2 - 12.0 * tC1 + 1.0) / 3.0;
                result(11, 1) = -128.0 * (tC2 - 0.5 * tC1 + 1.0 / 24.0) * y1 + (128.0 * tC3 - 96.0 * tC2 + 16.0 * tC1) / 3.0;
                //
                result(12, 0) = 256.0 * y1 * (-x1 * (tC1 - 0.125) + 0.5 * tC2 - 0.125 * tC1);
                result(12, 1) = 256.0 * x1 * (-y1 * (tC1 - 0.125) + 0.5 * tC2 - 0.125 * tC1);
                //
                result(13, 0) = -32.0 * y1 * (4.0 * x2 - x1) + 256.0 * (x1 - 0.125) * y1 * tC1;
                result(13, 1) = 128.0 * (x1 - 0.25) * (-y1 + tC1) * x1;
                //
                result(14, 0) = 128.0 * (y1 - 0.25) * y1 * (-x1 + tC1);
                result(14, 1) = -32.0 * x1 * (4.0 * y2 - y1) + 256.0 * (y1 - 0.125) * tC1 * x1;
                //
                d_shape_f_values[pnt] = result;
            }
            return d_shape_f_values;
        }

        // ========================================================================================
        // ========================================================================================
        static const IntegrationPointsContainerType AllIntegrationPoints()
        {
            IntegrationPointsContainerType integration_points =
            {
                {
                    Quadrature<TriangleGaussLegendreIntegrationPoints1, 2, IntegrationPoint<3> >::GenerateIntegrationPoints(),
                    Quadrature<TriangleGaussLegendreIntegrationPoints2, 2, IntegrationPoint<3> >::GenerateIntegrationPoints(),
                    Quadrature<TriangleGaussLegendreIntegrationPoints3, 2, IntegrationPoint<3> >::GenerateIntegrationPoints(),
                    Quadrature<TriangleGaussLegendreIntegrationPoints4, 2, IntegrationPoint<3> >::GenerateIntegrationPoints(),
                    Quadrature<TriangleGaussLegendreIntegrationPoints5, 2, IntegrationPoint<3> >::GenerateIntegrationPoints(),
                }
            };
            return integration_points;
        }

        // ========================================================================================
        // ========================================================================================
        static const ShapeFunctionsValuesContainerType AllShapeFunctionsValues()
        {
            ShapeFunctionsValuesContainerType shape_functions_values =
            {
                {
                    Triangle2D15<TPointType>::CalculateShapeFunctionsIntegrationPointsValues(GeometryData::IntegrationMethod::GI_GAUSS_1),
                    Triangle2D15<TPointType>::CalculateShapeFunctionsIntegrationPointsValues(GeometryData::IntegrationMethod::GI_GAUSS_2),
                    Triangle2D15<TPointType>::CalculateShapeFunctionsIntegrationPointsValues(GeometryData::IntegrationMethod::GI_GAUSS_3),
                    Triangle2D15<TPointType>::CalculateShapeFunctionsIntegrationPointsValues(GeometryData::IntegrationMethod::GI_GAUSS_4),
                    Triangle2D15<TPointType>::CalculateShapeFunctionsIntegrationPointsValues(GeometryData::IntegrationMethod::GI_GAUSS_5),
                }
            };
            return shape_functions_values;
        }

        // ========================================================================================
        // ========================================================================================
        static const ShapeFunctionsLocalGradientsContainerType
            AllShapeFunctionsLocalGradients()
        {
            ShapeFunctionsLocalGradientsContainerType shape_functions_local_gradients =
            {
                {
                    Triangle2D15<TPointType>::CalculateShapeFunctionsIntegrationPointsLocalGradients(GeometryData::IntegrationMethod::GI_GAUSS_1),
                    Triangle2D15<TPointType>::CalculateShapeFunctionsIntegrationPointsLocalGradients(GeometryData::IntegrationMethod::GI_GAUSS_2),
                    Triangle2D15<TPointType>::CalculateShapeFunctionsIntegrationPointsLocalGradients(GeometryData::IntegrationMethod::GI_GAUSS_3),
                    Triangle2D15<TPointType>::CalculateShapeFunctionsIntegrationPointsLocalGradients(GeometryData::IntegrationMethod::GI_GAUSS_4),
                    Triangle2D15<TPointType>::CalculateShapeFunctionsIntegrationPointsLocalGradients(GeometryData::IntegrationMethod::GI_GAUSS_5),
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

        template<class TOtherPointType> friend class Triangle2D15;

        ///@}
        ///@name Un accessible methods
        ///@{


        ///@}
    }; // Class Geometry

    ///@}
    ///@name Type Definitions
    ///@{

    // ========================================================================================
    // ========================================================================================
    ///@}
    ///@name Input and output
    ///@{
    /**
     * input stream functions
     */
    template<class TPointType>
    inline std::istream& operator >> (std::istream& rIStream, Triangle2D15<TPointType>& rThis);

    // ========================================================================================
    // ========================================================================================
    /**
     * output stream functions
     */
    template<class TPointType>
    inline std::ostream& operator << (std::ostream& rOStream, const Triangle2D15<TPointType>& rThis)
    {
        rThis.PrintInfo(rOStream);
        rOStream << std::endl;
        rThis.PrintData(rOStream);
        return rOStream;
    }

    ///@}

    // ========================================================================================
    // ========================================================================================
    template<class TPointType> const
        GeometryData Triangle2D15<TPointType>::msGeometryData(
            &msGeometryDimension,
            GeometryData::IntegrationMethod::GI_GAUSS_5,
            Triangle2D15<TPointType>::AllIntegrationPoints(),
            Triangle2D15<TPointType>::AllShapeFunctionsValues(),
            AllShapeFunctionsLocalGradients()
        );

    // ========================================================================================
    // ========================================================================================
    template<class TPointType>
    const GeometryDimension Triangle2D15<TPointType>::msGeometryDimension(2, 2, 2);

}// namespace Kratos.

#endif // KRATOS_QUADRILATERAL_2D_15_H_INCLUDED  defined