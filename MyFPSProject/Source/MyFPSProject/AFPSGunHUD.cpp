// Fill out your copyright notice in the Description page of Project Settings.


#include "AFPSGunHUD.h"

void AAFPSGunHUD::DrawHUD()
{
	Super::DrawHUD();
	//Êä³öÎÄ×Ö
	DrawText(StringPrintOnScreen,FColor::Red, Canvas->ClipX * 0.5f- 150, Canvas->ClipY * 0.5f-100);
}
