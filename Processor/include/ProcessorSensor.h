#ifndef PROCESSORSENSOR_H
#define PROCESSORSENSOR_H

#include "ICmdHandler.h"
#include "Singleton.hpp"
struct event;

namespace Net
{
namespace Protocol
{
    class ProcessorSensor;
    struct ProcessorSensorData
    {
        ProcessorSensorData()
            : telnetManagerM(NULL)
            , statCountM(0)
            , intervalM(3)
            , sensorM(NULL)
        {}
        ~ProcessorSensorData(){}
		struct event* timeoutEvtM;
		TelnetCmdManager* telnetManagerM;
		int statCountM;
		int intervalM;
        ProcessorSensor* sensorM;
    };

    class ProcessorSensor: public ICmdHandler
    {
	public:
		ProcessorSensor();
		virtual ~ProcessorSensor();
        //////////////////desc intent///////////////////////////////////////|
		virtual const char* getDesc() {return "prcstat                       show processor info.";};
		virtual const char* getPrompt(){return "";};
		virtual void handle(TelnetCmdManager* theManager, CmdArgsList& theArgs);

        virtual void* newCmdSessionData(){return new ProcessorSensorData;}
        virtual void  freeCmdSessionData(void* theData){delete (ProcessorSensorData*)theData;} 
		virtual void handle(TelnetCmdManager* theManager, CmdArgsList& theArgs, void* theSessionData);

		void stat(ProcessorSensorData* theData);
		void addTimer(ProcessorSensorData* theData);
    };
    typedef class DesignPattern::Singleton<ProcessorSensor> ProcessorSensorSingleton; 
}
}

#endif /* PROCESSORSENSOR_H */

