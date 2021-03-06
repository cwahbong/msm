#include "msm/miner.h"

#include "msm/land.h"
#include "msm/land_viewer.h"
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

    Result<std::unique_ptr<LandViewer>> GetLandViewer() const override;
    Result<MinerStatus> GetStatus() const override;

private:
    std::unique_ptr<Land> _land;
    std::shared_ptr<Observer> _observer;
    std::unordered_set<Location> _digged;
    MinerStatus _minerStatus;
};

MinerImpl::MinerImpl(std::unique_ptr<Land> && land, std::shared_ptr<Observer> observer):
    _land(std::move(land)),
    _observer(observer),
    _digged(),
    _minerStatus(MinerStatus::UNDETERMINED)
{}

MinerImpl::~MinerImpl() = default;

Result<Void>
MinerImpl::Dig(const Location & location)
{
    const bool hasMine = VALUE_OR_RETURN(_land->HasMine(location));
    VALUE_OR_RETURN(_land->SetDig(location, true));
    _observer->OnDig(location, VALUE_OR_RETURN(_land->GetNeighborsMineCount(location)), hasMine);
    if (_minerStatus == MinerStatus::UNDETERMINED) {
        if (hasMine) {
            _minerStatus = MinerStatus::LOSE;
        } else if (false/* All safe is digged. */) {
            _minerStatus = MinerStatus::WIN;
        }
    }
    return VOID;
}

Result<Void>
MinerImpl::Flag(const Location & location, bool flag)
{
    VALUE_OR_RETURN(_land->SetFlag(location, flag));
    _land->SetFlag(location, flag);
    return VOID;
}

Result<std::unique_ptr<LandViewer>>
MinerImpl::GetLandViewer() const
{
    return land_viewer::New(*_land);
}

Result<MinerStatus>
MinerImpl::GetStatus() const
{
    return _minerStatus;
}

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
