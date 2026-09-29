

#pragma once

#include "CoreMinimal.h"
/**
 * 
 */
class GAMECORE_API GameCore : public IModuleInterface
{
public:
	GameCore();
	~GameCore();
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
