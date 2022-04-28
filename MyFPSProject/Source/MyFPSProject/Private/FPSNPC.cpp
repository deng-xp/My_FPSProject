// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSNPC.h"
#include "Components/BoxComponent.h"
#include "FPSProjectile.h"
#include "kismet/GameplayStatics.h"

// Sets default values
AFPSNPC::AFPSNPC()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	//设置碰撞
	FPSNPCBox = CreateDefaultSubobject<UBoxComponent>(TEXT("FPSNPCBox"));
	FPSNPCBox->SetBoxExtent(FVector(60.0f));
	//采用和子弹一样的碰撞
	FPSNPCBox->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	
	RootComponent = FPSNPCBox;
	//设置静态网格体(开启模拟物理效果)
	FPSNPCMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	//FPSNPCMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//FPSNPCMeshComponent->SetSimulatePhysics(true);
	//设置材质
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("'/Game/Materials/M_BasicBox.M_BasicBox'"));
	if (Material.Succeeded())
	{
		FPSNPCMaterial = UMaterialInstanceDynamic::Create(Material.Object, FPSNPCMeshComponent);
	}
	FPSNPCMeshComponent->SetMaterial(0, FPSNPCMaterial);
	FPSNPCMeshComponent->SetupAttachment(RootComponent);

	//速度初始化
	Speed = 200;
}

// Called when the game starts or when spawned
void AFPSNPC::BeginPlay()
{
	Super::BeginPlay();
	FPSNPCBox->OnComponentHit.AddDynamic(this, &AFPSNPC::OnHit);
}

// Called every frame
void AFPSNPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//APlayerController* CurControl=UGameplayStatics::GetPlayerController(GetWorld(),0);
	APawn* CurPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (CurPawn)
	{
		FVector FinalLocation = CurPawn->GetActorLocation();
		FVector NewLocation = this->GetActorLocation();
		FRotator NewRotation = (FinalLocation - NewLocation).Rotation();
		NewRotation.Pitch=0.0f;
		FVector2D MoveDirection = FVector2D((FinalLocation.X - NewLocation.X), (FinalLocation.Y - NewLocation.Y));

		MoveDirection = MoveDirection / MoveDirection.Size();
		MoveDirection = Speed * DeltaTime * MoveDirection;
		FVector DeltaLocation = FVector(MoveDirection.X, MoveDirection.Y, 0);
		NewLocation += DeltaLocation;
		//UE_LOG(LogTemp, Warning, TEXT("%f,%f,%f"), NewLocation.X, NewLocation.Y, NewLocation.Z);
		SetActorLocationAndRotation(NewLocation,NewRotation);
	}
}

void AFPSNPC::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp,Warning,TEXT("ON HIT"));
	
}