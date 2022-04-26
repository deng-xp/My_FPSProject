// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework//ProjectileMovementComponent.h"
#include "FPSProjectile.generated.h"

UCLASS()
class MYFPSPROJECT_API AFPSProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/****************成员变量*********************/
	//组件与参数
	//球体碰撞组件：用于检测子弹的碰撞
	UPROPERTY(VisibleDefaultsOnly, Category = Project)
	USphereComponent* CollisionComponent;
	//发射物移动组件（控制发射出去的子弹移动）
	//该组件中内置了许多运行相关的参数，例如最大速度、初始速度等
	UPROPERTY(VisibleAnywhere, Category = Movement)
	UProjectileMovementComponent* ProjectileMovementComponent;
	//发射物网格体
	UPROPERTY(VisibleAnywhere,Category=Projectile)
	UStaticMeshComponent* ProjectileMeshComponent;
	//发射物材质（动态材质，可在运行中修改）
	UPROPERTY(VisibleDefaultsOnly,Category=Projectile)
	UMaterialInstanceDynamic* ProjectileMaterialInstance;

	/****************成员函数*********************/
	//初始化发射物的发射方向
	void FireInDirection(const FVector& ShootDirection);
	//碰撞函数
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
