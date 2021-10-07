//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Philipp Bucher, Jordi Cotela
//
// See Master-Thesis P.Bucher
// "Development and Implementation of a Parallel
//  Framework for Non-Matching Grid Mapping"

// System includes
#include <cmath>
#include <limits>

// External includes

// Project includes
#include "includes/model_part.h"
#include "utilities/parallel_utilities.h"
#include "custom_utilities/mapper_utilities.h"
#include "interface_communicator.h"

namespace Kratos {

typedef std::size_t IndexType;
typedef std::size_t SizeType;

/***********************************************************************************/
/* PUBLIC Methods */
/***********************************************************************************/

InterfaceCommunicator::InterfaceCommunicator(
    ModelPart& rModelPartOrigin,
    MapperLocalSystemPointerVector& rMapperLocalSystems,
    Parameters SearchSettings)
      : mrModelPartOrigin(rModelPartOrigin),
        mrMapperLocalSystems(rMapperLocalSystems),
        mSearchSettings(SearchSettings)
{
    Parameters search_defaults( R"({
        "search_radius"                 : 0.0,
        "max_search_radius"             : 0.0,
        "search_radius_increase_factor" : 0.0,
        "max_num_search_iterations"     : 0,
        "echo_level"                    : 0
    })");
    // deliberately only validating defaults, but not assigning, since computing the defaults is expensive
    // see "ExchangeInterfaceData"
    mSearchSettings.ValidateDefaults(search_defaults);

    mEchoLevel = mSearchSettings.Has("echo_level") ? mSearchSettings["echo_level"].GetInt() : 0;
    mMapperInterfaceInfosContainer.resize(1);
}

void InterfaceCommunicator::ExchangeInterfaceData(const Communicator& rComm,
                                                  const MapperInterfaceInfoUniquePointerType& rpInterfaceInfo)
{
    KRATOS_TRY;

    InitializeSearch(rpInterfaceInfo); // to create the bins and the data structure needed in the following for determining search radius etc

    const SizeType num_interface_obj_bin = mpInterfaceObjectsOrigin->size();

    double init_search_radius = 0.0;
    double max_search_radius = 0.0;
    double increase_factor = 2.0; // default value
    int max_search_iterations = 3; // default value

    if (num_interface_obj_bin > 0) { // this partition has a part of the interface
        auto log_base = [](const double Val, const double Base) -> double {
            return std::log(Val) / std::log(Base);
        };

        if (mSearchSettings.Has("search_radius_increase_factor")) {
            increase_factor = mSearchSettings["search_radius_increase_factor"].GetDouble();
            KRATOS_ERROR_IF(increase_factor < std::numeric_limits<double>::epsilon()) << "Search radius increase factor must be larger than 0.0!" << std::endl;
        }

        if (mSearchSettings.Has("search_radius")) {
            init_search_radius = mSearchSettings["search_radius"].GetDouble();
            KRATOS_ERROR_IF(init_search_radius < std::numeric_limits<double>::epsilon()) << "Search radius must be larger than 0.0!" << std::endl;
        } else {
            const array_1d<double, 3> box_size = mpLocalBinStructure->GetMaxPoint() - mpLocalBinStructure->GetMinPoint();
            init_search_radius = (*std::max_element(box_size.begin(), box_size.end())) / num_interface_obj_bin;
        }

        if (mSearchSettings.Has("max_search_radius")) {
            max_search_radius = mSearchSettings["max_search_radius"].GetDouble();
            KRATOS_ERROR_IF(max_search_radius < std::numeric_limits<double>::epsilon()) << "Maximum radius must be larger than 0.0!" << std::endl;
        } else {
            max_search_radius = MapperUtilities::ComputeSearchRadius(mrModelPartOrigin, mEchoLevel);
        }
        max_search_radius = std::max(MapperUtilities::ComputeSearchRadius(mrModelPartOrigin, mEchoLevel), init_search_radius);

        if (mSearchSettings.Has("max_num_search_iterations")) {
            max_search_iterations = mSearchSettings["max_num_search_iterations"].GetInt();
            KRATOS_ERROR_IF(max_search_iterations < 1) << "Number of search iterations must be larger than 0!" << std::endl;
        } else {
            max_search_iterations = std::max(max_search_iterations,static_cast<int>(
                    std::ceil(log_base(max_search_radius,  increase_factor)-
                              log_base(init_search_radius, increase_factor)))+1);
        }

        KRATOS_INFO_IF("Mapper search", mEchoLevel>1)
            << "\n    Initial search radius: " << init_search_radius
            << "\n    Maximum search radius: " << max_search_radius
            << "\n    Maximum number of search iterations: " << max_search_iterations
            << "\n    Search radius increase factor: " << increase_factor << std::endl;
    }

    mSearchRadius = init_search_radius;

    int num_iteration = 1;

    // First Iteration is done outside the search loop bcs it has
    // to be done in any case
    // one search iteration should be enough in most cases (if the search
    // radius was either computed or specified properly)
    // only if some points did not find a neighbor or dont have a valid
    // projection, more search iterations are necessary
    mMeshesAreConforming = 1;
    ConductSearchIteration(rpInterfaceInfo, rComm);

    while (++num_iteration <= max_search_iterations && !AllNeighborsFound(rComm)) {
        mSearchRadius *= increase_factor;

        // If all neighbours were not found in the first iteration, the meshes are not conforming
        // for the initial given search radius.
        mMeshesAreConforming = 0;

        KRATOS_INFO_IF("Mapper search", mEchoLevel >= 1)
            << "search radius was increased, another search iteration is conducted\n    "
            << "search iteration: " << num_iteration << " / "<< max_search_iterations << " | "
            << "search radius: " << mSearchRadius << std::endl;

        ConductSearchIteration(rpInterfaceInfo, rComm);
    }

    FinalizeSearch();

    if (rComm.GetDataCommunicator().IsDefinedOnThisRank()) {
        rComm.GetDataCommunicator().Barrier();
    }

    KRATOS_CATCH("");
}

