#include "HttpServerSettings.h"
#include "Serialization.h"

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <fstream>
#include <sstream>

namespace NSerialization
{
bool SaveToXml(const SHttpServerSettings& settings, const std::filesystem::path& filePath)
{
    std::ofstream stream(filePath, std::ios::out | std::ios::trunc);
    if (!stream)
    {
        return false;
    }

    {
        boost::archive::xml_oarchive archive(stream);
        archive << BOOST_SERIALIZATION_NVP(settings);
    }
    return true;
}

bool LoadFromXml(SHttpServerSettings& settings, const std::filesystem::path& filePath)
{
    std::ifstream stream(filePath, std::ios::in);
    if (!stream)
    {
        return false;
    }

    {
        boost::archive::xml_iarchive archive(stream);
        archive >> BOOST_SERIALIZATION_NVP(settings);
    }
    return true;
}

std::string ToXmlString(const SHttpServerSettings& settings)
{
    std::ostringstream stream;
    {
        // Archive must be destroyed before the stream is read: footer is written in the destructor.
        boost::archive::xml_oarchive archive(stream);
        archive << BOOST_SERIALIZATION_NVP(settings);
    }
    return stream.str();
}

SHttpServerSettings FromXmlString(const std::string& xml)
{
    SHttpServerSettings settings;
    std::istringstream stream(xml);
    {
        boost::archive::xml_iarchive archive(stream);
        archive >> BOOST_SERIALIZATION_NVP(settings);
    }
    return settings;
}
} // namespace NSerialization
