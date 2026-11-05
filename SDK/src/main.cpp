#include <cstdint>
#include <iostream>

#include <boost/date_time/posix_time/posix_time.hpp>

uint64_t to_milliseconds(const boost::posix_time::ptime& time)
{
    static const boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
    auto diff = time - epoch;

    return diff.total_milliseconds();
}

int main()
{
    auto now = boost::posix_time::second_clock::local_time();
    std::cout << "ptime=" << now << " milliseconds=" << to_milliseconds(now) << std::endl;

    return 0;
}