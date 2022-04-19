// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

class UBoxComponent;
UCLASS()
class MYFPSPROJECT_API ADoor : public AActor
{
	GENERATED_BODY()

	//使用左右横向开门，因此有两扇门
	//静态网格体组件（显示左边门）
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* LeftDoorMesh;
	//静态网格体组件（显示右边门）
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* RightDoorMesh;
	//碰撞组件
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerBox;

	//门的原始位置
	UPROPERTY()
	float LeftDoorStartLocationY;
	UPROPERTY()
	float RightDoorStartLocationY;
	//门的极限位置
	UPROPERTY()
	float LeftDoorEndLocationY;
	UPROPERTY()
	float RightDoorEndLocationY;

	//门打开的大小，由两扇门的初始位置求得
	UPROPERTY()
	float DoorOpenWidth;
	//门完全打开所需的时间
	UPROPERTY(EditAnywhere, Category = "time")
	float OpenTime;
	//门打开的速度
	UPROPERTY()
	float Speed;
	//标志位，用于标志门是打开还是关闭
	bool IsDoorOpen;

public:
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//门打开
	UFUNCTION()
	void DoorOpen(float DeltaTime);
	//门关闭
	UFUNCTION()
	void DoorClose(float DeltaTime);
	//重叠开始响应事件
	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//重叠结束事件
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
