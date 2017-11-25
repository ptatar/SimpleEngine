#pragma once

#include "Types.hpp"

namespace engine
{
    class IJob: public virtual Referenceable
    {
    public:
        virtual ~IJob() {}
        virtual Bool Work() = 0;
    };

} // namespace engine
