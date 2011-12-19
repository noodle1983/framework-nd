#ifndef PROCESSORSENSOR_H
#define PROCESSORSENSOR_H

#include "ICmdHandler.h"
#include "Singleton.hpp"
struct event;

namespace Net
{
namespace Protocol
{
    class ProcessorSensor: public ICmdHandler
    {
	public:
		ProcessorSensor();
		virtual ~ProcessorSensor();
        //////////////////desc intent///////////////////////////////////////|
		virtual const char* getDesc() {return "prcstat                       show processor info.";};
		virtual const char* getPrompt(){return "";};
		virtual void handle(TelnetCmdManager* theManager, CmdArgsList& theArgs);

		void stat();
		void addTimer();
	private:
		bool isRunningM;
		struct event* timeoutEvtM;
		TelnetCmdManager* telnetManagerM;
		int statCountM;
		int intervalM;

    };
    typedef class DesignPattern::Singleton<ProcessorSensor> ProcessorSensorSingleton; 
}
}

#endif /* PROCESSORSENSOR_H */

