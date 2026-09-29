


#include "GameCore.h"

IMPLEMENT_MODULE(GameCore, GameCore)

void GameCore::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("? GameCore module initialized."));
}

void GameCore::ShutdownModule()
{
	UE_LOG(LogTemp, Log, TEXT("?? GameCore module shut down."));
}

GameCore::GameCore()
{
}

GameCore::~GameCore()
{
}
