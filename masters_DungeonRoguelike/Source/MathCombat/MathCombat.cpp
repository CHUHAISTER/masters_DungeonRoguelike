


#include "MathCombat.h"


IMPLEMENT_MODULE(MathCombat, MathCombat)


MathCombat::MathCombat()
{
}

MathCombat::~MathCombat()
{
}


void MathCombat::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("?? MathCombat module initialized."));
}

void MathCombat::ShutdownModule()
{
	UE_LOG(LogTemp, Log, TEXT("?? MathCombat module shut down."));
}



