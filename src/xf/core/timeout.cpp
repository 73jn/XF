#include "xf/timeout.h"

XFTimeout::XFTimeout(int id, int interval, interface::XFReactive *pBehavior) : XFEvent(XFEventType::Timeout, id, pBehavior)
{
    _interval = interval;
    _relTicks = interval;
}

bool XFTimeout::operator ==(const XFTimeout &timeout) const
{
    return ((_interval == timeout._interval) && (_relTicks == timeout._relTicks) && (_id == timeout._id) && (_pBehavior == timeout._pBehavior));
}

bool XFTimeout::deleteAfterConsume() const
{
    return true;
}

void XFTimeout::substractFromRelTicks(int ticksToSubstract)
{
    if (_relTicks > ticksToSubstract){
        _relTicks -= ticksToSubstract;
    }
    else {
        _relTicks = 0;
    }

}
