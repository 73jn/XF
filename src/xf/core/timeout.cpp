#include "xf/timeout.h"

XFTimeout::XFTimeout(int id, int interval, interface::XFReactive *pBehavior) : XFEvent(XFEventType::Timeout, id, pBehavior)
{
    _interval = interval;
    _relTicks = interval;
}

bool XFTimeout::operator ==(const XFTimeout &timeout) const
{
    return ((_interval == timeout._interval) && (_relTicks == timeout._relTicks) && (_id == timeout._id) && (_pBehavior == timeout._pBehavior)); //Overload of operator ==
}

bool XFTimeout::deleteAfterConsume() const
{
    return true;
}

void XFTimeout::substractFromRelTicks(int ticksToSubstract)
{
    if (_relTicks > ticksToSubstract){ //Check if _reltick is negative after substract
        _relTicks -= ticksToSubstract; //If the subsract is going to be positive we substract
    }
    else {
        _relTicks = 0; //Else we set the relticks to 0
    }

}
