#pragma once

#include "Types.hpp"
#include "Assert.hpp"

namespace engine
{
    template<typename T>
    class Array
    {
        public:
            Array(): m_size(0), m_data(nullptr) {}

            Array(Uint32 size): m_size(size)
            {
                m_data = new T[size];
            }

            ~Array()
            {
                delete[] m_data;
                m_data = nullptr;
            }

            inline T& operator[](Uint32 index)
            {
                ASSERT(index < m_size);
                return m_data[index];
            }

            template<typename... Args>
            void Emplace(Uint32 index, Args&&... args)
            {
                ASSERT(index < m_size);
                T* addr = m_data + index;
                addr->~T();
                new(addr) T(std::forward<Args>(args)...);
            }

            inline Uint32 Size() const
            {
                return m_size;
            }

        private:
            Uint32 m_size;
            T* m_data;
    };

} // namespace engine
