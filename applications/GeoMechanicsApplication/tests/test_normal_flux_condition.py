import sys
import os

from KratosMultiphysics import Tester
import KratosMultiphysics.KratosUnittest as KratosUnittest
import KratosMultiphysics.GeoMechanicsApplication

class TestNormalFluxCondition(KratosUnittest.TestCase):

    # Tester.RunAllTestCases() #Test all cases
    def setUp(self):
        Tester.SetVerbosity(Tester.Verbosity.TESTS_OUTPUTS) # Set the verbosity level
        pass

    def tearDown(self):
        # Code here will be placed AFTER every test in this TestCase.
        pass

    def test_horizontal_normal_flux(self):
        exitcode = Tester.RunTestCases("TestCalculateHorizontalNormalFlux")
        self.assertTrue(exitcode == 0)

    def test_inclined_normal_flux(self):
        exitcode = Tester.RunTestCases("TestCalculateInclinedNormalFlux")
        self.assertTrue(exitcode == 0)

if __name__ == '__main__':
    # Tester.RunTestSuite("KratosGeoMechanicsFastSuite")
    # Tester.RunAllTestCases()
    KratosUnittest.main()