#ifndef MSM__LAND_VIEWER_H
#define MSM__LAND_VIEWER_H

#include "msm_export.h"
#include "result.h"
#include "type.h"

#include <memory>

namespace msm {

class Land;
class LocationRange;

struct CellView {
    bool digged;
    bool flagged;
    bool has_mine;
    Size neighbors_mine_count;
};

class MSM_EXPORT LandViewer {
public:
    virtual ~LandViewer() = 0;

    virtual Result<CellView> View(const Location & location) const = 0;
    virtual Result<std::unique_ptr<LocationRange>> GetAllLocations() const = 0;
};

namespace land_viewer {

MSM_EXPORT std::unique_ptr<LandViewer> New(const Land &);

} // namespace land_viewer

} // namespace msm

#endif // MSM__LAND_VIEWER_H
