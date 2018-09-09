#pragma once

#include <string>
#include <map>
#include <set>
#include <iterator>
#include <vector>
#include <memory>

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

class Arguments
{
    public:
        Arguments(int argc, char** argv)
        {
            for(int i = 0; i < argc; i++)
            {
                m_argv.emplace_back(argv[i]);
            }
        }

        ArgumentForwardIter Begin() const
        {
            return m_argv.data();
        }

        ArgumentForwardIter End() const
        {
            return m_argv.data() + m_argv.size();
        }

        unsigned Count() const { return m_argv.size(); }

    private:
        std::vector<std::string> m_argv;
};


enum class ValueType
{
    Undefined,
    Path,
};


struct ArgumentDesc
{
    ArgumentDesc() {}

    ArgumentDesc(
            const std::string& name,
            const std::string& abbreviation,
            const std::string& help,
            ValueType type)
        : m_fullname(name)
        , m_abbreviation(abbreviation)
        , m_help(help)
        , m_type(type){}

    bool operator==(const std::string& name) const
    {
        if (m_fullname.compare(name) == 0 ||
            m_abbreviation.compare(name) == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    virtual bool operator==(const ArgumentDesc& other)
    {
        return m_fullname.compare(other.m_fullname) == 0;
    }

    virtual bool operator<(const ArgumentDesc& other) const
    {
        return m_fullname.compare(other.m_fullname) < 0;
    }

    virtual bool operator>(const ArgumentDesc& other) const
    {
        return m_fullname.compare(other.m_fullname) > 0;
    }

    std::string m_fullname;
    std::string m_abbreviation;
    std::string m_help;
    ValueType m_type;
};


class Value
{
    public:
        Value(ValueType type) : m_type(type) {}
        ValueType GetType() const { return m_type; }
        virtual bool Parse(ArgumentForwardIter& ) = 0;


    private:
        std::string m_fullname;
        std::string m_abbreviation;
        ValueType m_type;
};

class ValuePath: public Value
{
    public:
        ValuePath(): Value(ValueType::Path) {}

        virtual bool Parse(ArgumentForwardIter&) override { return true; };
};


class ArgumentParser;
class ArgumentMap
{
    public:
        ArgumentMap() {}

        Value* GetByFullname(const std::string& fullname);

        Value* GetByAbbreviation(const std::string& abbreviation);

    private:
        bool AddValue(
                const std::string& fullname,
                const std::string& abbreviation,
                std::unique_ptr<Value> value);

    private:
        std::map<std::string, Value*> m_fullnameValueMap;

        std::map<std::string, Value*> m_abbreviationValueMap;

        std::vector<std::unique_ptr<Value>> m_values;

    friend ArgumentParser;
};


class ArgumentParser
{
    public:
        ArgumentParser() {}

        ~ArgumentParser() {}

        bool AddArgument(const std::string& name,
                         const std::string& abbreviation,
                         const std::string& help,
                         ValueType type);

        bool Parse(const Arguments& arguments);

    private:
        bool IsFullname(const std::string& name) const;

        bool IsAbbreviation(const std::string& name) const;

        std::string GetFullname(const std::string& name) const;

        std::string GetAbbreviation(const std::string& name) const;

        int Strcmp(const std::string& str0, const std::string& str1) const;

    private:
        std::vector<ArgumentDesc> m_argumentDescs;
        std::set<std::string> m_nameSet;
        std::set<std::string> m_abbreviationSet;
        ArgumentMap m_argumentMap;
};
