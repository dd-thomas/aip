#include "TimeUtils.h"

#include <limits>

namespace
{
const auto EPOCH = boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1));
} // namespace

namespace NTimeUtils
{
uint64_t PtimeToMilliseconds(const boost::posix_time::ptime& time) noexcept
{
    if (time.is_not_a_date_time())
        return std::numeric_limits<uint64_t>::max();

    const auto duration = time - EPOCH;
    return static_cast<uint64_t>(duration.total_milliseconds());
}

boost::posix_time::ptime MillisecondsToPtime(uint64_t milliseconds) noexcept
{
    return EPOCH + boost::posix_time::milliseconds(static_cast<int64_t>(milliseconds));
}
} // namespace NTimeUtils
