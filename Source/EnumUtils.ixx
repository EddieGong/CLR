module; 

#include <type_traits>

export module CLR.EnumUtils;

namespace CLR
{
    // TODO: specify begin/end enum
    export template<typename TemplateType>
    class EnumIterator
    {
        using ValueType = typename std::underlying_type_t<TemplateType>;

    public:
        class Iterator
        {
            TemplateType   mValue;

        public:
            Iterator(ValueType value) : mValue{TemplateType(value)}
            {}

            const ValueType operator*() const       { return static_cast<ValueType>(mValue); }
            void operator++()                       { mValue = static_cast<TemplateType>(static_cast<ValueType>(mValue) + 1); }
            bool operator!=(Iterator right) const   { return static_cast<ValueType>(mValue) != static_cast<ValueType>(right.mValue); }
        };

        constexpr Iterator begin()     { return Iterator(0); }
        constexpr Iterator end()       { return Iterator(ValueType(TemplateType::Count)); }
    };

    export template<typename EnumType, typename MapType>
    MapType MapEnum(EnumType enumValue, std::initializer_list<MapType>&& names)
    {
        // TODO: size check, assert, etc...
        return names.begin()[static_cast<size_t>(enumValue)];
    }
}