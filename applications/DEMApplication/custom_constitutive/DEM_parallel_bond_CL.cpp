/////////////////////////////////////////////////
// Author: Chengshun Shang (CIMNE)
// Email: chengshun.shang1996@gmail.com
// Date: June 2022
/////////////////////////////////////////////////

// System includes
#include <string>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <algorithm>

// Project includes
#include "DEM_parallel_bond_CL.h"
#include "custom_elements/spheric_continuum_particle.h"

namespace Kratos{

//TODO: understand this
DEMContinuumConstitutiveLaw::Pointer DEM_parallel_bond::Clone() const{
    DEMContinuumConstitutiveLaw::Pointer p_clone(new DEM_parallel_bond(*this));
    return p_clone;
}

//*************************************
// Parameters preparation
//*************************************

void DEM_parallel_bond::TransferParametersToProperties(const Parameters& parameters, Properties::Pointer pProp){
    BaseClassType::TransferParametersToProperties(parameters, pProp);

    //TODO: add the parameters need to be transferred

}

void DEM_parallel_bond::Check(Properties::Pointer pProp) const {
    
    //How many parameters do I need?
    //two parts: discontinuum part and continuum part

    //*********discontinuum part **************
    //for [DEM_D_Hertz_viscous_Coulomb]
    //here we only mention static_friction and dynamic_friction 

    if(!pProp->Has(STATIC_FRICTION)){
        KRATOS_WARNING("DEM")<<std::endl;
        KRATOS_WARNING("DEM")<<"WARNING: Variable STATIC_FRICTION should be present in the properties when using DEMContinuumConstitutiveLaw. 0.0 value assigned by default."<<std::endl;
        KRATOS_WARNING("DEM")<<std::endl;
        pProp->GetValue(STATIC_FRICTION) = 0.0;
    }

    if(!pProp->Has(DYNAMIC_FRICTION)){
        KRATOS_WARNING("DEM")<<std::endl;
        KRATOS_WARNING("DEM")<<"WARNING: Variable DYNAMIC_FRICTION should be present in the properties when using DEMContinuumConstitutiveLaw. 0.0 value assigned by default."<<std::endl;
        KRATOS_WARNING("DEM")<<std::endl;
        pProp->GetValue(DYNAMIC_FRICTION) = 0.0;
    }

    if(!pProp->Has(FRICTION_DECAY)) {
        KRATOS_WARNING("DEM")<<std::endl;
        KRATOS_WARNING("DEM")<<"WARNING: Variable FRICTION_DECAY should be present in the properties when using DEMContinuumConstitutiveLaw. 500.0 value assigned by default."<<std::endl;
        KRATOS_WARNING("DEM")<<std::endl;
        pProp->GetValue(FRICTION_DECAY) = 500.0;
    }

    if(!pProp->Has(COEFFICIENT_OF_RESTITUTION)) {
        KRATOS_WARNING("DEM")<<std::endl;
        KRATOS_WARNING("DEM")<<"WARNING: Variable COEFFICIENT_OF_RESTITUTION should be present in the properties when using DEMContinuumConstitutiveLaw. 0.0 value assigned by default."<<std::endl;
        KRATOS_WARNING("DEM")<<std::endl;
        pProp->GetValue(COEFFICIENT_OF_RESTITUTION) = 0.0;
    }

    if(!pProp->Has(ROLLING_FRICTION)) {
        KRATOS_WARNING("DEM")<<std::endl;
        KRATOS_WARNING("DEM")<<"WARNING: Variable ROLLING_FRICTION should be present in the properties when using DEMContinuumConstitutiveLaw. 0.0 value assigned by default."<<std::endl;
        KRATOS_WARNING("DEM")<<std::endl;
        pProp->GetValue(ROLLING_FRICTION) = 0.0;
    }

    if(!pProp->Has(ROLLING_FRICTION_WITH_WALLS)) {
        KRATOS_WARNING("DEM")<<std::endl;
        KRATOS_WARNING("DEM")<<"WARNING: Variable ROLLING_FRICTION_WITH_WALLS should be present in the properties when using DEMContinuumConstitutiveLaw. 0.0 value assigned by default."<<std::endl;
        KRATOS_WARNING("DEM")<<std::endl;
        pProp->GetValue(ROLLING_FRICTION_WITH_WALLS) = 0.0;
    }

    //********** continuum part ***************
    //for [DEM_parallel_bond_CL]

    if(!pProp->Has(BOND_YOUNG_MODULUS)) {

        KRATOS_ERROR << "Variable BONDED_MATERIAL_YOUNG_MODULUS should be present in the properties when using DEM_parallel_bond_CL."<<std::endl;

    }

    if(!pProp->Has(BOND_KNKS_RATIO)) {
        KRATOS_WARNING("DEM")<<std::endl;
        KRATOS_WARNING("DEM")<<"WARNING: Variable BOND_KNKS_RATIO should be present in the properties when using DEM_parallel_bond_CL. 2.5 value assigned by default."<<std::endl;
        KRATOS_WARNING("DEM")<<std::endl;
        pProp->GetValue(BOND_KNKS_RATIO) = 2.5;
    }

    if(!pProp->Has(BOND_SIGMA_MAX)) {

        KRATOS_ERROR << "Variable BOND_SIGMA_MAX should be present in the properties when using DEM_parallel_bond_CL."<<std::endl;

    }

    if(!pProp->Has(BOND_SIGMA_MAX_DEVIATION)) {
        KRATOS_WARNING("DEM")<<std::endl;
        KRATOS_WARNING("DEM")<<"WARNING: Variable BOND_SIGMA_MAX_DEVIATION should be present in the properties when using DEM_parallel_bond_CL. 0.0 value assigned by default."<<std::endl;
        KRATOS_WARNING("DEM")<<std::endl;
        pProp->GetValue(BOND_SIGMA_MAX_DEVIATION) = 0.0;
    }

    if(!pProp->Has(BOND_TAU_ZERO)) {

        KRATOS_ERROR << "Variable BOND_TAU_ZERO should be present in the properties when using DEM_parallel_bond_CL."<<std::endl;

    }

    if(!pProp->Has(BOND_TAU_ZERO_DEVIATION)) {
        KRATOS_WARNING("DEM")<<std::endl;
        KRATOS_WARNING("DEM")<<"WARNING: Variable BOND_TAU_ZERO_DEVIATION should be present in the properties when using DEM_parallel_bond_CL. 0.0 value assigned by default."<<std::endl;
        KRATOS_WARNING("DEM")<<std::endl;
        pProp->GetValue(BOND_TAU_ZERO_DEVIATION) = 0.0;
    }

    if(!pProp->Has(BOND_INTERNAL_FRICC)) {

        KRATOS_ERROR << "Variable BOND_INTERNAL_FRICC should be present in the properties when using DEM_parallel_bond_CL."<<std::endl;

    }

    if(!pProp->Has(BOND_ROTATIONAL_MOMENT_COEFFICIENT)) {
        KRATOS_WARNING("DEM")<<std::endl;
        KRATOS_WARNING("DEM")<<"WARNING: Variable BOND_ROTATIONAL_MOMENT_COEFFICIENT should be present in the properties when using DEM_parallel_bond_CL. 0.1 value assigned by default."<<std::endl;
        KRATOS_WARNING("DEM")<<std::endl;
        pProp->GetValue(BOND_ROTATIONAL_MOMENT_COEFFICIENT) = 0.1;
    }

    if(!pProp->Has(BOND_RADIUS_FACTOR)) {
        KRATOS_WARNING("DEM")<<std::endl;
        KRATOS_WARNING("DEM")<<"WARNING: Variable BOND_RADIUS_FACTOR should be present in the properties when using DEM_parallel_bond_CL. 1.0 value assigned by default."<<std::endl;
        KRATOS_WARNING("DEM")<<std::endl;
        pProp->GetValue(BOND_RADIUS_FACTOR) = 1.0;
    }

    if (!pProp->Has(IS_UNBREAKABLE)) {
        KRATOS_WARNING("DEM")<<std::endl;
        KRATOS_WARNING("DEM")<<"WARNING: Variable IS_UNBREAKABLE was not present in the properties when using DEM_parallel_bond_CL. False value assigned by default."<<std::endl;
        KRATOS_WARNING("DEM")<<std::endl;
        pProp->GetValue(IS_UNBREAKABLE) = false;
    }
} // CHECK()

// TODO: Calculate Contact Area = Calculate Bond area?
void DEM_parallel_bond::CalculateContactArea(double radius, double other_radius, double& calculation_area) {

    KRATOS_TRY

    double aim_radius = min(radius, other_radius);
    calculation_area = Globals::Pi * aim_radius * aim_radius;

    KRATOS_CATCH("")
}

double DEM_parallel_bond::CalculateContactArea(double radius, double other_radius, Vector& v) {
    
    KRATOS_TRY

    double a = 0.0;
    CalculateContactArea(radius, other_radius, a);
    unsigned int old_size = v.size();
    Vector backup = v;
    v.resize(old_size + 1, false);
    v[old_size] = a;
    for (unsigned int i=0; i<old_size; i++) {
        v[i] = backup[i];
    }
    return a;

    KRATOS_CATCH("")
}

void DEM_parallel_bond::GetcontactArea(const double radius, const double other_radius, const Vecotr& vector_of_initial_areas, const int neighbour_position, double& calculation_area) {
    
    KRATOS_TRY

    if(vector_of_initial_areas.size()){
        calculation_area = vector_of_initial_areas[neighbour_position];
    }
    else{
        CalculateContactArea(radius, other_radius, calculation_area);
    }

    KRATOS_CATCH("")
}

// TODO: In this function, it is better to replace 'kn_el' with 'kn_bond' and 'kt_el' with 'kt_bond'.
void DEM_parallel_bond::CalculateElasticConstants(double& kn_el, double& kt_el, double initial_dist, double equiv_young,
                                double equiv_poisson, double calculation_area, SphericContinuumParticle* element1, SphericContinuumParticle* element2, double indentation) {

    KRATOS_TRY

    //for unbonded part
    //TODO: check whether the unbonded elastic parameters should be calculated here
    //maybe they should not be here.
    const double my_radius       = element1->GetRadius();
    const double other_radius    = element2->GetRadius();
    const double radius_sum      = my_radius + other_radius;
    const double radius_sum_inv  = 1.0 / radius_sum;
    const double equiv_radius    = my_radius * other_radius * radius_sum_inv;

    //Get equivalent Young's Modulus
    const double my_young        = element1->GetYoung();
    const double other_young     = element2->GetYoung();
    const double my_poisson      = element1->GetPoisson();
    const double other_poisson   = element2->GetPoisson();
    const double equiv_young     = my_young * other_young / (other_young * (1.0 - my_poisson * my_poisson) + my_young * (1.0 - other_poisson * other_poisson));

    //Get equivalent Shear Modulus
    const double my_shear_modulus = 0.5 * my_young / (1.0 + my_poisson);
    const double other_shear_modulus = 0.5 * other_young / (1.0 + other_poisson);
    const double equiv_shear = 1.0 / ((2.0 - my_poisson)/my_shear_modulus + (2.0 - other_poisson)/other_shear_modulus);

    //Normal and Tangent elastic constants
    mUnbondedNormalElasticConstant = equiv_young * Globals::Pi * min(my_radius, other_radius) / radius_sum;

    //for bonded part
    const double bond_equiv_young = (*mpProperties)[BOND_YOUNG_MODULUS];
    kn_el = bond_equiv_young * calculation_area / initial_dist;
    kt_el = kn_el / (*mpProperties)[BOND_KNKS_RATIO];

    KRATOS_CATCH("")

}

double DEM_parallel_bond::LocalMaxSearchDistance(const int i,
                            SphericContinuumParticle* element1,
                            SphericContinuumParticle* element2) {
    KRATOS_TRY

    // TODO: maybe this function is unnecessary. Try to understand it

    KRATOS_CATCH("")
}

double DEM_parallel_bond::GetContactSigmaMax(){

    KRATOS_TRY

    // TODO: maybe this function is unnecessary

    KRATOS_CATCH("")    
}

//check bond state
void CheckFailure(const int i_neighbour_count, SphericContinuumParticle* element1, SphericContinuumParticle* element2){
    
    KRATOS_TRY

    int& failure_type = element1->mIniNeighbourFailureId[i_neighbour_count];

    if (failure_type == 0) {

        Vector principal_stresses(3);
        noalias(principal_stresses) = AuxiliaryFunctions::EigenValuesDirectMethod(average_stress_tensor);

        const double& mohr_coulomb_c = (*mpProperties)[INTERNAL_COHESION];
        const double& mohr_coulomb_phi = (*mpProperties)[INTERNAL_FRICTION_ANGLE];
        const double mohr_coulomb_phi_in_radians = mohr_coulomb_phi * Globals::Pi / 180.0;
        const double sinphi = std::sin(mohr_coulomb_phi_in_radians);

        if(function_value > 0) {
            failure_type = 4;
        }

    }

    KRATOS_CATCH("")    
}

//*************************************
// Force calculation
//*************************************

void DEM_parallel_bond::CalculateForces(const ProcessInfo& r_process_info,
                            double OldLocalElasticContactForce[3],
                            double LocalElasticContactForce[3],
                            double LocalElasticExtraContactForce[3],
                            double LocalCoordSystem[3][3],
                            double LocalDeltDisp[3],
                            const double kn_el,
                            const double kt_el,
                            double& contact_sigma,
                            double& contact_tau,
                            double& failure_criterion_state,
                            double equiv_young,
                            double equiv_shear,
                            double indentation,
                            double calculation_area,
                            double& acumulated_damage,
                            SphericContinuumParticle* element1,
                            SphericContinuumParticle* element2,
                            int i_neighbour_count,
                            int time_steps,
                            bool& sliding,
                            double &equiv_visco_damp_coeff_normal,
                            double &equiv_visco_damp_coeff_tangential,
                            double LocalRelVel[3],
                            double ViscoDampingLocalContactForce[3]) {
                           
    KRATOS_TRY

    CalculateNormalForces(LocalElasticContactForce,
                        kn_el,
                        equiv_young,
                        indentation,
                        calculation_area,
                        acumulated_damage,
                        element1,
                        element2,
                        i_neighbour_count,
                        time_steps,
                        r_process_info);
            
    CalculateViscoDampingCoeff(equiv_visco_damp_coeff_normal,
                            equiv_visco_damp_coeff_tangential,
                            element1,
                            element2,
                            kn_el,
                            kt_el);

    CalculateViscoDamping(LocalRelVel,
                        ViscoDampingLocalContactForce,
                        indentation,
                        equiv_visco_damp_coeff_normal,
                        equiv_visco_damp_coeff_tangential,
                        sliding,
                        element1->mIniNeighbourFailureId[i_neighbour_count]);

    // Tangential forces are calculated after the viscodamping because the frictional limit bounds the sum of elastic plus viscous forces
    CalculateTangentialForces(OldLocalElasticContactForce,
                            LocalElasticContactForce,
                            LocalElasticExtraContactForce,
                            ViscoDampingLocalContactForce,
                            LocalCoordSystem,
                            LocalDeltDisp,
                            LocalRelVel,
                            kt_el,
                            equiv_shear,
                            contact_sigma,
                            contact_tau,
                            indentation,
                            calculation_area,
                            failure_criterion_state,
                            element1,
                            element2,
                            i_neighbour_count,
                            sliding,
                            r_process_info);

    KRATOS_CATCH("") 
}


void DEM_parallel_bond::ComputeNormalUnbondedForce(double unbonded_indentation){
    
    KRATOS_TRY

    if (unbonded_indentation > 0.0) {
        mUnbondedLocalElasticContactForce2 = mUnbondedNormalElasticConstant * unbonded_indentation;
    } else {
        mUnbondedLocalElasticContactForce2 = 0.0;
    }

    KRATOS_CATCH("")
}

void DEM_parallel_bond::CalculateNormalForces(double LocalElasticContactForce[3],
                const double kn_el,
                double equiv_young,
                double indentation,
                double calculation_area,
                double& acumulated_damage,
                SphericContinuumParticle* element1,
                SphericContinuumParticle* element2,
                int i_neighbour_count,
                int time_steps,
            const ProcessInfo& r_process_info) {

    KRATOS_TRY

    int& failure_type = element1->mIniNeighbourFailureId[i_neighbour_count];
    double BondedLocalElasticContactForce2 = 0.0;
    const double bonded_indentation = indentation - mInitialIndentationForBondedPart;                                                                                                          
    double unbonded_indentation = indentation - element1->GetInitialDelta(i_neighbour_count);
    //const double& bond_sigma_max = (*mpProperties)[BOND_SIGMA_MAX];
    //double bond_current_sigma = 0.0;
    //const double& bond_rotational_moment_coefficient =(*mpProperties)[BOND_ROTATIONAL_MOMENT_COEFFICIENT];

    if (!failure_type){ //if the bond is not broken
        BondedLocalElasticContactForce2 = kn_el * bonded_indentation;
    } else { //else the bond is broken
        BondedLocalElasticContactForce2 = 0.0;
    }

    ComputeNormalUnbondedForce(unbonded_indentation);

    LocalElasticContactForce[2] = BondedLocalElasticContactForce2 + mUnbondedLocalElasticContactForce2;

    KRATOS_CATCH("")  

} // CalculateNormalForces

void DEM_parallel_bond::CalculateViscoDampingCoeff(double &equiv_visco_damp_coeff_normal,
                        double &equiv_visco_damp_coeff_tangential,
                        SphericContinuumParticle* element1,
                        SphericContinuumParticle* element2,
                        const double kn_el,
                        const double kt_el) {

    KRATOS_TRY

    const double my_mass    = element1->GetMass();
    const double other_mass = element2->GetMass();

    const double equiv_mass = 1.0 / (1.0/my_mass + 1.0/other_mass);

    // TODO: check the value of gamma
    const double equiv_gamma = (*mpProperties)[DAMPING_GAMMA];

    equiv_visco_damp_coeff_normal     = 2.0 * equiv_gamma * sqrt(equiv_mass * kn_el);
    equiv_visco_damp_coeff_tangential = 2.0 * equiv_gamma * sqrt(equiv_mass * kt_el);

    mUnbondedEquivViscoDampCoeffNormal = 2.0 * equiv_gamma * sqrt(equiv_mass * mUnbondedNormalElasticConstant);
    mUnbondedEquivViscoDampCoeffTangential = 2.0 * equiv_gamma * sqrt(equiv_mass * mUnbondedTangentialElasticConstant);

    KRATOS_CATCH("")                    
}

void DEM_parallel_bond::CalculateViscoDamping(double LocalRelVel[3],
                        double ViscoDampingLocalContactForce[3],
                        double indentation,
                        double equiv_visco_damp_coeff_normal,
                        double equiv_visco_damp_coeff_tangential,
                        bool& sliding,
                        int failure_id,
                        int i_neighbour_count) {
    KRATOS_TRY

    mUnbondedViscoDampingLocalContactForce[0] = 0.0;
    mUnbondedViscoDampingLocalContactForce[1] = 0.0;
    mUnbondedViscoDampingLocalContactForce[2] = 0.0;
    mBondedViscoDampingLocalContactForce[0] = 0.0;
    mBondedViscoDampingLocalContactForce[1] = 0.0;
    mBondedViscoDampingLocalContactForce[2] = 0.0;

    double unbonded_indentation = indentation - element1->GetInitialDelta(i_neighbour_count);

    if (unbonded_indentation > 0) {
        mUnbondedViscoDampingLocalContactForce[0] = -mUnbondedEquivViscoDampCoeffTangential * LocalRelVel[0];
        mUnbondedViscoDampingLocalContactForce[1] = -mUnbondedEquivViscoDampCoeffTangential * LocalRelVel[1];
        mUnbondedViscoDampingLocalContactForce[2] = -mUnbondedEquivViscoDampCoeffNormal * LocalRelVel[2];
    }

    if (!failure_id) {
        mBondedViscoDampingLocalContactForce[0] = -equiv_visco_damp_coeff_tangential * LocalRelVel[0];
        mBondedViscoDampingLocalContactForce[1] = -equiv_visco_damp_coeff_tangential * LocalRelVel[1];
        mBondedViscoDampingLocalContactForce[2] = -equiv_visco_damp_coeff_normal * LocalRelVel[2];
    }

    ViscoDampingLocalContactForce[0] = mUnbondedViscoDampingLocalContactForce[0] + mBondedViscoDampingLocalContactForce[0];
    ViscoDampingLocalContactForce[1] = mUnbondedViscoDampingLocalContactForce[1] + mBondedViscoDampingLocalContactForce[1];
    ViscoDampingLocalContactForce[2] = mUnbondedViscoDampingLocalContactForce[2] + mBondedViscoDampingLocalContactForce[2];

    double unbonded_normal_contact_force = mUnbondedLocalElasticContactForce2 + mUnbondedViscoDampingLocalContactForce[2];

    if (unbonded_normal_contact_force < 0.0) {
        mUnbondedViscoDampingLocalContactForce[2] = -1.0 * mUnbondedLocalElasticContactForce2;
        ViscoDampingLocalContactForce[2] = mUnbondedViscoDampingLocalContactForce[2] + mBondedViscoDampingLocalContactForce[2];
    }

    KRATOS_CATCH("") 
}

void DEM_parallel_bond::CalculateTangentialForces(double OldLocalElasticContactForce[3],
                        double LocalElasticContactForce[3],
                        double LocalElasticExtraContactForce[3],
                        double ViscoDampingLocalContactForce[3],
                        double LocalCoordSystem[3][3],
                        double LocalDeltDisp[3],
                        double LocalRelVel[3],
                        const double kt_el,
                        const double equiv_shear,
                        double& contact_sigma,
                        double& contact_tau,
                        double indentation,
                        double calculation_area,
                        double& failure_criterion_state,
                        SphericContinuumParticle* element1,
                        SphericContinuumParticle* element2,
                        int i_neighbour_count,
                        bool& sliding,
                        const ProcessInfo& r_process_info) {

    KRATOS_TRY

    // TODO: add [BOND_TAU_ZERO_DEVIATION]
    const double& bond_tau_zero = (*mpProperties)[BOND_TAU_ZERO];
    const double& internal_friction = (*mpProperties)[BOND_INTERNAL_FRICC];
    int& failure_type = element1->mIniNeighbourFailureId[i_neighbour_count];

    // The [mBondedScalingFactor] is divided into two direction [0] and [1]. June, 2022
    double OldBondedLocalElasticContactForce[2] = {0.0};
    double OldBondedLocalElasticContactForce[0] = mBondedScalingFactor[0] * OldLocalElasticContactForce[0];
    double OldBondedLocalElasticContactForce[1] = mBondedScalingFactor[1] * OldLocalElasticContactForce[1];

    // bond force
    if (!failure_type) {
        mAccumulatedBondedTangentialLocalDisplacement[0] += LocalDeltDisp[0];
        mAccumulatedBondedTangentialLocalDisplacement[1] += LocalDeltDisp[1];
        BondedLocalElasticContactForce[0] -= kt_el * mAccumulatedBondedTangentialLocalDisplacement[0]; // 0: first tangential
        BondedLocalElasticContactForce[1] -= kt_el * mAccumulatedBondedTangentialLocalDisplacement[1]; // 1: second tangential
    } else {
        BondedLocalElasticContactForce[0] = 0.0; // 0: first tangential
        BondedLocalElasticContactForce[1] = 0.0; // 1: second tangential
    }

    //unbonded force
    double OldUnbondedLocalElasticContactForce[2] = {0.0};
    double UnbondedLocalElasticContactForce[2] = {0.0};
    double ActualTotalShearForce = 0.0;
    double max_admissible_shear_force = 0.0;
    double fraction = 0.0;
    double unbonded_indentation = indentation - element1->GetInitialDelta(i_neighbour_count);

    if (unbonded_indentation <= 0.0) {
        UnbondedLocalElasticContactForce[0] = 0.0;
        UnbondedLocalElasticContactForce[1] = 0.0;
    } else {
        // Here, unBondedScalingFactor[] = 1 - mBondedScalingFactor[]
        OldUnbondedLocalElasticContactForce[0] = (1 - mBondedScalingFactor[0]) * OldLocalElasticContactForce[0];
        OldUnbondedLocalElasticContactForce[1] = (1 - mBondedScalingFactor[1]) * OldLocalElasticContactForce[1];

        UnbondedLocalElasticContactForce[0] = OldUnbondedLocalElasticContactForce[0] - mUnbondedTangentialElasticConstant * LocalDeltDisp[0];
        UnbondedLocalElasticContactForce[1] = OldUnbondedLocalElasticContactForce[1] - mUnbondedTangentialElasticConstant * LocalDeltDisp[1];

        const double& equiv_tg_of_static_fri_ang = (*mpProperties)[STATIC_FRICTION];
        const double& equiv_tg_of_dynamic_fri_ang = (*mpProperties)[DYNAMIC_FRICTION];
        const double& equiv_friction_decay_coefficient = (*mpProperties)[FRICTION_DECAY];

        const double ShearRelVel = sqrt(LocalRelVel[0] * LocalRelVel[0] + LocalRelVel[1] * LocalRelVel[1]);
        // TODO: where this equation from? [equiv_friction]
        double equiv_friction = equiv_tg_of_dynamic_fri_ang + (equiv_tg_of_static_fri_ang - equiv_tg_of_dynamic_fri_ang) * exp(-equiv_friction_decay_coefficient * ShearRelVel);

        max_admissible_shear_force = (mUnbondedLocalElasticContactForce2 + mUnbondedViscoDampingLocalContactForce[2]) * equiv_friction;

        if (equiv_tg_of_static_fri_ang < 0.0 || equiv_tg_of_dynamic_fri_ang < 0.0) {
            KRATOS_ERROR << "The averaged friction is negative for one contact of element with Id: "<< element1->Id()<<std::endl;
        }

        const double tangential_contact_force_0 = UnbondedLocalElasticContactForce[0] + mUnbondedViscoDampingLocalContactForce[0];
        const double tangential_contact_force_1 = UnbondedLocalElasticContactForce[1] + mUnbondedViscoDampingLocalContactForce[1];

        ActualTotalShearForce = sqrt(tangential_contact_force_0 * tangential_contact_force_0 
                                    + tangential_contact_force_1 * tangential_contact_force_1);

        if (ActualTotalShearForce > max_admissible_shear_force) {
            const double ActualElasticShearForce = sqrt(UnbondedLocalElasticContactForce[0] * UnbondedLocalElasticContactForce[0] 
                                                        + UnbondedLocalElasticContactForce[1] * UnbondedLocalElasticContactForce[1]);

            const double dot_product = UnbondedLocalElasticContactForce[0] * mUnbondedViscoDampingLocalContactForce[0] 
                                        + UnbondedLocalElasticContactForce[1] * mUnbondedViscoDampingLocalContactForce[1];
            const double ViscoDampingLocalContactForceModule = sqrt(mUnbondedViscoDampingLocalContactForce[0] * mUnbondedViscoDampingLocalContactForce[0] +
                                                                    mUnbondedViscoDampingLocalContactForce[1] * mUnbondedViscoDampingLocalContactForce[1]);

            if (dot_product >= 0.0) {
                if (ActualElasticShearForce > max_admissible_shear_force) {
                    // if the ActualElasticShearForce is too big, it should be reduced
                    fraction = max_admissible_shear_force / ActualElasticShearForce;
                    UnbondedLocalElasticContactForce[0]      = UnbondedLocalElasticContactForce[0] * fraction;
                    UnbondedLocalElasticContactForce[1]      = UnbondedLocalElasticContactForce[1] * fraction;
                    mUnbondedViscoDampingLocalContactForce[0] = 0.0;
                    mUnbondedViscoDampingLocalContactForce[1] = 0.0;
                }
                else {
                    const double ActualViscousShearForce = max_admissible_shear_force - ActualElasticShearForce;
                    fraction = ActualViscousShearForce / ViscoDampingLocalContactForceModule;
                    mUnbondedViscoDampingLocalContactForce[0] *= fraction;
                    mUnbondedViscoDampingLocalContactForce[1] *= fraction;
                }
            }
            else {
                if (ViscoDampingLocalContactForceModule >= ActualElasticShearForce) {
                    fraction = (max_admissible_shear_force + ActualElasticShearForce) / ViscoDampingLocalContactForceModule;
                    mUnbondedViscoDampingLocalContactForce[0] *= fraction;
                    mUnbondedViscoDampingLocalContactForce[1] *= fraction;
                }
                else {
                    fraction = max_admissible_shear_force / ActualElasticShearForce;
                    UnbondedLocalElasticContactForce[0]      = UnbondedLocalElasticContactForce[0] * fraction;
                    UnbondedLocalElasticContactForce[1]      = UnbondedLocalElasticContactForce[1] * fraction;
                    mUnbondedViscoDampingLocalContactForce[0] = 0.0;
                    mUnbondedViscoDampingLocalContactForce[1] = 0.0;
                }
            }
            ViscoDampingLocalContactForce[0] = mBondedViscoDampingLocalContactForce[0] + mUnbondedViscoDampingLocalContactForce[0];
            ViscoDampingLocalContactForce[1] = mBondedViscoDampingLocalContactForce[1] + mUnbondedViscoDampingLocalContactForce[1];
            sliding = true;
        }
    }

    LocalElasticContactForce[0] = BondedLocalElasticContactForce[0] + UnbondedLocalElasticContactForce[0];
    LocalElasticContactForce[1] = BondedLocalElasticContactForce[1] + UnbondedLocalElasticContactForce[1];

    double local_elastic_force_modulus = sqrt(LocalElasticContactForce[0] * LocalElasticContactForce[0] +
                                                LocalElasticContactForce[1] * LocalElasticContactForce[1]);

    // Here, we only calculate the BondedScalingFactor and [unBondedScalingFactor = 1 - BondedScalingFactor].
    if (local_elastic_force_modulus) {
        mBondedScalingFactor[0] = BondedLocalElasticContactForce[0] / LocalElasticContactForce[0]; 
        mBondedScalingFactor[1] = BondedLocalElasticContactForce[1] / LocalElasticContactForce[1];
    } else {
        mBondedScalingFactor[0] = mBondedScalingFactor[1] = 0.0;
    }

    KRATOS_CATCH("")
}

//*************************************
// Moment calculation
//*************************************

double DEM_KDEM::GetYoungModulusForComputingRotationalMoments(const double& equiv_young){
        return equiv_young;
    }

void DEM_KDEM::ComputeParticleRotationalMoments(SphericContinuumParticle* element,
                                                SphericContinuumParticle* neighbor,
                                                double equiv_young,
                                                double distance,
                                                double calculation_area,
                                                double LocalCoordSystem[3][3],
                                                double ElasticLocalRotationalMoment[3],
                                                double ViscoLocalRotationalMoment[3],
                                                double equiv_poisson,
                                                double indentation) {

    KRATOS_TRY
    const double& rotational_moment_coeff = (*mpProperties)[ROTATIONAL_MOMENT_COEFFICIENT];
    //double LocalRotationalMoment[3]     = {0.0};
    double LocalDeltaRotatedAngle[3]    = {0.0};
    double LocalDeltaAngularVelocity[3] = {0.0};

    array_1d<double, 3> GlobalDeltaRotatedAngle;
    noalias(GlobalDeltaRotatedAngle) = element->GetGeometry()[0].FastGetSolutionStepValue(PARTICLE_ROTATION_ANGLE) 
                                        - neighbor->GetGeometry()[0].FastGetSolutionStepValue(PARTICLE_ROTATION_ANGLE);
    array_1d<double, 3> GlobalDeltaAngularVelocity;
    noalias(GlobalDeltaAngularVelocity) = element->GetGeometry()[0].FastGetSolutionStepValue(ANGULAR_VELOCITY) 
                                        - neighbor->GetGeometry()[0].FastGetSolutionStepValue(ANGULAR_VELOCITY);

    GeometryFunctions::VectorGlobal2Local(LocalCoordSystem, GlobalDeltaRotatedAngle, LocalDeltaRotatedAngle);
    GeometryFunctions::VectorGlobal2Local(LocalCoordSystem, GlobalDeltaAngularVelocity, LocalDeltaAngularVelocity);

    const double equivalent_radius = std::sqrt(calculation_area / Globals::Pi);
    const double element_mass  = element->GetMass();
    const double neighbor_mass = neighbor->GetMass();
    const double equiv_mass    = element_mass * neighbor_mass / (element_mass + neighbor_mass);

    const double young_modulus = GetYoungModulusForComputingRotationalMoments(equiv_young);

    const double Inertia_I     = 0.25 * Globals::Pi * equivalent_radius * equivalent_radius * equivalent_radius * equivalent_radius;
    const double Inertia_J     = 2.0 * Inertia_I; // This is the polar inertia

    const double& damping_gamma = (*mpProperties)[DAMPING_GAMMA];

    //Viscous parameter taken from Olmedo et al., 'Discrete element model of the dynamic response of fresh wood stems to impact'
    array_1d<double, 3> visc_param;
    visc_param[0] = 2.0 * damping_gamma * std::sqrt(equiv_mass * young_modulus * Inertia_I / distance); // OLMEDO
    visc_param[1] = 2.0 * damping_gamma * std::sqrt(equiv_mass * young_modulus * Inertia_I / distance); // OLMEDO
    visc_param[2] = 2.0 * damping_gamma * std::sqrt(equiv_mass * young_modulus * Inertia_J / distance); // OLMEDO

    double aux = (element->GetRadius() + neighbor->GetRadius()) / distance; // This is necessary because if spheres are not tangent the DeltaAngularVelocity has to be interpolated

    array_1d<double, 3> LocalEffDeltaRotatedAngle;
    LocalEffDeltaRotatedAngle[0] = LocalDeltaRotatedAngle[0] * aux;
    LocalEffDeltaRotatedAngle[1] = LocalDeltaRotatedAngle[1] * aux;
    LocalEffDeltaRotatedAngle[2] = LocalDeltaRotatedAngle[2] * aux;

    array_1d<double, 3> LocalEffDeltaAngularVelocity;
    LocalEffDeltaAngularVelocity[0] = LocalDeltaAngularVelocity[0] * aux;
    LocalEffDeltaAngularVelocity[1] = LocalDeltaAngularVelocity[1] * aux;
    LocalEffDeltaAngularVelocity[2] = LocalDeltaAngularVelocity[2] * aux;

    ElasticLocalRotationalMoment[0] = -young_modulus * Inertia_I * LocalEffDeltaRotatedAngle[0] / distance;
    ElasticLocalRotationalMoment[1] = -young_modulus * Inertia_I * LocalEffDeltaRotatedAngle[1] / distance;
    ElasticLocalRotationalMoment[2] = -young_modulus * Inertia_J * LocalEffDeltaRotatedAngle[2] / distance;

    ViscoLocalRotationalMoment[0] = -visc_param[0] * LocalEffDeltaAngularVelocity[0];
    ViscoLocalRotationalMoment[1] = -visc_param[1] * LocalEffDeltaAngularVelocity[1];
    ViscoLocalRotationalMoment[2] = -visc_param[2] * LocalEffDeltaAngularVelocity[2];

    DEM_MULTIPLY_BY_SCALAR_3(ElasticLocalRotationalMoment, rotational_moment_coeff);
    DEM_MULTIPLY_BY_SCALAR_3(ViscoLocalRotationalMoment, rotational_moment_coeff);

    KRATOS_CATCH("")
}//ComputeParticleRotationalMoments
    
} //namespace Kratos