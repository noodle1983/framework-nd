#include "XmlGroup.h"
#include "Log.h"

using namespace Config;
using namespace rapidxml;

const std::string XmlGroup::GROUP_TAG = "Group";
const std::string XmlGroup::NAME_ATTR_TAG = "name";
//-----------------------------------------------------------------------------

int XmlGroup::parse(xml_node<>* theNode)
{
    if (NULL == theNode)
    {
        CFG_ERROR("invalid group.");
        return -1;
    }

    if (theNode->name() != GROUP_TAG)    
    {
        CFG_ERROR("invalid group tag:" << theNode->name());
        return -1;
    }

    xml_attribute<>* attr = theNode->first_attribute();  
    for (; NULL != attr; attr = attr->next_attribute()) 
    {
        if (attr->name() == NAME_ATTR_TAG)
        {
            nameM = attr->value();
        }
        else
        {
            CFG_ERROR("invalid attribute tag:" << attr->name());
            return -1;
        }
    }

    xml_node<>* node = theNode->first_node();  
    for (; NULL != node; node = node->next_sibling()) 
    {
        if (node->name() == GROUP_TAG)
        {
            XmlGroup group;
            if (0 != group.parse(node))
            {
                return -1;
            }
            subGroupsM.push_back(group);
        }
        else if (node->name() == XmlParameter::PARAMETER_TAG)
        {
            XmlParameter param;
            if (0 != param.parse(node))
            {
                return -1;
            }
            paramsM.push_back(param);
        }
        else
        {
            CFG_ERROR("invalid sub-node tag:" << node->name());
            return -1;
        }
    }
    return 0;
}

//-----------------------------------------------------------------------------

xml_node<>* XmlGroup::genNode(xml_document<>* theDoc)
{
    xml_node<>* group =  theDoc->allocate_node(node_element, GROUP_TAG.c_str(), NULL);  
    group->append_attribute(theDoc->allocate_attribute(NAME_ATTR_TAG.c_str(), nameM.c_str()));

    std::vector<XmlGroup>::iterator subGroupIt = subGroupsM.begin();
    for (; subGroupIt != subGroupsM.end(); subGroupIt++)
    {
        group->append_node(subGroupIt->genNode(theDoc));
    }

    std::vector<XmlParameter>::iterator paramIt = paramsM.begin();
    for (; paramIt != paramsM.end(); paramIt++)
    {
        group->append_node(paramIt->genNode(theDoc));
    }

    return group;
}

//-----------------------------------------------------------------------------

int XmlGroup::convertToMap(
        IntParamMap& theIntParamMap,
        StringParamMap& theStringParamMap)
{
    std::vector<XmlGroup>::iterator subGroupIt = subGroupsM.begin();
    for (; subGroupIt != subGroupsM.end(); subGroupIt++)
    {
       subGroupIt->convertToMap(theIntParamMap, theStringParamMap);
    }

    std::vector<XmlParameter>::iterator paramIt = paramsM.begin();
    for (; paramIt != paramsM.end(); paramIt++)
    {
        const std::string& type = paramIt->getValueType();
        if (type == XmlParameter::TYPE_INT)
        {
            IntParamMap::iterator it = theIntParamMap.find(paramIt->getId());
            if (it == theIntParamMap.end())
            {
                IntParameter intParam(paramIt->getName());
                intParam.set(paramIt->getValue());
                intParam.setRange(paramIt->getValueRange());
                theIntParamMap.insert(std::pair<std::string, IntParameter>(paramIt->getId(), intParam));
            }
            else
            {
                it->second.set(paramIt->getValue());
                it->second.setRange(paramIt->getValueRange());
            }

        }
        else if (type == XmlParameter::TYPE_STRING)
        {
            StringParamMap::iterator it = theStringParamMap.find(paramIt->getId());
            if (it == theStringParamMap.end())
            {
                StringParameter strParam(paramIt->getName());
                strParam.set(paramIt->getValue());
                theStringParamMap.insert(std::pair<std::string, StringParameter>(paramIt->getId(), strParam));
            }
            else
            {
                it->second.set(paramIt->getValue());
            }

        }
    }

    return 0;

}

//-----------------------------------------------------------------------------

void XmlGroup::refreshFromMap(
                IntParamMap& theIntParamMap,
                StringParamMap& theStringParamMap)
{
    std::vector<XmlGroup>::iterator subGroupIt = subGroupsM.begin();
    for (; subGroupIt != subGroupsM.end(); subGroupIt++)
    {
       subGroupIt->refreshFromMap(theIntParamMap, theStringParamMap);
    }

    std::vector<XmlParameter>::iterator paramIt = paramsM.begin();
    for (; paramIt != paramsM.end(); paramIt++)
    {
        const std::string& type = paramIt->getValueType();
        if (type == XmlParameter::TYPE_INT)
        {
            IntParamMap::iterator it = theIntParamMap.find(paramIt->getId());
            if (it == theIntParamMap.end())
            {
                paramIt->setValue(-1);
            }
            else
            {
                paramIt->setValue(it->second.get());
            }
        }
        else if (type == XmlParameter::TYPE_STRING)
        {
            StringParamMap::iterator it = theStringParamMap.find(paramIt->getId());
            if (it == theStringParamMap.end())
            {
                paramIt->setValue("");
            }
            else
            {
                paramIt->setValue(it->second.get());
            }
        }
    }
}

//-----------------------------------------------------------------------------

