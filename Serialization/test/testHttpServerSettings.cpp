#define BOOST_TEST_MODULE HttpServerSettingsTests
#include <boost/test/unit_test.hpp>

#include "HttpServerSettings.h"
#include "Serialization.h"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace
{
std::filesystem::path assetPath(const std::filesystem::path& fileName)
{
    return std::filesystem::path(SERIALIZATION_ASSETS_DIR) / fileName;
}

std::string readFile(const std::filesystem::path& path)
{
    std::ifstream stream(path, std::ios::in);
    if (!stream)
    {
        BOOST_FAIL("Failed to open asset: " + path.string());
    }
    std::ostringstream buffer;
    buffer << stream.rdbuf();
    return buffer.str();
}
} // namespace

BOOST_AUTO_TEST_SUITE(HttpServerSettingsTestSuite)

BOOST_AUTO_TEST_CASE(MigrateFromVersion1To2, *boost::unit_test::description("Load version 1 settings and verify migration defaults for version 2 fields"))
{
    const auto xml = readFile(assetPath("settings_v1.xml"));
    const SHttpServerSettings settings = NSerialization::FromXmlString(xml);

    BOOST_TEST(settings.ip == "192.168.1.10");
    BOOST_TEST(settings.port == 8080u);
    BOOST_TEST(settings.prefix.empty());
    BOOST_TEST(settings.use_ssl == false);
}

BOOST_AUTO_TEST_CASE(MigrateFromVersion2To3, *boost::unit_test::description("Load version 2 settings and verify migration defaults for version 3 fields"))
{
    const auto xml = readFile(assetPath("settings_v2.xml"));
    const SHttpServerSettings settings = NSerialization::FromXmlString(xml);

    BOOST_TEST(settings.ip == "192.168.1.10");
    BOOST_TEST(settings.port == 8080u);
    BOOST_TEST(settings.prefix == "/v2");
    BOOST_TEST(settings.use_ssl == false);
}

BOOST_AUTO_TEST_CASE(SerializeCurrentVersion, *boost::unit_test::description("Round-trip serialize settings in current version 3"))
{
    const SHttpServerSettings original("10.0.0.5", 9443, "/http", true);

    const std::string xml = NSerialization::ToXmlString(original);
    const SHttpServerSettings loaded = NSerialization::FromXmlString(xml);

    BOOST_TEST(loaded.ip == original.ip);
    BOOST_TEST(loaded.port == original.port);
    BOOST_TEST(loaded.prefix == original.prefix);
    BOOST_TEST(loaded.use_ssl == original.use_ssl);

    const auto assetXml = readFile(assetPath("settings_v3.xml"));
    const SHttpServerSettings fromAsset = NSerialization::FromXmlString(assetXml);

    BOOST_TEST(fromAsset.ip == "127.0.0.1");
    BOOST_TEST(fromAsset.port == 8443u);
    BOOST_TEST(fromAsset.prefix == "/api");
    BOOST_TEST(fromAsset.use_ssl == true);
}

BOOST_AUTO_TEST_SUITE_END()
