#include <config/xf-config.h>

#if (USE_XF_RESOURCE_FACTORY_DEFAULT_IMPLEMENTATION != 0)

#include "resourcefactory-default.h"
#include "mutex-default.h"
// TODO: Implement code for XFResourceFactoryPort class
interface::XFResourceFactory * interface::XFResourceFactory::getInstance()
{
    return XFResourceFactoryDefault::getInstance();
}

interface::XFResourceFactory *XFResourceFactoryDefault::getInstance()
{
    static XFResourceFactoryDefault xfResourceFactoryPort;
    return &xfResourceFactoryPort;
}

interface::XFDispatcher *XFResourceFactoryDefault::getDefaultDispatcher()
{
    static XFDispatcherDefault defDisp;
    return &defDisp;
}

interface::XFDispatcher *XFResourceFactoryDefault::createDispatcher()
{
    return getDefaultDispatcher();
}

interface::XFThread *XFResourceFactoryDefault::createThread(interface::XFThreadEntryPointProvider *pProvider, interface::XFThread::EntryMethodBody entryMethod, const char *threadName, const uint32_t stackSize)
{
	return nullptr;
}

interface::XFMutex *XFResourceFactoryDefault::createMutex()
{
	return interface::XFMutex::create(); ////////////PAAAAAAAAAAASZUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUR
}

#endif // USE_XF_RESOURCE_FACTORY_DEFAULT_IMPLEMENTATION
