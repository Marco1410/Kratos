# Kratos imports
import KratosMultiphysics
from KratosMultiphysics.python_solver import PythonSolver
import KratosMultiphysics.FreeSurfaceApplication as FreeSurface

# STL imports
import enum
import pathlib


def DebuggedMember(member_function):
    def Wrapper(this, *args, **kwargs):
        if not hasattr(this, "__indent_level"):
            this.__indent_level = 0

        function_name = member_function.__name__
        indent = "".join('    ' for _ in range(this.__indent_level))

        KratosMultiphysics.Logger.PrintInfo(this.__class__.__name__, "DEBUG: {}calling '{}'".format(indent, function_name))
        this.__indent_level += 1
        result = member_function(this, *args, **kwargs)
        this.__indent_level -= 1
        KratosMultiphysics.Logger.PrintInfo(this.__class__.__name__, "DEBUG: {}finished '{}'".format(indent, function_name))

        return result
    return Wrapper



class EdgeBasedLevelSetSolver(PythonSolver):

    class PorousResistanceComputation(enum.IntEnum):
        none   = 0
        ergun  = 1
        custom = 2


    def __init__(self,
                 model: KratosMultiphysics.Model,
                 parameters: KratosMultiphysics.Parameters):
        """
        Default parameters.
        {
            "model_part_name"                       : "",
            "domain_size"                           : 3,
            "body_force"                            : [0.0, 0.0, 0.0],
            "density"                               : 0.0,
            "viscosity"                             : 0.0,
            "wall_law_y"                            : 0.0,
            "stabdt_pressure_factor"                : 1.0,
            "stabdt_convection_factor"              : 0.1,
            "use_mass_correction"                   : true,
            "redistance_frequency"                  : 5,
            "extrapolation_layers"                  : 5,
            "tau2_factor"                           : 0.0,
            "edge_detection_angle"                  : 45.0,
            "max_safety_factor"                     : 0.5,
            "max_time_step_size"                    : 1e-2,
            "initial_time_step_size"                : 1e-5,
            "number_of_initial_time_steps"          : 10,
            "reduction_on_failure"                  : 0.3,
            "assume_constant_pressure"              : true,
            "use_parallel_distance_calculation"     : false,
            "compute_porous_resistance_law"         : "none",    ["none", "ergun", "custom"]
            "echo_level"                            : 0,
            "model_import_settings"                 : { }
        }
        """
        # Base class defines:
        # - self.model
        # - self.settings
        PythonSolver.__init__(self, model, parameters)

        # Get root ModelPart
        model_part_name = self.settings["model_part_name"].GetString()
        if self.model.HasModelPart(model_part_name):
            self.model_part = self.model.GetModelPart(model_part_name)
        else:
            self.model_part = self.model.CreateModelPart(model_part_name)

        # Parse numeric parameters
        self.domain_size                   = self.settings["domain_size"].GetInt()
        self.body_force                    = self.settings["body_force"].GetVector()
        self.density                       = self.settings["density"].GetDouble()
        self.viscosity                     = self.settings["viscosity"].GetDouble()
        self.wall_law_y                    = self.settings["wall_law_y"].GetDouble()
        self.stabdt_pressure_factor        = self.settings["stabdt_pressure_factor"].GetDouble()
        self.stabdt_convection_factor      = self.settings["stabdt_convection_factor"].GetDouble()
        self.redistance_frequency          = self.settings["redistance_frequency"].GetInt()
        self.extrapolation_layers          = self.settings["extrapolation_layers"].GetInt()
        self.tau2_factor                   = self.settings["tau2_factor"].GetDouble()
        self.edge_detection_angle          = self.settings["edge_detection_angle"].GetDouble()
        self.max_safety_factor             = self.settings["max_safety_factor"].GetDouble()
        self.max_time_step_size            = self.settings["max_time_step_size"].GetDouble()
        self.initial_time_step_size        = self.settings["initial_time_step_size"].GetDouble()
        self.number_of_initial_time_steps  = self.settings["number_of_initial_time_steps"].GetInt()
        self.reduction_on_failure          = self.settings["reduction_on_failure"].GetDouble()

        # Parse options
        self.use_mass_correction               = self.settings["use_mass_correction"].GetBool()
        self.assume_constant_pressure          = self.settings["assume_constant_pressure"].GetBool()
        self.use_parallel_distance_calculation = self.settings["use_parallel_distance_calculation"].GetBool()
        self.compute_porous_resistance_law     = EdgeBasedLevelSetSolver.PorousResistanceComputation[self.settings["compute_porous_resistance_law"].GetString()]

        # Other linear solvers considered in the original script:
        # - CGSolver(1e3, 5000)
        # - CGSolver(1e3, 500, DiagonalPreconditioner())
        # - BICGSTabSolver(1e3, 5000, DiagonalPreconditioner())
        self.pressure_linear_solver = KratosMultiphysics.BICGSTABSolver(1e-3, 5000)

        # Preparations before the model part is read
        # (apart from adding variables)
        self.model_part.ProcessInfo.SetValue(KratosMultiphysics.DOMAIN_SIZE, self.domain_size)

        self.matrix_container = self.__MakeMatrixContainer()
        self.distance_utils   = self.__MakeDistanceUtilities()

        # Declare other members
        self.distance_size         = 0
        self.fluid_solver          = None # initialized by __MakeEdgeBasedLevelSet after reading the ModelPart
        self.safety_factor         = self.max_safety_factor
        self.current_step_size     = self.initial_time_step_size
        self.current_max_step_size = self.max_time_step_size
        self.timer                 = KratosMultiphysics.Timer()


    def GetDefaultParameters(self) -> KratosMultiphysics.Parameters:
        return KratosMultiphysics.Parameters("""{
            "model_part_name"                       : "",
            "domain_size"                           : 3,
            "body_force"                            : [0.0, 0.0, 0.0],
            "density"                               : 0.0,
            "viscosity"                             : 0.0,
            "wall_law_y"                            : 0.0,
            "stabdt_pressure_factor"                : 1.0,
            "stabdt_convection_factor"              : 0.1,
            "use_mass_correction"                   : true,
            "redistance_frequency"                  : 5,
            "extrapolation_layers"                  : 5,
            "tau2_factor"                           : 0.0,
            "edge_detection_angle"                  : 45.0,
            "max_safety_factor"                     : 0.5,
            "max_time_step_size"                    : 1e-2,
            "initial_time_step_size"                : 1e-5,
            "number_of_initial_time_steps"          : 10,
            "reduction_on_failure"                  : 0.3,
            "assume_constant_pressure"              : true,
            "use_parallel_distance_calculation"     : false,
            "compute_porous_resistance_law"         : "none",
            "echo_level"                            : 0,
            "model_import_settings"                 : { }
        }""")


    @staticmethod
    def GetMinimumBufferSize() -> int:
        return 2


    @DebuggedMember
    def AddVariables(self) -> None:
        self.model_part.AddNodalSolutionStepVariable(KratosMultiphysics.VELOCITY)
        self.model_part.AddNodalSolutionStepVariable(KratosMultiphysics.PRESSURE)
        self.model_part.AddNodalSolutionStepVariable(KratosMultiphysics.NORMAL)
        self.model_part.AddNodalSolutionStepVariable(KratosMultiphysics.AUX_INDEX)
        self.model_part.AddNodalSolutionStepVariable(KratosMultiphysics.DISTANCE)
        self.model_part.AddNodalSolutionStepVariable(KratosMultiphysics.PRESS_PROJ)
        self.model_part.AddNodalSolutionStepVariable(KratosMultiphysics.POROSITY)
        self.model_part.AddNodalSolutionStepVariable(KratosMultiphysics.VISCOSITY)
        self.model_part.AddNodalSolutionStepVariable(KratosMultiphysics.DIAMETER)
        self.model_part.AddNodalSolutionStepVariable(KratosMultiphysics.LIN_DARCY_COEF)
        self.model_part.AddNodalSolutionStepVariable(KratosMultiphysics.NONLIN_DARCY_COEF)
        self.model_part.AddNodalSolutionStepVariable(KratosMultiphysics.NODAL_AREA)
        self.model_part.AddNodalSolutionStepVariable(KratosMultiphysics.STRUCTURE_VELOCITY)


    @DebuggedMember
    def AddDofs(self) -> None:
        for node in self.model_part.Nodes:
            node.AddDof(KratosMultiphysics.PRESSURE)
            node.AddDof(KratosMultiphysics.VELOCITY_X)
            node.AddDof(KratosMultiphysics.VELOCITY_Y)
            node.AddDof(KratosMultiphysics.VELOCITY_Z)


    @DebuggedMember
    def ImportModelPart(self) -> None:
        self._ImportModelPart(self.model_part, self.settings["model_import_settings"].Clone())


    @DebuggedMember
    def PrepareModelPart(self) -> None:
        # Set buffer size
        self.model_part.SetBufferSize(self.GetMinimumBufferSize())


    @DebuggedMember
    def Check(self) -> None:
        if all(not component for component in self.body_force):
            raise ValueError("Body force cannot be a zero vector")


    @DebuggedMember
    def Initialize(self) -> None:
        # Get rid of isolated nodes
        KratosMultiphysics.FindNodalNeighboursProcess(self.model_part).Execute()
        KratosMultiphysics.EliminateIsolatedNodesProcess(self.model_part).Execute()

        # Initialize remaining nodes
        initializer = self.density * self.body_force
        small_value = 1e-4
        active_node_count = 0
        for node in self.model_part.Nodes:
            # Initialize DISTANCE
            if node.GetSolutionStepValue(KratosMultiphysics.DISTANCE) < 0.0:
                active_node_count += 1
                node.SetSolutionStepValue(KratosMultiphysics.DISTANCE, 0, -small_value)
            else:
                node.SetSolutionStepValue(KratosMultiphysics.DISTANCE, 0, small_value)

            # Make sure no node has null porosity and diameter
            # Note: this was set in the main script, not sure if it needs to be here
            if node.GetSolutionStepValue(KratosMultiphysics.POROSITY) == 0.0:
                node.SetSolutionStepValue(KratosMultiphysics.POROSITY, 0, 1.0)
            if node.GetSolutionStepValue(KratosMultiphysics.DIAMETER) == 0.0:
                node.SetSolutionStepValue(KratosMultiphysics.DIAMETER, 0, 1.0)

            porosity = node.GetSolutionStepValue(KratosMultiphysics.POROSITY)
            node.SetSolutionStepValue(KratosMultiphysics.PRESS_PROJ, 0, initializer * porosity)

        if not active_node_count:
            raise RuntimeError("At least 1 node must be initialized with a negative DISTANCE")

        # Build edge data structure
        self.matrix_container.ConstructCSRVector(self.model_part)
        self.matrix_container.BuildCSRData(self.model_part)

        # Initialize solver
        self.fluid_solver  = self.__MakeEdgeBasedLevelSet()
        self.distance_size = 3.0 * self.distance_utils.FindMaximumEdgeSize(self.model_part)
        self.fluid_solver.SetShockCapturingCoefficient(0.0)

        # Note:
        # original script prints the number of edges with positive/negative DISTANCE values
        # before and after calling self.fluid_solver.Initialize
        self.fluid_solver.Initialize()
        self.__Redistance()
        if 1e-10 < self.wall_law_y:
            self.fluid_solver.ActivateWallResistance(self.wall_law_y)


    @DebuggedMember
    def AdvanceInTime(self, current_time: float) -> float:
        """
        Compute a bounded variable time step and create new step data.
        Note: this function modifies
            -> self.safety_factor
            -> self.current_max_step_size
            -> self.current_step_size
        """
        # Bound time step size
        self.current_max_step_size = self.initial_time_step_size
        self.model_part.ProcessInfo[KratosMultiphysics.STEP] += 1

        if self.number_of_initial_time_steps < self.model_part.ProcessInfo[KratosMultiphysics.STEP]:
            self.current_max_step_size = self.max_time_step_size

            self.safety_factor *= 1.2
            if self.max_safety_factor < self.safety_factor:
                self.safety_factor = self.max_safety_factor

        # Advance
        self.current_step_size = self.__EstimateTimeStep(self.safety_factor, self.current_max_step_size)
        new_time = current_time + self.current_step_size
        self.model_part.CloneTimeStep(new_time)

        return new_time


    @DebuggedMember
    def SolveSolutionStep(self) -> bool:
        """Perform a local solution loop until the time step size shrinks to the target bounds."""

        # DEBUG (original line: while True and 2 < self.model_part.ProcessInfo[KratosMultiphysics.STEP]:)
        while True and (3 < self.model_part.ProcessInfo[KratosMultiphysics.STEP]):
            self.__SolveLocalSolutionStep()

            # Check step size and revert if it's too large
            # Note: is the fixed 0.95 safety factor intended here?
            step_size_estimate = self.__EstimateTimeStep(0.95, self.current_max_step_size)

            if step_size_estimate < self.current_step_size:
                # Log failure
                self.__Log("Reducing time step ...")
                time = self.model_part.ProcessInfo[KratosMultiphysics.TIME]
                self.__Log("Failed time: {}".format(time))
                self.__Log("Failed step size: {}".format(self.current_step_size))

                # Clear step data
                self.fluid_solver.ReduceTimeStep(self.model_part, time)

                # Get reduced step
                time -= self.current_step_size
                self.safety_factor *= self.reduction_on_failure
                self.current_step_size = self.__EstimateTimeStep(
                    self.safety_factor,
                    self.current_max_step_size)
                time += self.current_step_size

                self.__Log("Reduced time: {}".format(time))
                self.__Log("Reduced step size: {}".format(self.current_step_size))

                self.fluid_solver.ReduceTimeStep(self.model_part, time)
            else:
                break

        return True


    def GetComputingModelPart(self) -> KratosMultiphysics.ModelPart:
        return self.model_part


    @DebuggedMember
    def ExportModelPart(self) -> None:
        file_name = pathlib.Path(self.settings["model_import_settings"]["input_file_name"].GetString() + ".out")
        KratosMultiphysics.ModelPartIO(
            file_name,
            KratosMultiphysics.IO.WRITE).WriteModelPart(self.model_part)

        self.__Log("Model part written to '{}'".format(file_name))


    @DebuggedMember
    def __MakeMatrixContainer(self) -> FreeSurface.MatrixContainer3D:
        if self.domain_size == 2:
            return FreeSurface.MatrixContainer2D()
        elif self.domain_size == 3:
            return FreeSurface.MatrixContainer3D()
        else:
            raise ValueError("Invalid domain size: {}".format(self.domain_size))


    @DebuggedMember
    def __MakeDistanceUtilities(self) -> KratosMultiphysics.ParallelDistanceCalculator3D:
        if self.domain_size == 2:
            if self.use_parallel_distance_calculation:
                return KratosMultiphysics.ParallelDistanceCalculator2D()
            else:
                return KratosMultiphysics.SignedDistanceCalculationUtils2D()
        elif self.domain_size == 3:
            if self.use_parallel_distance_calculation:
                return KratosMultiphysics.ParallelDistanceCalculator3D()
            else:
                return KratosMultiphysics.SignedDistanceCalculationUtils3D()
        else:
            raise ValueError("Invalid domain size: {}".format(self.domain_size))


    @DebuggedMember
    def __MakeEdgeBasedLevelSet(self) -> FreeSurface.EdgeBasedLevelSet3D:
        if self.domain_size == 2:
            EdgeBasedLevelSet = FreeSurface.EdgeBasedLevelSet2D
        elif self.domain_size == 3:
            # Conditions' neighbours must be evaluated in 3D
            KratosMultiphysics.FindConditionsNeighboursProcess(
                self.model_part, self.domain_size, 10).Execute()
            EdgeBasedLevelSet = FreeSurface.EdgeBasedLevelSet3D
        else:
            raise ValueError("Invalid domain size: {}".format(self.domain_size))

        return EdgeBasedLevelSet(
            self.matrix_container,
            self.model_part,
            self.viscosity,
            self.density,
            self.body_force,
            self.use_mass_correction,
            self.edge_detection_angle,
            self.stabdt_pressure_factor,
            self.stabdt_convection_factor,
            self.edge_detection_angle,
            self.assume_constant_pressure)


    @DebuggedMember
    def __EstimateTimeStep(self, safety_factor: float, max_time_step_size: float) -> float:
        step_size = self.fluid_solver.ComputeTimeStep(safety_factor, max_time_step_size)
        if max_time_step_size < step_size:
            step_size = max_time_step_size

        return step_size


    @DebuggedMember
    def __Redistance(self) -> None:
        # DEBUG: debug section begin        #
        print("redistance")                 #
        # DEBUG: debug section end          #
        if self.use_parallel_distance_calculation:
            self.distance_utils.CalculateDistances(
                self.model_part,
                KratosMultiphysics.DISTANCE,
                KratosMultiphysics.NODAL_AREA,
                self.extrapolation_layers,
                self.distance_size)
        else:
            self.distance_utils.CalculateDistances(
                self.model_part,
                KratosMultiphysics.DISTANCE,
                self.distance_size)


    @DebuggedMember
    def __SolveLocalSolutionStep(self):
        """Solve the system in its current state without checking the time step."""
        if self.extrapolation_layers < 3:
            raise ValueError("Insufficient number of extrapolation layers: {} (minimum is 3)".format(self.extrapolation_layers))

        self.timer.Start("Calculate porous resistance law")
        self.fluid_solver.CalculatePorousResistanceLaw(int(self.compute_porous_resistance_law))
        self.timer.Stop("Calculate porous resistance law")

        self.timer.Start("Update fixed velocity values")
        self.fluid_solver.UpdateFixedVelocityValues()
        self.timer.Stop("Update fixed velocity values")

        self.timer.Start("Extrapolate values")
        self.fluid_solver.ExtrapolateValues(self.extrapolation_layers)
        self.timer.Stop("Extrapolate values")

        self.timer.Start("Convect distance")
        self.fluid_solver.ConvectDistance()
        self.timer.Stop("Convect distance")

        step = self.model_part.ProcessInfo[KratosMultiphysics.STEP]
        # DEBUG: original section begin
        # if step and (step % self.redistance_frequency) == 0:
        # DEBUG: original section end
        # DEBUG: debug section begin                                #
        if not hasattr(self, "_step"):                              #
            self._step = 0                                          #
        if self._step == self.redistance_frequency:                 #
            self._step = 0                                          #
        # DEBUG: debug section end                                  #
            self.timer.Start("Redistance")
            self.__Log("redistance")
            self.__Redistance()
            self.timer.Stop("Redistance")
        # DEBUG: debug section begin                                #
        self._step += 1                                             #
        # DEBUG: debug section end                                  #

        # Solve fluid
        self.timer.Start("Solve step 1")
        self.fluid_solver.SolveStep1()
        self.timer.Stop("Solve step 1")

        self.timer.Start("Solve step 2")
        self.fluid_solver.SolveStep2(self.pressure_linear_solver)
        self.timer.Stop("Solve step 2")

        self.timer.Start("Solve step 3")
        self.fluid_solver.SolveStep3()
        self.timer.Stop("Solve step 3")


    def __Log(self, message: str) -> None:
        KratosMultiphysics.Logger.PrintInfo(self.__class__.__name__, message)





def CreateSolver(model: KratosMultiphysics.Model, parameters: KratosMultiphysics.Parameters) -> EdgeBasedLevelSetSolver:
    if not issubclass(type(model), KratosMultiphysics.Model):
        raise TypeError("Expecting a KratosMultiphysics.Model object, but got {}".format(type(model)))

    if not issubclass(type(parameters), KratosMultiphysics.Parameters):
        raise TypeError("Expecting a KratosMultiphysics.Parameters object, but got {}".format(type(parameters)))

    return EdgeBasedLevelSetSolver(model, parameters)