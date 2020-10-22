#include "xf/initialevent.h"

XFInitialEvent::XFInitialEvent() : XFEvent(XFEventType::Initial, 0, this->getBehavior())
{

}

bool XFInitialEvent::deleteAfterConsume() const
{
    return true;
}
