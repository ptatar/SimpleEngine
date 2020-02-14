#pragma once

#include "ArgumentForwardIter.hpp"

#include <iterator>
#include <vector>
#include <cstring>


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

inline bool ArgCmp(const std::string& arg, const std::string& longName, const std::string& shortName = "")
{
    return std::strcmp(arg.c_str(), longName.c_str()) == 0 || std::strcmp(arg.c_str(), shortName.c_str()) == 0;
}
