// KRATOS  ___|  |                   |                   |
//       \___ \  __|  __| |   |  __| __| |   |  __| _` | |
//             | |   |    |   | (    |   |   | |   (   | |
//       _____/ \__|_|   \__,_|\___|\__|\__,_|_|  \__,_|_| MECHANICS
//
//  License:         BSD License
//                   license: structural_mechanics_application/license.txt
//
//  Main authors:    Vicente Mataix Ferrandiz
//
// System includes
#include <iostream>

// External includes

// Project includes
#include "custom_constitutive/axisym_elastic_isotropic.h"

#include "structural_mechanics_application_variables.h"

namespace Kratos
{
//******************************CONSTRUCTOR*******************************************
//************************************************************************************

AxisymElasticIsotropic::AxisymElasticIsotropic()
    : ElasticIsotropic3D()
{
}

//******************************COPY CONSTRUCTOR**************************************
//************************************************************************************

AxisymElasticIsotropic::AxisymElasticIsotropic(const AxisymElasticIsotropic& rOther)
    : ElasticIsotropic3D(rOther)
{
}

//********************************CLONE***********************************************
//************************************************************************************

ConstitutiveLaw::Pointer AxisymElasticIsotropic::Clone() const
{
    AxisymElasticIsotropic::Pointer p_clone(new AxisymElasticIsotropic(*this));
    return p_clone;
}

//*******************************DESTRUCTOR*******************************************
//************************************************************************************

AxisymElasticIsotropic::~AxisymElasticIsotropic()
{
};

//*************************CONSTITUTIVE LAW GENERAL FEATURES *************************
//************************************************************************************

void AxisymElasticIsotropic::GetLawFeatures(Features& rFeatures)
{
    //Set the type of law
    rFeatures.mOptions.Set( AXISYMMETRIC_LAW );
    rFeatures.mOptions.Set( INFINITESIMAL_STRAINS );
    rFeatures.mOptions.Set( ISOTROPIC );

    //Set strain measure required by the consitutive law
    rFeatures.mStrainMeasures.push_back(StrainMeasure_Infinitesimal);
    rFeatures.mStrainMeasures.push_back(StrainMeasure_Deformation_Gradient);

    //Set the strain size
    rFeatures.mStrainSize = 4;

    //Set the spacedimension
    rFeatures.mSpaceDimension = 2;
}

//************************************************************************************
//************************************************************************************

void AxisymElasticIsotropic::CalculateElasticMatrix(VoigtSizeMatrixType& C, ConstitutiveLaw::Parameters& rValues)
{
    const Properties& MaterialProperties = rValues.GetMaterialProperties();
    const double& E = MaterialProperties[YOUNG_MODULUS];
    const double& NU = MaterialProperties[POISSON_RATIO];

    if (C.size1() != 4 || C.size2() != 4)
        C.resize(4, 4, false);
    noalias(C) = ZeroMatrix(4, 4);

    const double c1 = E / ((1.00 + NU) * (1.0 - 2.0 * NU));
    const double c0 = c1 * (1.0 - NU);
    const double c2 = c1 * (1.0 - 2.0 * NU) / (2.0);
    const double c3 = NU * c1;

    C(0, 0) = c0;
    C(0, 1) = c3;
    C(0, 2) = c3;

    C(1, 0) = c3;
    C(1, 1) = c0;
    C(1, 2) = c3;

    C(2, 0) = c3;
    C(2, 1) = c3;
    C(2, 2) = c0;

    C(3, 3) = c2;
}

/***********************************************************************************/
/***********************************************************************************/

void AxisymElasticIsotropic::CalculatePK2Stress(
    const Vector& rStrainVector,
    ConstitutiveLaw::StressVectorType& rStressVector,
    ConstitutiveLaw::Parameters& rValues
    )
{
    const Properties& r_material_properties = rValues.GetMaterialProperties();
    const double E = r_material_properties[YOUNG_MODULUS];
    const double NU = r_material_properties[POISSON_RATIO];

    const double c1 = E / ((1.0 + NU) * (1.0 - 2.0 * NU));
    const double c0 = c1 * (1.0 - NU);
    const double c2 = c1 * (1.0 - 2.0 * NU) / (2.0);
    const double c3 = NU * c1;

    rStressVector[0] = (c0 * rStrainVector[0] + c3 * rStrainVector[1] + c3 * rStrainVector[2]);
    rStressVector[1] = (c3 * rStrainVector[0] + c0 * rStrainVector[1] + c3 * rStrainVector[2]);
    rStressVector[2] = (c3 * rStrainVector[0] + c3 * rStrainVector[1] + c0 * rStrainVector[2]);
    rStressVector[3] = (c2 * rStrainVector[3]);
}

//************************************************************************************
//************************************************************************************

void AxisymElasticIsotropic::CalculateCauchyGreenStrain(
    ConstitutiveLaw::Parameters& rValues,
    ConstitutiveLaw::StrainVectorType& rStrainVector
)
{
    //1.-Compute total deformation gradient
    const Matrix& F = rValues.GetDeformationGradientF();

    const Matrix RightCauchyGreen = prod(trans(F),F);
    rStrainVector[0] = 0.5 * ( RightCauchyGreen( 0, 0 ) - 1.00 );
    rStrainVector[1] = 0.5 * ( RightCauchyGreen( 1, 1 ) - 1.00 );
    rStrainVector[2] = 0.5 * ( RightCauchyGreen( 2, 2 ) - 1.00 );
    rStrainVector[3] = RightCauchyGreen( 0, 1 );
}

} // Namespace Kratos
