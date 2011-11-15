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
        static const std::string TYPE_INT;
        static const std::string TYPE_STRING;

        const std::string& getName(){return nameM;}
        const std::string& getId(){return idM;}
        const std::string& getDesc(){return descM;}
        const std::string& getValueType(){return valueTypeM;}
        const std::string& getValueRange(){return valueRangeM;}
        const std::string& getDefaultValue(){return defaultValueM;}
        const std::string& getValue(){return valueM;}

        void setName(const std::string& theName){nameM = theName;}
        void setId(const std::string& theId){idM = theId;}
        void setDesc(const std::string& theDesc){descM = theDesc;}
        void setValueType(const std::string& theType){valueTypeM = theType;}
        void setValueRange(const std::string& theRange){valueRangeM = theRange;}
        void setDefaultValue(const std::string& theDefaultValue){defaultValueM = theDefaultValue;}
        void setValue(const std::string& theValue){valueM = theValue;}
        void setValue(const int theValue);
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