/***********************************************************************************/
/* PROTECTED Methods */
/***********************************************************************************/
void InterfaceCommunicator::InitializeSearch(const MapperInterfaceInfoUniquePointerType& rpRefInterfaceInfo)
{
    KRATOS_TRY;

    CreateInterfaceObjectsOrigin(rpRefInterfaceInfo);
    InitializeBinsSearchStructure(); // This cannot be updated, has to be recreated

    KRATOS_CATCH("");
}

void InterfaceCommunicator::FinalizeSearch()
{
    KRATOS_TRY;

    for (auto& r_interface_infos_rank : mMapperInterfaceInfosContainer) {
        r_interface_infos_rank.clear();
    }

    KRATOS_CATCH("");
}

void InterfaceCommunicator::InitializeSearchIteration(const MapperInterfaceInfoUniquePointerType& rpRefInterfaceInfo)
{
    KRATOS_TRY;

    // Creating the MapperInterfaceInfos
    mMapperInterfaceInfosContainer[0].clear();

    auto& r_mapper_interface_infos = mMapperInterfaceInfosContainer[0];
    r_mapper_interface_infos.reserve(mrMapperLocalSystems.size());

    IndexType local_sys_idx = 0;
    for (const auto& r_local_sys : mrMapperLocalSystems) {
        if (!r_local_sys->HasInterfaceInfo()) { // Only the local_systems that have not received an InterfaceInfo create a new one
            const auto& r_coords = r_local_sys->Coordinates();
            r_mapper_interface_infos.push_back(rpRefInterfaceInfo->Create(r_coords, local_sys_idx, 0)); // dummy-rank of 0
        }
        ++local_sys_idx;
    }

    KRATOS_CATCH("");
}

void InterfaceCommunicator::FinalizeSearchIteration(const MapperInterfaceInfoUniquePointerType& rpInterfaceInfo)
{
    KRATOS_TRY;

    FilterInterfaceInfosSuccessfulSearch();
    AssignInterfaceInfos();

    KRATOS_CATCH("");
}

void InterfaceCommunicator::FilterInterfaceInfosSuccessfulSearch()
{
    KRATOS_TRY;

    // Erasing all the MapperLocalSystems that don't have a successful search
    // using the erase–remove idiom
    for (IndexType i_rank=0; i_rank<mMapperInterfaceInfosContainer.size(); ++i_rank) {
        auto& r_interface_infos_rank = mMapperInterfaceInfosContainer[i_rank];

        auto new_end = std::remove_if(
            r_interface_infos_rank.begin(),
            r_interface_infos_rank.end(),
            [](const MapperInterfaceInfoPointerType& rpInterfaceInfo)
            { return !(rpInterfaceInfo->GetLocalSearchWasSuccessful()); });

        r_interface_infos_rank.erase(new_end, r_interface_infos_rank.end());
    }

    KRATOS_CATCH("");
}

void InterfaceCommunicator::AssignInterfaceInfos()
{
    KRATOS_TRY;

    // NOTE: mMapperInterfaceInfosContainer must contain only the ones that are a successfuls search!
    const SizeType comm_size = mMapperInterfaceInfosContainer.size();

    for (IndexType i_rank=0; i_rank<comm_size; ++i_rank) {
        for (const auto& rp_interface_info : mMapperInterfaceInfosContainer[i_rank]) {
            mrMapperLocalSystems[rp_interface_info->GetLocalSystemIndex()]
                ->AddInterfaceInfo(rp_interface_info);
        }
    }

    KRATOS_CATCH("");
}

