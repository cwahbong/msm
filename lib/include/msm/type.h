#ifndef MSM__TYPE_H
#define MSM__TYPE_H

#include <cstddef>
#include <cstdint>
#include <functional>

namespace msm {

using Size = size_t;
using Index = size_t;
using Axis = int_least64_t;

struct MSM_EXPORT Location {
    Axis x, y;
};

inline bool operator==(const Location & lhs, const Location & rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}


} // namespace msm

namespace std {

template<>
struct hash<msm::Location> {
    using result_type = size_t;
    using argument_type = msm::Location;

    result_type operator()(const argument_type & arg) const {
        const std::hash<msm::Axis> axis_hash;
        return axis_hash(arg.x) + axis_hash(arg.y);
    }
};

} // namespace std

#endif // MSM__TYPE_H
