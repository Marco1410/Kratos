# STD imports
import sys
import re
import os
from pathlib import Path
import shutil
import subprocess
import typing

# External imports
try:
    from mypy.stubgen import parse_options, generate_stubs
except ImportError:
    print("Please install mypy using  \"pip install mypy\" to generate pyi stub files for libraries generated by pybind11.")
    exit(0)

warnings_and_errors_list = []

def PostProcessGeneratedStubFiles():
    kratos_installation_path = Path(sys.argv[1]).absolute()

    files = kratos_installation_path.rglob("*.pyi")
    for file in files:
        with open(str(file.absolute()), "r") as file_input:
            data = file_input.read()

        data = re.sub(R"import +Kratos([a-zA-Z0-9_]+)\n", R"import KratosMultiphysics.\1 as Kratos\1\n", data)
        data = re.sub(R"import +Kratos.([a-zA-Z0-9_.]+)\n", R"import KratosMultiphysics.\1\n", data)
        data = data.replace("import Kratos\n", "import KratosMultiphysics as Kratos\n")

        # adding Kratos import to every pyi file because, some of the
        # stub files may not have it.
        data = "import KratosMultiphysics as Kratos\n" + data

        with open(file.absolute(), "w") as file_output:

            file_output.write(data)

class KratosPythonCppLib:
    def __init__(self, cpp_lib_path: Path) -> None:
        self.base_path = cpp_lib_path.parent.parent
        self.cpp_lib_path = cpp_lib_path
        self.python_module: typing.Optional[Path] = None

        self.dependent_modules: 'list[KratosPythonCppLib]' = []

    def AddDependentModule(self, kratos_python_cpp_lib) -> None:
        if kratos_python_cpp_lib is not None:
            self.dependent_modules.append(kratos_python_cpp_lib)

    def GetDependentModulesList(self) -> 'list[KratosPythonCppLib]':
        return self.dependent_modules

    def GetPythonModule(self) -> str:
        relative_path = self.python_module.relative_to(self.base_path)
        if relative_path.name == "__init__.py":
            return str(relative_path.parent).replace("/", ".")
        else:
            return str(relative_path)[:-3].replace("/", ".")

    def GetCppLibModule(self) -> str:
        return self.GetCppLibModuleName(self.cpp_lib_path)

    def SetPythonModule(self, python_module: Path) -> None:
        self.python_module = python_module

    def GetCppLibPath(self) -> Path:
        return self.cpp_lib_path

    def GetPythonPath(self) -> Path:
        return self.python_module

    def IsPythonModuleDefined(self) -> bool:
        return self.python_module != None

    @staticmethod
    def GetCppLibModuleName(cpp_lib_path: Path) -> str:
        if os.name == 'nt': # This means "Windows"
            if cpp_lib_path.name.endswith(".pyd"):
                return cpp_lib_path.name.split(".")[0]
            else:
                return ""
        else:
            loc = cpp_lib_path.name.find(".cpython")
            if loc == -1:
                return ""
            else:
                return cpp_lib_path.name[:loc]

def GetCppLibs(kratos_libs_path: Path) -> 'list[KratosPythonCppLib]':
    # generate list of cpp modules
    list_of_python_cpp_libs: 'list[KratosPythonCppLib]' = []
    for cpp_lib_path in kratos_libs_path.iterdir():
        if KratosPythonCppLib.GetCppLibModuleName(cpp_lib_path) != "":
            list_of_python_cpp_libs.append(KratosPythonCppLib(cpp_lib_path))
    return list_of_python_cpp_libs

def FindModule(module_name: str, list_of_python_cpp_libs: 'list[KratosPythonCppLib]') -> KratosPythonCppLib:
    for kratos_dependent_module in list_of_python_cpp_libs:
        if kratos_dependent_module.GetPythonModule() == module_name:
            return kratos_dependent_module
    return None

def MoveKratosModuleStubFilesToPythonModule(stub_file_path: Path, list_of_python_cpp_libs: 'list[KratosPythonCppLib]') -> None:
    for kratos_module in list_of_python_cpp_libs:
        stub_source_file_name = stub_file_path / f"{kratos_module.GetCppLibModule()}.pyi"

        if not Path(stub_source_file_name).is_file():
            # it is not a single stub file, it has multiple nested packages.
            # hence it is a directory
            stub_source_file_name = stub_source_file_name.parent / stub_source_file_name.name[:-4]
            stub_dest_file_name = kratos_module.GetPythonPath().parent
            shutil.copytree(stub_source_file_name, stub_dest_file_name, dirs_exist_ok=True)
            shutil.rmtree(stub_source_file_name)
        else:
            # it is a single stub file.
            if kratos_module.GetPythonPath().name == "__init__.py":
                stub_dest_file_name = kratos_module.GetPythonPath().parent / "__init__.pyi"
            else:
                stub_dest_file_name = kratos_module.GetPythonPath().parent / f"{kratos_module.GetPythonPath().name[:-2]}.pyi"
            shutil.move(str(stub_source_file_name), str(stub_dest_file_name))

