// KRATOS___
//     //   ) )
//    //         ___      ___
//   //  ____  //___) ) //   ) )
//  //    / / //       //   / /
// ((____/ / ((____   ((___/ /  MECHANICS
//
//  License:         geo_mechanics_application/license.txt
//
//  Main authors:    Vahid Galavi
//

#if !defined(KRATOS_GEO_PW_ELEMENT_H_INCLUDED )
#define  KRATOS_GEO_PW_ELEMENT_H_INCLUDED

// Project includes
#include "includes/serializer.h"

// Application includes
#include "custom_utilities/comparison_utilities.hpp"
#include "custom_elements/U_Pw_small_strain_element.hpp"
#include "custom_utilities/element_utilities.hpp"
#include "geo_mechanics_application_variables.h"

namespace Kratos
{

template< unsigned int TDim, unsigned int TNumNodes >
class KRATOS_API(GEO_MECHANICS_APPLICATION) TransientOnePhaseFlowElement : public UPwSmallStrainElement<TDim, TNumNodes>
{

public:

    KRATOS_CLASS_INTRUSIVE_POINTER_DEFINITION( TransientOnePhaseFlowElement );

    typedef std::size_t IndexType;
    typedef Properties PropertiesType;
    typedef Node <3> NodeType;
    typedef Geometry<NodeType> GeometryType;
    typedef Geometry<NodeType>::PointsArrayType NodesArrayType;
    typedef Vector VectorType;
    typedef Matrix MatrixType;
    typedef Element::DofsVectorType DofsVectorType;
    typedef Element::EquationIdVectorType EquationIdVectorType;

    /// The definition of the sizetype
    typedef std::size_t SizeType;
    using UPwSmallStrainElement<TDim,TNumNodes>::mRetentionLawVector;
    using UPwSmallStrainElement<TDim,TNumNodes>::mConstitutiveLawVector;
    using UPwSmallStrainElement<TDim,TNumNodes>::mIsInitialised;
    using UPwSmallStrainElement<TDim,TNumNodes>::CalculateRetentionResponse;

    typedef typename UPwSmallStrainElement<TDim,TNumNodes>::ElementVariables ElementVariables;

///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    /// Default Constructor
    TransientOnePhaseFlowElement(IndexType NewId = 0) : UPwSmallStrainElement<TDim,TNumNodes>( NewId ) {}

    /// Constructor using an array of nodes
    TransientOnePhaseFlowElement(IndexType NewId,
              const NodesArrayType& ThisNodes) : UPwSmallStrainElement<TDim,TNumNodes>(NewId, ThisNodes) {}

    /// Constructor using Geometry
    TransientOnePhaseFlowElement(IndexType NewId,
              GeometryType::Pointer pGeometry) : UPwSmallStrainElement<TDim,TNumNodes>(NewId, pGeometry) {}

    /// Constructor using Properties
    TransientOnePhaseFlowElement(IndexType NewId,
              GeometryType::Pointer pGeometry,
              PropertiesType::Pointer pProperties) : UPwSmallStrainElement<TDim,TNumNodes>( NewId, pGeometry, pProperties ) {}

    /// Destructor
    ~TransientOnePhaseFlowElement() override {}

///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    Element::Pointer Create( IndexType NewId,
                             NodesArrayType const& ThisNodes,
                             PropertiesType::Pointer pProperties ) const override;

    Element::Pointer Create( IndexType NewId,
                             GeometryType::Pointer pGeom,
                             PropertiesType::Pointer pProperties ) const override;

///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    int Check(const ProcessInfo& rCurrentProcessInfo) const override;

    void Initialize(const ProcessInfo& rCurrentProcessInfo) override;

    void InitializeSolutionStep(const ProcessInfo& rCurrentProcessInfo) override;

    void InitializeNonLinearIteration(const ProcessInfo& rCurrentProcessInfo) override;

    void FinalizeNonLinearIteration(const ProcessInfo& rCurrentProcessInfo) override;

    void FinalizeSolutionStep(const ProcessInfo& rCurrentProcessInfo) override;

