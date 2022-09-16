import KratosMultiphysics as KM
import KratosMultiphysics.KratosUnittest as KratosUnittest
import KratosMultiphysics.kratos_utilities as kratos_utils

from KratosMultiphysics.testing import utilities as testing_utils

import sys, os
import argparse
import subprocess

from importlib import import_module

class Commander(object):
    def __init__(self):
        self.process = None
        self.exitCode = 0

    def RunTestSuit(self, application, applicationPath, path, level, verbose, command, timer):
        ''' Calls the script that will run the tests.

        Input
        -----
        application: string
            Name of the application that will be tested.

        path: string
            Absoulte path with the location of the application.

        level: string
            minimum level of the test that will be run if possible.

        verbose: int
            detail of the ouptut. The grater the verbosity level, the greate the
            detail will be.

        command: string
            command to be used to call the tests. Ex: Python, Python3, Runkratos

        timer: integer
            limit time considered to execute the tests

        '''

        self.exitCode = 0
        appNormalizedPath = applicationPath.lower().replace('_', '')

        possiblePaths = [
            {'Found': p, 'FoundNormalized': p.split('/')[-1].lower().replace('_', ''), 'Expected': applicationPath, 'ExpectedNormalized': appNormalizedPath} for p in os.listdir(path) if p.split('/')[-1].lower().replace('_', '') == appNormalizedPath
        ]

        if len(possiblePaths) < 1:
            if verbose > 0:
                print(
                    '[Warning]: No directory found for {}'.format(
                        application),
                    file=sys.stderr)
                sys.stderr.flush()
        elif len(possiblePaths) > 1:
            if verbose > 0:
                print('Unable to determine correct path for {}'.format(application), file=sys.stderr)
                print(
                    'Please try to follow the standard naming convention \'FooApplication\' Snake-Capital string  without symbols.',
                    file=sys.stderr)
            if verbose > 1:
                print('Several possible options were found:', file=sys.stderr)
                for p in possiblePaths:
                    print('\t', p, file=sys.stderr)
        else:
            script = path+'/'+possiblePaths[0]['Found']+'/tests/'+'test_'+application+'.py'
            print(script, file=sys.stderr)

            if possiblePaths[0]['Found'] != possiblePaths[0]['Expected']:
                print(
                    '[Warning]: Application has been found in "{}" directory but it was expected in "{}". Please check the naming convention.'.format(
                        possiblePaths[0]['Found'],
                        possiblePaths[0]['Expected']),
                    file=sys.stderr)

            if os.path.isfile(script):
                try:
                    self.process = subprocess.Popen([
                        command,
                        script,
                        '-l'+level,
                        '-v'+str(verbose)
                    ], stdout=subprocess.PIPE, cwd=os.path.dirname(os.path.abspath(script)))
                except OSError:
                    # Command does not exist
                    print('[Error]: Unable to execute {}'.format(command), file=sys.stderr)
                    self.exitCode = 1
                except ValueError:
                    # Command does exist, but the arguments are invalid (It sohuld never enter here. Just to be safe)
                    print('[Error]: Invalid arguments when calling {} {} {} {}'.format(command, script, '-l'+level, '-v'+str(verbose)), file=sys.stderr)
                    self.exitCode = 1
                else:
                    # Used instead of wait to "soft-block" the process and prevent deadlocks
                    # and capture the first exit code different from OK
                    try:
                        process_stdout, process_stderr = self.process.communicate(timeout=timer)
                    except subprocess.TimeoutExpired:
                        # Timeout reached
                        self.process.kill()
                        print('[Error]: Tests for {} took too long. Process Killed.'.format(application), file=sys.stderr)
                        self.exitCode = 1
                    else:
                        if process_stdout:
                            print(process_stdout.decode('ascii'), file=sys.stdout)
                        if process_stderr:
                            print(process_stderr.decode('ascii'), file=sys.stderr)

                    # Running out of time in the tests will send the error code -15. We may want to skip
                    # that one in a future. Right now will throw everything different from 0.
                    self.exitCode = int(self.process.returncode != 0)
            else:
                if verbose > 0:
                    print(
                        '[Warning]: No test script found for {}'.format(
                            application),
                        file=sys.stderr)
                    sys.stderr.flush()
                if verbose > 1:
                    print(
                        '  expected file: "{}"'.format(
                            script),
                        file=sys.stderr)
                    sys.stderr.flush()

    def RunCppTests(self, applications):
        ''' Calls the cpp tests directly
        '''

        self.exitCode = 0

        # importing the apps such that they get registered for the cpp-tests
        for application in applications:
            import_module("KratosMultiphysics." + application)

        try:
            KM.Tester.SetVerbosity(KM.Tester.Verbosity.PROGRESS)
            self.exitCode = KM.Tester.RunAllTestCases()
        except Exception as e:
            print('[Warning]:', e, file=sys.stderr)
            self.exitCode = 1


