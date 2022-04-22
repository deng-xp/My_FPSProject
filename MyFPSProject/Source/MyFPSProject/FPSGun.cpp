// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGun.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "FPSProjectile.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AFPSGun::AFPSGun()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//������ʼ��
	Zoomtime = 0.3f;
	IsZoom = false;
	MuzzleOffset=FVector(60.0f,0.0f,-10.0f);

	//�����ʼ��
	//�������
	GunBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("GunBoxComponent"));
	GunBoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GunBoxComponent->SetCollisionResponseToAllChannels(ECR_Block);
	GunBoxComponent->SetBoxExtent(FVector(50.0f, 20.0f, 20.0f));
	if (RootComponent == nullptr)
	{
		RootComponent = GunBoxComponent;
	}

	//���ɱ����
	CamerSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CamerSpringArm"));
	CamerSpringArm->SetupAttachment(RootComponent);
	//���õ��ɱ۳���
	CamerSpringArm->TargetArmLength = 30.0f;
	//����������ӳ٣���Ϊ�棬��������ͺ�������ƽ���˶���
	CamerSpringArm->bEnableCameraLag = true;
	//����������ӳ��ٶ�
	CamerSpringArm->CameraLagSpeed = 0.1f;
	//���õ��ɱ����λ����Ƕ�
	CamerSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));

	//������
	GunCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("GunCameraComponent"));
	//ȷ����������Ϊ�գ�Ϊ���򲻽��к�������
	check(GunCameraComponent != nullptr);
	//��������ӵ����ɱ����
	GunCameraComponent->SetupAttachment(CamerSpringArm, USpringArmComponent::SocketName);

	//�������������
	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	check(GunMesh != nullptr);
	GunMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFPSGun::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFPSGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/***************�ӽǵ������**************/
	//��תActor�������������Actor�໥�󶨣����Actorת����CameraҲ�ᶯ
	FRotator NewRotation = GetActorRotation();
	NewRotation.Pitch += CameraInput.Y;
	//������ת�Ƕ���һ����Χ��
	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, -80.0f, 80.f);
	SetActorRotation(NewRotation);
	NewRotation.Yaw += CameraInput.X;
	NewRotation.Yaw = FMath::Clamp(NewRotation.Yaw, -75.0f, 75.0f);
	SetActorRotation(NewRotation);

	/*****************�������*****************/
	//��ȡ�������Ұ�Ƕȣ������Ƕ�Ϊ30�㣬����Ϊ90�㣩
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
	//��Fire����
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSGun::Fire);
	//��ͷ��ת
	PlayerInputComponent->BindAxis("Turn", this, &AFPSGun::YawCamera);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSGun::PitchCamera);
	//���ؾ�
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &AFPSGun::ZoomIn);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &AFPSGun::ZoomOut);
}
//�����﷢��
void AFPSGun::Fire()
{
	UE_LOG(LogTemp, Log, TEXT("On Fire"));
	if (ProjectileClass)
	{
		//��ȡ����ı任
		FVector CameraLocation=GunCameraComponent->GetComponentLocation();
		FRotator CameraRotation=GunCameraComponent->GetComponentRotation();
		//GetActorEyesViewPoint(CameraLocation,CameraRotation);

		//���÷���������λ�ã�ƫ��λ��������������Ƶģ��������������ռ��ڣ�Ҫ����ת����������ϵ��
		FVector MuzzleLocation=CameraLocation+FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation=CameraRotation;

		//���ɷ�����
		UWorld* World=GetWorld();
		if (World)
		{
			//���ò���
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner=this;
			SpawnParams.Instigator=GetInstigator();

			//���ɷ�����(���͡�λ�ã�
			AFPSProjectile* Projectile=World->SpawnActor<AFPSProjectile>(ProjectileClass,MuzzleLocation,MuzzleRotation,SpawnParams);
			if (Projectile)
			{
				//���÷�����Ĺ켣���õ������﷢��ķ��򣬴�����AFPSProjectile���У��������ó�ʼ�ٶȣ�
				FVector LauchDirection=MuzzleRotation.Vector();
				Projectile->FireInDirection(LauchDirection);
			}
		}
	}

}
//���������ת��
void AFPSGun::PitchCamera(float AxisValue)
{
	CameraInput.Y = -AxisValue;
}
//���������ת��
void AFPSGun::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}
//����
void AFPSGun::ZoomIn()
{
	//UE_LOG(LogTemp, Warning, TEXT("Zoom"));
	IsZoom=true;
}
//��������
void AFPSGun::ZoomOut()
{
	//UE_LOG(LogTemp, Warning, TEXT("ZoomOut"));
	IsZoom=false;
}