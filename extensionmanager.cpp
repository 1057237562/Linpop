#include "extensionmanager.h"

ExtensionManager::ExtensionManager()
{
    QSharedMemory interface("Interface");
    interface.create(1024);
}
