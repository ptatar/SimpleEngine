#include "Types.hpp"

namespace engine
{
    template<typename T>
    class DynamicArray
    {
        public:
            explicit DynamicArray(): m_data(nullptr), m_size(0), m_capacity(0) {};

            explicit DynamicArray(std::initializer_list<T> initList)
            {
                Alloc(initList.size());
                Uint32 i = 0;
                for (auto it = initList.begin(); it < initList.end(); it++)
                {
                    m_data[i++] = *it;
                }
            }

            ~DynamicArray()
            {
                Clear();
            }

            void Clear()
            {
                if (m_data)
                {
                    delete[] m_data;
                    m_data = nullptr;
                    m_size = 0;
                    m_capacity = 0;
                }
            }

            inline const T* GetData() const
            {
                return m_data;
            }

            inline T* GetData()
            {
                return m_data;
            }

            inline Uint32 Size() const
            {
                return m_size;
            }

            template<typename... Args>
            void Emplace(Uint32 index, Args&&... args)
            {
                ASSERT(index < m_size);
                T* addr = m_data + index;
                addr->~T();
                new(addr) T(std::forward<Args>(args)...);
            }

            template<typename... Args>
            void EmplaceBack(Args&&... args)
            {
                if (m_size < m_capacity)
                {
                    T* addr = m_data + m_size;
                }
                else
                {
                    Realloc();
                }
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

        private:
            void Alloc(Uint32 size)
            {

                //malloc
                m_data = new T[size];
                m_size = size;
                m_capacity = size;
            }

            void Realloc(Uint32 size)
            {
                realloc(m_data, size);
            }
        private:
            T* m_data;
            Uint32 m_size;
            Uint32 m_capacity;
    };

} // namespace common
