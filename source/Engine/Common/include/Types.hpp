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
    struct Size
    {
        Size()
        : width(0)
        , height(0) {}

        Size(const T& _width, const T& _height)
        : width(_width)
        , height(_height) {}
        ~Size() {}

        T width;
        T height;
    };

    typedef Size<Uint32> SizeI;
    typedef Size<Float> SizeF;

} // namespace engine
