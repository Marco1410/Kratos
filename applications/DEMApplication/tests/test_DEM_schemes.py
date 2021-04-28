import os
import KratosMultiphysics
from KratosMultiphysics import Logger
Logger.GetDefaultOutput().SetSeverity(Logger.Severity.WARNING)
import KratosMultiphysics.DEMApplication as DEM
import KratosMultiphysics.KratosUnittest as KratosUnittest
import KratosMultiphysics.DEMApplication.DEM_analysis_stage

import KratosMultiphysics.kratos_utilities as kratos_utils

import auxiliary_functions_for_tests

this_working_dir_backup = os.getcwd()

def GetFilePath(fileName):
    return os.path.join(os.path.dirname(os.path.realpath(__file__)), fileName)


def SetHardcodedProperties(properties, properties_walls):

    properties[DEM.PARTICLE_DENSITY] = 4000.0
    properties[KratosMultiphysics.YOUNG_MODULUS] = 1.0e9
    properties[KratosMultiphysics.POISSON_RATIO] = 0.20
    properties[DEM.STATIC_FRICTION] = 0.5
    properties[DEM.DYNAMIC_FRICTION] = 0.5
    properties[DEM.FRICTION_DECAY] = 500.0
    properties[DEM.PARTICLE_COHESION] = 0.0
    properties[DEM.COEFFICIENT_OF_RESTITUTION] = 0.5
    properties[KratosMultiphysics.PARTICLE_MATERIAL] = 1
    properties[DEM.ROLLING_FRICTION] = 0.0
    properties[DEM.DEM_CONTINUUM_CONSTITUTIVE_LAW_NAME] = "DEM_KDEM"
    properties[DEM.DEM_DISCONTINUUM_CONSTITUTIVE_LAW_NAME] = "DEM_D_Hertz_viscous_Coulomb"
    properties[DEM.CONTACT_TAU_ZERO] = 0.5e6
    properties[DEM.CONTACT_SIGMA_MIN] = 1e6
    properties[DEM.CONTACT_INTERNAL_FRICC] = 1.0
    properties[DEM.ROTATIONAL_MOMENT_COEFFICIENT] = 0.0

    properties_walls[DEM.STATIC_FRICTION] = 0.0
    properties_walls[DEM.DYNAMIC_FRICTION] = 0.0
    properties_walls[DEM.WALL_COHESION] = 0.0
    properties_walls[DEM.COMPUTE_WEAR] = 0
    properties_walls[DEM.SEVERITY_OF_WEAR] = 0.001
    properties_walls[DEM.IMPACT_WEAR_SEVERITY] = 0.001
    properties_walls[DEM.BRINELL_HARDNESS] = 200.0
    properties_walls[KratosMultiphysics.YOUNG_MODULUS] = 1.0e20
    properties_walls[KratosMultiphysics.POISSON_RATIO] = 0.23

