// Fill out your copyright notice in the Description page of Project Settings.


#include "AFPSGunHUD.h"

void AAFPSGunHUD::DrawHUD()
{
	Super::DrawHUD();
	if (CrosshairTexture)
	{
		//��ȡ�������ĵ�(ClipX��ʾ�������µ�X��
		FVector2D Center(Canvas->ClipX*0.5f,Canvas->ClipY*0.5f);
		//ƫ��������ʹ׼�������뻭�����Ķ��루���Ͽ�ʼ���꣩
		FVector2D CrossShairTexture(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f), Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));
		//����׼��
		FCanvasTileItem TileItem(CrossShairTexture,CrosshairTexture->Resource,FLinearColor::White);
		TileItem.BlendMode=SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}