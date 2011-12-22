#include "ProcessorSensor.h"
#include "TelnetCmdManager.h"

using namespace Net::Protocol;
//-----------------------------------------------------------------------------

ProcessorSensor::ProcessorSensor()
{
}

//-----------------------------------------------------------------------------

ProcessorSensor::~ProcessorSensor()
{

}

//-----------------------------------------------------------------------------

void ProcessorSensor::handle(TelnetCmdManager* theManager, CmdArgsList& theArgs)
{
    ProcessorSensorData* data = (ProcessorSensorData*)theManager->takeOverInputHandler(this);
    data->telnetManagerM = theManager;
    data->statCountM = 0;
    data->intervalM = 3;
    data->timeoutEvtM = NULL;
    data->sensorM = this;
    stat(data);
}

//-----------------------------------------------------------------------------

void ProcessorSensor::handle(
        TelnetCmdManager* theManager, 
        CmdArgsList& theArgs, 
        void* theSessionData)
{
    ProcessorSensorData* data = (ProcessorSensorData*) theSessionData;
    if (data->timeoutEvtM)
    {
        theManager->cancelLocalTimer(data->timeoutEvtM);
        data->timeoutEvtM = NULL;
    }
    data->telnetManagerM = NULL;
    data->statCountM = 0;
    data->intervalM = 3;
    theManager->exitCurCmd();
}

//-----------------------------------------------------------------------------

void ProcessorSensor::stat(ProcessorSensorData* theData)
{
    assert(theData != NULL);
	char str[] = "hahahha          haha\r\n";
	theData->telnetManagerM->send(str, strlen(str));

	theData->statCountM++;
	addTimer(theData);
}

//-----------------------------------------------------------------------------

static void onProcessorSensorTimeOut(int theFd, short theEvt, void *theArg)
{
    ProcessorSensorData* data = (ProcessorSensorData*)theArg;
	data->timeoutEvtM = NULL;
    data->sensorM->stat(data);
}

void ProcessorSensor::addTimer(ProcessorSensorData* theData)
{
	if (theData->timeoutEvtM)
	{
		theData->telnetManagerM->cancelLocalTimer(theData->timeoutEvtM);
		theData->timeoutEvtM = NULL;
	}
    struct timeval tv;
    tv.tv_sec = theData->intervalM;
    tv.tv_usec = 0;
    theData->timeoutEvtM = theData->telnetManagerM->addLocalTimer(tv, onProcessorSensorTimeOut, theData);
}

//-----------------------------------------------------------------------------

