// Gihyeon's Inventory Plugin

#pragma once

#include "Modules/ModuleManager.h"

class FInventoryModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
