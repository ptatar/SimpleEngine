#include "Types.hpp"

#include "yas/serialize.hpp"
#include "yas/std_types.hpp"

namespace engine
{

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

            template<typename T>
            friend SerializationBuffer Serialize(T& object);
            template<typename T>
            friend T Deserialize(const SerializationBuffer& buffer);

    };

    class SerializationFrame
    {
        public:
            SerializationFrame(const Uint8* data, Uint32 size)
                : m_buffer(reinterpret_cast<const char*>(data), size) {}

            ~SerializationFrame() {}

        private:
            yas::intrusive_buffer m_buffer;

            template<typename T>
            friend T Deserialize(const SerializationFrame& buffer);
    };

    template<typename T>
    SerializationBuffer Serialize(T& object)
    {
        constexpr std::size_t flags = yas::mem | yas::binary;
        SerializationBuffer buffer;
        buffer.m_buffer = yas::save<flags>(object);
        return buffer;
    }

    template<typename T>
    T Deserialize(const SerializationBuffer& buffer)
    {
        constexpr std::size_t flags = yas::mem | yas::binary;
        T object;
        yas::load<flags>(buffer.m_buffer, &object);
        return object;
    }

    template<typename T>
    T Deserialize(const SerializationFrame& frame)
    {
        constexpr std::size_t flags = yas::mem | yas::binary;
        T object;
        yas::load<flags>(frame.m_buffer, object);
        return object;
    }

}
