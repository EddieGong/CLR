export module CLR.Math.Vector;

import <array>;

import <BasicTypes.h>;

export namespace CLR::Math
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
            this->mV = { v0, args... };
        }

        T Dot(Vector<T, N> const& v)
        {
            T result = static_cast<T>(0);
            for (size_t i = 0; i < N; ++i)
            {
                result += mV[i] * v.mV[i];
            }
            return result;
        }

        inline T operator[](const uint32 index) const { return mV.at(index); }

    private:
        std::array<T, N> mV;
    };

    using Vector2f = Vector<float, 2>;
    using Vector3f = Vector<float, 3>;
    using Vector4f = Vector<float, 4>;
}