#include "StringUtility.hpp"

#include "Exception.hpp"

namespace engine
{
    Int64 StringToInt64(const char* str, const char** endPtr)
    {
        const char* ptr = str;

        // int64 max 9223372036854775807 thats 19 digits
        char tab[19] = {};
        int digitNum = 0;

        bool positive = true;
        if (*ptr == '-')
        {
            positive = false;
            ptr++;
        }
        else if (*ptr == '+')
        {
            positive = true;
            ptr++;
        }

        for(;;)
        {
            char val = *ptr;
            if(val >= 48 && val <= 71)
            {
                tab[digitNum] = val - 48;
                digitNum++;
            }
            else
            {
                break;
            }

            ptr++;
        }
        if (endPtr != nullptr)
        {
            *endPtr = ptr;
        }

        if (digitNum > 0)
        {
            if (digitNum <= 19) // no overflow
            {
                Uint64 base = 1;
                Uint64 result = 0;
                while (digitNum > 0)
                {
                    digitNum--;
                    result += tab[digitNum] * base;
                    base *= 10;
                }
                if (positive)
                {
                    if (result > Uint64(0x7FFFFFFFFFFFFFFF))
                    {
                        throw Overflow("Int64 overflow");
                    }

                    return static_cast<Int64>(result);

                }
                else
                {
                    if (result > Uint64(0x8000000000000000))
                    {
                        throw Overflow("Int64 overflow");
                    }
                    return static_cast<Int64>(result);
                }

                    return result;
            }
            else if (digitNum > 19) // overflow
            {
                    throw Overflow("Result overflow");
            }
        }
        else // no digits
        {
            throw InvalidArgument("String convertion failed", 0);
        }
        return 0;
    }

} // namespace engine