def SetPythonModulesForCppModules(list_of_python_cpp_libs: 'list[KratosPythonCppLib]', kratos_python_module_path: Path) -> None:
    kratos_module_dependent_module_names_dict: 'dict[KratosPythonCppLib, list[str]]' = {}
    for file_path in kratos_python_module_path.rglob("*.py"):
        with open(file_path, "r") as file_input:
            lines = file_input.read()

            for kratos_python_cpp_lib in list_of_python_cpp_libs:
                if not kratos_python_cpp_lib.IsPythonModuleDefined():
                    kratos_module_dependent_module_names_dict[kratos_python_cpp_lib] = []
                    for match_cpp_lib_import in re.finditer(f"from +{kratos_python_cpp_lib.GetCppLibModule()} +import +\\*", lines):
                        kratos_python_cpp_lib.SetPythonModule(file_path)
                        for match_dependent_imports in re.finditer(f"import +([a-zA-Z0-9_.]+)", lines[:match_cpp_lib_import.start()]):
                            kratos_module_dependent_module_names_dict[kratos_python_cpp_lib].append(match_dependent_imports.group(1))
                        break

        if all([v.IsPythonModuleDefined() for v in list_of_python_cpp_libs]):
            break

    # there may be python libraries which are not directly imported by any application.
    # hence removing them from the stub generation
    i = len(list_of_python_cpp_libs) - 1
    while (i >= 0):
        if not list_of_python_cpp_libs[i].IsPythonModuleDefined():
            del list_of_python_cpp_libs[i]
        i -= 1

    # first add dependencies based on the python module name
    for kratos_module in list_of_python_cpp_libs:
        sub_names = kratos_module.GetPythonModule().split(".")
        sub_module_names: 'list[str]' = []
        for i, _ in enumerate(sub_names[:-1]):
            sub_module_names.append(".".join(sub_names[:i+1]))
        for sub_module_name in sub_module_names:
            kratos_module.AddDependentModule(FindModule(sub_module_name, list_of_python_cpp_libs))

    # now add specific dependencies from the python scripts.
    for kratos_module, dependent_module_names_list in kratos_module_dependent_module_names_dict.items():
        for dependent_module_name in dependent_module_names_list:
            kratos_module.AddDependentModule(FindModule(dependent_module_name, list_of_python_cpp_libs))

def SortModules(list_of_python_cpp_libs: 'list[KratosPythonCppLib]') -> 'list[KratosPythonCppLib]':
    iteration = 0
    all_modules_ordered = False
    while iteration < 100 and not all_modules_ordered:
        iteration += 1
        all_modules_ordered = True
        for i, kratos_module in enumerate(list_of_python_cpp_libs):
            for dependent_module in kratos_module.GetDependentModulesList():
                temp = FindModule(dependent_module.GetPythonModule(), list_of_python_cpp_libs[:i])
                if temp is None:
                    del list_of_python_cpp_libs[list_of_python_cpp_libs.index(dependent_module)]
                    list_of_python_cpp_libs.insert(i, dependent_module)
                    all_modules_ordered = False
                    break

            if not all_modules_ordered:
                break

    if iteration == 100:
        raise RuntimeError("Cyclic dependency detected.")

def Main():
    print("--- Generating python stub files from {:s}".format(sys.argv[1]))
    kratos_installation_path = (Path(sys.argv[1])).absolute()
    kratos_library_path = (kratos_installation_path / "libs").absolute()
    kratos_python_module_path = kratos_installation_path / "KratosMultiphysics"

    sys.path.insert(0, str(kratos_installation_path.absolute()))
    sys.path.insert(0, str(kratos_library_path.absolute()))

    # get list of cpp libs
    list_of_cpp_libs = GetCppLibs(kratos_library_path)

    # set python modules corresponding to cpp libs
    SetPythonModulesForCppModules(list_of_cpp_libs, kratos_python_module_path)

    # now sort
    SortModules(list_of_cpp_libs)

    for kratos_module in list_of_cpp_libs:
        print("Module information: ")
        print(f"           Python module name: {kratos_module.GetPythonModule()}")
        print(f"             Cpp  module name: {kratos_module.GetCppLibModule()}")
        print( "         List of dependencies:")
        for dependency in kratos_module.GetDependentModulesList():
            print(f"               {dependency.GetPythonModule()}")

    # now generate the stubs
    args: 'list[str]' = ["-o", str(kratos_library_path)]
    for k in list_of_cpp_libs:
        args.extend(["-p", k.GetCppLibModule()])
    options = parse_options(args)
    generate_stubs(options)

    MoveKratosModuleStubFilesToPythonModule(kratos_library_path, list_of_cpp_libs)

    ## special edit for the KratosMultiphysics pyi
    with open(Path(__file__).parent / "kratos__init__.pyi", "r") as file_input:
        data = file_input.read()
    with open(kratos_python_module_path / "__init__.pyi", "a") as file_output:
        file_output.write("\n\n# ==== Start of predefined stubs === #\n\n")
        file_output.write(data)
        file_output.write("\n\n# ==== End of predefined stubs === #\n\n")

if __name__ == "__main__":
    error_and_warning_outut_file = f"{sys.argv[1]}/stub_generation_errors_and_warnings.txt"
    if "--quiet" in sys.argv: # suppress output from Kratos imports
        args = [arg for arg in sys.argv if arg != "--quiet"]
        subprocess.run([sys.executable] + args, stdout = subprocess.PIPE, stderr = sys.stderr, check=True)
    else:
        Main()
        PostProcessGeneratedStubFiles()
