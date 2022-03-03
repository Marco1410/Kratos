# ==============================================================================
#  KratosShapeOptimizationApplication
#
#  License:         BSD License
#                   license: ShapeOptimizationApplication/license.txt
#
#  Main authors:    Baumgaertner Daniel, https://github.com/dbaumgaertner
#                   Geiser Armin, https://github.com/armingeiser
#
# ==============================================================================

# Making KratosMultiphysics backward compatible with python 2.6 and 2.7
from __future__ import print_function, absolute_import, division

# Kratos Core and Apps
import KratosMultiphysics as KM
import KratosMultiphysics.OptimizationApplication as KOA
from KratosMultiphysics import Parameters, Logger

# Additional imports
from KratosMultiphysics.OptimizationApplication.algorithms.algorithm_base import OptimizationAlgorithm
from KratosMultiphysics.ShapeOptimizationApplication.utilities.custom_timer import Timer



# ==============================================================================
class AlgorithmSteepestDescent(OptimizationAlgorithm):
    # --------------------------------------------------------------------------
    def __init__(self,name,opt_settings,model,model_parts_controller,analyses_controller,responses_controller,controls_controller):
        super().__init__(name,opt_settings,model,model_parts_controller,analyses_controller,responses_controller,controls_controller)
        default_algorithm_settings = Parameters("""
        {
            "max_iterations"     : 100,
            "relative_tolerance" : 1e-3
        }""")

        self.opt_settings["algorithm_settings"].RecursivelyValidateAndAssignDefaults(default_algorithm_settings)
        self.algorithm_settings = self.opt_settings["algorithm_settings"]
        self.max_iterations = self.algorithm_settings["max_iterations"].GetInt()

        Logger.PrintInfo("::[AlgorithmSteepestDescent]:: ", "Construction finished")


    # --------------------------------------------------------------------------
    def InitializeOptimizationLoop(self):
        super().InitializeOptimizationLoop() 
    # --------------------------------------------------------------------------
    def RunOptimizationLoop(self):

        timer = Timer()
        timer.StartTimer()        

        for self.optimization_iteration in range(1,self.max_iterations+1):
            Logger.Print("")
            Logger.Print("===============================================================================")
            Logger.PrintInfo("AlgorithmSteepestDescent", "",timer.GetTimeStamp(), ": Starting optimization iteration ",self.optimization_iteration)
            Logger.Print("===============================================================================\n")

            self.model_parts_controller.UpdateTimeStep(self.optimization_iteration)

            self.analyses_controller.RunAnalyses(self.analyses)

            self.responses_controller.CalculateResponsesValue(self.objectives)

            for control,responses_model_parts_dict in self.controls_responses_model_parts.items():
                control_type = self.controls_type[control]
                control_variable_name =  self.supported_control_types_variables_name[control_type]
                responses_variable_names = []
                responses_gradient_variable_names = []
                for response,model_parts in responses_model_parts_dict.items():
                    self.responses_controller.CalculateResponseGradientsForTypeAndObjects(response,control_type,model_parts,False)
                    reponse_gradient_variable_name = self.responses_controller.GetResponseGradientVariableNameForType(response,control_type,False)
                    response_variable_name = self.responses_controller.GetResponseVariableName(response)
                    responses_variable_names.append(response_variable_name)
                    responses_gradient_variable_names.append(reponse_gradient_variable_name)
                
                responses_variable_names = list(set(responses_variable_names))
                responses_gradient_variable_names = list(set(responses_gradient_variable_names))
                if not len(responses_variable_names) == len(responses_gradient_variable_names):
                    raise RuntimeError("AlgorithmSteepestDescent:RunOptimizationLoop: something went wrong !")
                for i in range(len(responses_variable_names)):
                    response_variable_name = responses_variable_names[i]
                    reponse_gradient_variable_name = responses_gradient_variable_names[i]
                    response_control_gradient_variable_name = "D_"+response_variable_name+"_D_"+control_variable_name
                    self.controls_controller.MapControlFirstDerivative(control,KM.KratosGlobals.GetVariable(reponse_gradient_variable_name), KM.KratosGlobals.GetVariable(response_control_gradient_variable_name), False)
            

            # now output 
            for control_model_part,vtkIO in self.controlling_model_parts_vtkIOs.items():
                root_control_model_part = self.model_parts_controller.GetRootModelPart(control_model_part)
                OriginalTime = root_control_model_part.ProcessInfo[KM.TIME]
                root_control_model_part.ProcessInfo[KM.TIME] = self.optimization_iteration

                vtkIO.ExecuteInitializeSolutionStep()
                if(vtkIO.IsOutputStep()):
                    vtkIO.PrintOutput()
                vtkIO.ExecuteFinalizeSolutionStep()

                root_control_model_part.ProcessInfo[KM.TIME] = OriginalTime


            Logger.Print("")
            Logger.PrintInfo("AlgorithmSteepestDescent", "Time needed for current optimization step = ", timer.GetLapTime(), "s")
            Logger.PrintInfo("AlgorithmSteepestDescent", "Time needed for total optimization so far = ", timer.GetTotalTime(), "s")

        #     if self.__isAlgorithmConverged():
        #         break
        #     else:
        #         self.__determineAbsoluteChanges()

    # --------------------------------------------------------------------------
    def FinalizeOptimizationLoop(self):
        super().FinalizeOptimizationLoop()

# ==============================================================================
