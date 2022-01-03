//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Miguel Maso Sotomayor
//

#ifndef KRATOS_SHALLOW_ELEMENT_H_INCLUDED
#define KRATOS_SHALLOW_ELEMENT_H_INCLUDED

// System includes


// External includes


// Project includes
#include "includes/define.h"
#include "includes/element.h"
#include "includes/serializer.h"
#include "shallow_element_data.h"
#include "custom_friction_laws/friction_law.h"

namespace Kratos
{

///@name Kratos Globals
///@{

///@}
///@name Type Definitions
///@{

///@}
///@name  Functions
///@{

///@}
///@name Kratos Classes
///@{

/**
 * @brief Generic implementation for shallow water elements.
 * @author Miguel Maso Sotomayor
 * @ingroup ShallowWaterApplication
 * @tparam TElementData
 */
template<class TElementData>
class ShallowElement : public Element
{
public:

    ///@name Type Definitions
    ///@{

    typedef std::size_t IndexType;

    typedef Node<3> NodeType;

    typedef typename TElementData::LocalVectorType LocalVectorType;

    typedef typename TElementData::LocalMatrixType LocalMatrixType;

    static constexpr std::size_t TNumNodes = TElementData::NumNodes;

    static constexpr std::size_t TLocalSize = TElementData::TLocalSize;

    ///@}
    ///@name Pointer definition
    ///@{

    KRATOS_CLASS_INTRUSIVE_POINTER_DEFINITION(ShallowElement);

    ///@}
    ///@name Life Cycle
    ///@{

    /**
     * @brief Default constructor
     */
    ShallowElement() : Element(){}

    /**
     * @brief Constructor using an array of nodes
     */
    ShallowElement(IndexType NewId, const NodesArrayType& ThisNodes) : Element(NewId, ThisNodes){}

    /**
     * @brief Constructor using Geometry
     */
    ShallowElement(IndexType NewId, GeometryType::Pointer pGeometry) : Element(NewId, pGeometry){}

    /**
     * @brief Constructor using Geometry and Properties
     */
    ShallowElement(IndexType NewId, GeometryType::Pointer pGeometry, PropertiesType::Pointer pProperties) : Element(NewId, pGeometry, pProperties){}

    /**
     * @brief Destructor
     */
    ~ ShallowElement() override {};

    ///@}
    ///@name Operations
    ///@{

    /**
     * @brief Create a new element pointer
     * @param NewId: the ID of the new element
     * @param ThisNodes: the nodes of the new element
     * @param pProperties: the properties assigned to the new element
     * @return a Pointer to the new element
     */
    Element::Pointer Create(IndexType NewId, NodesArrayType const& ThisNodes, PropertiesType::Pointer pProperties) const override
    {
        return Kratos::make_intrusive<ShallowElement<TElementData>>(NewId, this->GetGeometry().Create(ThisNodes), pProperties);
    }

    /**
     * @brief Create a new element pointer
     * @param NewId: the ID of the new element
     * @param pGeom: the geometry to be employed
     * @param pProperties: the properties assigned to the new element
     * @return a Pointer to the new element
     */
    Element::Pointer Create(IndexType NewId, GeometryType::Pointer pGeom, PropertiesType::Pointer pProperties) const override
    {
        return Kratos::make_intrusive<ShallowElement<TElementData>>(NewId, pGeom, pProperties);
    }

    /**
     * @brief Create a new element pointer and clone the previous element data
     * @param NewId the ID of the new element
     * @param rThisNodes the nodes of the new element
     * @return a Pointer to the new element
     */
    Element::Pointer Clone(IndexType NewId, NodesArrayType const& ThisNodes) const override
    {
        Element::Pointer p_new_elem = Create(NewId, this->GetGeometry().Create(ThisNodes), this->pGetProperties());
        p_new_elem->SetData(this->GetData());
        p_new_elem->Set(Flags(*this));
        return p_new_elem;
    }

    /**
     * @brief Check that all required data containers are properly initialized and registered in Kratos
     * @return 0 if no errors are detected.
     */
    int Check(const ProcessInfo& rCurrentProcessInfo) const override;

    /**
     * @brief Fill given vector with the linear system row index for the element's degrees of freedom
     * @param rResult
     * @param rCurrentProcessInfo
     */
    void EquationIdVector(EquationIdVectorType& rResult, const ProcessInfo& rCurrentProcessInfo) const override;

    /**
     * @brief Fill given array with containing the element's degrees of freedom
     * @param rElementalDofList
     * @param rCurrentProcessInfo
     */
    void GetDofList(DofsVectorType& rElementalDofList, const ProcessInfo& rCurrentProcessInfo) const override;

    /**
     * @brief Get the variable which defines the degrees of freedom
     */
    void GetValuesVector(Vector& rValues, int Step = 0) const override;

    /**
     * @brief Get the time derivative of variable which defines the degrees of freedom
     */
    void GetFirstDerivativesVector(Vector& rValues, int Step = 0) const override;

    /**
     * @brief Get the second time derivative of variable which defines the degrees of freedom
     */
    void GetSecondDerivativesVector(Vector& rValues, int Step = 0) const override;

