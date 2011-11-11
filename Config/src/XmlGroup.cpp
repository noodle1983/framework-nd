#include "XmlGroup.h"
#include "Log.h"

using namespace Config;
using namespace rapidxml;

const std::string XmlGroup::GROUP_TAG = "Group";
const std::string XmlGroup::NAME_ATTR_TAG = "name";
//-----------------------------------------------------------------------------

int XmlGroup::parse(xml_node<>* theNode)
{
    if (theNode->name() != GROUP_TAG)    
    {
        ERROR("invalid group tag:" << theNode->name());
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
            ERROR("invalid attribute tag:" << attr->name());
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
            ERROR("invalid sub-node tag:" << node->name());
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

