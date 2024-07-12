//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Richard Faasse
//

#pragma once

#include "shallow_water_application.h"
#include "testing/testing.h"

namespace Kratos::Testing {

class KratosShallowWaterFastSuite : public KratosCoreFastSuite {
public:
  KratosShallowWaterFastSuite();

private:
  KratosShallowWaterApplication::Pointer mpShallowWaterApp;
  //  KratosLinearSolversApplication::Pointer mpLinearSolversApp;
};

} // namespace Kratos::Testing
