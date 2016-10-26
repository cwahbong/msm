#ifndef MSM__AGENT_H
#define MSM__AGENT_H

#include "msm_export.h"
#include "result.h"
#include "type.h"

namespace msm {

class Land;

enum class ActionType {
    DIG,
    FLAG,
    UNFLAG,
    END,
};

struct Action {
    ActionType type;
    Location location;
};

class MSM_EXPORT Agent {
public:
    virtual ~Agent() = 0;

    virtual Result<Action> GenAction(const Land &) noexcept = 0;
};

} // namespace msm

#endif // MSM__AGENT_H