/***********************************************************************************/
/* PRIVATE Methods */
/***********************************************************************************/
void InterfaceCommunicator::CreateInterfaceObjectsOrigin(const MapperInterfaceInfoUniquePointerType& rpRefInterfaceInfo)
{
    KRATOS_TRY;

    mpInterfaceObjectsOrigin = Kratos::make_unique<InterfaceObjectContainerType>();

    if (mrModelPartOrigin.GetCommunicator().GetDataCommunicator().IsNullOnThisRank()) {
        return;
    }

    const auto interface_obj_type = rpRefInterfaceInfo->GetInterfaceObjectType();

    if (interface_obj_type == InterfaceObject::ConstructionType::Node_Coords) {
        const SizeType num_nodes = mrModelPartOrigin.GetCommunicator().LocalMesh().NumberOfNodes();
        const auto nodes_begin = mrModelPartOrigin.GetCommunicator().LocalMesh().Nodes().ptr_begin();

        mpInterfaceObjectsOrigin->resize(num_nodes);

        IndexPartition<std::size_t>(num_nodes).for_each([&nodes_begin, this](const std::size_t i){
            auto it_node = nodes_begin + i;
            (*mpInterfaceObjectsOrigin)[i] = Kratos::make_unique<InterfaceNode>((*it_node).get());
        });
    }

    else if (interface_obj_type == InterfaceObject::ConstructionType::Geometry_Center) {
        Communicator& r_comm = mrModelPartOrigin.GetCommunicator();
        const SizeType num_elements = r_comm.LocalMesh().NumberOfElements();
        const SizeType num_conditions = r_comm.LocalMesh().NumberOfConditions();

        const auto elements_begin = r_comm.LocalMesh().Elements().ptr_begin();
        const auto conditions_begin = r_comm.LocalMesh().Conditions().ptr_begin();

        int num_elements_global = r_comm.GetDataCommunicator().SumAll(static_cast<int>(num_elements));
        int num_conditions_global = r_comm.GetDataCommunicator().SumAll(static_cast<int>(num_conditions));

        KRATOS_ERROR_IF(num_elements_global > 0 && num_conditions_global > 0)
            << "Both Elements and Conditions are present which is not allowed!\n"
            << "Name of ModelPart: " << mrModelPartOrigin.Name()
            << "\nNumber of Elements: " << num_elements
            << "; Number of Condition: " << num_conditions << std::endl;

        KRATOS_ERROR_IF(num_elements_global+num_conditions_global == 0)
            << "No Elements and Conditions are present which is not allowed!\n"
            << "Name of ModelPart: " << mrModelPartOrigin.Name() << std::endl;

        mpInterfaceObjectsOrigin->resize(num_elements+num_conditions); // one of them has to be zero!!!

        IndexPartition<std::size_t>(num_elements).for_each([&elements_begin, this](const std::size_t i){
            auto it_elem = elements_begin + i;
            (*mpInterfaceObjectsOrigin)[i] = Kratos::make_unique<InterfaceGeometryObject>((*it_elem)->pGetGeometry().get());
        });
        IndexPartition<std::size_t>(num_conditions).for_each([&conditions_begin, this](const std::size_t i){
            auto it_cond = conditions_begin + i;
            (*mpInterfaceObjectsOrigin)[i] = Kratos::make_unique<InterfaceGeometryObject>((*it_cond)->pGetGeometry().get());
        });
    }
    else {
        KRATOS_ERROR << "Type of interface object construction not implemented" << std::endl;
    }

    // Making sure that the data-structure was correctly initialized
    int num_interface_objects = mpInterfaceObjectsOrigin->size(); // int bcs of MPI
    num_interface_objects = mrModelPartOrigin.GetCommunicator().GetDataCommunicator().SumAll(num_interface_objects);

    KRATOS_ERROR_IF_NOT(num_interface_objects > 0)
        << "No interface objects were created in Origin-ModelPart \""
        << mrModelPartOrigin.Name() << "\"!" << std::endl;

    KRATOS_CATCH("");
}

void InterfaceCommunicator::InitializeBinsSearchStructure()
{
    KRATOS_TRY;

    if (mpInterfaceObjectsOrigin->size() > 0) { // only construct the bins if the partition has a part of the interface
        mpLocalBinStructure = Kratos::make_unique<BinsObjectDynamic<InterfaceObjectConfigure>>(
            mpInterfaceObjectsOrigin->begin(), mpInterfaceObjectsOrigin->end());
    }

    KRATOS_CATCH("");
}

