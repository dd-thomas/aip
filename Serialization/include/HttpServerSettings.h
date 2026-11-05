#pragma once

#include <cstdint>
#include <string>

#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>

struct SHttpServerSettings
{
    std::string ip;
    std::uint32_t port = 0;
    std::string prefix;
    bool use_ssl = false;

    SHttpServerSettings() = default;

    SHttpServerSettings(std::string ip, std::uint32_t port, std::string prefix = {}, bool use_ssl = false)
        : ip(std::move(ip))
        , port(port)
        , prefix(std::move(prefix))
        , use_ssl(use_ssl)
    {
    }

private:
    friend class boost::serialization::access;

    template <class Archive>
    void serialize(Archive& archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(ip);
        archive & BOOST_SERIALIZATION_NVP(port);
        if (version >= 2)
        {
            archive & BOOST_SERIALIZATION_NVP(prefix);
        }
        if (version >= 3)
        {
            archive & BOOST_SERIALIZATION_NVP(use_ssl);
        }
    }
};

BOOST_CLASS_VERSION(SHttpServerSettings, 3)
