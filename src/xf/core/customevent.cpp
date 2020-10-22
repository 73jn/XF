#include "xf/customevent.h"

XFCustomEvent::XFCustomEvent(int id, interface::XFReactive *pBehavior) : XFEvent(XFEventType::Event, id, pBehavior)
{

}
