#include "msm/location_range.h"

namespace msm {

LocationRange::~LocationRange() = default;

LocationRange::ConstIterator
LocationRange::begin() const
{
    return ConstIterator(*this);
}

LocationRange::ConstIterator
LocationRange::end() const
{
    return ConstIterator();
}

} // namespace msm
