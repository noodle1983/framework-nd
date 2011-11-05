#include "Config.h"
#include "Log.h"

#include <boost/property_tree/xml_parser.hpp>

using namespace Config;

//-----------------------------------------------------------------------------

boost::shared_mutex ConfigCenter::configCenterMutexM;
ConfigCenterPtr ConfigCenter::configCenterM;

//-----------------------------------------------------------------------------

ConfigCenterPtr ConfigCenter::instance()
{
    if (NULL == configCenterM.get())
    {
        boost::unique_lock<boost::shared_mutex> lock(configCenterMutexM);
        if (NULL == configCenterM.get())
        {
            configCenterM.reset(new ConfigCenter());
            configCenterM->loadXml("config.xml");
        }
    }
    boost::shared_lock<boost::shared_mutex> lock(configCenterMutexM);
    return configCenterM;
}

//-----------------------------------------------------------------------------

void ConfigCenter::loadConfig(const std::string theInputXmlFile)
{
    ConfigCenterPtr newConfigCenter(new ConfigCenter());
    if (0 == newConfigCenter->loadXml(theInputXmlFile))
    {
        DEBUG("loaded config file:" << theInputXmlFile);
        boost::unique_lock<boost::shared_mutex> lock(configCenterMutexM);
        configCenterM = newConfigCenter;
    }
    else if (NULL == configCenterM.get())
    {
        WARN("load xml file failed, the default config will be applied.");
        boost::unique_lock<boost::shared_mutex> lock(configCenterMutexM);
        configCenterM = newConfigCenter;
    }
    else 
    {
        WARN("config center is not changed.");
    }

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


int ConfigCenter::loadXml(const std::string theXmlPath)
{
    try
    {
        boost::property_tree::xml_parser::read_xml(theXmlPath, configDataM);
    }
    catch (boost::property_tree::xml_parser::xml_parser_error& e)
    {
        WARN("config file is not found:" << theXmlPath);
        return -1;
    }
    return 0;
}

//-----------------------------------------------------------------------------

int ConfigCenter::saveXml(const std::string theXmlPath)
{
    boost::property_tree::xml_parser::write_xml(theXmlPath, configDataM);
    return 0;
}




