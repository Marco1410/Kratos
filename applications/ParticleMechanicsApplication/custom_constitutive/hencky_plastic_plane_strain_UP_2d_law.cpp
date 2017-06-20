//
//   Project Name:        KratosParticleMechanicsApplication $
//   Created by:          $Author:                 IIaconeta $
//   Date:                $Date:               February 2017 $
//   Revision:            $Revision:                     0.0 $
//
//

// System includes
#include <iostream>

// External includes
#include<cmath>

// Project includes
#include "includes/properties.h"
#include "custom_constitutive/hencky_plastic_plane_strain_UP_2d_law.hpp"
//#include "custom_utilities/solid_mechanics_math_utilities.hpp"
#include "particle_mechanics_application.h"

namespace Kratos
{

//******************************CONSTRUCTOR*******************************************
//************************************************************************************

HenckyElasticPlasticPlaneStrainUP2DLaw::HenckyElasticPlasticPlaneStrainUP2DLaw()
    : HenckyElasticPlasticUP3DLaw()
{

}


//******************************CONSTRUCTOR*******************************************
//************************************************************************************

HenckyElasticPlasticPlaneStrainUP2DLaw::HenckyElasticPlasticPlaneStrainUP2DLaw(MPMFlowRulePointer pMPMFlowRule, YieldCriterionPointer pYieldCriterion, HardeningLawPointer pHardeningLaw)
    : HenckyElasticPlasticUP3DLaw()
{
}

//******************************COPY CONSTRUCTOR**************************************
//************************************************************************************

HenckyElasticPlasticPlaneStrainUP2DLaw::HenckyElasticPlasticPlaneStrainUP2DLaw(const HenckyElasticPlasticPlaneStrainUP2DLaw& rOther)
    : HenckyElasticPlasticUP3DLaw(rOther)
{

}

//********************************CLONE***********************************************
//************************************************************************************

ConstitutiveLaw::Pointer HenckyElasticPlasticPlaneStrainUP2DLaw::Clone() const
{
    HenckyElasticPlasticPlaneStrainUP2DLaw::Pointer p_clone(new HenckyElasticPlasticPlaneStrainUP2DLaw(*this));
    return p_clone;
}

//*******************************DESTRUCTOR*******************************************
//************************************************************************************

HenckyElasticPlasticPlaneStrainUP2DLaw::~HenckyElasticPlasticPlaneStrainUP2DLaw()
{
}



//************* COMPUTING  METHODS
//************************************************************************************
//************************************************************************************

//***********************COMPUTE TOTAL STRAIN*****************************************
//************************************************************************************

void HenckyElasticPlasticPlaneStrainUP2DLaw::CalculateGreenLagrangeStrain( const Matrix & rRightCauchyGreen,
        Vector& rStrainVector )
{

    //E= 0.5*(FT*F-1)
    rStrainVector[0] = 0.5 * ( rRightCauchyGreen( 0, 0 ) - 1.00 );
    rStrainVector[1] = 0.5 * ( rRightCauchyGreen( 1, 1 ) - 1.00 );
    rStrainVector[2] = rRightCauchyGreen( 0, 1 );

}


//***********************COMPUTE TOTAL STRAIN*****************************************
//************************************************************************************

void HenckyElasticPlasticPlaneStrainUP2DLaw::CalculateAlmansiStrain( const Matrix & rLeftCauchyGreen,
        Vector& rStrainVector )
{

    // e= 0.5*(1-invbT*invb)
    Matrix InverseLeftCauchyGreen ( rLeftCauchyGreen.size1() , rLeftCauchyGreen.size2() );
    double det_b=0;
    MathUtils<double>::InvertMatrix( rLeftCauchyGreen, InverseLeftCauchyGreen, det_b);

    rStrainVector.clear();
    rStrainVector[0] = 0.5 * ( 1.0 - InverseLeftCauchyGreen( 0, 0 ) );
    rStrainVector[1] = 0.5 * ( 1.0 - InverseLeftCauchyGreen( 1, 1 ) );
    rStrainVector[2] = -InverseLeftCauchyGreen( 0, 1 );
    
    //if(abs(rStrainVector[0]) > 100)
    //{
		//std::cout<<"rStrainVector[0] "<<rStrainVector[0]<<std::endl;
		//std::cout<<"rLeftCauchyGreen "<<rLeftCauchyGreen<<std::endl;
	//}
    //std::cout<<" in CalculateAlmansiStrain "<<std::endl;


}


Vector HenckyElasticPlasticPlaneStrainUP2DLaw::SetStressMatrixToAppropiateVectorDimension(Vector& rStressVector, const Matrix& rStressMatrix)
{
	 rStressVector = MathUtils<double>::StressTensorToVector( rStressMatrix, rStressVector.size() );
	 //std::cout<<"rStressMatrix in 2D"<<rStressMatrix<<std::endl;
	 //std::cout<<"rStressVector in 2D"<<rStressVector<<std::endl;
	 //rStressVector(0) = rStressMatrix(0,0);
	 //rStressVector(1) = rStressMatrix(1,1);
	 //rStressVector(2) = rStressMatrix(0,1);
	 
     return rStressVector;

}


Matrix HenckyElasticPlasticPlaneStrainUP2DLaw::SetConstitutiveMatrixToAppropiateDimension(Matrix& rConstitutiveMatrix, const Matrix& rElastoPlasticTangentMatrix)
{
     //Matrix Result = ZeroMatrix(3,3);
	 //ilaria: it has been modified for the use of II mixed formulation
	 if(rConstitutiveMatrix.size1() == 6)
	 {
		 rConstitutiveMatrix = rElastoPlasticTangentMatrix;
	 }
	 else{
		 
     rConstitutiveMatrix(0, 0) = rElastoPlasticTangentMatrix(0, 0);
     rConstitutiveMatrix(0, 1) = rElastoPlasticTangentMatrix(0, 1);
     rConstitutiveMatrix(1, 0) = rElastoPlasticTangentMatrix(1, 0);
     rConstitutiveMatrix(1, 1) = rElastoPlasticTangentMatrix(1, 1);


     rConstitutiveMatrix(2, 0) = rElastoPlasticTangentMatrix(3, 0);
     rConstitutiveMatrix(2, 1) = rElastoPlasticTangentMatrix(3, 1);
     rConstitutiveMatrix(2, 2) = rElastoPlasticTangentMatrix(3, 3);


     rConstitutiveMatrix(0, 2) = rElastoPlasticTangentMatrix(0, 3);
     rConstitutiveMatrix(1, 2) = rElastoPlasticTangentMatrix(1, 3);
	}
	 //std::cout<<"ConstitutiveMatrix in 2D"<<rConstitutiveMatrix<<std::endl;
     return rConstitutiveMatrix;

}
void HenckyElasticPlasticPlaneStrainUP2DLaw::GetLawFeatures(Features& rFeatures)
{
    	//Set the type of law
	rFeatures.mOptions.Set( PLANE_STRAIN_LAW );
	rFeatures.mOptions.Set( FINITE_STRAINS );
	rFeatures.mOptions.Set( ISOTROPIC );
	rFeatures.mOptions.Set( U_P_LAW );

	//Set strain measure required by the consitutive law
	rFeatures.mStrainMeasures.push_back(StrainMeasure_Deformation_Gradient);
	
	//Set the strain size
	rFeatures.mStrainSize = GetStrainSize();

	//Set the spacedimension
	rFeatures.mSpaceDimension = WorkingSpaceDimension();

}










} // Namespace Kratos

