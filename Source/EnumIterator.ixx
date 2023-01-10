export module CLR.EnumIterator;

import <type_traits>;

namespace CLR
{
    template<typename TemplateType>
    class EnumIterator
    {
        using ValueType = typename std::underlying_type_t<TemplateType>;

        class Iterator
        {
            TemplateType   mValue;

        public:
            Iterator(ValueType value) : mValue{TemplateType(value)}
            {
                
            }
        };

        static Iterator begin()     { return Iterator(0); }
        static Iterator end()       { return Iterator(ValueType(TemplateType::Count)); }
    };
}