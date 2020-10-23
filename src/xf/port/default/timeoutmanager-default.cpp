
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
    for(it=_timeouts.begin();it!=_timeouts.end();it++) //We look all the list
    {
        if((*it)->getId()==timeoutId && (*it)->getBehavior()==pReactive){ //If its the timeout that we're looking for
            //toDelete = *it;
            it = _timeouts.erase(it); //We delete the timeout
            //delete toDelete;

            break;
        }
    }
    _pMutex->unlock();
}

void XFTimeoutManagerDefault::tick()
{
    _pMutex->lock();
    if(_timeouts.size()==0){ //If size == 0 we don't do anything
        _pMutex->unlock();
        return;
    }
    TimeoutList::const_iterator  it = _timeouts.begin();
    (*it)->substractFromRelTicks(XF_tickIntervalInMilliseconds());
    if((*it)->getRelTicks()<=0){ //If the new remaining tick of the timeouts is negative of null, we send the timeout to the dispatcher
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
    _pMutex->lock(); //Lock the mutex
    if (_timeouts.size()==0){ //If there nothing in the timeouts list, we add a timeout in the list
        _timeouts.push_back(pNewTimeout);
        _pMutex->unlock();
        return;
    }

    TimeoutList::const_iterator  it; // Construction d’un itérateur pour parcourir les éléments de la liste
    for(it=_timeouts.begin();it!=_timeouts.end();it++) //We point to each timeout of the list
    {
        if ((*it)->getRelTicks()>pNewTimeout->getRelTicks()){ //If timeouts in the list is bigger than the new timeout
            _timeouts.insert(it,pNewTimeout); //We insert the timeout before the timeout
             if(pNewTimeout!=nullptr){
                (*it)->substractFromRelTicks(pNewTimeout->getRelTicks()); //We decrement the remaining ticks
             }
            _pMutex->unlock();
            return; //We return to break and don't do the next statement
        }else { //Else we decrement the remaining ticks of the timeouts
            if(pNewTimeout!=nullptr){
                pNewTimeout->substractFromRelTicks((*it)->getRelTicks());
            }
        }

    }
    _timeouts.push_back(pNewTimeout); //Push at the end of the list

    _pMutex->unlock();
}

void XFTimeoutManagerDefault::returnTimeout(XFTimeout *pTimeout)
{
    pTimeout->getBehavior()->pushEvent(pTimeout);
}
