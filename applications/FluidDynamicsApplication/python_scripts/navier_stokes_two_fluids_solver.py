from __future__ import print_function, absolute_import, division  # makes KratosMultiphysics backward compatible with python 2.6 and 2.7

# Importing the Kratos Library
import KratosMultiphysics
import KratosMultiphysics.kratos_utilities as KratosUtilities

# Import applications
import KratosMultiphysics.FluidDynamicsApplication as KratosCFD
have_conv_diff = KratosUtilities.CheckIfApplicationsAvailable("ConvectionDiffusionApplication")
if have_conv_diff:
    import KratosMultiphysics.ConvectionDiffusionApplication as KratosConvDiff

# Import base class file
from KratosMultiphysics.FluidDynamicsApplication.fluid_solver import FluidSolver
from KratosMultiphysics.FluidDynamicsApplication.read_distance_from_file import DistanceImportUtility

import KratosMultiphysics.python_linear_solver_factory as linear_solver_factory

def CreateSolver(model, custom_settings):
    return NavierStokesTwoFluidsSolver(model, custom_settings)

class NavierStokesTwoFluidsSolver(FluidSolver):

    @classmethod
    def GetDefaultSettings(cls):
        ##settings string in json format
        default_settings = KratosMultiphysics.Parameters("""
        {
            "solver_type": "two_fluids_solver_from_defaults",
            "model_part_name": "",
            "domain_size": -1,
            "model_import_settings": {
                "input_type": "mdpa",
                "input_filename": "unknown_name",
                "reorder": false
            },
            "material_import_settings": {
                "materials_filename": ""
            },
            "distance_reading_settings"    : {
                "import_mode"         : "from_mdpa",
                "distance_file_name"  : "no_distance_file"
            },
            "maximum_iterations": 7,
            "echo_level": 0,
            "time_order": 2,
            "time_scheme": "bdf2",
            "compute_reactions": false,
            "reform_dofs_at_each_step": false,
            "consider_periodic_conditions": false,
            "relative_velocity_tolerance": 1e-3,
            "absolute_velocity_tolerance": 1e-5,
            "relative_pressure_tolerance": 1e-3,
            "absolute_pressure_tolerance": 1e-5,
            "linear_solver_settings"       : {
                "solver_type"         : "amgcl"
            },
            "volume_model_part_name" : "volume_model_part",
            "skin_parts": [""],
            "assign_neighbour_elements_to_conditions": true,
            "no_skin_parts":[""],
            "time_stepping"                : {
                "automatic_time_step" : true,
                "CFL_number"          : 1,
                "minimum_delta_time"  : 1e-2,
                "maximum_delta_time"  : 1.0,
                "time_step"           : 0.0
            },
            "periodic": "periodic",
            "move_mesh_flag": false,
            "formulation": {
                "dynamic_tau": 1.0
            },
            "bfecc_convection" : false,
            "bfecc_number_substeps" : 10
        }""")

        default_settings.AddMissingParameters(super(NavierStokesTwoFluidsSolver, cls).GetDefaultSettings())
        return default_settings

    def __init__(self, model, custom_settings):
        self._validate_settings_in_baseclass=True # To be removed eventually

        # TODO: DO SOMETHING IN HERE TO REMOVE THE "time_order" FROM THE DEFAULT SETTINGS BUT KEEPING THE BACKWARDS COMPATIBILITY

        super(NavierStokesTwoFluidsSolver,self).__init__(model,custom_settings)

        self.element_name = "TwoFluidNavierStokes"
        self.condition_name = "NavierStokesWallCondition"
        self.element_integrates_in_time = True
        self.element_has_nodal_properties = True

        self.min_buffer_size = 3

        self._bfecc_convection = self.settings["bfecc_convection"].GetBool()

        dynamic_tau = self.settings["formulation"]["dynamic_tau"].GetDouble()
        self.main_model_part.ProcessInfo.SetValue(KratosMultiphysics.DYNAMIC_TAU, dynamic_tau)

        ## Set the distance reading filename
        # TODO: remove the manual "distance_file_name" set as soon as the problem type one has been tested.
        if (self.settings["distance_reading_settings"]["import_mode"].GetString() == "from_GiD_file"):
            self.settings["distance_reading_settings"]["distance_file_name"].SetString(self.settings["model_import_settings"]["input_filename"].GetString()+".post.res")

        KratosMultiphysics.Logger.PrintInfo("NavierStokesTwoFluidsSolver", "Construction of NavierStokesTwoFluidsSolver finished.")

    def AddVariables(self):
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.DENSITY)
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.DYNAMIC_VISCOSITY)
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.PRESSURE)
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.VELOCITY)
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.ACCELERATION)
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.MESH_VELOCITY)
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.IS_STRUCTURE)
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.BODY_FORCE)
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.NODAL_H)
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.NODAL_AREA)
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.REACTION)
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.REACTION_WATER_PRESSURE)
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.NORMAL)
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.EXTERNAL_PRESSURE)
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.FLAG_VARIABLE)
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.DISTANCE)              # Distance function nodal values
        self.main_model_part.AddNodalSolutionStepVariable(KratosMultiphysics.DISTANCE_GRADIENT)     # Distance gradient nodal values

        KratosMultiphysics.Logger.PrintInfo("NavierStokesTwoFluidsSolver", "Fluid solver variables added correctly.")

    def PrepareModelPart(self):
        # Initialize the level-set function
        if not self.main_model_part.ProcessInfo[KratosMultiphysics.IS_RESTARTED]:
            ## Setting the nodal distance
            self._set_distance_function()

        # Call the base solver PrepareModelPart()
        super(NavierStokesTwoFluidsSolver, self).PrepareModelPart()

    def Initialize(self):
        computing_model_part = self.GetComputingModelPart()

        # Calculate boundary normals
        KratosMultiphysics.NormalCalculationUtils().CalculateOnSimplex(
            computing_model_part,
            computing_model_part.ProcessInfo[KratosMultiphysics.DOMAIN_SIZE])

        # Elemental neighbours calculation
        data_communicator = computing_model_part.GetCommunicator().GetDataCommunicator()
        neighbour_search = KratosMultiphysics.FindGlobalNodalElementalNeighboursProcess(
            data_communicator,
            computing_model_part)
        neighbour_search.Execute()

        # Set and initialize the solution strategy
        solution_strategy = self.get_solution_strategy()
        solution_strategy.SetEchoLevel(self.settings["echo_level"].GetInt())
        solution_strategy.Initialize()

        KratosMultiphysics.Logger.PrintInfo("NavierStokesTwoFluidsSolver", "Solver initialization finished.")

    def InitializeSolutionStep(self):
        if self._TimeBufferIsInitialized():
            # Recompute the BDF2 coefficients
            (self.time_discretization).ComputeAndSaveBDFCoefficients(self.GetComputingModelPart().ProcessInfo)

            # Perform the level-set convection according to the previous step velocity
            if self._bfecc_convection:
                self.get_level_set_convection_process().BFECCconvect(
                    self.main_model_part,
                    KratosMultiphysics.DISTANCE,
                    KratosMultiphysics.VELOCITY,
                    self.settings["bfecc_number_substeps"].GetInt())
            else:
                self.get_level_set_convection_process().Execute()

            # Recompute the distance field according to the new level-set position
            self.get_variational_distance_process().Execute()

            # Update the DENSITY and DYNAMIC_VISCOSITY values according to the new level-set
            self._SetNodalProperties()

            # Initialize the solver current step
            self.get_solution_strategy().InitializeSolutionStep()

    def FinalizeSolutionStep(self):
        if self._TimeBufferIsInitialized():
            self.get_solution_strategy().FinalizeSolutionStep()
            self.get_acceleration_limitation_utility().Execute()

    # TODO: Remove this method as soon as the subproperties are available
    def _SetPhysicalProperties(self):
        import os
        warn_msg  = '\nThe materials import mechanism used in the two fluids solver is DEPRECATED!\n'
        warn_msg += 'It will be removed to use the base fluid_solver.py one as soon as the subproperties are available.\n'
        KratosMultiphysics.Logger.PrintWarning('\n\x1b[1;31mDEPRECATION-WARNING\x1b[0m', warn_msg)

        # Check if the fluid properties are provided using a .json file
        materials_filename = self.settings["material_import_settings"]["materials_filename"].GetString()
        if (materials_filename != ""):
            with open(materials_filename,'r') as materials_file:
                materials = KratosMultiphysics.Parameters(materials_file.read())

            # Create and read an auxiliary materials file for each one of the fields
            for i_material in materials["properties"]:
                aux_materials = KratosMultiphysics.Parameters()
                aux_materials.AddEmptyArray("properties")
                aux_materials["properties"].Append(i_material)
                prop_id = i_material["properties_id"].GetInt()

                aux_materials_filename = materials_filename + "_" + str(prop_id) + ".json"
                with open(aux_materials_filename,'w') as aux_materials_file:
                    aux_materials_file.write(aux_materials.WriteJsonString())
                    aux_materials_file.close()

                aux_material_settings = KratosMultiphysics.Parameters("""{"Parameters": {"materials_filename": ""}} """)
                aux_material_settings["Parameters"]["materials_filename"].SetString(aux_materials_filename)
                KratosMultiphysics.ReadMaterialsUtility(aux_material_settings, self.model)

                os.remove(aux_materials_filename)

            materials_imported = True
        else:
            materials_imported = False

        # If the element uses nodal material properties, transfer them to the nodes
        if self.element_has_nodal_properties:
            self._SetNodalProperties()

        return materials_imported

    def _SetNodalProperties(self):
        # Get fluid 1 and 2 properties
        properties_1 = self.main_model_part.Properties[1]
        properties_2 = self.main_model_part.Properties[2]

        rho_1 = properties_1.GetValue(KratosMultiphysics.DENSITY)
        rho_2 = properties_2.GetValue(KratosMultiphysics.DENSITY)
        mu_1 = properties_1.GetValue(KratosMultiphysics.DYNAMIC_VISCOSITY)
        mu_2 = properties_2.GetValue(KratosMultiphysics.DYNAMIC_VISCOSITY)

        # Check fluid 1 and 2 properties
        if rho_1 <= 0.0:
            raise Exception("DENSITY set to {0} in Properties {1}, positive number expected.".format(rho_1, properties_1.Id))
        if rho_2 <= 0.0:
            raise Exception("DENSITY set to {0} in Properties {1}, positive number expected.".format(rho_2, properties_2.Id))
        if mu_1 <= 0.0:
            raise Exception("DYNAMIC_VISCOSITY set to {0} in Properties {1}, positive number expected.".format(mu_1, properties_1.Id))
        if mu_2 <= 0.0:
            raise Exception("DYNAMIC_VISCOSITY set to {0} in Properties {1}, positive number expected.".format(mu_2, properties_2.Id))

        # Transfer density and (dynamic) viscostity to the nodes
        for node in self.main_model_part.Nodes:
            if node.GetSolutionStepValue(KratosMultiphysics.DISTANCE) <= 0.0:
                node.SetSolutionStepValue(KratosMultiphysics.DENSITY, rho_1)
                node.SetSolutionStepValue(KratosMultiphysics.DYNAMIC_VISCOSITY, mu_1)
            else:
                node.SetSolutionStepValue(KratosMultiphysics.DENSITY, rho_2)
                node.SetSolutionStepValue(KratosMultiphysics.DYNAMIC_VISCOSITY, mu_2)

    def _set_distance_function(self):
        ## Set the nodal distance function
        if (self.settings["distance_reading_settings"]["import_mode"].GetString() == "from_GiD_file"):
            DistanceUtility = DistanceImportUtility(self.main_model_part, self.settings["distance_reading_settings"])
            DistanceUtility.ImportDistance()
        elif (self.settings["distance_reading_settings"]["import_mode"].GetString() == "from_mdpa"):
            KratosMultiphysics.Logger.PrintInfo("Navier Stokes Embedded Solver","Distance function taken from the .mdpa input file.")

    def get_acceleration_limitation_utility(self):
        if not hasattr(self, '_acceleration_limitation_utility'):
            self._acceleration_limitation_utility = self._create_acceleration_limitation_utility()
        return self._acceleration_limitation_utility

    def get_level_set_convection_process(self):
        if not hasattr(self, '_level_set_convection_process'):
            self._level_set_convection_process = self._create_level_set_convection_process()
        return self._level_set_convection_process

    def get_variational_distance_process(self):
        if not hasattr(self, '_variational_distance_process'):
            self._variational_distance_process = self._create_variational_distance_process()
        return self._variational_distance_process

    def _create_acceleration_limitation_utility(self):
        maximum_multiple_of_g_acceleration_allowed = 5.0
        acceleration_limitation_utility = KratosCFD.AccelerationLimitationUtilities(
            self.GetComputingModelPart(),
            maximum_multiple_of_g_acceleration_allowed)

        return acceleration_limitation_utility

    def _create_level_set_convection_process(self):
        # Construct the level set convection process
        domain_size = self.main_model_part.ProcessInfo[KratosMultiphysics.DOMAIN_SIZE]
        computing_model_part = self.GetComputingModelPart()
        if self._bfecc_convection:
            if have_conv_diff:
                if domain_size == 2:
                    locator = KratosMultiphysics.BinBasedFastPointLocator2D(computing_model_part).UpdateSearchDatabase()
                    level_set_convection_process = KratosConvDiff.BFECCConvection2D(locator)
                else:
                    locator = KratosMultiphysics.BinBasedFastPointLocator3D(computing_model_part).UpdateSearchDatabase()
                    level_set_convection_process = KratosConvDiff.BFECCConvection3D(locator)
            else:
                raise Exception("The BFECC level set convection requires the Kratos ConvectionDiffusionApplication compilation.")
        else:
            linear_solver = self.get_linear_solver()
            if domain_size == 2:
                level_set_convection_process = KratosMultiphysics.LevelSetConvectionProcess2D(
                    KratosMultiphysics.DISTANCE,
                    computing_model_part,
                    linear_solver)
            else:
                level_set_convection_process = KratosMultiphysics.LevelSetConvectionProcess3D(
                    KratosMultiphysics.DISTANCE,
                    computing_model_part,
                    linear_solver)

        return level_set_convection_process

    def _create_variational_distance_process(self):
        # Construct the variational distance calculation process
        maximum_iterations = 2 #TODO: Make this user-definable
        linear_solver = self.get_linear_solver()
        computing_model_part = self.GetComputingModelPart()
        if self.main_model_part.ProcessInfo[KratosMultiphysics.DOMAIN_SIZE] == 2:
            variational_distance_process = KratosMultiphysics.VariationalDistanceCalculationProcess2D(
                computing_model_part,
                linear_solver,
                maximum_iterations,
                KratosMultiphysics.VariationalDistanceCalculationProcess2D.CALCULATE_EXACT_DISTANCES_TO_PLANE)
        else:
            variational_distance_process = KratosMultiphysics.VariationalDistanceCalculationProcess3D(
                computing_model_part,
                linear_solver,
                maximum_iterations,
                KratosMultiphysics.VariationalDistanceCalculationProcess3D.CALCULATE_EXACT_DISTANCES_TO_PLANE)

        return variational_distance_process
