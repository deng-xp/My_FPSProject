// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSNPC.generated.h"

class UBoxComponent;
UCLASS()
class MYFPSPROJECT_API AFPSNPC : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSNPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//显示NPC外观的组件
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* FPSNPCMeshComponent;
	//碰撞盒
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* FPSNPCBox;
	//材质
	UPROPERTY(EditAnywhere)
	UMaterialInstanceDynamic* FPSNPCMaterial;
	//NPC移动速度
	UPROPERTY(EditAnywhere)
	float Speed;

	//碰撞发生事件
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