class DEM3D_ForwardEulerTestSolution(KratosMultiphysics.DEMApplication.DEM_analysis_stage.DEMAnalysisStage):

    def Initialize(self):
        super().Initialize()
        for node in self.spheres_model_part.Nodes:
            self.initial_normal_vel = node.GetSolutionStepValue(KratosMultiphysics.VELOCITY_Z)

    @classmethod
    def GetMainPath(self):
        return os.path.join(os.path.dirname(os.path.realpath(__file__)), "test_schemes")

    def GetProblemNameWithPath(self):
        return os.path.join(self.main_path, self.DEM_parameters["problem_name"].GetString())

    def CheckValues(self, x_vel, dem_pressure):
        tol = 1.0000+1.0e-3
        #reference values
        x_vel_ref = 0.02043790
        dem_pressure_ref = 21558.5

        if not (abs(x_vel_ref) < abs(x_vel*tol) and abs(x_vel_ref) > abs(x_vel/tol)):
            raise ValueError('Incorrect value for VELOCITY_X: expected value was '+ str(x_vel_ref) + ' but received ' + str(x_vel))

        if not (abs(dem_pressure_ref) < abs(dem_pressure*tol) and abs(dem_pressure_ref) > abs(dem_pressure/tol)):
            raise ValueError('Incorrect value for DEMPRESSURE: expected value was '+ str(dem_pressure_ref) + ' but received ' + str(dem_pressure))

    def Finalize(self):
        for node in self.spheres_model_part.Nodes:
            if node.Id == 1:
                x_vel = node.GetSolutionStepValue(KratosMultiphysics.VELOCITY_X)

        for node in self.rigid_face_model_part.Nodes:
            if node.Id == 5:
                dem_pressure = node.GetSolutionStepValue(DEM.DEM_PRESSURE)

        self.CheckValues(x_vel, dem_pressure)
        super().Finalize()


    def ReadModelParts(self, max_node_Id=0, max_elem_Id=0, max_cond_Id=0):
        properties = KratosMultiphysics.Properties(0)
        properties_walls = KratosMultiphysics.Properties(0)
        SetHardcodedProperties(properties, properties_walls)
        self.spheres_model_part.AddProperties(properties)
        self.rigid_face_model_part.AddProperties(properties_walls)

        DiscontinuumConstitutiveLaw = getattr(DEM, properties[DEM.DEM_DISCONTINUUM_CONSTITUTIVE_LAW_NAME])()
        DiscontinuumConstitutiveLaw.SetConstitutiveLawInProperties(properties, False)

        translational_scheme = DEM.ForwardEulerScheme()
        translational_scheme.SetTranslationalIntegrationSchemeInProperties(properties, True)
        rotational_scheme = DEM.ForwardEulerScheme()
        rotational_scheme.SetRotationalIntegrationSchemeInProperties(properties, True)

        element_name = "SphericContinuumParticle3D"
        DEM.PropertiesProxiesManager().CreatePropertiesProxies(self.spheres_model_part)

        coordinates = KratosMultiphysics.Array3()
        coordinates[0] = -1
        coordinates[1] = 0.0
        coordinates[2] = 0.0
        radius = 1
        self.creator_destructor.CreateSphericParticle(self.spheres_model_part, coordinates, properties, radius, element_name)

        coordinates = KratosMultiphysics.Array3()
        coordinates[0] = 0.95
        coordinates[1] = 0.0
        coordinates[2] = 0.0
        radius = 1
        self.creator_destructor.CreateSphericParticle(self.spheres_model_part, coordinates, properties, radius, element_name)

        for node in self.spheres_model_part.Nodes:
            node.SetSolutionStepValue(DEM.COHESIVE_GROUP, 1)

        for node in self.spheres_model_part.Nodes:
            if node.Id == 2:
                node.SetSolutionStepValue(KratosMultiphysics.VELOCITY_X, 0.0)
            if node.Id == 1:
                node.SetSolutionStepValue(KratosMultiphysics.VELOCITY_X, 0.1)


        self.rigid_face_model_part.CreateNewNode(3, -5, 5, -1.008)
        self.rigid_face_model_part.CreateNewNode(4, 5, 5, -1.008)

        self.rigid_face_model_part.CreateNewNode(5, -5, -5, -1.008)
        self.rigid_face_model_part.CreateNewNode(6, 5, -5, -1.008)

        condition_name = "RigidFace3D3N"
        self.rigid_face_model_part.CreateNewCondition(condition_name, 7, [5, 6, 3], self.rigid_face_model_part.GetProperties()[0])
        self.rigid_face_model_part.CreateNewCondition(condition_name, 8, [3, 6, 4], self.rigid_face_model_part.GetProperties()[0])





