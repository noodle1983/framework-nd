#ifndef APP_H
#define APP_H

#include "Singleton.hpp"
#include "client/linux/handler/exception_handler.h"

namespace Utility
{
    class App 
    {
    public:
        friend class DesignPattern::Singleton<App>;
        ~App();

        void setRunInBackground();
        void setDumpWhenCrash();

    private:
        App();
        static bool dumpCallback(
            const char* dump_path,
            const char* minidump_id,
            void* context,
            bool succeeded);

    private:
        google_breakpad::ExceptionHandler* crashDumpHandlerM;
    };

    typedef DesignPattern::Singleton<App> AppSingleton;
}

#endif /* APP_H */

