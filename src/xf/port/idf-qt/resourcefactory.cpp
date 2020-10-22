#include <config/xf-config.h>

#if (USE_XF_PORT_IDF_QT_RESOURCE_FACTORY_IMPLEMENTATION != 0)

#include <QtGlobal>
#include "thread-default.h"
#include "default/dispatcher-active.h"
#include "xf/interface/mutex.h"
#include "resourcefactory.h"

using interface::XFMutex;

//static
interface::XFResourceFactory * interface::XFResourceFactory::getInstance()
{
    return XFResourceFactoryPort::getInstance();
}

// TODO: Implement code for XFResourceFactoryPort class

#endif // USE_XF_PORT_IDF_QT_RESOURCE_FACTORY_IMPLEMENTATION

interface::XFResourceFactory *XFResourceFactoryPort::getInstance()
{
    static XFResourceFactoryPort xfResourceFactoryPort;
    return &xfResourceFactoryPort;
}

interface::XFDispatcher *XFResourceFactoryPort::getDefaultDispatcher()
{
    static XFDispatcherActiveDefault defDisp;
    return &defDisp;
}

interface::XFDispatcher *XFResourceFactoryPort::createDispatcher()
{
    return getDefaultDispatcher();
}

interface::XFThread *XFResourceFactoryPort::createThread(interface::XFThreadEntryPointProvider *pProvider, interface::XFThread::EntryMethodBody entryMethod, const char *threadName, const uint32_t stackSize)
{
    XFThreadDefault* t1 = new XFThreadDefault(pProvider, entryMethod, threadName, stackSize);
    return t1;
}

interface::XFMutex *XFResourceFactoryPort::createMutex()
{
    return XFMutex::create(); ////////////PAAAAAAAAAAASZUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUR
}

XFResourceFactoryPort::XFResourceFactoryPort()
{

}