class DEM3D_TaylorTestSolution(KratosMultiphysics.DEMApplication.DEM_analysis_stage.DEMAnalysisStage):

    def Initialize(self):
        super().Initialize()
        for node in self.spheres_model_part.Nodes:
            self.initial_normal_vel = node.GetSolutionStepValue(KratosMultiphysics.VELOCITY_Z)

    @classmethod
    def GetMainPath(self):
        return os.path.join(os.path.dirname(os.path.realpath(__file__)), "test_schemes")

    def GetProblemNameWithPath(self):
        return os.path.join(self.main_path, self.DEM_parameters["problem_name"].GetString())

    def CheckValues(self, x_vel, dem_pressure):
        tol = 1.0000+1.0e-3
        #reference values
        x_vel_ref = 0.02036355217285354
        dem_pressure_ref = 21558.5

        if not (abs(x_vel_ref) < abs(x_vel*tol) and abs(x_vel_ref) > abs(x_vel/tol)):
            raise ValueError('Incorrect value for VELOCITY_X: expected value was '+ str(x_vel_ref) + ' but received ' + str(x_vel))

        if not (abs(dem_pressure_ref) < abs(dem_pressure*tol) and abs(dem_pressure_ref) > abs(dem_pressure/tol)):
            raise ValueError('Incorrect value for DEMPRESSURE: expected value was '+ str(dem_pressure_ref) + ' but received ' + str(dem_pressure))

    def Finalize(self):
        for node in self.spheres_model_part.Nodes:
            if node.Id == 1:
                x_vel = node.GetSolutionStepValue(KratosMultiphysics.VELOCITY_X)

        for node in self.rigid_face_model_part.Nodes:
            if node.Id == 5:
                dem_pressure = node.GetSolutionStepValue(DEM.DEM_PRESSURE)

        self.CheckValues(x_vel, dem_pressure)
        super().Finalize()


    def ReadModelParts(self, max_node_Id=0, max_elem_Id=0, max_cond_Id=0):
        properties = KratosMultiphysics.Properties(0)
        properties_walls = KratosMultiphysics.Properties(0)
        SetHardcodedProperties(properties, properties_walls)
        self.spheres_model_part.AddProperties(properties)
        self.rigid_face_model_part.AddProperties(properties_walls)

        DiscontinuumConstitutiveLaw = getattr(DEM, properties[DEM.DEM_DISCONTINUUM_CONSTITUTIVE_LAW_NAME])()
        DiscontinuumConstitutiveLaw.SetConstitutiveLawInProperties(properties, False)

        translational_scheme = DEM.ForwardEulerScheme()
        translational_scheme.SetTranslationalIntegrationSchemeInProperties(properties, True)
        rotational_scheme = DEM.ForwardEulerScheme()
        rotational_scheme.SetRotationalIntegrationSchemeInProperties(properties, True)

        element_name = "SphericContinuumParticle3D"
        DEM.PropertiesProxiesManager().CreatePropertiesProxies(self.spheres_model_part)

        coordinates = KratosMultiphysics.Array3()
        coordinates[0] = -1
        coordinates[1] = 0.0
        coordinates[2] = 0.0
        radius = 1
        self.creator_destructor.CreateSphericParticle(self.spheres_model_part, coordinates, properties, radius, element_name)

        coordinates = KratosMultiphysics.Array3()
        coordinates[0] = 0.95
        coordinates[1] = 0.0
        coordinates[2] = 0.0
        radius = 1
        self.creator_destructor.CreateSphericParticle(self.spheres_model_part, coordinates, properties, radius, element_name)

        for node in self.spheres_model_part.Nodes:
            node.SetSolutionStepValue(DEM.COHESIVE_GROUP, 1)

        for node in self.spheres_model_part.Nodes:
            if node.Id == 2:
                node.SetSolutionStepValue(KratosMultiphysics.VELOCITY_X, 0.0)
            if node.Id == 1:
                node.SetSolutionStepValue(KratosMultiphysics.VELOCITY_X, 0.1)


        self.rigid_face_model_part.CreateNewNode(3, -5, 5, -1.008)
        self.rigid_face_model_part.CreateNewNode(4, 5, 5, -1.008)

        self.rigid_face_model_part.CreateNewNode(5, -5, -5, -1.008)
        self.rigid_face_model_part.CreateNewNode(6, 5, -5, -1.008)

        condition_name = "RigidFace3D3N"
        self.rigid_face_model_part.CreateNewCondition(condition_name, 7, [5, 6, 3], self.rigid_face_model_part.GetProperties()[0])
        self.rigid_face_model_part.CreateNewCondition(condition_name, 8, [3, 6, 4], self.rigid_face_model_part.GetProperties()[0])



