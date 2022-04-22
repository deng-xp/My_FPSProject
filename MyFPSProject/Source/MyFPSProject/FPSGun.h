// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FPSGun.generated.h"

class UCameraComponent;
class UBoxComponent;
class USpringArmComponent;
UCLASS()
class MYFPSPROJECT_API AFPSGun : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFPSGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//枪(炮台)的组件
	//静态网格体组件，用于显示枪的外观
	UPROPERTY(VisibleDefaultsOnly,Category=Mesh)
	UStaticMeshComponent* GunMesh;
	//摄像机
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* GunCameraComponent;
	//碰撞组件
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* GunBoxComponent;
	//弹簧臂组件
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CamerSpringArm;
	//发射物类型（只显示AFPSProjectile的子类与AFPSProjectile类）
	UPROPERTY(EditDefaultsOnly,Category=Projectile)
	TSubclassOf<class AFPSProjectile> ProjectileClass;

	//输入变量
	//记录视角旋转角度
	FVector2D CameraInput;
	//开镜时长
	UPROPERTY(EditAnywhere)
	float Zoomtime;
	//标记是否开镜
	bool IsZoom;
	//发射物射出位置相对相机的偏移量
	UPROPERTY(EditAnywhere)
	FVector MuzzleOffset;

	//发射物发射函数，与Fire输入绑定
	UFUNCTION()
	void Fire();
	//摄像机上下转动
	void PitchCamera(float AxisValue);
	//摄像机左右转动
	void YawCamera(float AxisValue);
	//开镜
	void ZoomIn();
	//结束开镜
	void ZoomOut();
};
