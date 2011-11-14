#ifndef XMLGROUP_H
#define XMLGROUP_H

#include "XmlParameter.h"
#include "IntParameter.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <vector>
#include <string>

namespace Config
{
    class XmlGroup 
    {
    public:
        XmlGroup(){};
        ~XmlGroup(){};

        int parse(rapidxml::xml_node<>* theGroupNode);
        rapidxml::xml_node<>* genNode(rapidxml::xml_document<>* theDoc);
        int convert(
                IntParamMap& theIntParamMap);


        static const std::string GROUP_TAG;
        static const std::string NAME_ATTR_TAG;
    private:
        std::string nameM;
        std::vector<XmlGroup> subGroupsM;
        std::vector<XmlParameter> paramsM;
    };
}

#endif /* XMLGROUP_H */

