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
    this->_hasOwnDispatcher = ownDispatcher;
    if(ownDispatcher){
        _pDispatcher = XFResourceFactory::getInstance()->createDispatcher();
    }
    else
    {
        _pDispatcher = XFResourceFactory::getInstance()->getDefaultDispatcher();
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
    GEN(XFInitialEvent);
    //XFEvent *ev = new XFEvent(XFEvent::Initial,0,this);
    //_pDispatcher->pushEvent(new XFEvent(XFEvent::Initial,0,this));


}

void XFBehavior::pushEvent(XFEvent *pEvent)
{
    pEvent->setBehavior(this);
    this->getDispatcher()->pushEvent(pEvent);
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
    //this->getCurrentEvent(); //NOT SUREEEEEEEEEEEEEEEEE
    if(getCurrentEvent()->getEventType()==XFEvent::Timeout){
        return ((XFTimeout*)getCurrentEvent());
    } else{
        return nullptr;
    }
}

void XFBehavior::scheduleTimeout(int timeoutId, int interval) {
    getDispatcher()->scheduleTimeout(timeoutId, interval, this);
}

void XFBehavior::unscheduleTimeout(int timeoutId) {
    getDispatcher()->unscheduleTimeout(timeoutId, this);
}

void XFBehavior::setCurrentEvent(const XFEvent *pEvent)
{
    _pCurrentEvent = pEvent;
}

interface::XFReactive::TerminateBehavior XFBehavior::process(const XFEvent *pEvent)
{
    setCurrentEvent(pEvent);
    if(processEvent() == XFEventStatus::Terminate)
    {
        return deleteOnTerminate_;
    }
    else
    {
        return false;
    }
}
