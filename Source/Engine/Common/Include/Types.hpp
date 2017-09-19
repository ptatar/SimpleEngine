#pragma once

namespace engine
{

    typedef signed   char Int8;
    typedef unsigned char Uint8;
    typedef signed   short Int16;
    typedef unsigned short Uint16;
    typedef signed   int   Int32;
    typedef unsigned int   Uint32;
    typedef signed   long long Int64;
    typedef unsigned long long Uint64;
    typedef float Float;
    typedef double Double;
    typedef bool Bool;

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


    class ObjectRef;
    class Referenceable
    {
    public:
        virtual ~Referenceable() {}
        void AddRef() const { ++m_refCount; }
        Uint32 RemoveRef() const { return --m_refCount; }
    private:
        mutable Uint32 m_refCount = 0;
        friend ObjectRef;
    };

    template<typename T>
    class ObjectRef
    {
    public:
        ObjectRef(T* type) : m_type(type)
        {
            type->AddRef();
        }
        ObjectRef(const ObjectRef& objectRef) : m_type(objectRef.m_type)
        {
            type->AddRef();
        }
        ObjectRef(const ObjectRef&& objectRef) : m_type(objectRef.m_type)
        {
            type->AddRef();
        }
        ~ObjectRef()
        {
            if (!type->RemoveRef())
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
    private:
        T m_type;
    };

} // namespace engine
