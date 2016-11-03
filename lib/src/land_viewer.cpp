#include "msm/land_viewer.h"

#include "msm/land.h"
#include "msm/location_range.h"

namespace msm {

namespace {

class LandViewerImpl: public LandViewer {
public:
    LandViewerImpl(const Land & land);
    ~LandViewerImpl() override;

    Result<CellView> View(const Location & location) const override;
    Result<std::unique_ptr<LocationRange>> GetAllLocations() const override;

private:
    const Land & _land;
};

LandViewerImpl::LandViewerImpl(const Land & land):
    LandViewer(),
    _land(land)
{}

LandViewerImpl::~LandViewerImpl() = default;

Result<CellView>
LandViewerImpl::View(const Location & location) const
{
    return VALUE_OR_RETURN(_land.IsDigged(location)) ?
        CellView {
            .digged = true,
            .flagged = false,
            .has_mine = VALUE_OR_RETURN(_land.HasMine(location)),
            .neighbors_mine_count = VALUE_OR_RETURN(_land.GetNeighborsMineCount(location)),
        }:
        CellView {
            .digged = false,
            .flagged = VALUE_OR_RETURN(_land.HasFlag(location)),
            .has_mine = false,
            .neighbors_mine_count = 0,
        };
}

Result<std::unique_ptr<LocationRange>>
LandViewerImpl::GetAllLocations() const
{
    return _land.GetAllLocations();
}

} // namespace

LandViewer::~LandViewer() = default;

namespace land_viewer {

std::unique_ptr<LandViewer>
New(const Land & land)
{
    return std::make_unique<LandViewerImpl>(land);
}

} // namespace land_viewer


} // namespace msm
