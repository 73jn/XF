#include <cassert>
#include <xf/behavior.h>
#include "xf/xf.h"
#include "xf/interface/resourcefactory.h"
#include "xf/interface/dispatcher.h"
#include "xf/timeout.h"
#include "xf/initialevent.h"
#include "xf/behavior.h"

using interface::XFResourceFactory;

XFBehavior::XFBehavior(bool ownDispatcher)
{
    this->_hasOwnDispatcher = ownDispatcher; //Set the boolean owndispatcher
    if(ownDispatcher){ //If he has his own dispatcher
        _pDispatcher = XFResourceFactory::getInstance()->createDispatcher(); //We create a new dispatcher and _pDispatcher point on it
    }
    else
    {
        _pDispatcher = XFResourceFactory::getInstance()->getDefaultDispatcher(); //We give to _pDispatcher the instance of the default dispatcher
    }
}

XFBehavior::~XFBehavior()
{
    if(_hasOwnDispatcher){
        delete _pDispatcher;
    }
}

void XFBehavior::startBehavior()
{
    GEN(XFInitialEvent); //= pushEvent(new event)
}

void XFBehavior::pushEvent(XFEvent *pEvent)
{
    pEvent->setBehavior(this); //Set the behaviour
    this->getDispatcher()->pushEvent(pEvent); //Send it to dispatcher
}

bool XFBehavior::deleteOnTerminate() const
{
    return deleteOnTerminate_;
}

void XFBehavior::setDeleteOnTerminate(bool deleteBehaviour)
{
    this->deleteOnTerminate_ = deleteBehaviour;
}

const XFEvent *XFBehavior::getCurrentEvent() const
{
    return _pCurrentEvent;
}

interface::XFDispatcher *XFBehavior::getDispatcher()
{
    return this->_pDispatcher;
}

const XFTimeout *XFBehavior::getCurrentTimeout()
{
    if(getCurrentEvent()->getEventType()==XFEvent::Timeout){ //Return XFEvent cast in XFTimeout if its a timeout
        return ((XFTimeout*)getCurrentEvent());
    } else{
        return nullptr; //Else we return a nullptr
    }
}

void XFBehavior::scheduleTimeout(int timeoutId, int interval) {
    getDispatcher()->scheduleTimeout(timeoutId, interval, this); //Send the timeout to the dispatcher
}

void XFBehavior::unscheduleTimeout(int timeoutId) {
    getDispatcher()->unscheduleTimeout(timeoutId, this); //Send to the dispatcher the timeout that we want to unschedule
}

void XFBehavior::setCurrentEvent(const XFEvent *pEvent)
{
    _pCurrentEvent = pEvent;
}

interface::XFReactive::TerminateBehavior XFBehavior::process(const XFEvent *pEvent)
{
    setCurrentEvent(pEvent); //We set the current event
    if(processEvent() == XFEventStatus::Terminate) //If its a terminate event we return deleteOnTerminate_
    {
        return deleteOnTerminate_;
    }
    else
    {
        return false;
    }
}
