#include "Config.h"
#include "Log.h"

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace Config;

//-----------------------------------------------------------------------------

static boost::mutex configCenterInstanceMutex;
static boost::shared_ptr<ConfigCenter> configCenterInstanceReleaser;
ConfigCenter* ConfigCenter::configCenterM;

//-----------------------------------------------------------------------------

ConfigCenter* ConfigCenter::instance()
{
    if (NULL == configCenterM)
    {
        boost::lock_guard<boost::mutex> lock(configCenterInstanceMutex);
        if (NULL == configCenterM)
        {
            configCenterM = new ConfigCenter();
            configCenterInstanceReleaser.reset(configCenterM);
            configCenterM->init();
        }
    }
    return configCenterM;
}

//-----------------------------------------------------------------------------

ConfigCenter::ConfigCenter()
{
}

//-----------------------------------------------------------------------------

ConfigCenter::~ConfigCenter()
{
}

//-----------------------------------------------------------------------------

void ConfigCenter::init()
{
    loadXml("config.xml");
}

//-----------------------------------------------------------------------------


void ConfigCenter::loadXml(const std::string theXmlPath)
{
    try
    {
        boost::property_tree::xml_parser::read_xml(theXmlPath, configDataM);
    }
    catch (boost::property_tree::xml_parser::xml_parser_error& e)
    {
        WARN("config file is not found:" << theXmlPath);
    }
}

//-----------------------------------------------------------------------------

void ConfigCenter::saveXml(const std::string theXmlPath)
{
    boost::property_tree::xml_parser::write_xml(theXmlPath, configDataM);
}




