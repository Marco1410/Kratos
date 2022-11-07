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
    RegistryItem(const std::string& rName)
        : mName(rName), mpValue(Kratos::make_shared<SubRegistryItemType>()) {}

    /// Constructor with the name and value
    RegistryItem(
        const std::string&  rName,
        std::any pValue)
        : mName(rName), mpValue(pValue) {}

    // Copy constructor deleted
    RegistryItem(RegistryItem const& rOther) = delete;

    /// Destructor.
    virtual ~RegistryItem() = default;

    /// Assignment operator deleted.
    RegistryItem& operator=(RegistryItem& rOther) = delete;

    ///@}
    ///@name Items
    ///@{

    // Adding sub value item
    template<typename TItemType, class... TArgumentsList>
    RegistryItem& AddItem(
        std::string const& ItemName,
        TArgumentsList&&... Arguments)
    {
        KRATOS_ERROR_IF(this->HasItem(ItemName))
            << "The RegistryItem '" << this->Name() << "' already has an item with name "
            << ItemName << "." << std::endl;

        using ValueType = typename std::conditional<std::is_same<TItemType, RegistryItem>::value, SubRegistryItemFunctor, SubValueItemFunctor<TItemType>>::type;

        auto insert_result = GetSubRegistryItemMap().emplace(
            std::make_pair(
                ItemName,
                ValueType::Create(ItemName, std::forward<TArgumentsList>(Arguments)...)
                )
            );

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

    template<typename TDataType> TDataType const& GetValue() const
    {
        KRATOS_TRY

        return *(std::any_cast<std::shared_ptr<TDataType>>(mpValue));

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
    virtual std::string Info() const;

    /// Print information about this object.
    virtual void PrintInfo(std::ostream& rOStream) const;

    /// Print object's data.
    virtual void PrintData(std::ostream& rOStream) const;

    virtual std::string ToJson(std::string const& rTabSpacing = "", const std::size_t Level = 0) const;

    virtual std::string GetValueString() const;

    ///@}
private:
    ///@name Private Member Variables
    ///@{

    std::string mName;
    std::any mpValue;

    ///@}
    ///@name Private classes
    ///@{

    // This class is only a helper class to have the Json output
    // with proper strings for each registry item. Therefore,
    // this is hidden and not accessible from outside
    // RegistryItem class
    template<typename TItemType>
    class RegistryValueItem;

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
            return Kratos::make_shared<RegistryValueItem<TItemType>>(ItemName, Kratos::make_shared<TItemType>(std::forward<TArgumentsList>(Arguments)...));
        }

    };

    ///@}
    ///@name Un accessible methods
    ///@{

    SubRegistryItemType& GetSubRegistryItemMap();

    SubRegistryItemType& GetSubRegistryItemMap() const;

    ///@}

}; // Class RegistryItem

// This class is only a helper class to have the Json output
// with proper strings for each registry item. Therefore,
// this is hidden and not accessible from outside
// RegistryItem class
template<typename TItemType>
class RegistryItem::RegistryValueItem : public RegistryItem
{
public:
    ///@name Type Definitions
    ///@{

    /// Pointer definition of RegistryValueItem
    KRATOS_CLASS_POINTER_DEFINITION(RegistryValueItem);

    ///@}
    ///@name Life Cycle
    ///@{

    RegistryValueItem(
        std::string const& rName,
        std::any pValue)
        : RegistryItem(rName, pValue) {}

    ///@}
    ///@name Input and output
    ///@{

    // Overrides to get the value string
    // for JSON output.
    std::string GetValueString() const override
    {
        std::stringstream buffer;
        buffer << this->GetValue<TItemType>();
        return buffer.str();
    }

    ///@}

};

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