class DEM3D_SymplecticTestSolution(KratosMultiphysics.DEMApplication.DEM_analysis_stage.DEMAnalysisStage):

    def Initialize(self):
        super().Initialize()
        for node in self.spheres_model_part.Nodes:
            self.initial_normal_vel = node.GetSolutionStepValue(KratosMultiphysics.VELOCITY_Z)

    @classmethod
    def GetMainPath(self):
        return os.path.join(os.path.dirname(os.path.realpath(__file__)), "test_schemes")

    def GetProblemNameWithPath(self):
        return os.path.join(self.main_path, self.DEM_parameters["problem_name"].GetString())

    def CheckValues(self, x_vel, dem_pressure):
        tol = 1.0000+1.0e-3
        #reference values
        x_vel_ref = 0.020296313440
        dem_pressure_ref = 21525.9

        if not (abs(x_vel_ref) < abs(x_vel*tol) and abs(x_vel_ref) > abs(x_vel/tol)):
            raise ValueError('Incorrect value for VELOCITY_X: expected value was '+ str(x_vel_ref) + ' but received ' + str(x_vel))

        if not (abs(dem_pressure_ref) < abs(dem_pressure*tol) and abs(dem_pressure_ref) > abs(dem_pressure/tol)):
            raise ValueError('Incorrect value for DEMPRESSURE: expected value was '+ str(dem_pressure_ref) + ' but received ' + str(dem_pressure))

    def Finalize(self):
        for node in self.spheres_model_part.Nodes:
            if node.Id == 1:
                x_vel = node.GetSolutionStepValue(KratosMultiphysics.VELOCITY_X)

        for node in self.rigid_face_model_part.Nodes:
            if node.Id == 5:
                dem_pressure = node.GetSolutionStepValue(DEM.DEM_PRESSURE)

        self.CheckValues(x_vel, dem_pressure)
        super().Finalize()


    def ReadModelParts(self, max_node_Id=0, max_elem_Id=0, max_cond_Id=0):
        properties = KratosMultiphysics.Properties(0)
        properties_walls = KratosMultiphysics.Properties(0)
        SetHardcodedProperties(properties, properties_walls)
        self.spheres_model_part.AddProperties(properties)
        self.rigid_face_model_part.AddProperties(properties_walls)

        DiscontinuumConstitutiveLaw = getattr(DEM, properties[DEM.DEM_DISCONTINUUM_CONSTITUTIVE_LAW_NAME])()
        DiscontinuumConstitutiveLaw.SetConstitutiveLawInProperties(properties, False)

        translational_scheme = DEM.ForwardEulerScheme()
        translational_scheme.SetTranslationalIntegrationSchemeInProperties(properties, True)
        rotational_scheme = DEM.ForwardEulerScheme()
        rotational_scheme.SetRotationalIntegrationSchemeInProperties(properties, True)

        element_name = "SphericContinuumParticle3D"
        DEM.PropertiesProxiesManager().CreatePropertiesProxies(self.spheres_model_part)

        coordinates = KratosMultiphysics.Array3()
        coordinates[0] = -1
        coordinates[1] = 0.0
        coordinates[2] = 0.0
        radius = 1
        self.creator_destructor.CreateSphericParticle(self.spheres_model_part, coordinates, properties, radius, element_name)

        coordinates = KratosMultiphysics.Array3()
        coordinates[0] = 0.95
        coordinates[1] = 0.0
        coordinates[2] = 0.0
        radius = 1
        self.creator_destructor.CreateSphericParticle(self.spheres_model_part, coordinates, properties, radius, element_name)

        for node in self.spheres_model_part.Nodes:
            node.SetSolutionStepValue(DEM.COHESIVE_GROUP, 1)

        for node in self.spheres_model_part.Nodes:
            if node.Id == 2:
                node.SetSolutionStepValue(KratosMultiphysics.VELOCITY_X, 0.0)
            if node.Id == 1:
                node.SetSolutionStepValue(KratosMultiphysics.VELOCITY_X, 0.1)


        self.rigid_face_model_part.CreateNewNode(3, -5, 5, -1.008)
        self.rigid_face_model_part.CreateNewNode(4, 5, 5, -1.008)

        self.rigid_face_model_part.CreateNewNode(5, -5, -5, -1.008)
        self.rigid_face_model_part.CreateNewNode(6, 5, -5, -1.008)

        condition_name = "RigidFace3D3N"
        self.rigid_face_model_part.CreateNewCondition(condition_name, 7, [5, 6, 3], self.rigid_face_model_part.GetProperties()[0])
        self.rigid_face_model_part.CreateNewCondition(condition_name, 8, [3, 6, 4], self.rigid_face_model_part.GetProperties()[0])






