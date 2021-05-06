import KratosMultiphysics as Kratos
from KratosMultiphysics.DEMApplication import *

import sys

class MaterialsAssignationUtility:

    def __init__(self, model, spheres_model_part, DEM_material_parameters):
        self.model = model
        self.spheres_model_part = spheres_model_part
        self.DEM_material_parameters = DEM_material_parameters
        self.read_materials_utility = Kratos.ReadMaterialsUtility(model)

    def AssignMaterialParametersToProperties(self):

        materials_parameters = self.DEM_material_parameters
        list_of_materials = materials_parameters["materials"]
        list_of_material_relations = materials_parameters["material_relations"]

        for material in list_of_materials:
            material_id = material["material_id"].GetInt()
            if  not self.spheres_model_part.HasProperties(material_id):
                self.spheres_model_part.CreateNewProperties(material_id)

            properties_of_model_part_with_this_id = self.spheres_model_part.GetProperties()[material_id]
            properties = material["Variables"]
            properties_of_model_part_with_this_id[Kratos.PARTICLE_MATERIAL] = material_id
            self.read_materials_utility.AssignMaterialToProperty(material, properties_of_model_part_with_this_id)

            for material_relation in list_of_material_relations:
                subprops = None
                material_ids_list = material_relation["material_ids_list"].GetVector()
                if material_id == material_ids_list[0]:
                    index_of_the_other_material = int(material_ids_list[1])
                    subprops = Kratos.Properties(index_of_the_other_material)
                elif material_id == material_ids_list[1]:
                    index_of_the_other_material = int(material_ids_list[0])
                    subprops = Kratos.Properties(index_of_the_other_material)

                if subprops:
                    contact_properties = material_relation["Variables"]
                    self.read_materials_utility.AssignMaterialToProperty(material_relation, subprops)

                    discontinuum_constitutive_law_instance = globals().get(subprops[DEM_DISCONTINUUM_CONSTITUTIVE_LAW_NAME])()
                    discontinuum_constitutive_law_instance.SetConstitutiveLawInProperties(subprops, True)

                    if subprops.Has(DEM_CONTINUUM_CONSTITUTIVE_LAW_NAME):
                        continuum_constitutive_law_instance = globals().get(subprops[DEM_CONTINUUM_CONSTITUTIVE_LAW_NAME])()
                        continuum_constitutive_law_instance.SetConstitutiveLawInPropertiesWithParameters(subprops, material_relation, True)

                    properties_of_model_part_with_this_id.AddSubProperties(subprops)


    def AssignPropertiesToEntities(self):
        materials_parameters = self.DEM_material_parameters
        list_of_materials = materials_parameters["materials"]
        material_assignation_table = materials_parameters["material_assignation_table"]

        for pair in material_assignation_table:
            submodelpart_name_in_assignation_table = pair[0].GetString()
            submodelpart = self.model.GetModelPart(submodelpart_name_in_assignation_table)

            if pair[1].IsString():
                material_name_in_assignation_table = pair[1].GetString()
                for material in list_of_materials:
                    material_name_in_materials_list = material["material_name"].GetString()
                    if material_name_in_assignation_table == material_name_in_materials_list:
                        material_id = material["material_id"].GetInt()
            elif pair[1].IsInt():
                material_id = pair[1].GetInt()
            else:
                Kratos.Logger.PrintInfo("DEM", "Error: while reading the materials assignation table, the material was not identified with a string or an integer. Exiting.")
                sys.exit()

            props = self.spheres_model_part.GetProperties()[material_id]
            submodelpart.SetValue(PROPERTIES_ID, material_id)
            for element in submodelpart.Elements:
                element.Properties = props
            for condition in submodelpart.Conditions:
                condition.Properties = props
