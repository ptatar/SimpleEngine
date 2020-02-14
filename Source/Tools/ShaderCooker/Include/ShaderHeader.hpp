#include "Types.hpp"

#include "Serializable.hpp"
#include "String.hpp"

#include <vector>

namespace engine
{
    class ShaderDataType
    {
        enum class Mask: Uint32
        {
            BaseType = 0xFFFFFF00,
            Col      = 0x000000F0,
            Row      = 0x0000000F,
        };

        enum class Shift: Uint32
        {
            Col      = 4,
            Row      = 0,
        };

        public:
            enum class BaseType : Uint32
            {
                Invalid = 0,
                Float = 0x100,
                Int = 0x200,
                Sampler = 0x1000,
                Image = 0x2000,
            };

            enum class ComplexType: Uint32
            {
                Invalid  = Uint32(BaseType::Invalid),
                Float    = Uint32(BaseType::Float) | 0x11,
                Float2   = Uint32(BaseType::Float) | 0x12,
                Float3   = Uint32(BaseType::Float) | 0x13,
                Float4   = Uint32(BaseType::Float) | 0x14,
                Float2x2 = Uint32(BaseType::Float) | 0x22,
                Float2x3 = Uint32(BaseType::Float) | 0x23,
                Float2x4 = Uint32(BaseType::Float) | 0x24,
                Float3x2 = Uint32(BaseType::Float) | 0x32,
                Float3x3 = Uint32(BaseType::Float) | 0x33,
                Float3x4 = Uint32(BaseType::Float) | 0x34,
                Float4x2 = Uint32(BaseType::Float) | 0x42,
                Float4x3 = Uint32(BaseType::Float) | 0x43,
                Float4x4 = Uint32(BaseType::Float) | 0x44,
                Int      = Uint32(BaseType::Int)   | 0x11,
                Sampler  = Uint32(BaseType::Sampler),
                Image    = Uint32(BaseType::Image),
            };

            ShaderDataType(): m_type(ComplexType::Invalid) {}

            ShaderDataType(const ShaderDataType& other): m_type(other.m_type) {}

            ShaderDataType(ShaderDataType&& other): m_type(other.m_type) {}

            ShaderDataType(ComplexType type): m_type(type) {}

            ~ShaderDataType() {}

            bool IsValid() const { return m_type != ComplexType::Invalid; }

            BaseType GetBaseType() const { return BaseType(Uint32(m_type) & Uint32(Mask::BaseType)); }

            Uint32 GetColNum() const { return (Uint32(m_type) & Uint32(Mask::Col)) >> Uint32(Shift::Col); }

            Uint32 GetRowNum() const { return (Uint32(m_type) & Uint32(Mask::Row)) >> Uint32(Shift::Row); }

            String ToString() const
            {
                BaseType baseType = GetBaseType();
                String str;
                switch(baseType)
                {
                    case BaseType::Float:
                    {
                        str = "Float";
                        Uint32 col = GetColNum();
                        Uint32 row = GetRowNum();
                        str = String::Format("Float%dx%d", col, row);
                        break;
                    }
                    case BaseType::Int:
                    {
                        str = "Int";
                        Uint32 col = GetColNum();
                        Uint32 row = GetRowNum();
                        str = String::Format("Int%dx%d", col, row);
                        break;
                    }
                    case BaseType::Image:
                        str = "Image";
                        break;
                    case BaseType::Sampler:
                        str = "Sampler";
                        break;
                    case BaseType::Invalid:
                    default:
                        str = "Invalid";
                        break;

                }

                return str;
            }

            static ShaderDataType Invalid()
            {
                return ShaderDataType(ComplexType::Invalid);
            }

            static ShaderDataType Build(BaseType baseType)
            {
                return ShaderDataType(ComplexType(Uint32(baseType)));
            }

            static ShaderDataType Build(BaseType baseType, Uint32 cols, Uint32 rows)
            {
                ASSERT(cols <= 4 && rows <= 4);
                return ShaderDataType(ComplexType(Uint32(baseType) | (cols << 4) | rows));
            }

            bool operator==(const ShaderDataType& other) const
            {
                return m_type == other.m_type;
            }

            bool operator!=(const ShaderDataType& other) const
            {
                return m_type != other.m_type;
            }

            template<typename S>
            void serialize(S& s)
            {
                s & YAS_OBJECT(
                    nullptr,
                    m_type
                );
            }

        private:
            ComplexType m_type;
    };

    class InterfaceSlot
    {
        public:
            InterfaceSlot()
                : m_name()
                , m_slot(0)
                , m_type(){}

            InterfaceSlot(const std::string& name, Uint32 slot, ShaderDataType& type)
                : m_name(name)
                , m_slot(slot)
                , m_type(type) {}

            std::string m_name;

            Uint32 m_slot;

            ShaderDataType m_type;

            template<typename S>
            void serialize(S& s)
            {
                s & YAS_OBJECT(
                    nullptr,
                    m_name,
                    m_slot,
                    m_type
                );
            }
    };

    class Constant
    {
        public:
            Constant()
                : m_name()
                , m_binding(0) {}

            Constant(const std::string& name, Uint32 binding)
                : m_name(name)
                , m_binding(0) {}

            class Member
            {
                public:
                    Member()
                        : m_name()
                        , m_binding(0)
                        , m_type()
                        , m_offset(0) {}

                    Member(const std::string& name, Uint32 binding, ShaderDataType& type, Uint32 offset)
                        : m_name(name)
                        , m_binding(binding)
                        , m_type(type)
                        , m_offset(offset) {}

                    template<typename S>
                    void serialize(S& s)
                    {
                        s & YAS_OBJECT(
                            nullptr,
                            m_name,
                            m_binding,
                            m_type,
                            m_offset
                        );
                    }

                public:
                    std::string m_name;

                    Uint32 m_binding;

                    ShaderDataType m_type;

                    Uint32 m_offset;
            };

            template<typename S>
            void serialize(S& s)
            {
                s & YAS_OBJECT(
                    nullptr,
                    m_name,
                    m_binding,
                    m_size,
                    m_members
                );
            }
        public:
            std::string m_name;

            Uint32 m_binding;

            Uint32 m_size;

            std::vector<Member> m_members;
    };

    class ShaderHeader
    {
        public:
            ShaderHeader() {}

            virtual ~ShaderHeader() {}

            std::vector<InterfaceSlot>& GetInput() { return m_input; }

            std::vector<InterfaceSlot>& GetOutput() { return m_output; }

            std::vector<Constant>& GetConstant() {return m_constants; }

            const std::vector<InterfaceSlot>& GetInput() const { return m_input; }

            const std::vector<InterfaceSlot>& GetOutput() const { return m_output; }

            const std::vector<Constant>& GetConstant() const {return m_constants; }

            template<typename S>
            void serialize(S& s)
            {
                s & YAS_OBJECT(
                    nullptr,
                    m_name,
                    m_input,
                    m_output,
                    m_constants
                );
            }

        private:
            std::string m_name;

            std::vector<InterfaceSlot> m_input;

            std::vector<InterfaceSlot> m_output;

            std::vector<Constant> m_constants;
    };

    struct ShaderBinary
    {
        ShaderBinary() {}

        ShaderBinary(const ShaderHeader& header, const std::vector<Uint32>& binary)
            : m_header(header)
            , m_binary(binary) {}

        bool IsValid() const { return m_binary.size(); }

        ShaderHeader m_header;
        std::vector<Uint32> m_binary;
        template <typename S>
        void serialize(S& s)
        {
            s & YAS_OBJECT(
                nullptr,
                m_header,
                m_binary
            );
        }
    };
}
