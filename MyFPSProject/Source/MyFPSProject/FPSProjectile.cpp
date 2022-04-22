// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectile.h"

// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//初始化碰撞组件
	CollisionComponent=CreateDefaultSubobject<USphereComponent>(TEXT("Projectile"));
	//初始化半径
	CollisionComponent->InitSphereRadius(10.0f);	
	//将碰撞组件设置为根组件
	RootComponent=CollisionComponent;
	//初始化静态网格体组件
	ProjectileMeshComponent=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	ProjectileMeshComponent->SetupAttachment(RootComponent);

	//通过发射物移动组件驱动发射物移动
	if (!ProjectileMovementComponent)
	{
		ProjectileMovementComponent=CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		//绑定要控制的组件
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		//设置初始速度
		ProjectileMovementComponent->InitialSpeed=6000.0f;
		//设置最大速度
		ProjectileMovementComponent->MaxSpeed=6000.0f;
		//子弹的旋转将在每一帧中更新，以匹配其速度
		ProjectileMovementComponent->bRotationFollowsVelocity=true;
		//设置反弹
		ProjectileMovementComponent->bShouldBounce=true;
		//设置反弹系数，即反弹能力的强弱
		ProjectileMovementComponent->Bounciness=0.3f;
		//设置重力系数
		ProjectileMovementComponent->ProjectileGravityScale=0.0f;
		//设置生命周期（多久自动销毁）
		InitialLifeSpan=3.0f;	//3秒后销毁
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

//发射物速度（矢量，包含发射方向；参数ShootDirection由控制器得到输入）
void AFPSProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity=ShootDirection*ProjectileMovementComponent->InitialSpeed;
}
