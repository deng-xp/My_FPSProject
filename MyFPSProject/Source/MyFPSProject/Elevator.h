// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "Elevator.generated.h"

class UBoxComponent;
UCLASS()
class MYFPSPROJECT_API AElevator : public AActor
{
	GENERATED_BODY()

	//静态网格体组件（电梯平台）
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	class UStaticMeshComponent* PlatformMesh;
	//碰撞组件
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* TriggerBox;
	//灯光组件
	UPROPERTY(EditAnywhere)
	class ULightComponent* ElevetorLight;

	//进行动画处理的时间轴组件
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* ElevatorTimelineComp;
	//曲线资产，决定Timeline变化趋势
	UPROPERTY(EditAnywhere)
	UCurveFloat* ElevatorTimelineFloatCurve;
	//用于处理更新轨道事件的浮点轨道签名(委托将处理函数值更新时的函数的签名）
	FOnTimelineFloat UpdateFunctionFloat;

	//记录电梯初始位置
	UPROPERTY(VisibleAnywhere)
	float StartLocationZ;

public:
	// Sets default values for this actor's properties
	AElevator();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//更新电梯位置的函数
	UFUNCTION()
	void UpdataTimelineComp(float Output);
	
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
