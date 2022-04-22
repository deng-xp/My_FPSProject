// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "FPSGun.generated.h"

class UCameraComponent;
class UBoxComponent;
class USpringArmComponent;
UCLASS()
class MYFPSPROJECT_API AFPSGun : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AFPSGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//ǹ(��̨)�����
	//��̬�����������������ʾǹ�����
	UPROPERTY(VisibleDefaultsOnly,Category=Mesh)
	UStaticMeshComponent* GunMesh;
	//�����
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* GunCameraComponent;
	//��ײ���
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* GunBoxComponent;
	//���ɱ����
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CamerSpringArm;
	//���������ͣ�ֻ��ʾAFPSProjectile��������AFPSProjectile�ࣩ
	UPROPERTY(EditDefaultsOnly,Category=Projectile)
	TSubclassOf<class AFPSProjectile> ProjectileClass;

	//�������
	//��¼�ӽ���ת�Ƕ�
	FVector2D CameraInput;
	//����ʱ��
	UPROPERTY(EditAnywhere)
	float Zoomtime;
	//����Ƿ񿪾�
	bool IsZoom;
	//���������λ����������ƫ����
	UPROPERTY(EditAnywhere)
	FVector MuzzleOffset;

	//�����﷢�亯������Fire�����
	UFUNCTION()
	void Fire();
	//���������ת��
	void PitchCamera(float AxisValue);
	//���������ת��
	void YawCamera(float AxisValue);
	//����
	void ZoomIn();
	//��������
	void ZoomOut();
};