// Fill out your copyright notice in the Description page of Project Settings.


#include "AFPSGunHUD.h"

void AAFPSGunHUD::DrawHUD()
{
	Super::DrawHUD();
	if (CrosshairTexture)
	{
		//获取画布中心点(ClipX表示画布右下点X）
		FVector2D Center(Canvas->ClipX*0.5f,Canvas->ClipY*0.5f);
		//偏移纹理，使准星中心与画布中心对齐（左上开始坐标）
		FVector2D CrossShairTexture(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));
		//绘制准星
		FCanvasTileItem TileItem(CrossShairTexture,CrosshairTexture->Resource,FLinearColor::White);
		TileItem.BlendMode=SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}
