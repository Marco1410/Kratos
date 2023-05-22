//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Suneth Warnakulasuriya
//

#pragma once

// System includes
#include <atomic>
#include <ostream>
#include <string>
#include <vector>

// Project includes
#include "includes/define.h"

namespace Kratos {

///@name Kratos Classes
///@{

/**
 * @brief Base class or all the expression types.
 *
 * This is used to represent an expression for arithmetic evaluation
 *
 */
class KRATOS_API(KRATOS_CORE) Expression {
public:
    ///@name Type definitions
    ///@{

    using Pointer = Kratos::intrusive_ptr<const Expression>;

    using IndexType = std::size_t;

    ///@}
    ///@name Public classes
    ///@{

    class ExpressionIterator {
    public:
        ///@name Type definitions
        ///@{

        KRATOS_CLASS_POINTER_DEFINITION(ExpressionIterator);

        ///@}
        ///@name Life cycle
        ///@{

        ExpressionIterator(Expression::Pointer pExpression);

        /**
         * @brief Copy constructor
         *
         * @param rOther
         */
        ExpressionIterator(const ExpressionIterator& rOther);

        ///@}
        ///@name Public operations
        ///@{

        Expression::Pointer GetExpression() const;

        ///@}
        ///@name Public operators
        ///@{

        double operator*() const;

        bool operator==(const ExpressionIterator& rOther) const;

        bool operator!=(const ExpressionIterator& rOther) const;

        ExpressionIterator& operator=(const ExpressionIterator& rOther);

        ExpressionIterator& operator++();

        ExpressionIterator operator++(int);

        ///@}

    private:
        ///@name Private member variables
        ///@{

        Expression::Pointer mpExpression;

        IndexType mEntityIndex;

        IndexType mEntityDataBeginIndex;

        IndexType mComponentIndex;

        IndexType mFlattenedShapeSize;

        ///@}
        ///@name Friend classes
        ///@{

        friend class Expression;

        ///@}
    };

    ///@}
    ///@name Life cycle
    ///@{

    Expression(const IndexType NumberOfEntities) : mNumberOfEntities(NumberOfEntities) {}

    virtual ~Expression() = default;

    ///@}
    ///@name Public operations
    ///@{

    /**
     * @brief Evalute the expression for the given entity data start index and component index and returns the value
     *
     * @param EntityIndex           Index of the entity.
     * @param EntityDataBeginIndex  Index at which entity data starts.
     * @param ComponentIndex        Component index.
     * @return double               Evaluated expression.
     */
    virtual double Evaluate(
        const IndexType EntityIndex,
        const IndexType EntityDataBeginIndex,
        const IndexType ComponentIndex) const = 0;

    /**
     * @brief Get the Shape of the expression
     *
     * @return const std::vector<IndexType>     Size of each dimension is in the vector elements.
     */
    virtual const std::vector<IndexType> GetShape() const = 0;

    /**
     * @brief Get the maximum number of entities allowed for this expression.
     *
     * @return IndexType
     */
    inline IndexType NumberOfEntities() const { return mNumberOfEntities; };

    /**
     * @brief Get the Local Size of the expression
     *
     * @return IndexType
     */
    IndexType GetFlattenedShapeSize() const;

    ///@}
    ///@name Input and output
    ///@{

    ExpressionIterator cbegin() const;

    ExpressionIterator cend() const;

    virtual std::string Info() const = 0;

    ///@}

private:
    ///@name Private member variables
    ///@{

    const IndexType mNumberOfEntities;

    ///@}
    ///@name Private operations
    ///@{

    //*********************************************
    // this block is needed for refcounting in the @ref intrusive ptr
    mutable std::atomic<int> mReferenceCounter{0};

    friend void intrusive_ptr_add_ref(const Expression* x)
    {
        x->mReferenceCounter.fetch_add(1, std::memory_order_relaxed);
    }

    friend void intrusive_ptr_release(const Expression* x)
    {
        if (x->mReferenceCounter.fetch_sub(1, std::memory_order_release) == 1) {
            std::atomic_thread_fence(std::memory_order_acquire);
            delete x;
        }
    }

    //*********************************************

    ///@}
};

/// output stream functions
inline std::ostream& operator<<(
    std::ostream& rOStream,
    const Expression& rThis)
{
    return rOStream << rThis.Info();
}

} // namespace Kratos