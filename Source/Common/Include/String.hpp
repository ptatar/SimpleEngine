#include "Types.hpp"

namespace engine
{
    class StringImpl
    {
        public:
            StringImpl();

            StringImpl(const char* cstr);

            StringImpl(const char* cstr, Uint32 size);

            StringImpl(const StringImpl& str);

            StringImpl(StringImpl&& str);

            const char* GetCStr() const { return m_buffer; };

            Uint32 GetSize() const { return m_size; }

            Uint32 GetCapacity() const { return m_capacity; }

            Int64 ToInt64(Uint32 startPos, Uint32& endPos);

            StringImpl Strip() const;

            StringImpl operator+(const StringImpl& str) const;

        private:

            char* m_buffer;
            Uint32 m_size;
            Uint32 m_capacity;

    }; // StringImpl

typedef StringImpl String;

} // namespace engine
