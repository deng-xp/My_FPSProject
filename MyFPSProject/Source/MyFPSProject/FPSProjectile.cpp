// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//初始化碰撞组件
	CollisionComponent=CreateDefaultSubobject<USphereComponent>(TEXT("Projectile"));
	//初始化半径
	CollisionComponent->InitSphereRadius(10.0f);
	//用项目中预设的碰撞文件初始化碰撞
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
	//将碰撞组件设置为根组件
	RootComponent=CollisionComponent;
	//初始化静态网格体组件
	ProjectileMeshComponent=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMeshComponent>Mesh(TEXT("'/Game/Geometry/Meshes/CubeMaterial.CubeMaterial'"));
	if (Mesh.Succeeded())
	{
		ProjectileMeshComponent=Mesh.Object;
	} 

	//初始化发射物材质
	//直接访问材质文件，并赋值
	static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Materials/Projectile_Red.Projectile_Red'"));
	if (Material.Succeeded())
	{
		//设置材质，并将其与发射物网格体组件绑定
		ProjectileMaterialInstance=UMaterialInstanceDynamic::Create(Material.Object,ProjectileMeshComponent);
	}
	ProjectileMeshComponent->SetMaterial(0,ProjectileMaterialInstance);
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
		ProjectileMovementComponent->bShouldBounce=false;
		//设置反弹系数，即反弹能力的强弱
		ProjectileMovementComponent->Bounciness=0.3f;
		//设置重力系数
		ProjectileMovementComponent->ProjectileGravityScale=0.0f;
		//设置生命周期（多久自动销毁）
		InitialLifeSpan=2.0f;	//2秒后销毁
	}

}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	//绑定碰撞事件
	CollisionComponent->OnComponentHit.AddDynamic(this,&AFPSProjectile::OnHit);
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//速度为0时销毁（为0表示已经发生碰撞了）
	if (ProjectileMovementComponent->Velocity == FVector(0.0f))
	{
		this->Destroy();
	}
}

//发射物速度（矢量，包含发射方向；参数ShootDirection由控制器得到输入）
void AFPSProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity=ShootDirection*ProjectileMovementComponent->InitialSpeed;
}

//碰撞事件;参数中Hit存储碰撞发生位置等信息
void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//保证不是发射物与发射物碰撞，且被击中的组件有开启物理模拟时，施加冲击
	if (OtherActor != this&& OtherComp->IsSimulatingPhysics())
	{
		//在指定位置（Hit.ImpactPoint）施加特点大小的冲击（ProjectileMovementComponent->Velocity*100.0f）
		OtherComp->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
		//显示粒子效果
		if (FireParticle != nullptr)
		{
			UGameplayStatics::SpawnEmitterAttached(FireParticle, OtherComp, "Fired");
			//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),FireParticle,OtherActor->GetActorTransform());
		}
	}
}
