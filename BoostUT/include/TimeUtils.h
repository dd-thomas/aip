#pragma once

#include <cstdint>

#include <boost/date_time/posix_time/posix_time.hpp>

namespace NTimeUtils
{
// Количество миллисекунд от эпохи Unix (1970-01-01 00:00:00).
// Для not_a_date_time возвращает std::numeric_limits<uint64_t>::max().
uint64_t PtimeToMilliseconds(const boost::posix_time::ptime& time) noexcept;

// Обратное преобразование: эпоха Unix + milliseconds.
boost::posix_time::ptime MillisecondsToPtime(uint64_t milliseconds) noexcept;
} // namespace NTimeUtils
