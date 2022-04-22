// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyFPSProjectGameMode.h"
#include "MyFPSProjectCharacter.h"
#include "FPSGun.h"
#include "UObject/ConstructorHelpers.h"

AMyFPSProjectGameMode::AMyFPSProjectGameMode()
{
	// set default pawn class to our Blueprinted character  
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_FPSGun"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
