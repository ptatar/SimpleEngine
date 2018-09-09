#include "ArgumentParser.hpp"

#include <iostream>
#include <memory>
#include <cstring>

Value* CreateValueFromType(ValueType type)
{
    switch(type)
    {
        case ValueType::Path:
            return new ValuePath();
        default:
            return nullptr;
    }
}


Value* ArgumentMap::GetByFullname(const std::string& fullname)
{
    auto found = m_fullnameValueMap.find(fullname);
    if (found == m_fullnameValueMap.end())
    {
        return nullptr;
    }
    return found->second;
}


Value* ArgumentMap::GetByAbbreviation(const std::string& abbreviation)
{
    auto found = m_abbreviationValueMap.find(abbreviation);
    if (found == m_abbreviationValueMap.end())
    {
        return nullptr;
    }
    return found->second;
}

bool ArgumentMap::AddValue(
        const std::string& fullname,
        const std::string& abbreviation,
        std::unique_ptr<Value> value)
{
    if (value != nullptr)
    {
        m_fullnameValueMap[fullname] = value.get();
        m_abbreviationValueMap[abbreviation] = value.get();
        m_values.push_back(std::move(value));
    }
    return false;
}


bool ArgumentParser::AddArgument(
        const std::string& name,
        const std::string& abbreviation,
        const std::string& help,
        ValueType type)
{
    {
        auto it = m_nameSet.find(name);
        if (it != m_nameSet.end())
        {
            std::cout << "Error: Argument name already in use" << std::endl;
            return false;
        }
    }
    {
        auto it = m_abbreviationSet.find(abbreviation);
        if (it != m_abbreviationSet.end())
        {
            std::cout << "Error: Argument abbreviation already in use" << std::endl;
            return false;
        }
    }

    m_nameSet.insert(name);
    m_abbreviationSet.insert(abbreviation);
    m_argumentDescs.emplace_back(name, abbreviation, help, type);

    return true;
}


bool ArgumentParser::Parse(const Arguments& arguments)
{
    if (arguments.Count() < 1)
    {
        return false;
    }

    for(auto iter = ++arguments.Begin(); iter != arguments.End(); iter++)
    {
        ValueType type = ValueType::Undefined;
        if(IsFullname(*iter))
        {
            std::string fullname = GetFullname(*iter);
            for (size_t i = 0; i < m_argumentDescs.size(); ++i)
            {
                auto& arg = m_argumentDescs[i];
                if (Strcmp(fullname, arg.m_fullname) == 0)
                {
                    std::unique_ptr<Value> value = std::unique_ptr<Value>(CreateValueFromType(arg.m_type));
                    if (!value->Parse(iter))
                    {
                        return false;
                    }

                    m_argumentMap.AddValue(arg.m_fullname, arg.m_abbreviation, std::move(value));
                    break;
                }
            }
        }
        else if (IsAbbreviation(*iter))
        {
            std::string abbreviation = GetAbbreviation(*iter);
            for (size_t i = 0; i < m_argumentDescs.size(); ++i)
            {
                auto& arg = m_argumentDescs[i];
                if (Strcmp(abbreviation, arg.m_abbreviation) == 0)
                {
                    std::unique_ptr<Value> value = std::unique_ptr<Value>(CreateValueFromType(arg.m_type));
                    if (!value->Parse(iter))
                    {
                        return false;
                    }

                    m_argumentMap.AddValue(arg.m_fullname, arg.m_abbreviation, std::move(value));
                    break;
                }
            }
        }
        else
        {
            std::cout << "Invalid argument: " << *iter << std::endl;
            return false;
        }

    }

    return true;
}

bool ArgumentParser::IsFullname(const std::string& name) const
{
    if (name.size() > 2)
    {
        if(name[0] == '-' && name[1] == '-')
        {
            return true;
        }
    }
    return false;
}

bool ArgumentParser::IsAbbreviation(const std::string& name) const
{
    if (name.size() >= 2)
    {
        if (name[0] == '-' && name[1] != '-' )
        {
            return true;
        }
    }
    return false;
}


std::string ArgumentParser::GetFullname(const std::string& name) const
{
    if (name.size() > 2)
    {
        return name.substr(2);
    }
    else
    {
        std::cout << "Name is not a fullname" << std::endl;
        return "";
    }
}


std::string ArgumentParser::GetAbbreviation(const std::string& name) const
{
    if (name.size() > 1)
    {
        return name.substr(1);
    }
    else
    {
        std::cout << "Name is not a abbreviation" << std::endl;
        return "";
    }
}

int ArgumentParser::Strcmp(const std::string& str0, const std::string& str1) const
{
    return std::strcmp(str0.c_str(), str1.c_str());
}
