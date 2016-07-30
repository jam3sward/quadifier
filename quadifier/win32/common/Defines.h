#ifndef __Defines_h
#define __Defines_h

//-----------------------------------------------------------------------------

namespace hive {

// used to silence warnings in while (false)
const struct {
    inline operator bool() const { return false; }
} false_value;

} // namespace hive

//-----------------------------------------------------------------------------

#endif//__Defines_h
