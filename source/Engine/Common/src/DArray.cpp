#include "DArray.hpp"

namespace common
{

    DArray::DArray() {}
    DArray::~DArray() {}
    Uint32 DArray::Get() const
    {
       volatile Uint32 smth = 2;
       return smth;
    }

};