    void GetDofList(DofsVectorType& rElementalDofList, 
                    const ProcessInfo& rCurrentProcessInfo) const override;

    void EquationIdVector( EquationIdVectorType& rResult,
                           const ProcessInfo& rCurrentProcessInfo ) const override;

    void CalculateMassMatrix( MatrixType& rMassMatrix,
                              const ProcessInfo& rCurrentProcessInfo ) override;

    void CalculateDampingMatrix( MatrixType& rDampingMatrix,
                                 const ProcessInfo& rCurrentProcessInfo ) override;

    void GetValuesVector(Vector& rValues, int Step = 0) const override;

    void GetFirstDerivativesVector(Vector& rValues, int Step = 0) const override;

    void GetSecondDerivativesVector(Vector& rValues, int Step = 0) const override;

///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    void CalculateOnIntegrationPoints(const Variable<double>& rVariable,
                                      std::vector<double>& rOutput,
                                      const ProcessInfo& rCurrentProcessInfo) override;

    void CalculateOnIntegrationPoints(const Variable<array_1d<double,3>>& rVariable,
                                      std::vector<array_1d<double,3>>& rOutput,
                                      const ProcessInfo& rCurrentProcessInfo) override;

    void CalculateOnIntegrationPoints(const Variable<Matrix>& rVariable,
                                      std::vector<Matrix>& rOutput,
                                      const ProcessInfo& rCurrentProcessInfo) override;

    // Turn back information as a string.
    std::string Info() const override
    {
        std::stringstream buffer;
        buffer << "transient one-phase flow Element #" << this->Id() << "\nRetention law: " << mRetentionLawVector[0]->Info();
        return buffer.str();
    }

    // Print information about this object.
    void PrintInfo(std::ostream& rOStream) const override
    {
        rOStream << "transient one-phase flow Element #" << this->Id() << "\nRetention law: " << mRetentionLawVector[0]->Info();
    }

///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

protected:

    /// Member Variables

///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    void CalculateAll( MatrixType &rLeftHandSideMatrix,
                       VectorType &rRightHandSideVector,
                       const ProcessInfo& CurrentProcessInfo,
                       const bool CalculateStiffnessMatrixFlag,
                       const bool CalculateResidualVectorFlag ) override;

    void InitializeElementVariables( ElementVariables &rVariables,
                                     const ProcessInfo &CurrentProcessInfo ) override;

    void CalculateAndAddLHS(MatrixType &rLeftHandSideMatrix, ElementVariables &rVariables) override;

    void CalculateAndAddRHS(VectorType &rRightHandSideVector, ElementVariables &rVariables) override;

    void CalculateKinematics( ElementVariables &rVariables, const unsigned int &PointNumber ) override;

    void CalculateAndAddCompressibilityMatrix(MatrixType &rLeftHandSideMatrix, ElementVariables& rVariables) override;
    void CalculateAndAddPermeabilityFlow(VectorType &rRightHandSideVector, ElementVariables& rVariables) override;
    void CalculateAndAddFluidBodyFlow(VectorType &rRightHandSideVector, ElementVariables &rVariables) override;
    void CalculateAndAddPermeabilityMatrix(MatrixType &rLeftHandSideMatrix, ElementVariables &rVariables) override;
    unsigned int GetNumberOfDOF() const override;
///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

private:

    /// Member Variables

///----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    /// Serialization

    friend class Serializer;

    void save(Serializer& rSerializer) const override
    {
        KRATOS_SERIALIZE_SAVE_BASE_CLASS( rSerializer, Element )
    }

    void load(Serializer& rSerializer) override
    {
        KRATOS_SERIALIZE_LOAD_BASE_CLASS( rSerializer, Element )
    }

    // Assignment operator.
    TransientOnePhaseFlowElement & operator=(TransientOnePhaseFlowElement const& rOther);

    // Copy constructor.
    TransientOnePhaseFlowElement(TransientOnePhaseFlowElement const& rOther);

}; // Class TransientOnePhaseFlowElement

} // namespace Kratos

#endif // KRATOS_GEO_PW_ELEMENT_H_INCLUDED  defined
