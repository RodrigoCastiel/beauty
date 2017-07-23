#include "Primitives.h"

#include <cstdio>

namespace renderer
{

BoundingBox BoundingBox::Split(float value, int axis, int side) const
{
    BoundingBox subBB = *this;

    if (side > 0) 
        subBB.mMin[axis] = value;
    else 
        subBB.mMax[axis] = value;

    return subBB;
}

void BoundingBox::Print() const
{
    printf("|BB| x: [%4.2f, %4.2f]; y: [%4.2f, %4.2f]; z: [%4.2f, %4.2f]\n",
           mMin.x, mMax.x, mMin.y, mMax.y, mMin.z, mMax.z);

}

}  // namespace renderer.