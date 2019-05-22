//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:        BSD License
//                  Kratos default license: kratos/license.txt
//
//  Main authors:    Marc Núñez,
//

#ifndef KRATOS_COMPUTE_EMBEDDED_LIFT_PROCESS_H
#define KRATOS_COMPUTE_EMBEDDED_LIFT_PROCESS_H

#include "includes/define.h"
#include "includes/model_part.h"
#include "processes/process.h"

namespace Kratos
{

class ComputeEmbeddedLiftProcess: public Process
{
public:
    ///@name Type Definitions
    ///@{

    /// Pointer definition of Process
    KRATOS_CLASS_POINTER_DEFINITION(ComputeEmbeddedLiftProcess);

    // Constructor for ComputeEmbeddedLiftProcess Process
    ComputeEmbeddedLiftProcess(ModelPart& rModelPart, Parameters ThisParameters);
    ComputeLiftLevelSetProcess(ModelPart& rModelPart, Vector& rResultForce):

    /// Destructor.
    ~ComputeEmbeddedLiftProcess() = default;

    /// Assignment operator.
    ComputeEmbeddedLiftProcess& operator=(ComputeEmbeddedLiftProcess const& rOther) = delete;

    /// Copy constructor.
    ComputeEmbeddedLiftProcess(ComputeEmbeddedLiftProcess const& rOther) = delete;

    /// This operator is provided to call the process as a function and simply calls the Execute method.
    void operator()()
    {
        Execute();
    }

    void Execute() override;

    /// Turn back information as a string.
    std::string Info() const override
    {
        return "ComputeEmbeddedLiftProcess";
    }

    /// Print information about this object.
    void PrintInfo(std::ostream& rOStream) const override
    {
        rOStream << "ComputeEmbeddedLiftProcess";
    }

    /// Print object's data.
    void PrintData(std::ostream& rOStream) const override
    {
        this->PrintInfo(rOStream);
    }

private:
    ///@name Static Member Variables
    ///@{


    ///@}
    ///@name Member Variables
    ///@{

    ModelPart& mrModelPart;
    Vector& mrResultForce;

}; // Class Process
} // namespace Kratos


#endif // KRATOS_MOVE_MODEL_PART_PROCESS_H
