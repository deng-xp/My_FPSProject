// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "AFPSGunHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYFPSPROJECT_API AAFPSGunHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	//HUD绘制的主要调用，进行重写
	virtual void DrawHUD() override;
protected:
	//准星资源
	UPROPERTY(EditAnywhere)
	UTexture2D* CrosshairTexture;

};
