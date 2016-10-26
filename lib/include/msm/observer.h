#ifndef MSM__OBSERVER_H
#define MSM__OBSERVER_H

#include "msm_export.h"
#include "result.h"
#include "type.h"

namespace msm {

class MSM_EXPORT Observer {
public:
    virtual ~Observer() = 0;

    virtual void OnDig(const Location & location, Size neighrsMineCount, bool hasMine) noexcept = 0;
    virtual void OnEnd(bool win) noexcept = 0;
};

} // namespace msm

#endif // MSM__OBSERVER_H
