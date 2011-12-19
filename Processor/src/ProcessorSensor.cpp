#include "ProcessorSensor.h"
#include "TelnetCmdManager.h"

using namespace Net::Protocol;
//-----------------------------------------------------------------------------

ProcessorSensor::ProcessorSensor()
	: isRunningM(false)
	, telnetManagerM(NULL)
	, statCountM(0)
	, intervalM(3)
{
}

//-----------------------------------------------------------------------------

ProcessorSensor::~ProcessorSensor()
{

}

//-----------------------------------------------------------------------------

void ProcessorSensor::handle(TelnetCmdManager* theManager, CmdArgsList& theArgs)
{
	if (isRunningM)
	{
		isRunningM = false;
		if (timeoutEvtM)
		{
			theManager->cancelLocalTimer(timeoutEvtM);
			timeoutEvtM = NULL;
		}
		telnetManagerM = NULL;
		statCountM = 0;
		intervalM = 3;
	}
	else
	{
		isRunningM = true;
		statCountM = 0;
		intervalM = 3;
		telnetManagerM = theManager;
		telnetManagerM->takeOverInputHandler(this);
		stat();
	}
}

//-----------------------------------------------------------------------------

void ProcessorSensor::stat()
{
	char str[] = "hahahha          haha\r\n";
	telnetManagerM->send(str, strlen(str));

	statCountM++;
	addTimer();
}

//-----------------------------------------------------------------------------

static void onProcessorSensorTimeOut(int theFd, short theEvt, void *theArg)
{
    ProcessorSensor* sensor = (ProcessorSensor*)theArg;
    sensor->stat();
}

void ProcessorSensor::addTimer()
{
	if (timeoutEvtM)
	{
		telnetManagerM->cancelLocalTimer(timeoutEvtM);
		timeoutEvtM = NULL;
	}
    struct timeval tv;
    tv.tv_sec = intervalM;
    tv.tv_usec = 0;
    timeoutEvtM = telnetManagerM->addLocalTimer(tv, onProcessorSensorTimeOut, (void*)this);
}

//-----------------------------------------------------------------------------

