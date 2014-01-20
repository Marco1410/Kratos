from __future__ import unicode_literals, print_function, absolute_import, division #makes KratosMultiphysics backward compatible with python 2.6 and 2.7
from KratosIncompressibleFluidApplication import *
application = KratosIncompressibleFluidApplication()
application_name = "KratosIncompressibleFluidApplication"
application_folder = "incompressible_fluid_application"

# The following lines are common for all applications
import application_importer
import inspect
caller = inspect.stack()[1]  # Information about the file that imported this, to check for unexpected imports
application_importer.ImportApplication(application, application_name, application_folder, caller)
