#include "msm/miner.h"

#include "msm/land.h"
#include "msm/observer.h"

#include <unordered_set>

namespace msm {

namespace {

class MinerImpl: public Miner {
public:
    MinerImpl(std::unique_ptr<Land> && land, std::shared_ptr<Observer> observer);
    ~MinerImpl();

    Result<Void> Dig(const Location & location) override;
    Result<Void> Flag(const Location & location, bool flag) override;

    Result<std::unique_ptr<Land>> GetLandView() const override;
    // Result<bool> IsDigged(const Location & location) const override;

    // Result<Size> GetNeighborsMineCount(const Location & location) const override;
    // Result<std::unique_ptr<LocationViewer>> GetAllLocations() const override;

private:
    std::unique_ptr<Land> _land;
    std::shared_ptr<Observer> _observer;
    std::unordered_set<Location> _digged;
};

MinerImpl::MinerImpl(std::unique_ptr<Land> && land, std::shared_ptr<Observer> observer):
    _land(std::move(land)),
    _observer(observer),
    _digged()
{}

MinerImpl::~MinerImpl() = default;

Result<Void>
MinerImpl::Dig(const Location & location)
{
    const bool hasMine = VALUE_OR_RETURN(_land->HasMine(location));
    _digged.insert(location);
    _observer->OnDig(location, VALUE_OR_RETURN(land::GetNeighborsMineCount(*_land, location)), hasMine);
    return VOID;
}

Result<Void>
MinerImpl::Flag(const Location & location, bool flag)
{
    // TODO
}

Result<std::unique_ptr<Land>>
MinerImpl::GetLandView() const
{
    // XXX LandView instead
}

// Result<bool>
// MinerImpl::IsDigged(const Location & location) const
// {
//     return _digged.find(location) != _digged.end();
// }
// 
// Result<Size>
// MinerImpl::GetNeighborsMineCount(const Location & location) const
// {
//     return land::GetNeighborsMineCount(*_land, location);
// }
// 
// Result<std::unique_ptr<LocationViewer>>
// MinerImpl::GetAllLocations() const
// {
//     return _land->GetAllLocations();
// }

} // namespace

Miner::~Miner() = default;

namespace miner {

std::unique_ptr<Miner>
New(std::unique_ptr<Land> && land, std::shared_ptr<Observer> observer)
{
    return std::make_unique<MinerImpl>(std::move(land), observer);
}

} // namespace miner

} // namespace msm
