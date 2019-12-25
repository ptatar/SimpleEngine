#include "yas/serialize.hpp"

#include "Types.hpp"

namespace engine
{

    class ISerializable;

    class SerializationBuffer
    {
        public:
            SerializationBuffer() {}
            virtual ~SerializationBuffer() {}

            Uint32 GetSize() const
            {
                return m_buffer.size;
            }

            const Uint8* GetData() const
            {
                return reinterpret_cast<Uint8*>(m_buffer.data.get());
            }

            Uint8* GetData()
            {
                return reinterpret_cast<Uint8*>(m_buffer.data.get());
            }

        private:
            yas::shared_buffer m_buffer;

            friend ISerializable;
    };

    class ISerializable
    {
        public:
            virtual ~ISerializable() {}

            virtual SerializationBuffer Serialize() const
            {
                SerializationBuffer buffer;
                buffer.m_buffer = yas::save<s_flags>(*this);
                return buffer;
            }

            virtual void Deserialize(const SerializationBuffer& buffer)
            {
                yas::load<s_flags>(buffer.m_buffer, *this);
            }

        protected:
            template<typename Ar>
            void serialize(Ar &ar)
            {
            }

            const static std::size_t s_flags = yas::mem | yas::binary;
    };
}
