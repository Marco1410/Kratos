#    |  /           |
#    ' /   __| _` | __|  _ \   __|
#    . \  |   (   | |   (   |\__ `
#   _|\_\_|  \__,_|\__|\___/ ____/
#                   Multi-Physics
#
#  License:		 BSD License
#					 license: OptimizationApplication/license.txt
#
#  Main authors:    Suneth Warnakulasuriya
#

import time as timer
import datetime
import KratosMultiphysics as Kratos


def AddFileLoggerOutput(logger_file_name):
    logger_file = Kratos.FileLoggerOutput(logger_file_name)
    default_severity = Kratos.Logger.GetDefaultOutput().GetSeverity()
    Kratos.Logger.GetDefaultOutput().SetSeverity(Kratos.Logger.Severity.WARNING)
    Kratos.Logger.AddOutput(logger_file)

    return default_severity, logger_file

def RemoveFileLoggerOutput(default_severity, logger_file):
    Kratos.Logger.Flush()
    Kratos.Logger.RemoveOutput(logger_file)
    Kratos.Logger.GetDefaultOutput().SetSeverity(default_severity)

class FileLogger:
    """@brief A context responsible for managing the lifetime of logger files."""
    def __init__(self, logger_file_name: str):
        self.__logger_file_name = logger_file_name

    def __enter__(self):
        self.__default_severity, self.__logger_file = AddFileLoggerOutput(self.__logger_file_name)

    def __exit__(self, exit_type, exit_value, exit_traceback):
        RemoveFileLoggerOutput(self.__default_severity, self.__logger_file)

class TimeLogger:
    """@brief A context responsible for outputting execution times."""
    def __init__(self, topic: str, entry_msg: str , exit_msg: str):
        self.topic = topic
        self.entry_msg = entry_msg
        self.exit_msg = exit_msg
        self.start_time = None

    def __enter__(self):
        if self.entry_msg is not None:
            Kratos.Logger.PrintInfo(self.topic, self.entry_msg)
        self.start_time = timer.time()

    def __exit__(self, exit_type, exit_value, exit_traceback):
        if self.exit_msg is not None:
            elapsed_time = timer.time() - self.start_time
            Kratos.Logger.PrintInfo(self.topic, "{:s} - [ Elapsed time: {:s} ]".format(self.exit_msg, str(datetime.timedelta(seconds=round(elapsed_time))))+"\n")

class OptimizationAlgorithmTimeLogger:
    """@brief A context responsible for outputting execution times."""
    def __init__(self, optimizer_name: str, optimization_itr: int):
        self.start_time = None
        self.optimization_itr = optimization_itr
        self.optimizer_name = optimizer_name

    def __enter__(self):
        self.start_time = timer.time()

    def __exit__(self, exit_type, exit_value, exit_traceback):

        current_time = timer.time()
        elapsed_time = current_time - self.start_time
        elapsed_time_string = str(datetime.timedelta(seconds=round(elapsed_time)))

        iteration_text = f"{self.optimizer_name} EoF Iteration {self.optimization_itr}"
        iteration_output = f"{'#'}  {iteration_text} [Elapsed Time: {elapsed_time_string}]  {'#'}"


        divided_line = len(iteration_output) * '#'

        to_print = f"{divided_line}\n{iteration_output}\n{divided_line}\n"

        Kratos.Logger.Print(to_print)

class OptimizationAnalysisTimeLogger:

    def __enter__(self):
        start_text = "Optimization Start"

        self.start_time = datetime.datetime.now()
        start_time_string = self.start_time.strftime("%Y-%m-%d %H:%M:%S")

        time_string = f"**   {start_time_string}   **"
        separator_string = len(time_string) * '*'
        center_string = f"**{start_text.center(len(str(time_string))-4)}**"
        final_string = f"{separator_string}\n{center_string}\n{time_string}\n{separator_string}"

        Kratos.Logger.Print(final_string)

    def __exit__(self, exit_type, exit_value, exit_traceback):

        end_text = "Optimization End"

        end_time = datetime.datetime.now()
        end_time_string = end_time.strftime("%Y-%m-%d %H:%M:%S")

        elapsed_time = end_time - self.start_time
        elapsed_time_string = str(datetime.timedelta(seconds=round(elapsed_time.total_seconds())))

        time_string = f"**   {end_time_string}  [Elapsed Time: {elapsed_time_string}]   **"
        separator_string = len(time_string) * '*'
        center_string = f"**{end_text.center(len(str(time_string))-4)}**"
        final_string = f"{separator_string}\n{center_string}\n{time_string}\n{separator_string}"

        Kratos.Logger.Print(final_string)

def TablulizeData(title, data):
    # Determine the maximum length of labels
    max_label_len = max(len(str(label)) for label, _ in data)

    # Determine the maximum length of values
    for i in range(len(data)):
        key, value = data[i]
        if isinstance(value, float):
            data[i] = (key, round(value, 6))

    max_value_len = max(len(f"{value:.6f}" if isinstance(value, float) else str(value)) for _, value in data)

    title_len = len(str(title)) + 8

    # Calculate the row width
    if title_len > max_label_len + max_value_len + 7:
        row_width = title_len
        max_value_len = row_width - max_label_len - 7
    else:
        row_width = max_label_len + max_value_len + 7


    # Create format strings for the labels and values
    label_format = f"| {{:<{max_label_len}}} |"
    value_format = f"{{:>{max_value_len}.6f}} |\n"



    # Build the table
    table = '-' * row_width + "\n"
    table += f"|{str(title).center(row_width - 2)}|\n"
    table += '-' * row_width + "\n"

    # Add the data to the table
    for label, value in data:
        # Handle both numeric and string types for value
        if isinstance(value, (float, int)):
            table += f"{label_format.format(label)} {value_format.format(round(value, 6))}"
        else:
            table += f"{label_format.format(label)} {value:>{max_value_len}} |\n"

    table += '-' * row_width

    return table

