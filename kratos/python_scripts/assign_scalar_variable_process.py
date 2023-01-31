# Importing the Kratos Library
import KratosMultiphysics

def Factory(settings, model):
    if not isinstance(settings, KratosMultiphysics.Parameters):
        raise Exception("expected input shall be a Parameters object, encapsulating a json string")
    return CheckSameModelPartUsingSkinDistanceProcess(model, settings["Parameters"])

class CheckSameModelPartUsingSkinDistanceProcess(KratosMultiphysics.Process):
    """This process checks that the model part is the same using the skin distance

    Only the member variables listed below should be accessed directly.

    Public member variables:
    Model -- the container of the different model parts.
    settings -- Kratos parameters containing solver settings.
    """

    def __init__(self, model, settings):
        """ The default constructor of the class

        Keyword arguments:
        self -- It signifies an instance of a class.
        model -- the container of the different model parts.
        settings -- Kratos parameters containing solver settings.
        """
        KratosMultiphysics.Process.__init__(self)

        #The value can be a double or a string (function)
        default_settings = KratosMultiphysics.Parameters("""
        {
            "help"                                : "This process checks that the model part is the same using the skin distance",
            "skin_model_part_1_name"              : "PLEASE_SPECIFY_SKIN_MODEL_PART_2_NAME",
            "skin_model_part_2_name"              : "PLEASE_SPECIFY_SKIN_MODEL_PART_2_NAME",
            "tolerance"                           : 1.0e-3,
            "bounding_box_scale_factor"           : 1.5,
            "number_of_divisions_background_mesh" : 30,
            "discontinuous_distance_settings": {
            }
            }
        """
        )
        # Assigning values
        self.model = model
        settings.ValidateAndAssignDefaults(default_settings)

        # Create the process
        self.process = KratosMultiphysics.CheckSameModelPartUsingSkinDistanceProcess3D(self.model, settings)

    def ExecuteBeforeSolutionLoop(self):
        """This method is executed in before initialize the solution step

        Keyword arguments:
        self -- It signifies an instance of a class.
        """
        self.process.Execute()        