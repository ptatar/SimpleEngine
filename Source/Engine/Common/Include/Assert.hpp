#pragma once

#include <cassert>

#define ASSERT assert

#ifdef RELEASE_BUILD
    #define ASSERT_RETURN(arg) \
        if (!(arg)) \
        { \
            return false; \
        }
    #define ASSERT_INVOKE_COND(cond, pred) \
        if(cond) \
        { \
            pred; \
        }
#elif defined(DEBUG_BUILD)
    #define ASSERT_RETURN(arg) \
        if (!(arg)) \
        { \
            ASSERT(false); \
            return false; \
        }
    #define ASSERT_INVOKE_COND(cond, pred) \
        ASSERT(cond); \
        pred
#endif


