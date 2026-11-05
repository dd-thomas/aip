#include "HttpServerSettings.h"
#include "Serialization.h"

#include <filesystem>
#include <iostream>

int main()
{
    const std::filesystem::path filePath = "settings.xml";

    const SHttpServerSettings settings("127.0.0.1", 8080, "/api", true);

    if (!NSerialization::SaveToXml(settings, filePath))
    {
        std::cerr << "Failed to save settings to " << filePath << std::endl;
        return 1;
    }

    std::cout << "Saved to " << std::filesystem::absolute(filePath) << ":" << std::endl;
    std::cout << NSerialization::ToXmlString(settings) << std::endl;

    SHttpServerSettings loaded;
    if (!NSerialization::LoadFromXml(loaded, filePath))
    {
        std::cerr << "Failed to load settings from " << filePath << std::endl;
        return 1;
    }

    std::cout << "Loaded: "
              << "ip=" << loaded.ip
              << " port=" << loaded.port
              << " prefix=" << loaded.prefix
              << " use_ssl=" << std::boolalpha << loaded.use_ssl << std::endl;
    return 0;
}
