#pragma once

#include <type_traits>
#include <utility>

namespace engine
{

    typedef signed   char      Int8;
    typedef unsigned char      Uint8;
    typedef signed   short     Int16;
    typedef unsigned short     Uint16;
    typedef signed   int       Int32;
    typedef unsigned int       Uint32;
    typedef signed   long long Int64;
    typedef unsigned long long Uint64;
    typedef float              Float;
    typedef double             Double;
    typedef bool               Bool;

    static_assert(sizeof(Int8)   == 1, "Invalid type size");
    static_assert(sizeof(Uint8)  == 1, "Invalid type size");
    static_assert(sizeof(Int16)  == 2, "Invalid type size");
    static_assert(sizeof(Uint16) == 2, "Invalid type size");
    static_assert(sizeof(Int32)  == 4, "Invalid type size");
    static_assert(sizeof(Uint32) == 4, "Invalid type size");
    static_assert(sizeof(Int64)  == 8, "Invalid type size");
    static_assert(sizeof(Uint64) == 8, "Invalid type size");
    static_assert(sizeof(Float)  == 4, "Invalid type size");
    static_assert(sizeof(Double) == 8, "Invalid type size");
    static_assert(sizeof(Bool)   == 1, "Invalid type size");

    template<typename T>
    struct Extent
    {
        Extent()
        : width(0)
        , height(0) {}

        Extent(const T& _width, const T& _height)
        : width(_width)
        , height(_height) {}
        ~Extent() {}

        T width;
        T height;
    };

    typedef Extent<Uint32> ExtentI;
    typedef Extent<Float> ExtentF;

    class Referenceable;

    template<typename T>
    class ObjectRef
    {
    public:
        ObjectRef(T* type) : m_type(type)
        {
            static_assert(std::is_convertible<T*, Referenceable*>::value,
                    "ObjectRef can only hold Referenceable derived");
            if (type != nullptr)
                m_type->AddRef();
        }
        ObjectRef(const ObjectRef& objectRef) : m_type(objectRef.m_type)
        {
            m_type->AddRef();
        }
        ObjectRef(const ObjectRef&& objectRef) : m_type(objectRef.m_type)
        {
            m_type->AddRef();
        }
        ~ObjectRef()
        {
            if (m_type && !m_type->RemoveRef())
            {
                delete m_type;
                m_type = nullptr;
            }
        }
        ObjectRef& operator=(const ObjectRef& objectRef)
        {
            m_type = objectRef.m_type;
            m_type->AddRef();
            return *this;
        }
        ObjectRef& operator=(const ObjectRef&& objectRef)
        {
            // I dont know if this even works
            m_type = objectRef.m_type;
        }
        T* operator->()
        {
            return m_type;
        }
        Bool Valid() const
        {
            return m_type;
        }
        T* Get()
        {
            return m_type;
        }
        template<typename U>
        explicit operator ObjectRef<U>()
        {
            static_assert(std::is_convertible<T*, U*>::value, "Can only cast on releted types.");
            return ObjectRef<U>(static_cast<U*>(m_type));
        }
    private:
        T* m_type;
    };

    template<class T, typename ...Args >
    ObjectRef<T> MakeObjectRef(Args&&... args)
    {
        return ObjectRef<T>( new T(std::forward<Args>(args)...));
    }

    template<typename To, typename From >
    ObjectRef<To> ObjectRefCast(ObjectRef<From>& base)
    {
        static_assert(std::is_convertible<From*, To*>::value, "Can only cast on releted types.");
        return ObjectRef<To>(static_cast<To*>(base.Get()));
    }

    class Referenceable
    {
    public:
        virtual ~Referenceable() {}
        void AddRef() const { ++m_refCount; }
        Uint32 RemoveRef() const { return --m_refCount; }
    private:
        mutable Uint32 m_refCount = 0;
    };

} // namespace engine
