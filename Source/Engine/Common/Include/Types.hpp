#pragma once

#include <type_traits>
#include <utility>
#include <atomic>

#include "Logger.hpp"
#include "Assert.hpp"

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

    enum class Status
    {
        Success = 0,
        Error,
        Timeout,
        OutDated,
        Invalid,

    };

    template <typename T>
    struct Result
    {
        Result(Status _status): status(_status) {}
        Result(Status _status, T& _value): status(_status), value(_value) {}
        Result(Status _status, T&& _value): status(_status), value(std::move(_value)) {}
        Status status;
        T value;
        operator Bool() const { return status == Status::Success; }
        operator T() const { return value; }
    };

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

    class AtomicCounter
    {
    public:
        AtomicCounter():m_refCount(nullptr) {}
        AtomicCounter(std::atomic<Uint32>* refCount): m_refCount(refCount) {}
        AtomicCounter(const AtomicCounter& other):m_refCount(other.m_refCount)
        {
            AddRef();
        }
        virtual ~AtomicCounter() {}
        void Initialize()
        {
            m_refCount = new std::atomic<Uint32>();
            ASSERT(m_refCount);
        }
        Uint32 AddRef() const { return m_refCount->fetch_add(1); }
        Uint32 RemoveRef() const { return m_refCount->fetch_sub(1); }
        Uint32 Get() const { return m_refCount->load(); }
        Bool Valid() const { return m_refCount; }

    protected:
        std::atomic<Uint32>* m_refCount;
    };

    // Maybe rewrite it to aux ref counting
    template<typename T>
    class ObjectRef: protected AtomicCounter
    {
    public:
        ObjectRef(T* type) : m_type(type)
        {
            Initialize();
            AddRef();
        }
        ObjectRef(const ObjectRef& other)
            : AtomicCounter(static_cast<const AtomicCounter&>(other))
            , m_type(other.m_type)
        {
        }
        ObjectRef(ObjectRef&& other)
            : AtomicCounter(static_cast<AtomicCounter&>(other))
            , m_type(other.m_type)
        {
            other.m_type = nullptr;
            other.m_refCount = nullptr;
        }
        ~ObjectRef()
        {
            if (m_type && !RemoveRef())
            {
                delete m_type;
                m_type = nullptr;
            }
        }
        ObjectRef& operator=(const ObjectRef& other)
        {
            m_type = other.m_type;
            m_refCount = other.m_refCount;
            AddRef();
            return *this;
        }
        ObjectRef& operator=(ObjectRef&& other)
        {
            // I dont know if this even works
            m_type = other.m_type;
            m_refCount = m_refCount;
            other.m_type = nullptr;
            other.m_refCount = nullptr;
        }
        Bool operator==(const ObjectRef& other) const
        {
            if (m_type == other.m_type)
            {
                return true;
            }
            return false;
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
        ObjectRef(ObjectRef<U>& other)
            : AtomicCounter(static_cast<AtomicCounter&>(other))
            , m_type(static_cast<U*>(other.Get())) {}
    private:
        T* m_type;
    };

    template<class T, typename ...Args >
    ObjectRef<T> MakeObjectRef(Args&&... args)
    {
        return ObjectRef<T>( new T(std::forward<Args>(args)...));
    }

    // make it less bad
    template<typename To, typename From >
    ObjectRef<To> ObjectRefCast(ObjectRef<From>& base)
    {
        static_assert(std::is_convertible<From*, To*>::value, "Can only cast on releted types.");
        return ObjectRef<To>(base);
    }

} // namespace engine
