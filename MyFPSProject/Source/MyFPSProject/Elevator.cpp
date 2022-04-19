// Fill out your copyright notice in the Description page of Project Settings.


#include "Elevator.h"
#include "Components/BoxComponent.h"
#include "Components/LightComponent.h"
#include "MyFPSProjectCharacter.h"

// Sets default values
AElevator::AElevator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//碰撞组件初始化
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	//设置碰撞
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//忽略所有碰撞
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	//仅对玩家Pawn进行碰撞响应（重叠事件）
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	//设置碰撞范围
	TriggerBox->SetBoxExtent(FVector(50.0f, 50.0f, 20.0f));
	RootComponent = TriggerBox;

	//电梯平台初始化
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
	PlatformMesh->SetupAttachment(RootComponent);
	//时间轴初始化
	ElevatorTimelineComp=CreateDefaultSubobject<UTimelineComponent>(TEXT("ElevatorTimelineComp"));
	//灯光初始化
	ElevetorLight=CreateDefaultSubobject<ULightComponent>(TEXT("ElevetorLight"));
}

// Called when the game starts or when spawned
void AElevator::BeginPlay()
{
	Super::BeginPlay();

	//将浮点轨道绑定到UpdataTimelineComp函数，即Timeline输出更新时将调用的函数
	UpdateFunctionFloat.BindDynamic(this,&AElevator::UpdataTimelineComp);
	//将浮点曲线与更新函数绑定
	if (ElevatorTimelineFloatCurve)
	{
		ElevatorTimelineComp->AddInterpFloat(ElevatorTimelineFloatCurve,UpdateFunctionFloat);
	}

	//绑定碰撞事件
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AElevator::OnBoxOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AElevator::OnBoxEndOverlap);
	//设置灯不可见
	if (ElevetorLight)
	{
		ElevetorLight->SetVisibility(false);
	}
	//记录初始位置，决定电梯上升还是下降
	StartLocationZ=GetActorLocation().Z;
}

// Called every frame
void AElevator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//Output为Timeline的函数值输出
void AElevator::UpdataTimelineComp(float Output)
{
	FVector NewLocation=GetActorLocation();
	NewLocation.Z=Output;
	//以Timeline输出的值为新的Z坐标
	AElevator::SetActorLocation(NewLocation);
}

//重叠事件
void  AElevator::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Begin overlap"));

	AMyFPSProjectCharacter* MyPawn = Cast<AMyFPSProjectCharacter>(OtherActor);
	if (MyPawn)
	{
		if (GetActorLocation().Z == StartLocationZ)
		{
			ElevatorTimelineComp->Play();	//上升
		}
		else
		{
			ElevatorTimelineComp->Reverse();	//反向播放，即下降
		}
		//开灯
		if (ElevetorLight)
		{
			ElevetorLight->SetVisibility(true);
		}	
	}
}
//重叠结束
void AElevator::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("End overlap"));
	AMyFPSProjectCharacter* MyPawn = Cast<AMyFPSProjectCharacter>(OtherActor);
	if (MyPawn)
	{
		//关灯
		if (ElevetorLight)
		{
			ElevetorLight->SetVisibility(false);
		}
	}
}