// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/BoxComponent.h"
#include "MyFPSProjectCharacter.h"

// Sets default values
ADoor::ADoor()
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

	//门初始化
	LeftDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	LeftDoorMesh->SetupAttachment(RootComponent);
	RightDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	RightDoorMesh->SetupAttachment(RootComponent);

	//参数初始化
	OpenTime = 5.0f;
	IsDoorOpen = true;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	//其他参数初始化
	DoorOpenWidth = fabs(RightDoorMesh->GetRelativeLocation().Y - LeftDoorMesh->GetRelativeLocation().Y);
	Speed = (DoorOpenWidth / 2) / OpenTime;
	UE_LOG(LogTemp, Warning, TEXT("The DoorOpenWidth is: %f"), DoorOpenWidth);
	UE_LOG(LogTemp, Warning, TEXT("The speed is: %f"), Speed);
	LeftDoorStartLocationY = LeftDoorMesh->GetRelativeLocation().Y;
	RightDoorStartLocationY = RightDoorMesh->GetRelativeLocation().Y;
	LeftDoorEndLocationY = LeftDoorStartLocationY - DoorOpenWidth / 2;
	RightDoorEndLocationY = RightDoorStartLocationY + DoorOpenWidth / 2;

	//绑定重叠 
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnBoxOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnBoxEndOverlap);
	//关闭每帧调用Tick()
	ADoor::SetActorTickEnabled(false);
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsDoorOpen)
	{
		ADoor::DoorOpen(DeltaTime);
	}
	else
	{
		ADoor::DoorClose(DeltaTime);
	}
}


//门打开
void ADoor::DoorOpen(float DeltaTime)
{
	FVector NewLocationLeft = LeftDoorMesh->GetRelativeLocation();
	FVector NewLocationRight = RightDoorMesh->GetRelativeLocation();
	if (NewLocationLeft.Y > LeftDoorEndLocationY && NewLocationRight.Y < RightDoorEndLocationY)
	{
		NewLocationLeft.Y -= DeltaTime * Speed;
		NewLocationRight.Y += DeltaTime * Speed;
		LeftDoorMesh->SetRelativeLocation(NewLocationLeft);
		RightDoorMesh->SetRelativeLocation(NewLocationRight);
	}
	else
	{
		//停止每帧更新
		ADoor::SetActorTickEnabled(false);
	}
}
//电梯下降
void ADoor::DoorClose(float DeltaTime)
{
	FVector NewLocationLeft = LeftDoorMesh->GetRelativeLocation();
	FVector NewLocationRight = RightDoorMesh->GetRelativeLocation();
	if (NewLocationLeft.Y < LeftDoorStartLocationY && NewLocationRight.Y > RightDoorStartLocationY)
	{
		NewLocationLeft.Y += DeltaTime * Speed;
		NewLocationRight.Y -= DeltaTime * Speed;
		LeftDoorMesh->SetRelativeLocation(NewLocationLeft);
		RightDoorMesh->SetRelativeLocation(NewLocationRight);
	}
	else
	{
		//停止每帧更新
		ADoor::SetActorTickEnabled(false);
	}
}
//重叠事件
void  ADoor::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Begin overlap"));

	AMyFPSProjectCharacter* MyPawn = Cast<AMyFPSProjectCharacter>(OtherActor);
	if (MyPawn)
	{
		//标志位为上升
		IsDoorOpen = true;
		//开启每帧更新
		ADoor::SetActorTickEnabled(true);
	}
}
//重叠结束
void ADoor::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Log, TEXT("End overlap"));

	AMyFPSProjectCharacter* MyPawn = Cast<AMyFPSProjectCharacter>(OtherActor);
	if (MyPawn)
	{
		//标志位为上升
		IsDoorOpen = false;
		//开启每帧更新
		ADoor::SetActorTickEnabled(true);
	}
}