def print_test_header(application):
    print("\nRunning {} tests".format(application), file=sys.stderr, flush=True)

def print_test_footer(application, exit_code):
    appendix = " with exit code {}!".format(exit_code) if exit_code != 0 else "."
    print("Completed {} tests{}\n".format(application, appendix), file=sys.stderr, flush=True)

def print_summary(exit_codes):
    print("Test results summary:", file=sys.stderr)
    max_test_name_length = len(max(exit_codes.keys(), key=len))
    for test, exit_code in exit_codes.items():
        result_string = "OK" if exit_code == 0 else "FAILED"
        pretty_name = test.ljust(max_test_name_length)
        print("  {}: {}".format(pretty_name, result_string), file=sys.stderr)
    sys.stderr.flush()

def main():
    # Define the command
    cmd = testing_utils.GetPython3Command()
    
    # List of application 
    applications = kratos_utils.GetListOfAvailableApplications()

    # Keep the worst exit code
    exit_code = 0

    # Parse Commandline
    # parse command line options
    parser = argparse.ArgumentParser()

    parser.add_argument('-c', '--command', default=cmd, help="Use the provided command to launch test cases. If not provided, the default \'runkratos\' executable is used")
    parser.add_argument('-l', '--level', default='all', choices=['all', 'nightly', 'small', 'validation'], help="Minimum level of detail of the tests: \'all\'(Default) \'(nightly)\' \'(small)\'")
    parser.add_argument('-v', '--verbosity', default=1, type=int, choices=[0, 1, 2], help="Verbosity level: 0, 1 (Default), 2")
    parser.add_argument('-a', '--applications', default=applications, choices=applications, help="List of applications to run separated by \':\'. All compiled applications will be run by default")
    parser.add_argument('-m', '--using-mpi', default=False, help="If running in MPI and executing the MPI-tests")
    parser.add_argument('-t', '--timer', default=-1, help="Use the provided custom time limit for the execution. If not provided, the default values are used")

    args = parser.parse_args()

    # Set verbosity
    verbosity = args.verbosity

    # Set level
    level = args.level

    # Set if mpi
    if args.using_mpi:
        level = "mpi_" + level

    # Parser the applications
    if args.applications is str:
        parsedApps = args.applications.split(':')
    else:
        parsedApps = args.applications
    for a in parsedApps:
        if a not in applications + ['KratosCore']:
            print('Warning: Application {} does not exist'.format(a))
            sys.exit()
    applications = parsedApps
    if 'KratosCore' in applications:
        applications.remove('KratosCore')

    # Set timeout of the different levels
    signalTime = None
    if int(args.timer) > 0:
        signalTime = int(args.timer)
    else:
        if args.level == 'small':
            signalTime = 90
        elif args.level == 'nightly':
            signalTime = 900

    # Create the commands
    commander = Commander()

    exit_codes = {}

    # KratosCore must always be runned
    print_test_header("KratosCore")

    with KratosUnittest.SupressConsoleOutput():
        commander.RunTestSuit(
            'KratosCore',
            'kratos',
            os.path.dirname(kratos_utils.GetKratosMultiphysicsPath()),
            level,
            verbosity,
            cmd,
            signalTime
        )

    print_test_footer("KratosCore", commander.exitCode)
    exit_codes["KratosCore"] = commander.exitCode

    # Run the tests for the rest of the Applications
    for application in applications:
        print_test_header(application)

        with KratosUnittest.SupressConsoleOutput():
            commander.RunTestSuit(
                application,
                application,
                KM.KratosPaths.kratos_applications+'/',
                level,
                verbosity,
                cmd,
                signalTime
            )

        print_test_footer(application, commander.exitCode)
        exit_codes[application] = commander.exitCode

    # Run the cpp tests (does the same as run_cpp_tests.py)
    print_test_header("cpp")
    with KratosUnittest.SupressConsoleOutput():
        commander.RunCppTests(applications)
    print_test_footer("cpp", commander.exitCode)
    exit_codes["cpp"] = commander.exitCode

    print_summary(exit_codes)
    sys.exit(max(exit_codes.values()))

if __name__ == "__main__":
    main()
