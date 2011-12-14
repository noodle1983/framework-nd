#ifndef CONFIGCENTER_H
#define CONFIGCENTER_H

#include "IntParameter.h"
#include "StringParameter.h"

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <string>

namespace Config
{
    class XmlGroup;
    class ConfigCenter;

    typedef boost::shared_ptr<ConfigCenter> ConfigCenterPtr;
    typedef std::map<std::string, IntParameter> IntParamMap;
    typedef std::map<std::string, StringParameter> StringParamMap;

    typedef IntParameter::Watcher IntWatcher;
    typedef StringParameter::Watcher StringWatcher;

    class ConfigCenter
    {
    public:
        ~ConfigCenter();
        /**
         * static ConfigCenter* instance()
         * Description: the instance will load the xml file "config.xml".
         *              if the other files should be used, please call loadConfig in main.
         *
         *              The instance is a smart pointer, it will live longer than or as long as the session does.
         *              The instance will be replaced while loadConfig is called.
         *              Once the intance is got, the content of instance() will not changed any more.
         *
         *              There is no lock while accessing the instance's get/set method.
         *              The lock lies in the instance() method.
         *              So the performance reason, the user may consider saving the instance to a session. 
         */
        static ConfigCenterPtr instance();
        /**
         * static void loadConfig(const std::string theInputXmlFile)
         * Description: it will safely replace the ConfigCenterPtr::instance
         *              with the data stored in the input xml file.
         */
        static void loadConfig(const std::string& theInputXmlFile = "config.xml");

        void borrowFrom(ConfigCenterPtr theOtherConfig);

        int get(const std::string& theKey, const int theDefault);
        void set(const std::string& theKey, const int theValue);
        void setInt(const std::string& theKey, const std::string& theValue);
        void registValueWatcher(
                const std::string& theKey,
                void* theWatcherKey,
                IntWatcher theWatcher);
        void unregistIntValueWatcher(
                const std::string& theKey,
                void* theWatcherKey);

        const std::string get(const std::string& theKey, const std::string& theDefault);
        void set(const std::string& theKey, const std::string& theValue);
        void registValueWatcher(
                const std::string& theKey,
                void* theWatcherKey,
                StringWatcher theWatcher);
        void unregistStrValueWatcher(
                const std::string& theKey,
                void* theWatcherKey);

        int loadXml(const std::string& theXmlPath);
        int saveXml(const std::string& theXmlPath);

        static const std::string TOP_XMLNODE_NAME;
    private:
        static ConfigCenterPtr configCenterM;
        static boost::shared_mutex configCenterMutexM;

        ConfigCenter();
        XmlGroup*   topGroupM;
        IntParamMap intParamMapM;
        StringParamMap strParamMapM;
    };
}

#endif /* CONFIGCENTER_H */

