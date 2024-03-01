//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Vicente Mataix Ferrandiz
//

// System includes
#include <iomanip> // For std::boolalpha

// External includes

// Project includes
#include "containers/model.h"
#include "utilities/parallel_utilities.h"
#include "utilities/variable_utils.h"
#include "processes/calculate_nodal_distance_to_skin_process.h"
#include "spatial_containers/geometrical_objects_bins.h"

namespace Kratos
{
/// Local Flags
KRATOS_CREATE_LOCAL_FLAG( CalculateNodalDistanceToSkinProcess, HISTORICAL_VALUE,      0 );
KRATOS_CREATE_LOCAL_FLAG( CalculateNodalDistanceToSkinProcess, SAVE_DISTANCE_IN_SKIN, 1 );

/***********************************************************************************/
/***********************************************************************************/

CalculateNodalDistanceToSkinProcess::CalculateNodalDistanceToSkinProcess(
    ModelPart& rVolumeModelPart,
    ModelPart& rSkinModelPart,
    const bool HistoricalVariable,
    const std::string& rDistanceVariableName,
    const bool SaveDistanceInSkin
    ) : mrVolumeModelPart(rVolumeModelPart),
        mrSkinModelPart(rSkinModelPart)
{
    // Assign distance variable
    if (rDistanceVariableName != "") {
        mpDistanceVariable = &KratosComponents<Variable<double>>::Get(rDistanceVariableName);
    }

    // Setting flags
    this->Set(HISTORICAL_VALUE, HistoricalVariable);
    // If SaveDistanceInSkin is true we can not use parallel algorithms and therefore a warning will be raised
    KRATOS_WARNING_IF("CalculateNodalDistanceToSkinProcess", SaveDistanceInSkin) << "SaveDistanceInSkin is true, therefore the parallel algorithms will not be used." << std::endl;
    this->Set(SAVE_DISTANCE_IN_SKIN, SaveDistanceInSkin);

    // Check it is serial
    KRATOS_ERROR_IF(mrVolumeModelPart.IsDistributed()) << "Distributed computation still not supported. Please update implementation as soon as MPI search is merged. See https://github.com/KratosMultiphysics/Kratos/pull/11719" << std::endl;
}

/***********************************************************************************/
/***********************************************************************************/

CalculateNodalDistanceToSkinProcess::CalculateNodalDistanceToSkinProcess(
    Model& rModel,
    Parameters ThisParameters
    ) : mrVolumeModelPart(rModel.GetModelPart(ThisParameters["volume_model_part"].GetString())),
        mrSkinModelPart(rModel.GetModelPart(ThisParameters["skin_model_part"].GetString()))
{
    // Validate and assign defaults
    ThisParameters.ValidateAndAssignDefaults(GetDefaultParameters());

    // If historical or non-historical variables
    const std::string database = ThisParameters["distance_database"].GetString();
    if (database == "nodal_historical") {
        this->Set(HISTORICAL_VALUE, true);
    } else if (database == "nodal_non_historical") {
        this->Set(HISTORICAL_VALUE, false);
    } else {
        KRATOS_ERROR << "Only options are nodal_historical and nodal_non_historical, provided is: " << database << std::endl;
    }

    // Save distance in skin flag
    this->Set(SAVE_DISTANCE_IN_SKIN, ThisParameters["save_distance_in_skin"].GetBool());

    // Assign distance variable
    mpDistanceVariable = &KratosComponents<Variable<double>>::Get(ThisParameters["distance_variable"].GetString());

    // Check it is serial
    KRATOS_ERROR_IF(mrVolumeModelPart.IsDistributed()) << "Distributed computation still not supported. Please update implementation as soon as MPI search is merged. See https://github.com/KratosMultiphysics/Kratos/pull/11719" << std::endl;
}

/***********************************************************************************/
/***********************************************************************************/

void CalculateNodalDistanceToSkinProcess::Execute()
{
    // Define distance lambda
    const std::function<void(ModelPart::NodesContainerType& rNodes, GeometricalObjectsBins& rBins)> distance_lambda_historical = [this](ModelPart::NodesContainerType& rNodes, GeometricalObjectsBins& rBins) {
        const auto& r_variable = *mpDistanceVariable;
        block_for_each(rNodes, [&rBins, &r_variable](Node& rNode) {
            auto result = rBins.SearchNearest(rNode);
            rNode.FastGetSolutionStepValue(r_variable) = result.GetDistance();
        });
    };
    const std::function<void(ModelPart::NodesContainerType& rNodes, GeometricalObjectsBins& rBins)> distance_lambda_non_historical = [this](ModelPart::NodesContainerType& rNodes, GeometricalObjectsBins& rBins) {
        const auto& r_variable = *mpDistanceVariable;
        block_for_each(rNodes, [&rBins, &r_variable](Node& rNode) {
            auto result = rBins.SearchNearest(rNode);
            rNode.SetValue(r_variable, result.GetDistance());
        });
    };

    const std::function<void(ModelPart::NodesContainerType& rNodes, GeometricalObjectsBins& rBins)> distance_lambda_historical_save_skin = [this](ModelPart::NodesContainerType& rNodes, GeometricalObjectsBins& rBins) {
        const auto& r_variable = *mpDistanceVariable;
        for (Node& rNode: rNodes) {
            auto result = rBins.SearchNearest(rNode);
            const double value = result.GetDistance();
            auto p_result = result.Get().get();
            if (p_result->GetValue(r_variable) < value) {
                p_result->SetValue(r_variable, value);
                p_result->Set(VISITED);
            }
            rNode.FastGetSolutionStepValue(r_variable) = value;
        }
    };
    const std::function<void(ModelPart::NodesContainerType& rNodes, GeometricalObjectsBins& rBins)> distance_lambda_non_historica_save_skin = [this](ModelPart::NodesContainerType& rNodes, GeometricalObjectsBins& rBins) {
        const auto& r_variable = *mpDistanceVariable;
        for (Node& rNode: rNodes) {
            auto result = rBins.SearchNearest(rNode);
            const double value = result.GetDistance();
            auto p_result = result.Get().get();
            if (p_result->GetValue(r_variable) < value) {
                p_result->SetValue(r_variable, value);
                p_result->Set(VISITED);
            }
            rNode.SetValue(r_variable, value);
        }
    };

    // Call lambda depending on the flags
    const auto* p_distance_lambda = this->Is(HISTORICAL_VALUE) ? (this->Is(SAVE_DISTANCE_IN_SKIN) ? &distance_lambda_historical_save_skin : &distance_lambda_historical) : (this->Is(SAVE_DISTANCE_IN_SKIN) ? &distance_lambda_non_historica_save_skin : &distance_lambda_non_historical);

    // First try to detect if elements or conditions
    const std::size_t number_of_elements_skin = mrSkinModelPart.NumberOfElements();
    const std::size_t number_of_conditions_skin = mrSkinModelPart.NumberOfConditions();
    if (number_of_elements_skin > 0) {
        KRATOS_WARNING_IF("CalculateNodalDistanceToSkinProcess", number_of_conditions_skin > 0) << "Skin model part has elements and conditions. Considering elements. " << std::endl;
        GeometricalObjectsBins bins(mrSkinModelPart.ElementsBegin(), mrSkinModelPart.ElementsEnd());

        // Set to zero in skin if required
        if (this->Is(SAVE_DISTANCE_IN_SKIN)) {
            VariableUtils().SetNonHistoricalVariableToZero(*mpDistanceVariable, mrSkinModelPart.Elements());
        }

        // Call lambda
        (*p_distance_lambda)(mrVolumeModelPart.Nodes(), bins);
    } else if (number_of_conditions_skin > 0) {
        GeometricalObjectsBins bins(mrSkinModelPart.ConditionsBegin(), mrSkinModelPart.ConditionsEnd());

        // Set to zero in skin if required
        if (this->Is(SAVE_DISTANCE_IN_SKIN)) {
            VariableUtils().SetNonHistoricalVariableToZero(*mpDistanceVariable, mrSkinModelPart.Conditions());
        }

        // Call lambda
        (*p_distance_lambda)(mrVolumeModelPart.Nodes(), bins);
    }
}

/***********************************************************************************/
/***********************************************************************************/

const Parameters CalculateNodalDistanceToSkinProcess::GetDefaultParameters() const
{
    const Parameters default_parameters = Parameters(R"({
        "volume_model_part"     : "",
        "skin_model_part"       : "",
        "distance_database"     : "nodal_historical",
        "save_distance_in_skin" : false,
        "distance_variable"     : "DISTANCE"
    })");
    return default_parameters;
}

/***********************************************************************************/
/***********************************************************************************/

std::string CalculateNodalDistanceToSkinProcess::Info() const
{
    return "CalculateNodalDistanceToSkinProcess";
}

/***********************************************************************************/
/***********************************************************************************/


void CalculateNodalDistanceToSkinProcess::PrintInfo(std::ostream& rOStream) const
{
    rOStream << "CalculateNodalDistanceToSkinProcess" << std::endl;
}

/***********************************************************************************/
/***********************************************************************************/


void CalculateNodalDistanceToSkinProcess::PrintData(std::ostream& rOStream) const
{
    rOStream << "Volume model part:\n" << mrVolumeModelPart << "\nSkin model part:\n" << mrSkinModelPart << "\nHistorical variable: " << std::boolalpha << this->Is(HISTORICAL_VALUE) << "\nSave distance in skin: " << std::boolalpha << this->Is(SAVE_DISTANCE_IN_SKIN) << "\nDistance variable: " << mpDistanceVariable->Name() << std::endl;
}

} // namespace Kratos.