class DEM3D_VerletTestSolution(KratosMultiphysics.DEMApplication.DEM_analysis_stage.DEMAnalysisStage):

    def Initialize(self):
        super().Initialize()
        for node in self.spheres_model_part.Nodes:
            self.initial_normal_vel = node.GetSolutionStepValue(KratosMultiphysics.VELOCITY_Z)

    @classmethod
    def GetMainPath(self):
        return os.path.join(os.path.dirname(os.path.realpath(__file__)), "test_schemes")

    def GetProblemNameWithPath(self):
        return os.path.join(self.main_path, self.DEM_parameters["problem_name"].GetString())

    def CheckValues(self, x_vel, dem_pressure):
        tol = 1.0000+1.0e-3
        #reference values
        x_vel_ref = 0.020341990230218668
        dem_pressure_ref = 21558.5

        if not (abs(x_vel_ref) < abs(x_vel*tol) and abs(x_vel_ref) > abs(x_vel/tol)):
            raise ValueError('Incorrect value for VELOCITY_X: expected value was '+ str(x_vel_ref) + ' but received ' + str(x_vel))

        if not (abs(dem_pressure_ref) < abs(dem_pressure*tol) and abs(dem_pressure_ref) > abs(dem_pressure/tol)):
            raise ValueError('Incorrect value for DEMPRESSURE: expected value was '+ str(dem_pressure_ref) + ' but received ' + str(dem_pressure))

    def Finalize(self):
        for node in self.spheres_model_part.Nodes:
            if node.Id == 1:
                x_vel = node.GetSolutionStepValue(KratosMultiphysics.VELOCITY_X)

        for node in self.rigid_face_model_part.Nodes:
            if node.Id == 5:
                dem_pressure = node.GetSolutionStepValue(DEM.DEM_PRESSURE)

        self.CheckValues(x_vel, dem_pressure)
        super().Finalize()


    def ReadModelParts(self, max_node_Id=0, max_elem_Id=0, max_cond_Id=0):
        properties = KratosMultiphysics.Properties(0)
        properties_walls = KratosMultiphysics.Properties(0)
        SetHardcodedProperties(properties, properties_walls)
        self.spheres_model_part.AddProperties(properties)
        self.rigid_face_model_part.AddProperties(properties_walls)

        DiscontinuumConstitutiveLaw = getattr(DEM, properties[DEM.DEM_DISCONTINUUM_CONSTITUTIVE_LAW_NAME])()
        DiscontinuumConstitutiveLaw.SetConstitutiveLawInProperties(properties, False)

        translational_scheme = DEM.ForwardEulerScheme()
        translational_scheme.SetTranslationalIntegrationSchemeInProperties(properties, True)
        rotational_scheme = DEM.ForwardEulerScheme()
        rotational_scheme.SetRotationalIntegrationSchemeInProperties(properties, True)

        element_name = "SphericContinuumParticle3D"
        DEM.PropertiesProxiesManager().CreatePropertiesProxies(self.spheres_model_part)

        coordinates = KratosMultiphysics.Array3()
        coordinates[0] = -1
        coordinates[1] = 0.0
        coordinates[2] = 0.0
        radius = 1
        self.creator_destructor.CreateSphericParticle(self.spheres_model_part, coordinates, properties, radius, element_name)

        coordinates = KratosMultiphysics.Array3()
        coordinates[0] = 0.95
        coordinates[1] = 0.0
        coordinates[2] = 0.0
        radius = 1
        self.creator_destructor.CreateSphericParticle(self.spheres_model_part, coordinates, properties, radius, element_name)

        for node in self.spheres_model_part.Nodes:
            node.SetSolutionStepValue(DEM.COHESIVE_GROUP, 1)

        for node in self.spheres_model_part.Nodes:
            if node.Id == 2:
                node.SetSolutionStepValue(KratosMultiphysics.VELOCITY_X, 0.0)
            if node.Id == 1:
                node.SetSolutionStepValue(KratosMultiphysics.VELOCITY_X, 0.1)


        self.rigid_face_model_part.CreateNewNode(3, -5, 5, -1.008)
        self.rigid_face_model_part.CreateNewNode(4, 5, 5, -1.008)

        self.rigid_face_model_part.CreateNewNode(5, -5, -5, -1.008)
        self.rigid_face_model_part.CreateNewNode(6, 5, -5, -1.008)

        condition_name = "RigidFace3D3N"
        self.rigid_face_model_part.CreateNewCondition(condition_name, 7, [5, 6, 3], self.rigid_face_model_part.GetProperties()[0])
        self.rigid_face_model_part.CreateNewCondition(condition_name, 8, [3, 6, 4], self.rigid_face_model_part.GetProperties()[0])


