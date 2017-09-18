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

    template<typename T>
    class ObjectRef
    {
    public:
        ObjectRef(T* type): m_type(type)
        {
            m_refCount++;
        };
        ObjectRef(const ObjectRef& objectRef): m_type(objectRef.m_type)
        {
            m_refCount++;
        }
        ObjectRef(const ObjectRef&& objectRef) : m_type(objectRef.m_type)
        {
            m_refCount++;
        }
        ~ObjectRef()
        {
            m_refCount--;
            if (m_refCount)
            {

            }
        }
    private:
        T m_type;
        mutable Uint32 m_refCount;
    };

} // namespace engine
