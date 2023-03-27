//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Pooyan Dadvand
//                   Ruben Zorrilla
//                   Carlos Roig
//

#pragma once

// System includes
#include <string>
#include <iostream>
#include <unordered_map>
#include <any>

// External includes


// Project includes
#include "includes/define.h"

namespace Kratos
{
///@addtogroup KratosCore
///@{
///@name Kratos Classes
///@{

/// The registry item to be stored by Registry class. It is the base class for some more specific ones.
/** RegistryItem has a tree node structure and stores its name, an optional
 *  value, and an unorder_set of its sub data.
 *  This structure let us to have registry of the elements and then different
 *  registries for each elements inside it.
 *  Please note that RegistryItem stores a shared pointer to the value.
*/
class KRATOS_API(KRATOS_CORE) RegistryItem
{
public:
    ///@name Type Definitions
    ///@{

    /// Pointer definition of RegistryItem
    KRATOS_CLASS_POINTER_DEFINITION(RegistryItem);

    /// Subregistry item type definition
    using SubRegistryItemType = std::unordered_map<std::string, Kratos::shared_ptr<RegistryItem>>;

    /// Pointer definition of SubRegistryItemType
    using SubRegistryItemPointerType = Kratos::shared_ptr<SubRegistryItemType>;

    /// Custom iterator with key as return type to be used in the Python export
    class KeyReturnConstIterator
    {
    public:
        ///@name Type Definitions
        ///@{

        using BaseIterator      = SubRegistryItemType::const_iterator;
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = BaseIterator::difference_type;
        using value_type        = SubRegistryItemType::key_type;
        using const_pointer     = const value_type*;
        using const_reference   = const value_type&;

        ///@}
        ///@name Life Cycle
        ///@{

        KeyReturnConstIterator()
        {}

        KeyReturnConstIterator(const BaseIterator Iterator)
            : mIterator(Iterator)
        {}

        KeyReturnConstIterator(const KeyReturnConstIterator& rIterator)
            : mIterator(rIterator.mIterator)
        {}

        ///@}
        ///@name Operators
        ///@{

        KeyReturnConstIterator& operator=(const KeyReturnConstIterator& rIterator)
        {
            this->mIterator = rIterator.mIterator;
            return *this;
        }

        const_reference operator*() const
        {
            return mIterator->first;
        }

        const_pointer operator->() const
        {
            return &(mIterator->first);
        }

        KeyReturnConstIterator& operator++()
        {
            ++mIterator;
            return *this;
        }

        KeyReturnConstIterator operator++(int)
        {
            KeyReturnConstIterator tmp(*this);
            ++(*this);
            return tmp;
        }

        bool operator==(const KeyReturnConstIterator& rIterator) const
        {
            return this->mIterator == rIterator.mIterator;
        }

        bool operator!=(const KeyReturnConstIterator& rIterator) const
        {
            return this->mIterator != rIterator.mIterator;
        }

        ///@}
    private:
        ///@name Member Variables
        ///@{

        BaseIterator mIterator;

        ///@}
    };

    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructor deleted.
    RegistryItem() = delete;

    /// Constructor with the name
    RegistryItem(const std::string &rName)
        : mName(rName),
          mpCallable(Kratos::make_shared<SubRegistryItemType>()),
          mValueName(GetRegistryItemType()) {}

    /// Constructor with the name and lambda
    template <typename TItemType, typename... TArgs>
    RegistryItem(
        const std::string &rName,
        const std::function<TItemType(TArgs...)> &rValue)
        : mName(rName),
          mpCallable(rValue),
          mValueName(typeid(TItemType).name()) {}

    /// Constructor with the name and value
    template<class TItemType>
    RegistryItem(
        const std::string&  rName,
        const TItemType& rValue)
        : mName(rName),
          mpCallable((std::function<std::shared_ptr<TItemType>()>)[rValue](){return Kratos::make_shared<TItemType>(rValue);})
    {
        std::stringstream buffer;
        buffer << rValue;
        mValueName = buffer.str();
    }

    /// Constructor with the name and shared ptr
    template<class TItemType>
    RegistryItem(
        const std::string&  rName,
        const shared_ptr<TItemType>& pValue)
        : mName(rName),
          mpCallable((std::function<std::shared_ptr<TItemType>()>)[pValue](){return pValue;})
    {
        std::stringstream buffer;
        buffer << *pValue;
        mValueName = buffer.str();
    }

    // Copy constructor deleted
    RegistryItem(RegistryItem const& rOther) = delete;

    /// Destructor.
    ~RegistryItem() = default;

    /// Assignment operator deleted.
    RegistryItem& operator=(RegistryItem& rOther) = delete;

    ///@}
    ///@name Items
    ///@{

    // Adding sub value item
    template <typename TItemType, class... TArgumentsList>
    RegistryItem &AddItem(
        std::string const &ItemName,
        TArgumentsList &&...Arguments)
    {
        KRATOS_ERROR_IF(this->HasItem(ItemName))
            << "The RegistryItem '" << this->Name() << "' already has an item with name "
            << ItemName << "." << std::endl;

        if constexpr (std::is_same_v<TItemType, RegistryItem>) {
            return AddRegistryItem(ItemName, std::forward<TArgumentsList>(Arguments)...);
        } else {
            return AddRegistryValue<TItemType>(ItemName, std::forward<TArgumentsList>(Arguments)...);
        }
    }

    // Adding sub value item
    template <class... TArgumentsList>
    RegistryItem &AddRegistryItem(
        std::string const &ItemName,
        TArgumentsList &&...Arguments)
    {
        auto insert_result = GetSubRegistryItemMap().emplace(
            std::make_pair(
                ItemName,
                SubRegistryItemFunctor::Create(ItemName, std::forward<TArgumentsList>(Arguments)...)));

        KRATOS_ERROR_IF_NOT(insert_result.second)
            << "Error in inserting '" << ItemName
            << "' in registry item with name '" << this->Name() << "'." << std::endl;

        return *insert_result.first->second;
    }

