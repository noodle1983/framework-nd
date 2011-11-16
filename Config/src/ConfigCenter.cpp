#include "ConfigCenter.h"
#include "XmlGroup.h"
#include "Log.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <rapidxml_print.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace Config;
using namespace rapidxml;

const std::string ConfigCenter::TOP_XMLNODE_NAME = "Configuration";
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
            CFG_DEBUG("loaded config file:config.xml");
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
        CFG_DEBUG("loaded config file:" << theInputXmlFile);
        boost::unique_lock<boost::shared_mutex> lock(configCenterMutexM);
        configCenterM = newConfigCenter;
    }
    else if (NULL == configCenterM.get())
    {
        CFG_ERROR("load xml file failed, the default config will be applied.");
        boost::unique_lock<boost::shared_mutex> lock(configCenterMutexM);
        configCenterM = newConfigCenter;
    }
    else 
    {
        CFG_ERROR("config center is not changed.");
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
    if (topGroupM)
    {
        delete topGroupM;
        topGroupM = NULL;
    }
}

//-----------------------------------------------------------------------------


int ConfigCenter::loadXml(const std::string& theXmlPath)
{
    file<> fdoc(theXmlPath.c_str());  
    xml_document<>  doc;      
    doc.parse<0>(fdoc.data());   

    xml_node<>* root = doc.first_node();  
    if (!root)
    {
        CFG_ERROR("can't load xml file:" << theXmlPath);
        return -1;
    }
    if (root->name() != TOP_XMLNODE_NAME)
    {
        CFG_ERROR("not a xml configuration file:" << theXmlPath);
        return -1;
    }

    XmlGroup* group = new XmlGroup();
    if (0 != group->parse(root->first_node()))
    {
        CFG_ERROR("can't parse configuration file:" << theXmlPath);
        return -1;
    }

    if (topGroupM)
    {
        delete topGroupM;
        topGroupM = NULL;
    }
    topGroupM = group;
    topGroupM->convertToMap(intParamMapM, strParamMapM);


    return 0;
}

//-----------------------------------------------------------------------------

int ConfigCenter::saveXml(const std::string& theXmlPath)
{
    if (!topGroupM || theXmlPath.empty())
    {
        return -1;
    }
    xml_document<> doc;  

    xml_node<>* rot = doc.allocate_node(node_pi,doc.allocate_string("xml version='1.0' encoding='utf-8'"));
    doc.append_node(rot);

    xml_node<>* node =  doc.allocate_node(node_element, TOP_XMLNODE_NAME.c_str(),NULL);  
    doc.append_node(node);

    topGroupM->refreshFromMap(intParamMapM, strParamMapM);
    node->append_node(topGroupM->genNode(&doc));

    std::ofstream out(theXmlPath.c_str());
    out  <<  doc;
    out.close();
    return 0;
}

//-----------------------------------------------------------------------------

int ConfigCenter::get(const std::string& theKey, const int theDefault)
{
    IntParamMap::iterator it = intParamMapM.find(theKey);
    if (it != intParamMapM.end())
    {
        return it->second.get();
    }
    else
    {
        return theDefault;
    }
}

//-----------------------------------------------------------------------------

void ConfigCenter::set(const std::string& theKey, const int theValue)
{
    IntParamMap::iterator it = intParamMapM.find(theKey);
    if (it != intParamMapM.end())
    {
        it->second.set(theValue);
    }
    else
    {
        CFG_ERROR("config not found:" << theKey);
    }
}

//-----------------------------------------------------------------------------

void ConfigCenter::setInt(const std::string& theKey, const std::string& theValue)
{
    IntParamMap::iterator it = intParamMapM.find(theKey);
    if (it != intParamMapM.end())
    {
        it->second.set(theValue);
    }
    else
    {
        CFG_ERROR("config not found:" << theKey);
    }
}

//-----------------------------------------------------------------------------

const std::string ConfigCenter::get(const std::string& theKey, const std::string& theDefault)
{
    StringParamMap::iterator it = strParamMapM.find(theKey);
    if (it != strParamMapM.end())
    {
        return it->second.get();
    }
    else
    {
        return theDefault;
    }
}

//-----------------------------------------------------------------------------

void ConfigCenter::set(const std::string& theKey, const std::string& theValue)
{
    StringParamMap::iterator it = strParamMapM.find(theKey);
    if (it != strParamMapM.end())
    {
        it->second.set(theValue);
    }
    else
    {
        CFG_ERROR("config not found:" << theKey);
    }
}

