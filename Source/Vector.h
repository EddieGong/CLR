#pragma once

#include <array>
#include <BasicTypes.h>

namespace CLR::Math
{
    template<typename T, size_t N>
    class Vector
    {
        static_assert(N > 1 && N < 5);

    public:
        Vector() = default;

        //template<typename... Ts>
        //Vector(T v0, Ts... args)
        //{
        //    T v[1 + sizeof...(args)] = { v0, args... };
        //    static_assert(std::size(v) == N, "The number of elements in the list must match the N");

        //    for (uint32 i = 0; i < N; ++i)
        //    {
        //        this->v[i] = v[i];
        //    }
        //}

        template<typename... Ts>
        Vector(T v0, Ts... args)
        {
            this->v = {v0, args...};
        }

        inline T operator[](const uint32 index) const { return v.at(index); }

    private:
        std::array<T, N> v;
    };

    using Vector2f = Vector<float, 2>;
    using Vector3f = Vector<float, 3>;
    using Vector4f = Vector<float, 4>;
}