#pragma once

#include <filesystem>
#include <string>

struct SHttpServerSettings;

namespace NSerialization
{
bool SaveToXml(const SHttpServerSettings& settings, const std::filesystem::path& filePath);
bool LoadFromXml(SHttpServerSettings& settings, const std::filesystem::path& filePath);
std::string ToXmlString(const SHttpServerSettings& settings);
SHttpServerSettings FromXmlString(const std::string& xml);
} // namespace NSerialization
