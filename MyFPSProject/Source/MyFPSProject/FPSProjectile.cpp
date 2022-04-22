// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectile.h"

// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//��ʼ����ײ���
	CollisionComponent=CreateDefaultSubobject<USphereComponent>(TEXT("Projectile"));
	//��ʼ���뾶
	CollisionComponent->InitSphereRadius(10.0f);	
	//����ײ�������Ϊ�����
	RootComponent=CollisionComponent;
	//��ʼ����̬���������
	ProjectileMeshComponent=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	ProjectileMeshComponent->SetupAttachment(RootComponent);

	//ͨ���������ƶ���������������ƶ�
	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		//��Ҫ���Ƶ����
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		//���ó�ʼ�ٶ�
		ProjectileMovementComponent->InitialSpeed=6000.0f;
		//��������ٶ�
		ProjectileMovementComponent->MaxSpeed=6000.0f;
		//�ӵ�����ת����ÿһ֡�и��£���ƥ�����ٶ�
		ProjectileMovementComponent->bRotationFollowsVelocity=true;
		//���÷���
		ProjectileMovementComponent->bShouldBounce=true;
		//���÷���ϵ����������������ǿ��
		ProjectileMovementComponent->Bounciness=0.3f;
		//��������ϵ��
		ProjectileMovementComponent->ProjectileGravityScale=0.0f;
		//�����������ڣ�����Զ����٣�
		InitialLifeSpan=3.0f;	//3�������
	}

}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//�������ٶȣ�ʸ�����������䷽�򣻲���ShootDirection�ɿ������õ����룩
void AFPSProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity=ShootDirection*ProjectileMovementComponent->InitialSpeed;
}