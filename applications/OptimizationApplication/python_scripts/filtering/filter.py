import typing
import abc
from importlib import import_module

import KratosMultiphysics as Kratos
from KratosMultiphysics.OptimizationApplication.utilities.union_utilities import ContainerExpressionTypes
from KratosMultiphysics.OptimizationApplication.utilities.union_utilities import SupportedSensitivityFieldVariableTypes
from KratosMultiphysics.OptimizationApplication.utilities.component_data_view import ComponentDataView

class Filter(abc.ABC):
    """Base filter class

    This class unifies the filters used in the optimization application.
    """
    def __init__(self) -> None:
        self.__component_data_view: 'typing.Optional[ComponentDataView]' = None

    def SetComponentDataView(self, component_data_view: ComponentDataView) -> None:
        """Set the component data view of the control in which this filter is being used.

        This sets and allows the filter to add data/fields to space which belongs to control
        within the optimization problem.

        This component data view is not passed at the construction time because, filters are created
        when the controls are created, hence the control data space in the optimization problem does
        not exist until the control construction is finished.

        Args:
            component_data_view (ComponentDataView): Component data view of the control.
        """
        self.__component_data_view = component_data_view

    def GetComponentDataView(self) -> ComponentDataView:
        """Retrieve the component data view.

        This method retrieves the component data view of the control, which can be used
        to store data and fields.

        Raises:
            RuntimeError: If SetComponentDataView is not called first before calling this.

        Returns:
            ComponentDataView: Component data view of the control.
        """
        if self.__component_data_view is None:
            raise RuntimeError("Please use SetComponentDataView first.")
        return self.__component_data_view

    @abc.abstractmethod
    def Initialize(self) -> None:
        """Initializes the filter.

        This method initializes the filter. This is only called once in the whole optimization process.

        """
        pass

    @abc.abstractmethod
    def Check(self) -> None:
        """Checks the filter.

        This method checks the filter. This is only called once in the whole optimization process after calling the initialize.

        """
        pass

    @abc.abstractmethod
    def Finalize(self) -> None:
        """Finalizes the filter.

        This method checks the filter. This is only called once in the whole optimization process at the end.

        """
        pass

    @abc.abstractmethod
    def FilterField(self, unfiltered_field: ContainerExpressionTypes) -> ContainerExpressionTypes:
        """Filter the input unfiltered_field.

        This method filters the passed input field. Unfiltered field is assumed to be a
        non-integrated field over entity domains.

        Args:
            unfiltered_field (ContainerExpressionTypes): Input field

        Returns:
            ContainerExpressionTypes: Filtered output field.
        """
        pass

    @abc.abstractmethod
    def FilterIntegratedField(self, unfiltered_integrated_field: ContainerExpressionTypes) -> ContainerExpressionTypes:
        """Filter the input integrated field.

        This method filters the passed integrated input field. The unfiltered integrated field is
        assumed to be already integrated over entity domains.

        Args:
            unfiltered_integrated_field (ContainerExpressionTypes): Unfiltered integrated field.

        Returns:
            ContainerExpressionTypes: Filtered output field.
        """
        pass

    @abc.abstractmethod
    def Update(self) -> None:
        """Updates the filter.

        This method is used to update the filter state. This will be called by the control, when control changes
        the optimization domain.

        """
        pass

def Factory(model: Kratos.Model, filtering_model_part_name: str, variable: SupportedSensitivityFieldVariableTypes, data_location: Kratos.Globals.DataLocation, settings: Kratos.Parameters) -> Filter:
    """Factory method used to create filters.

    This factory allows to use different types of filters in different controls. Since the filtering should
    support sub-model parts as well, the filtering_model_part_name is passed as a string because, at the time of
    filter construction from this factory, model part may only have root model parts.

    Args:
        model (Kratos.Model): Model of the problem
        filtering_model_part_name (str): Model part on which the filtering is applied (Can be a sub-model part as well.)
        variable (SupportedSensitivityFieldVariableTypes): Variable to be filtered by this filter.
        data_location (Kratos.Globals.DataLocation): Location to be used with this filter.
        settings (Kratos.Parameters): Filter specific settings.

    Raises:
        RuntimeError: If "filter_type" is not specified in the settings
        RuntimeError: If the requested "filter_type" does not have the Factory method implemented.

    Returns:
        Filter: Filter specified by the "filter_type"
    """
    if not settings.Has("filter_type"):
        raise RuntimeError(f"\"filter_type\" not provided in the following filter settings:\n{settings}")

    filter_type = settings["filter_type"].GetString()
    filter_module_name = f"KratosMultiphysics.OptimizationApplication.filtering.{filter_type}"

    module = import_module(filter_module_name)
    if not hasattr(module, "Factory"):
        raise RuntimeError(f"Python module {filter_module_name} does not have a Factory method.")
    return getattr(module, "Factory")(model, filtering_model_part_name, variable, data_location, settings)
