#ifndef MSM__MINER_H
#define MSM__MINER_H

#include "msm_export.h"
#include "result.h"
#include "type.h"

#include <memory>

namespace msm {

class Land;
class LandViewer;
class LocationViewer;
class Observer;

enum class MinerStatus {
    WIN,
    LOSE,
    UNDETERMINED,
};

class MSM_EXPORT Miner {
public:
    virtual ~Miner() = 0;

    virtual Result<Void> Dig(const Location & location) = 0;
    virtual Result<Void> Flag(const Location & location, bool flag) = 0;

    virtual Result<std::unique_ptr<LandViewer>> GetLandViewer() const = 0;
    virtual Result<MinerStatus> GetStatus() const = 0;
};

namespace miner {

MSM_EXPORT std::unique_ptr<Miner> New(std::unique_ptr<Land> && land, std::shared_ptr<Observer> observer);

} // namespace miner

} // namespace msm

#endif // MSM__MINER_H
