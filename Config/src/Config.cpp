#include "Config.h"

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
    boost::property_tree::xml_parser::read_xml("config.xml", configDataM);
}

//-----------------------------------------------------------------------------




