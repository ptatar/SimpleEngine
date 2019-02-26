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

            Array(std::initializer_list<T> initList)
            {
                Uint32 i = 0;
                for(auto it = initList.begin(); it < initList.end(); it++)
                {
                    m_data[i++] = *it;
                }
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

            inline const T& operator[](Uint32 index) const
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

            inline T* Data()
            {
                return m_data;
            }

        private:
            Uint32 m_size;
            T* m_data;
    };

} // namespace engine
