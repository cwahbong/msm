#include "msm/runner.h"

#include "msm/agent.h"
#include "msm/land.h"
#include "msm/land_viewer.h"
#include "msm/miner.h"
#include "msm/observer.h"

namespace msm {

namespace {

class RunnerImpl: public Runner {
public:
    RunnerImpl(std::unique_ptr<Land> && land, std::shared_ptr<Agent> agent, std::shared_ptr<Observer> observer);
    ~RunnerImpl();

    Result<Void> Run() override;

private:
    std::shared_ptr<Agent> _agent;
    std::shared_ptr<Observer> _observer;
    std::unique_ptr<Miner> _miner;
};

RunnerImpl::RunnerImpl(std::unique_ptr<Land> && land, std::shared_ptr<Agent> agent, std::shared_ptr<Observer> observer):
    _agent(agent),
    _observer(observer),
    _miner(miner::New(std::move(land), observer))
{}

RunnerImpl::~RunnerImpl() = default;

Result<Void>
RunnerImpl::Run()
{
    while (true) {
        const auto action = _agent->GenAction(*VALUE_OR_RETURN(_miner->GetLandViewer()));
        switch(action.type) {
        case ActionType::DIG:
            VALUE_OR_RETURN(_miner->Dig(action.location));
            break;
        case ActionType::FLAG:
            VALUE_OR_RETURN(_miner->Flag(action.location, true));
            break;
        case ActionType::UNFLAG:
            VALUE_OR_RETURN(_miner->Flag(action.location, false));
            break;
        case ActionType::END:
            return VOID;
        }
        switch(VALUE_OR_RETURN(_miner->GetStatus())) {
        case MinerStatus::WIN:
            _observer->OnEnd(true);
            return VOID;
        case MinerStatus::LOSE:
            _observer->OnEnd(false);
            return VOID;
        case MinerStatus::UNDETERMINED:
            break;
        }
    }
    return VOID;
}

} // namespace

Runner::~Runner() = default;

namespace runner {

std::unique_ptr<Runner>
New(std::unique_ptr<Land> && land, std::shared_ptr<Agent> agent, std::shared_ptr<Observer> observer)
{
    return std::make_unique<RunnerImpl>(std::move(land), agent, observer);
}

} // namespace runner

} // namespace msm
