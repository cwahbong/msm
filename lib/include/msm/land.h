#ifndef MSM__LAND_H
#define MSM__LAND_H

#include "msm_export.h"
#include "result.h"
#include "type.h"

#include <memory>

namespace msm {

class MSM_EXPORT LocationViewer {
public:
    virtual ~LocationViewer() = 0;

    virtual Result<Location> Next() = 0;
    virtual Result<bool> End() = 0;
};

struct CellView {
    bool digged;
    bool has_mine;
    Size neighbors_mine_count;
};

class MSM_EXPORT LandViewer {
public:
    virtual ~LandViewer() = 0;

    virtual Result<CellView> View(const Location & location) const = 0;

    virtual Result<std::unique_ptr<LocationViewer>> GetAllLocations() const = 0;
    virtual Result<std::unique_ptr<LocationViewer>> GetNeighbors(const Location & location) const = 0;
};

class MSM_EXPORT Land: public LandViewer {
public:
    virtual ~Land() = 0;

    virtual Result<Void> SetNeighbor(const Location & location1, const Location & location2) = 0;

    virtual Result<Void> SetMine(const Location & location, bool isMine) = 0;

    virtual Result<bool> HasMine(const Location & location) const = 0;

    virtual Result<Void> SetFlag(const Location & location, bool flag) = 0;
    virtual Result<Void> SetDig(const Location & location, bool digged) = 0;
};

namespace land {

MSM_EXPORT Result<Size> GetNeighborsMineCount(const Land & land, const Location & location);

MSM_EXPORT std::unique_ptr<Land> NewSquare(Size row, Size col);

} // namespace land

} // namespace msm

#endif // MSM__LAND_H
