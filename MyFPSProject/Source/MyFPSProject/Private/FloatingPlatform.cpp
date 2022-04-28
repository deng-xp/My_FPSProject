// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->SetupAttachment(RootComponent);
	MoveSpeed=20;
	RotateSpeed=10;
	PhaseDifference=0.0f;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp,Warning,TEXT("PI is %f"), PI);
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation=GetActorLocation();
	FRotator NewRotation=GetActorRotation();
	//获取游戏开始到现在经历的时间
	float StartTime=GetGameTimeSinceCreation();
	float DeltaHeight=FMath::Sin(StartTime+ DeltaTime+PhaseDifference/PI)-FMath::Sin(StartTime+PhaseDifference/PI);
	NewLocation.Z+=DeltaHeight*MoveSpeed;
	NewRotation.Yaw+=DeltaTime*RotateSpeed;
	SetActorLocation(NewLocation);
	SetActorRotation(NewRotation);
}

