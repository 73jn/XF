
#include <config/xf-config.h>

#if (USE_XF_TIMEOUTMANAGER_DEFAULT_IMPLEMENTATION != 0)

#include <cassert>
#include "xf/interface/reactive.h"
#include "xf/interface/mutex.h"
#include "port-functions.h"
#include "timeoutmanager-default.h"

using interface::XFMutex;

interface::XFTimeoutManager * interface::XFTimeoutManager::getInstance()
{
    return XFTimeoutManagerDefault::getInstance();
}

// TODO: Implement code for XFTimeoutManagerDefault class

#endif // USE_XF_TIMEOUTMANAGER_DEFAULT_IMPLEMENTATION

interface::XFTimeoutManager *XFTimeoutManagerDefault::getInstance()
{
    static XFTimeoutManagerDefault timeoutmanager;
    return &timeoutmanager;
}

XFTimeoutManagerDefault::~XFTimeoutManagerDefault()
{
    _timeouts.clear();
    delete _pMutex;
}

void XFTimeoutManagerDefault::start()
{
    XF_startTimeoutManagerTimer(10);
}

void XFTimeoutManagerDefault::scheduleTimeout(int32_t timeoutId, int32_t interval, interface::XFReactive *pReactive)
{
    addTimeout(new XFTimeout(timeoutId, interval, pReactive));
}

void XFTimeoutManagerDefault::unscheduleTimeout(int32_t timeoutId, interface::XFReactive *pReactive)
{
    _pMutex->lock();
    TimeoutList::const_iterator  it; // Construction d’un itérateur pour parcourir les éléments de la liste
    XFTimeout *toDelete;
    for(it=_timeouts.begin();it!=_timeouts.end();it++)
    {
        if((*it)->getId()==timeoutId && (*it)->getBehavior()==pReactive){
            //toDelete = *it;
            it = _timeouts.erase(it);
            //delete toDelete;

            break;
        }
    }
    _pMutex->unlock();
}

void XFTimeoutManagerDefault::tick()
{
    _pMutex->lock();
    if(_timeouts.size()==0){
        _pMutex->unlock();
        return;
    }
    TimeoutList::const_iterator  it = _timeouts.begin();
    (*it)->substractFromRelTicks(XF_tickIntervalInMilliseconds());
    if((*it)->getRelTicks()<=0){
        //Send to dispatcher
        returnTimeout((*it));


        //Delete timeout
        //XFTimeout *toDelete;
        //toDelete = *it;
        it = _timeouts.erase(it);
        //delete toDelete;
    }
     _pMutex->unlock();
}

XFTimeoutManagerDefault::XFTimeoutManagerDefault()
{
    _timeouts.clear();
    _pMutex = interface::XFMutex::create(); //NOT SUUUUUUUUUUUUUUURE
}

void XFTimeoutManagerDefault::addTimeout(XFTimeout *pNewTimeout)
{
    _pMutex->lock();
    if (_timeouts.size()==0){
        _timeouts.push_back(pNewTimeout);
        _pMutex->unlock();
        return;
    }

    TimeoutList::const_iterator  it; // Construction d’un itérateur pour parcourir les éléments de la liste
    for(it=_timeouts.begin();it!=_timeouts.end();it++)
    {
        if ((*it)->getRelTicks()>pNewTimeout->getRelTicks()){
            _timeouts.insert(it,pNewTimeout);
             if(pNewTimeout!=nullptr){
                (*it)->substractFromRelTicks(pNewTimeout->getRelTicks());
             }
            _pMutex->unlock();
            return;
        }else {
            if(pNewTimeout!=nullptr){
                pNewTimeout->substractFromRelTicks((*it)->getRelTicks());
            }
        }

    }
    _timeouts.push_back(pNewTimeout);

    _pMutex->unlock();
}

void XFTimeoutManagerDefault::returnTimeout(XFTimeout *pTimeout)
{
    pTimeout->getBehavior()->pushEvent(pTimeout);
}