    /**
     * @brief Access for variables on Integration points
     * @param rVariable: the specified variable
     * @param rValues: where to store the values for the specified variable type at each integration point
     * @param rCurrentProcessInfo: the current process info instance
     */
    void CalculateOnIntegrationPoints(const Variable<double>& rVariable, std::vector<double>& rValues, const ProcessInfo& rCurrentProcessInfo) override;

    /**
     * @brief Is called in the beginning of each solution step
     */
    void InitializeSolutionStep(const ProcessInfo& rCurrentProcessInfo) override;

    /**
     * @brief Calculate the elemental contribution to the problem
     * @param rLeftHandSideMatrix Elemental left hand side matrix
     * @param rRightHandSideVector Elemental right hand side vector
     * @param rCurrentProcessInfo Reference to the ProcessInfo from the ModelPart containing the element
     */
    void CalculateLocalSystem(MatrixType& rLeftHandSideMatrix, VectorType& rRightHandSideVector, const ProcessInfo& rCurrentProcessInfo) override;

    /**
     * @brief Calculate the elemental mass matrix
     * @param rMassMatrix the elemental mass matrix
     * @param rCurrentProcessInfo the current process info instance
     */
    void CalculateMassMatrix(MatrixType& rMassMatrix, const ProcessInfo& rCurrentProcessInfo) override;

    /**
     * @brief Calculate the diffusion matrix for monotonic corrected schemes.
     * @param rDampingMatrix the elemental damping matrix
     * @param rCurrentProcessInfo the current process info instance
     */
    void CalculateDampingMatrix(MatrixType& rDampingMatrix, const ProcessInfo& rCurrentProcessInfo) override;

    ///@}
    ///@name Access
    ///@{


    ///@}
    ///@name Inquiry
    ///@{


    ///@}
    ///@name Input and output
    ///@{

    /**
     * @brief Turn back information as a string.
     */
    std::string Info() const override
    {
        return "ShallowElement";
    }

    /**
     * @brief Print information about this object.
     */
    void PrintInfo(std::ostream& rOStream) const override
    {
        rOStream << Info() << " : " << Id();
    }

    ///@}
    ///@name Friends
    ///@{


    ///@}

protected:

    ///@name Protected Operations
    ///@{

    virtual void CalculateArtificialViscosity(
        BoundedMatrix<double,3,3>& rViscosity,
        BoundedMatrix<double,2,2>& rDiffusion,
        const TElementData& rData,
        const BoundedMatrix<double,TNumNodes,2>& rDN_DX);

    virtual void CalculateArtificialDamping(
        BoundedMatrix<double,3,3>& rFriction,
        const TElementData& rData);

    void AddWaveTerms(
        LocalMatrixType& rMatrix,
        LocalVectorType& rVector,
        const TElementData& rData,
        const array_1d<double,TNumNodes>& rN,
        const BoundedMatrix<double,TNumNodes,2>& rDN_DX,
        const double Weight = 1.0);

    void AddFrictionTerms(
        LocalMatrixType& rMatrix,
        LocalVectorType& rVector,
        const TElementData& rData,
        const array_1d<double,TNumNodes>& rN,
        const BoundedMatrix<double,TNumNodes,2>& rDN_DX,
        const double Weight = 1.0);

    virtual void AddDispersiveTerms(
        LocalVectorType& rVector,
        const TElementData& rData,
        const array_1d<double,TNumNodes>& rN,
        const BoundedMatrix<double,TNumNodes,2>& rDN_DX,
        const double Weight = 1.0);

    void AddArtificialViscosityTerms(
        LocalMatrixType& rMatrix,
        const TElementData& rData,
        const BoundedMatrix<double,TNumNodes,2>& rDN_DX,
        const double Weight = 1.0);

    virtual void AddMassTerms(
        LocalMatrixType& rMatrix,
        const TElementData& rData,
        const array_1d<double,TNumNodes>& rN,
        const BoundedMatrix<double,TNumNodes,2>& rDN_DX,
        const double Weight = 1.0);

    virtual double StabilizationParameter(const TElementData& rData) const;

    double InverseHeight(const TElementData& rData) const;

    ///@}
    ///@name Protected  Access
    ///@{


    ///@}
    ///@name Protected Inquiry
    ///@{


    ///@}
    ///@name Protected LifeCycle
    ///@{


    ///@}

private:
    ///@name Static Member Variables
    ///@{


    ///@}
    ///@name Member Variables
    ///@{


    ///@}
    ///@name Serialization
    ///@{

    friend class Serializer;

    void save(Serializer& rSerializer) const override
    {
        KRATOS_SERIALIZE_SAVE_BASE_CLASS(rSerializer, Element);
    }

    void load(Serializer& rSerializer) override
    {
        KRATOS_SERIALIZE_LOAD_BASE_CLASS(rSerializer, Element);
    }

    ///@}
    ///@name Private Operators
    ///@{


    ///@}
    ///@name Private Operations
    ///@{

    ///@}
    ///@name Private  Access
    ///@{


    ///@}
    ///@name Private Inquiry
    ///@{


    ///@}
    ///@name Un accessible methods
    ///@{


    ///@}

}; // Class ShallowElement

///@}
///@name Type Definitions
///@{


///@}
///@name Input and output
///@{


///@}

///@} addtogroup block

}  // namespace Kratos.

#endif // KRATOS_SHALLOW_ELEMENT_H_INCLUDED  defined
