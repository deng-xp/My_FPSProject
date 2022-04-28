// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyFPSProjectGameMode.generated.h"
 
class AMyFPSProjectCharacter;
class AFPSGun;
UCLASS(minimalapi)
class AMyFPSProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyFPSProjectGameMode();

	virtual void BeginPlay() override;

	//更改控制器控制的Pawn
	template<typename T>
	void ChangePawn(T* NextPawn)
	{
		//UE_LOG(LogTemp,Warning,TEXT("F Pressed"));
		//判断是不是APawn的子类，确保可以被玩家控制器控制
		APawn* NewPawn = Cast<APawn, T>(NextPawn);
		if (NewPawn)
		{
			APlayerController* MyController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			MyController->Possess(NextPawn);
		}
	}
};



