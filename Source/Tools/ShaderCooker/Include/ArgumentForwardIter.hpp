#pragma once

#include <string>

class ArgumentForwardIter
{
    public:
        using value_type = std::string;
        using difference_type = std::ptrdiff_t;
        using pointer = const std::string*;
        using reference = const std::string&;
        using iterator_category = std::forward_iterator_tag;

        ArgumentForwardIter(): m_ptr(nullptr) {}

        ArgumentForwardIter(const std::string* ptr):m_ptr(ptr) {}

       reference operator*() const
        {
            return *m_ptr;
        }

        ArgumentForwardIter& operator++()
        {
            m_ptr += 1;
            return *this;
        }

        ArgumentForwardIter operator++(int)
        {
            ArgumentForwardIter tmp = *this;
            m_ptr += 1;
            return tmp;
        }

        ArgumentForwardIter& operator--()
        {
            m_ptr -= 1;
            return *this;
        }

        ArgumentForwardIter operator--(int)
        {
            ArgumentForwardIter tmp = *this;
            m_ptr -= 1;
            return tmp;
        }

        bool operator==(const ArgumentForwardIter& other)
        {
            return m_ptr == other.m_ptr;
        }

        bool operator!=(const ArgumentForwardIter& other)
        {
            return m_ptr  != other.m_ptr;
        }

    private:
        const std::string* m_ptr;
};
