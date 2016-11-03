#ifndef MSM__LOCATION_RANGE_H
#define MSM__LOCATION_RANGE_H

#include "msm_export.h"
#include "type.h"

namespace msm {

class MSM_EXPORT LocationRange {
public:
    class ConstIterator;

    virtual ~LocationRange() = 0;

    ConstIterator begin() const;
    ConstIterator end() const;

    virtual Location BeginLocation() const = 0;
    virtual Location EndLocation() const = 0;
    virtual Location NextLocation(const Location &) const = 0;

    class MSM_EXPORT ConstIterator: public std::iterator<std::forward_iterator_tag, Location> {
    public:
        ConstIterator(): _locationRange(nullptr), _location() {} // end
        ConstIterator(const LocationRange & locationRange): _locationRange(&locationRange), _location(locationRange.BeginLocation()) {} // begin

        ConstIterator & operator++() { _location = _locationRange->NextLocation(_location); return *this; }
        ConstIterator operator++(int) { const ConstIterator old = *this; _location = _locationRange->NextLocation(_location); return old; }

        const Location & operator*() { return _location; }
        const Location * operator->() { return &_location; }

        friend bool operator==(const ConstIterator & lhs, const ConstIterator & rhs) {
            if (lhs._locationRange != rhs._locationRange) {
                return false;
            }
            if (lhs._locationRange == nullptr) {
                return true;
            }
            return lhs._location == rhs._location;
        }

        friend bool operator!=(const ConstIterator & lhs, const ConstIterator & rhs) {
            return !(lhs == rhs);
        }

    private:
        const LocationRange * _locationRange;
        Location _location;
    };
};

} // namespace msm

#endif // MSM__LOCATION_RANGE_H