void InterfaceCommunicator::ConductLocalSearch(const Communicator& rComm)
{
    KRATOS_TRY;

    const SizeType num_interface_obj_bin = mpInterfaceObjectsOrigin->size();

    KRATOS_ERROR_IF(mSearchRadius < 0.0) << "Search-Radius has to be larger than 0.0!" << std::endl;

    int sum_num_results = 0;
    int sum_num_searched_objects = 0;

    if (num_interface_obj_bin > 0) { // this partition has a bin structure

        struct SearchTLS {
            explicit SearchTLS(std::size_t MaxNeighborResults) : mMaxNeighborResults(MaxNeighborResults) {}

            // the IndexPartition uses the CopyConstructor to create the thread local storage
            // hence using it to initialize the members
            SearchTLS(const SearchTLS& rOther)
            {
                mNeighborResults.resize(rOther.mMaxNeighborResults);
                mInterfaceObject = Kratos::make_shared<InterfaceObject>(array_1d<double, 3>(0.0));
            }

            InterfaceObjectConfigure::ResultContainerType mNeighborResults;
            Kratos::shared_ptr<InterfaceObject> mInterfaceObject;

        private:
            std::size_t mMaxNeighborResults = 0;
        };

        for (auto& r_interface_infos_rank : mMapperInterfaceInfosContainer) { // loop the ranks
            sum_num_searched_objects += r_interface_infos_rank.size();
            // intentionally not the outermost loop is parallelized as this one can be large
            sum_num_results += IndexPartition<std::size_t>(r_interface_infos_rank.size()).for_each<SumReduction<int>>(
                SearchTLS(num_interface_obj_bin), [
                &r_interface_infos_rank,
                num_interface_obj_bin,
                this](const std::size_t Index, SearchTLS& rTLS) {
                auto& r_interface_info = r_interface_infos_rank[Index];

                rTLS.mInterfaceObject->Coordinates() = r_interface_info->Coordinates();

                // reset the containers
                auto results_itr = rTLS.mNeighborResults.begin();

                const SizeType number_of_results = mpLocalBinStructure->SearchObjectsInRadius(
                    rTLS.mInterfaceObject, mSearchRadius, results_itr,
                    num_interface_obj_bin);

                for (IndexType j=0; j<number_of_results; ++j) {
                    r_interface_info->ProcessSearchResult(*(rTLS.mNeighborResults[j]));
                }

                // If the search did not result in a "valid" result (e.g. the projection fails)
                // we try to compute an approximation
                if (!r_interface_info->GetLocalSearchWasSuccessful()) {
                    for (IndexType j=0; j<number_of_results; ++j) {
                        r_interface_info->ProcessSearchResultForApproximation(*(rTLS.mNeighborResults[j]));
                    }
                }

                return number_of_results;
            });
        }
    }

    if (mEchoLevel > 1) {
        const auto& r_data_comm = rComm.GetDataCommunicator();
        if (r_data_comm.IsDefinedOnThisRank()) {
            sum_num_results = r_data_comm.Sum(sum_num_results, 0);
            sum_num_searched_objects = r_data_comm.Sum(sum_num_searched_objects, 0);
        }

        const double avg_num_results = sum_num_results / static_cast<double>(sum_num_searched_objects);

        KRATOS_INFO_IF("Mapper search", mEchoLevel > 1) << "An average of " << avg_num_results << " objects was found while searching" << std::endl;
        KRATOS_WARNING_IF("Mapper search", avg_num_results > 200) << "Many search results are found, consider adjusting the search settings for improving performance" << std::endl;
    }

    KRATOS_CATCH("");
}

void InterfaceCommunicator::ConductSearchIteration(const MapperInterfaceInfoUniquePointerType& rpInterfaceInfo,
                                                   const Communicator& rComm)
{
    KRATOS_TRY;

    InitializeSearchIteration(rpInterfaceInfo);

    ConductLocalSearch(rComm);

    FinalizeSearchIteration(rpInterfaceInfo);

    KRATOS_CATCH("");
}

bool InterfaceCommunicator::AllNeighborsFound(const Communicator& rComm) const
{
    KRATOS_TRY;

    int all_neighbors_found = 1; // set to "1" aka "true" by default in case
    // this partition doesn't have a part of the interface!

    for (const auto& local_sys : mrMapperLocalSystems) {
        if (!local_sys->HasInterfaceInfo()) {
            all_neighbors_found = 0;
            break;
        }
    }

    // This is necessary bcs not all partitions would start a new search iteration!
    if (rComm.GetDataCommunicator().IsDefinedOnThisRank()) {
        all_neighbors_found = rComm.GetDataCommunicator().MinAll(all_neighbors_found);
    }

    return all_neighbors_found > 0;

    KRATOS_CATCH("");
}

}  // namespace Kratos.
