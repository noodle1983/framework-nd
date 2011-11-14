#include "ConfigCenter.h"
#include "XmlGroup.h"
#include "Log.h"

#include <boost/property_tree/xml_parser.hpp>

using namespace Config;

static const std::string ConfigCenter::TOP_XMLNODE_NAME = "Configuration";
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
    : topGroupM(NULL)
{
}

//-----------------------------------------------------------------------------

ConfigCenter::~ConfigCenter()
{
}

//-----------------------------------------------------------------------------


int ConfigCenter::loadXml(const std::string theXmlPath)
{
    file<> fdoc(theXmlPath);  
    xml_document<>  doc;      
    doc.parse<0>(fdoc.data());   

    xml_node<>* root = doc.first_node();  
    if (!root)
    {
        ERROR("can't load xml file:" << theXmlPath);
        return -1;
    }
    if (root->name() != TOP_XMLNODE_NAME)
    {
        ERROR("not a xml configuration file:" << theXmlPath);
        return -1;
    }

    XmlGroup* group = new XmlGroup();
    if (0 != group->parse(root->first_node()))
    {
        ERROR("can't parse configuration file:" << theXmlPath);
        return -1;
    }

    topGroupM = group;

    return 0;
}

//-----------------------------------------------------------------------------

int ConfigCenter::saveXml(const std::string theXmlPath)
{
    boost::property_tree::xml_parser::write_xml(theXmlPath, configDataM);
    return 0;
}




