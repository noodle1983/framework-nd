#include "XmlParameter.h"
#include "Log.h"
#include <sstream>

using namespace Config;
using namespace rapidxml;


const std::string XmlParameter::PARAMETER_TAG    = "Parameter";
const std::string XmlParameter::NAME_ATTR_TAG    = "name";
const std::string XmlParameter::ID_ATTR_TAG      = "id";
const std::string XmlParameter::DESCRIPTION_TAG  = "Description";
const std::string XmlParameter::VALUETYPE_TAG    = "ValueType";
const std::string XmlParameter::VALUERANGE_TAG   = "ValueRange";
const std::string XmlParameter::DEFAULTVALUE_TAG = "DefaultValue";
const std::string XmlParameter::VALUE_TAG        = "Value";
const std::string XmlParameter::TYPE_INT         = "Int";
const std::string XmlParameter::TYPE_STRING      = "String";

//-----------------------------------------------------------------------------

int XmlParameter::parse(xml_node<>* theNode)
{
    if (theNode->name() != PARAMETER_TAG)    
    {
        CFG_ERROR("invalid paramter tag:" << theNode->name());
        return -1;
    }

    xml_attribute<>* attr = theNode->first_attribute();  
    for (; NULL != attr; attr = attr->next_attribute()) 
    {
        if (attr->name() == NAME_ATTR_TAG)
        {
            nameM = attr->value();
        }
        else if (attr->name() == ID_ATTR_TAG)
        {
            idM = attr->value();
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
        if (node->name() == DESCRIPTION_TAG)
        {
            descM = node->value();
        }
        else if (node->name() == VALUETYPE_TAG)
        {
            valueTypeM = node->value();
        }
        else if (node->name() == VALUERANGE_TAG)
        {
            valueRangeM = node->value();
        }
        else if (node->name() == DEFAULTVALUE_TAG)
        {
            defaultValueM = node->value();
        }
        else if (node->name() == VALUE_TAG)
        {
            valueM = node->value();
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

xml_node<>* XmlParameter::genNode(xml_document<>* theDoc)
{
    xml_node<>* param =  theDoc->allocate_node(node_element, PARAMETER_TAG.c_str(), NULL);  
    param->append_attribute(theDoc->allocate_attribute(NAME_ATTR_TAG.c_str(), nameM.c_str()));
    param->append_attribute(theDoc->allocate_attribute(ID_ATTR_TAG.c_str(), idM.c_str()));

    param->append_node(
            theDoc->allocate_node(node_element, DESCRIPTION_TAG.c_str(), descM.c_str()));
    param->append_node(
            theDoc->allocate_node(node_element, VALUETYPE_TAG.c_str(), valueTypeM.c_str()));

    if (!valueRangeM.empty())
    {
        param->append_node(
                theDoc->allocate_node(node_element, VALUERANGE_TAG.c_str(), valueRangeM.c_str()));
    }
    param->append_node(
            theDoc->allocate_node(node_element, DEFAULTVALUE_TAG.c_str(), defaultValueM.c_str()));
    param->append_node(
            theDoc->allocate_node(node_element, VALUE_TAG.c_str(), valueM.c_str()));

    return param;
}


//-----------------------------------------------------------------------------

void XmlParameter::setValue(const int theValue)
{
    std::ostringstream ostr;
    ostr << theValue;
    setValue(ostr.str());
}
