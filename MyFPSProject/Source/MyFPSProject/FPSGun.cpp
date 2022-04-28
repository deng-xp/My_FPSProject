// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGun.h"
#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "FPSProjectile.h"
#include "MyFPSProjectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MyFPSProjectGameMode.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AFPSGun::AFPSGun()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//参数初始化
	Zoomtime = 0.3f;
	IsZoom = false;
	Input=false;

	//组件初始化
	//盒体组件
	GunBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("GunBoxComponent"));
	GunBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GunBoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	GunBoxComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

	GunBoxComponent->SetBoxExtent(FVector(60.0f, 20.0f, 20.0f));
	if (RootComponent == nullptr)
	{
		RootComponent = GunBoxComponent;
	}
	//弹簧臂组件
	CamerSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CamerSpringArm"));
	CamerSpringArm->SetupAttachment(RootComponent);
	//设置弹簧臂长度
	CamerSpringArm->TargetArmLength = 30.0f;
	//开启摄像机延迟（若为真，则摄像机滞后于物体平滑运动）
	CamerSpringArm->bEnableCameraLag = true;
	//设置摄像机延迟速度
	CamerSpringArm->CameraLagSpeed = 0.1f;
	//设置弹簧臂组件位置与角度
	CamerSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));

	//相机组件
	GunCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("GunCameraComponent"));
	//确保相机组件不为空，为空则不进行后续操作
	check(GunCameraComponent != nullptr);
	//将相机附加到弹簧臂组件
	GunCameraComponent->SetupAttachment(CamerSpringArm, USpringArmComponent::SocketName);

	//创建网格体组件
	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	check(GunMesh != nullptr);
	GunMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFPSGun::BeginPlay()
{
	Super::BeginPlay();
	//绑定重叠开始事件，开启输入
	GunBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFPSGun::OnBeginOverLap);
	//绑定重叠结束事件，关闭输入
	GunBoxComponent->OnComponentEndOverlap.AddDynamic(this, &AFPSGun::OnEndOverLap);
	//子弹数量
	Ammo=30;
}

// Called every frame
void AFPSGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/***************视角调整相关**************/
	//旋转Actor：由于摄像机与Actor相互绑定，因此Actor转动，Camera也会动
	FRotator NewRotation = GetActorRotation();
	NewRotation.Pitch += CameraInput.Y;
	//设置旋转角度在一定范围内
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, -80.0f, 80.f);
	SetActorRotation(NewRotation);
	NewRotation.Yaw += CameraInput.X;
	NewRotation.Yaw = FMath::Clamp(NewRotation.Yaw, -75.0f, 75.0f);
	SetActorRotation(NewRotation);

	/*****************开镜相关*****************/
	//获取摄像机视野角度（开镜角度为30°，正常为90°）
	float CurView = GunCameraComponent->FieldOfView;
	if (IsZoom && CurView > 30.0f)
	{
		CurView -= (60.0 / Zoomtime) * DeltaTime;
		CurView=FMath::Clamp(CurView,30.0f,90.0f);
		GunCameraComponent->SetFieldOfView(CurView);
	}
	else if (!IsZoom && CurView < 90.0f)
	{
		CurView += (60.0 / Zoomtime) * DeltaTime;
		CurView = FMath::Clamp(CurView, 30.0f, 90.0f);
		GunCameraComponent->SetFieldOfView(CurView);
	}
}

// Called to bind functionality to input
void AFPSGun::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//绑定Fire输入
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSGun::Fire);
	//镜头旋转
	PlayerInputComponent->BindAxis("Turn", this, &AFPSGun::YawCamera);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSGun::PitchCamera);
	//开关镜
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AFPSGun::ZoomIn);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AFPSGun::ZoomOut);
}
//发射物发射
void AFPSGun::Fire()
{
	if (Ammo <= 0)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("Please replenish your ammunition"),false,FVector2D(1.5,1.5));
		return;
	}
	if (ProjectileClass)
	{
		//获取相机的变换
		FVector CameraLocation=GunCameraComponent->GetComponentLocation();
		FRotator CameraRotation=GunCameraComponent->GetComponentRotation();
		//GetActorEyesViewPoint(CameraLocation,CameraRotation);
		MuzzleOffset.Set(62.0f,0.0f,0.0f);
		//设置发射物生成位置（偏移位置是相对摄像机设计的，是在相机的坐标空间内，要将其转至世界坐标系）
		FVector MuzzleLocation=CameraLocation+FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation=CameraRotation;
		//生成发射物
		UWorld* World=GetWorld();
		if (World)
		{
			//设置参数
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner=this;
			SpawnParams.Instigator=GetInstigator();

			//生成发射物(类型、位置）
			AFPSProjectile* Projectile=World->SpawnActor<AFPSProjectile>(ProjectileClass,MuzzleLocation,MuzzleRotation,SpawnParams);
			if (Projectile)
			{
				//设置发射物的轨迹（得到发射物发射的方向，传递至AFPSProjectile类中，用于设置初始速度）
				FVector LauchDirection=MuzzleRotation.Vector();
				Projectile->FireInDirection(LauchDirection);
				//子弹数量减1
				Ammo--;
				UE_LOG(LogTemp, Log, TEXT("Ammo is: %i"),Ammo);
			}
		}
		//添加声音
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this,FireSound,GetActorLocation());
		}
	}
}
//摄像机上下转动
void AFPSGun::PitchCamera(float AxisValue)
{
	CameraInput.Y = -AxisValue;
}
//摄像机左右转动
void AFPSGun::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}
//开镜
void AFPSGun::ZoomIn()
{
	//UE_LOG(LogTemp, Warning, TEXT("Zoom"));
	IsZoom=true;
}
//结束开镜
void AFPSGun::ZoomOut()
{
	//UE_LOG(LogTemp, Warning, TEXT("ZoomOut"));
	IsZoom=false;
}
//开始重叠
void AFPSGun::OnBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Gun Begin Overlap"));
	if (OtherActor != this)
	{
		AMyFPSProjectCharacter* OverLapCharacter=Cast<AMyFPSProjectCharacter,AActor>(OtherActor);
		if (OverLapCharacter != nullptr)
		{
			//检测玩家控制器的输入(绑定控制Pawn切换的输入）
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->InputComponent->BindAction("Interaction", IE_Pressed,
				this, &AFPSGun::AcquireController);
			//记录导致重叠发生的角色
			OverLapFPSCharacter= OverLapCharacter;
		}
	}
}
//重叠结束
void AFPSGun::OnEndOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Gun end Overlap"));
	//解除绑定 控制Pawn切换的输入
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->InputComponent->RemoveActionBinding("Interaction",IE_Pressed);
}

void AFPSGun::AcquireController()
{
	AGameModeBase* CurGameMode = UGameplayStatics::GetGameMode(GetWorld());
	check(CurGameMode);
	AMyFPSProjectGameMode* CurFPSGameMode = Cast<AMyFPSProjectGameMode, AGameModeBase>(CurGameMode);
	//实现两个Pawn之间的来回切换
	if (CurFPSGameMode)
	{
		if (UGameplayStatics::GetPlayerPawn(GetWorld(), 0) == this)
		{
			CloseUI();
			CurFPSGameMode->ChangePawn<AMyFPSProjectCharacter>(OverLapFPSCharacter);
		}
		else
		{
			DisplayUI();
			CurFPSGameMode->ChangePawn<AFPSGun>(this);
		}
	}
}
