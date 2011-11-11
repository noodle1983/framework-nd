#ifndef CONFIGXMLPARAMETER_H
#define CONFIGXMLPARAMETER_H

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <vector>
#include <string>

namespace Config 
{
    class XmlParameter
    {
    public:    
        XmlParameter(){};
        ~XmlParameter(){};

        int parse(rapidxml::xml_node<>* theNode);
        rapidxml::xml_node<>* genNode(rapidxml::xml_document<>* theDoc);

        static const std::string PARAMETER_TAG;
        static const std::string NAME_ATTR_TAG;
        static const std::string ID_ATTR_TAG;
        static const std::string DESCRIPTION_TAG;
        static const std::string VALUETYPE_TAG;
        static const std::string VALUERANGE_TAG;
        static const std::string DEFAULTVALUE_TAG;
        static const std::string VALUE_TAG;
    private:
        std::string nameM;
        std::string idM;
        std::string descM;
        std::string valueTypeM;
        std::string valueRangeM;
        std::string defaultValueM;
        std::string valueM;
    };

}

#endif /* CONFIGXMLPARAMETER_H */

