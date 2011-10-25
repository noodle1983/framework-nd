#ifndef CONFIG_H
#define CONFIG_H

#include <boost/property_tree/ptree.hpp>
#include <string>

namespace Config
{
    class ConfigCenter
    {
    public:
        ~ConfigCenter();
        static ConfigCenter* instance();

        template<typename ValueType>
        inline ValueType get(const std::string& theKey, const ValueType theDefault)
        {
            return configDataM.get<ValueType>(theKey, theDefault);
        }
        template<typename ValueType>
        inline void set(const std::string& theKey, const ValueType& theValue)
        {
            configDataM.put(theKey, theValue);
        }

        void loadXml(const std::string theXmlPath);
        void saveXml(const std::string theXmlPath);

    private:
        ConfigCenter();
        void init();

        static ConfigCenter* configCenterM;

        /*config data implement*/
        boost::property_tree::ptree configDataM;
    };
}

#endif /* CONFIG_H */

