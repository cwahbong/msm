#ifndef MSM__RUNNER_H
#define MSM__RUNNER_H

#include "msm_export.h"
#include "result.h"
#include "type.h"

#include <memory>

namespace msm {

class Agent;
class Land;
class Observer;

class MSM_EXPORT Runner {
public:
    virtual ~Runner() = 0;

    virtual Result<Void> Run() = 0;
};

namespace runner {

MSM_EXPORT std::unique_ptr<Runner> New(std::unique_ptr<Land> && land, std::shared_ptr<Agent> agent, std::shared_ptr<Observer> observer);

} // namespace runner

} // namespace msm

#endif // MSM__RUNNER_H
