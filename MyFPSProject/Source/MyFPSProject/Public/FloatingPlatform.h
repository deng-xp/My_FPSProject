// Fill out your copyright notice in the Description page of Project Settings.

//旋转同时上下移动的平台
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class MYFPSPROJECT_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();

protected:
	//显示平台外观的组件
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PlatformMesh;
	//移动速度
	UPROPERTY(EditAnywhere)
	float MoveSpeed;
	//转动速度
	UPROPERTY(EditAnywhere)
	float RotateSpeed;
	//Sin函数移动的相位差（实现错位移动）
	UPROPERTY(EditAnywhere)
	float PhaseDifference;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
