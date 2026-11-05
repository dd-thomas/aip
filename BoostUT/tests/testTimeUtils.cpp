#define BOOST_TEST_MODULE TimeUtilsTests
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

#include <cstdint>
#include <limits>

#include "TimeUtils.h"

namespace bdata = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(TimeUtilsTestSuite)

BOOST_AUTO_TEST_CASE(TestPtimeToMillisecondsEpoch, *boost::unit_test::description("Test conversion of epoch time to milliseconds"))
{
    boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
    auto result = NTimeUtils::PtimeToMilliseconds(epoch);
    BOOST_TEST(result == 0u);
}

BOOST_AUTO_TEST_CASE(SpecialValueTest, *boost::unit_test::description("Test special values"))
{
    auto result = NTimeUtils::PtimeToMilliseconds(boost::posix_time::not_a_date_time);
    BOOST_TEST(result == std::numeric_limits<uint64_t>::max());
}

BOOST_AUTO_TEST_CASE(TestPtimeToMillisecondsOneSecond, *boost::unit_test::description("Test conversion of 1 second after epoch to milliseconds"))
{
    boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
    boost::posix_time::ptime oneSecond = epoch + boost::posix_time::seconds(1);
    auto result = NTimeUtils::PtimeToMilliseconds(oneSecond);
    BOOST_TEST(result == 1000u);
}

BOOST_AUTO_TEST_CASE(TestPtimeToMillisecondsMilliseconds, *boost::unit_test::description("Test conversion of 5.5 seconds after epoch to milliseconds"))
{
    boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
    boost::posix_time::ptime withMs = epoch + boost::posix_time::milliseconds(5500);
    auto result = NTimeUtils::PtimeToMilliseconds(withMs);
    BOOST_TEST(result == 5500u);
}

BOOST_AUTO_TEST_CASE(TestMillisecondsToPtimeEpoch, *boost::unit_test::description("Test conversion of 0 milliseconds to epoch"))
{
    boost::posix_time::ptime result = NTimeUtils::MillisecondsToPtime(0);
    boost::posix_time::ptime expected(boost::gregorian::date(1970, 1, 1));
    BOOST_TEST(result == expected);
}

BOOST_AUTO_TEST_CASE(TestMillisecondsToPtimeOneSecond, *boost::unit_test::description("Test conversion of 1000 milliseconds to 1 second"))
{
    boost::posix_time::ptime result = NTimeUtils::MillisecondsToPtime(1000);
    boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
    boost::posix_time::ptime expected = epoch + boost::posix_time::seconds(1);
    BOOST_TEST(result == expected);
}

BOOST_AUTO_TEST_CASE(TestMillisecondsToPtimeArbitrary, *boost::unit_test::description("Test conversion of 123456789 milliseconds to 123456789 milliseconds"))
{
    boost::posix_time::ptime result = NTimeUtils::MillisecondsToPtime(123456789);
    boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
    boost::posix_time::ptime expected = epoch + boost::posix_time::milliseconds(123456789);
    BOOST_TEST(result == expected);
}

BOOST_AUTO_TEST_CASE(TestRoundTripConversion, *boost::unit_test::description("Test round trip: ptime -> milliseconds -> ptime"))
{
    boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
    boost::posix_time::ptime original = epoch + boost::posix_time::milliseconds(987654321);

    auto ms = NTimeUtils::PtimeToMilliseconds(original);
    auto result = NTimeUtils::MillisecondsToPtime(ms);

    BOOST_TEST(original == result);
}

BOOST_AUTO_TEST_SUITE_END()

// ---------------------------------------------------------------------------
// Фикстура: общие данные для нескольких тестов
// ---------------------------------------------------------------------------
struct SEpochFixture
{
    const boost::posix_time::ptime epoch{boost::gregorian::date(1970, 1, 1)};
};

BOOST_FIXTURE_TEST_SUITE(EpochFixtureSuite, SEpochFixture)

BOOST_AUTO_TEST_CASE(TestOneDay)
{
    BOOST_TEST(NTimeUtils::PtimeToMilliseconds(epoch + boost::gregorian::days(1)) == 86'400'000u);
}

BOOST_AUTO_TEST_CASE(TestLeapDay)
{
    const boost::posix_time::ptime leapDay(boost::gregorian::date(2024, 2, 29));
    BOOST_TEST(NTimeUtils::PtimeToMilliseconds(leapDay) == 1'709'164'800'000u);
}

BOOST_AUTO_TEST_SUITE_END()

// ---------------------------------------------------------------------------
// Тест, управляемый данными: одна функция проверки -- несколько наборов данных
// ---------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(DataDrivenSuite)

BOOST_DATA_TEST_CASE(TestRoundTripData,
    bdata::make({uint64_t{0}, uint64_t{1}, uint64_t{999}, uint64_t{86'400'000}, uint64_t{1'793'914'607'123}}),
    milliseconds)
{
    BOOST_TEST(NTimeUtils::PtimeToMilliseconds(NTimeUtils::MillisecondsToPtime(milliseconds)) == milliseconds);
}

BOOST_AUTO_TEST_SUITE_END()