    // Adding sub value callable item
    template <typename TItemType, class TFunctionReturnType, class... TFArgumentsList>
    RegistryItem &AddRegistryValue(
        std::string const &ItemName,
        std::function<TFunctionReturnType(TFArgumentsList...)> &&ItemFunction)
    {
        auto insert_result = GetSubRegistryItemMap().emplace(
            std::make_pair(
                ItemName,
                std::make_shared<RegistryItem>(ItemName, ItemFunction)
            )
        );

        KRATOS_ERROR_IF_NOT(insert_result.second)
            << "Error in inserting '" << ItemName
            << "' in registry item with name '" << this->Name() << "'." << std::endl;

        return *insert_result.first->second;
    }

    // Adding sub value callable item
    template <typename TItemType, class... TArgumentsList>
    RegistryItem &AddRegistryValue(
        std::string const &ItemName,
        TArgumentsList &&...Arguments)
    {
        auto insert_result = GetSubRegistryItemMap().emplace(
            std::make_pair(
                ItemName,
                std::make_shared<RegistryItem>(ItemName, std::function<std::shared_ptr<TItemType>()>([Arguments...]() -> std::shared_ptr<TItemType> {
                    return std::make_shared<TItemType>(Arguments...);
                }))
            )
        );

        // Force the evaluation of the lambda in order to set mpCallable and mpValueName
        (*insert_result.first->second).template EvaluateValue<TItemType>();

        KRATOS_ERROR_IF_NOT(insert_result.second)
            << "Error in inserting '" << ItemName
            << "' in registry item with name '" << this->Name() << "'." << std::endl;

        return *insert_result.first->second;
    }

    ///@}
    ///@name Access
    ///@{

    SubRegistryItemType::iterator begin();

    SubRegistryItemType::const_iterator cbegin() const;

    SubRegistryItemType::iterator end();

    SubRegistryItemType::const_iterator cend() const;

    KeyReturnConstIterator KeyConstBegin() const;

    KeyReturnConstIterator KeyConstEnd() const;

    const std::string& Name() const  { return mName; }

    RegistryItem const& GetItem(std::string const& rItemName) const;

    RegistryItem& GetItem(std::string const& rItemName);

    template <typename TDataType>
    TDataType const &GetValue()
    {
        KRATOS_TRY

        // This is executed the first time we access the GetValue for this item
        using TFunctionReturnType = std::shared_ptr<TDataType>;

        if (std::any_cast<TFunctionReturnType>(&mpValue) == nullptr) {
            this->EvaluateValue<TDataType>();
        }

        return *(std::any_cast<TFunctionReturnType>(mpValue));

        KRATOS_CATCH("");
    }

    void RemoveItem(std::string const& rItemName);

    ///@}
    ///@name Inquiry
    ///@{

    std::size_t size();

    bool HasValue() const;

    bool HasItems() const;

    bool HasItem(std::string const& rItemName) const;

    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.
    std::string Info() const;

    /// Print information about this object.
    void PrintInfo(std::ostream& rOStream) const;

    /// Print object's data.
    void PrintData(std::ostream& rOStream) const;

    std::string ToJson(std::string const& rTabSpacing = "", const std::size_t Level = 0) const;

    ///@}
private:
    ///@name Private Member Variables
    ///@{

    std::string mName;
    std::any mpCallable;
    std::any mpValue = nullptr;
    std::string mValueName;

    ///@}
    ///@name Private operations
    ///@{

    std::string GetRegistryItemType() const
    {
        return mpCallable.type().name();
    }

    template <typename TDataType>
    void EvaluateValue()
    {
        KRATOS_TRY

        using TFunctionReturnType = std::shared_ptr<TDataType>;
        using TFunctionType = std::function<TFunctionReturnType()>;

        // Assign callable value
        TFunctionType func = std::any_cast<TFunctionType>(mpCallable);
        mpValue = func();

        // Set value name
        std::stringstream buffer;
        buffer << *(std::any_cast<TFunctionReturnType>(mpValue));
        mValueName = buffer.str();

        KRATOS_CATCH("");
    }

    ///@}
    ///@name Private classes
    ///@{

    class SubRegistryItemFunctor
    {
    public:
        template<class... TArgumentsList>
        static inline RegistryItem::Pointer Create(
            std::string const& ItemName,
            TArgumentsList&&... Arguments)
        {
            return Kratos::make_shared<RegistryItem>(ItemName);
        }
    };

    template<typename TItemType>
    class SubValueItemFunctor
    {
    public:
        template<class... TArgumentsList>
        static inline RegistryItem::Pointer Create(
            std::string const& ItemName,
            TArgumentsList&&... Arguments)
        {
            return Kratos::make_shared<RegistryItem>(ItemName, Kratos::make_shared<TItemType>(std::forward<TArgumentsList>(Arguments)...));
        }

    };

    ///@}
    ///@name Un accessible methods
    ///@{

    std::string GetValueString() const;

    SubRegistryItemType& GetSubRegistryItemMap();

    SubRegistryItemType& GetSubRegistryItemMap() const;

    ///@}

}; // Class RegistryItem

///@}
///@name Input and output
///@{

/// input stream function
inline std::istream& operator >> (
    std::istream& rIStream,
    RegistryItem& rThis);

/// output stream function
inline std::ostream& operator << (
    std::ostream& rOStream,
    const RegistryItem& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
}
///@}

///@} addtogroup block

}  // namespace Kratos.
