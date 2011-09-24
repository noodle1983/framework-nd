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

        inline int get(const std::string theKey, const int theDefault = 0)
        {
            return configDataM.get(theKey, theDefault);
        }
        inline std::string get(const std::string theKey, const std::string theDefault = "")
        {
            return configDataM.get<std::string>(theKey, theDefault);
        }

    private:
        ConfigCenter();
        void init();

        static ConfigCenter* configCenterM;

        /*config data implement*/
        boost::property_tree::ptree configDataM;
    };
}

#endif /* CONFIG_H */