class TestDEMSchemes(KratosUnittest.TestCase):

    def setUp(self):
        pass

    @classmethod
    def test_ForwardEuler(self):
        path = os.path.join(os.path.dirname(os.path.realpath(__file__)), "test_schemes")
        parameters_file_name = os.path.join(path, "ProjectParametersDEM_ForwardEuler.json")
        model = KratosMultiphysics.Model()
        auxiliary_functions_for_tests.CreateAndRunStageInSelectedNumberOfOpenMPThreads(DEM3D_ForwardEulerTestSolution, model, parameters_file_name, 1)

    @classmethod
    def test_Taylor(self):
        path = os.path.join(os.path.dirname(os.path.realpath(__file__)), "test_schemes")
        parameters_file_name = os.path.join(path, "ProjectParametersDEM_Taylor.json")
        model = KratosMultiphysics.Model()
        auxiliary_functions_for_tests.CreateAndRunStageInSelectedNumberOfOpenMPThreads(DEM3D_TaylorTestSolution, model, parameters_file_name, 1)

    @classmethod
    def test_Symplectic(self):
        path = os.path.join(os.path.dirname(os.path.realpath(__file__)), "test_schemes")
        parameters_file_name = os.path.join(path, "ProjectParametersDEM_Symplectic.json")
        model = KratosMultiphysics.Model()
        auxiliary_functions_for_tests.CreateAndRunStageInSelectedNumberOfOpenMPThreads(DEM3D_SymplecticTestSolution, model, parameters_file_name, 1)

    @classmethod
    def test_Verlet(self):
        path = os.path.join(os.path.dirname(os.path.realpath(__file__)), "test_schemes")
        parameters_file_name = os.path.join(path, "ProjectParametersDEM_Verlet.json")
        model = KratosMultiphysics.Model()
        auxiliary_functions_for_tests.CreateAndRunStageInSelectedNumberOfOpenMPThreads(DEM3D_VerletTestSolution, model, parameters_file_name, 1)


    def tearDown(self):
        from glob import glob

        files_to_delete_list = []
        files_to_delete_list.append(os.path.join("TimesPartialRelease"))
        files_to_delete_list.append(os.path.join("test_schemes", "test_scheme.post.lst"))
        files_to_delete_list.append(os.path.join("test_schemes", "flux_data_new.hdf5"))

        try:
            for to_erase_file in files_to_delete_list:
                os.remove(to_erase_file)
        except:
            pass

        #............Getting rid of unuseful folders
        folders_to_delete_list      = []
        folders_to_delete_list.append(os.path.join("test_schemes", "test_scheme_Graphs"))
        folders_to_delete_list.append(os.path.join("test_schemes", "test_scheme_MPI_results"))
        folders_to_delete_list.append(os.path.join("test_schemes", "test_scheme_Post_Files"))
        folders_to_delete_list.append(os.path.join("test_schemes", "test_scheme_Results_and_Data"))


        for to_erase_folder in folders_to_delete_list:
            import shutil
            shutil.rmtree(to_erase_folder)

        os.chdir(this_working_dir_backup)





if __name__ == "__main__":
    Logger.GetDefaultOutput().SetSeverity(Logger.Severity.WARNING)
    KratosUnittest.main()
