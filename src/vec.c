#include "vec.h"

bool IVec3_equal(const IVec3 a, const IVec3 b)
{
    return (a[0] == b[0]) & (a[1] == b[1]) & (a[2] == b[2]);
}
