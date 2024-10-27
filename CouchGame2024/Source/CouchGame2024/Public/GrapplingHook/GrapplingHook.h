// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GrapplingHook.generated.h"

class UFloatingPawnMovement;
class USphereComponent;
class UCableComponent;

UCLASS()
class COUCHGAME2024_API AGrapplingHook : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGrapplingHook();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SphereMesh;
	
	UPROPERTY(EditAnywhere)
	UFloatingPawnMovement* PawnMovement;

	UPROPERTY(EditAnywhere)
	UCableComponent* Cable;

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereCollision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "GrapplingHook")
	void OnMove(FVector InputVector);

	UFUNCTION(BlueprintCallable, Category = "GrapplingHook")
	void OnGrapplingStarted();

	UFUNCTION(BlueprintCallable, Category = "GrapplingHook")
	void OnGrapplingCompleted();

	UFUNCTION(BlueprintCallable, Category = "GrapplingHook")
	void OnGrapplingTriggered();
	
	UFUNCTION(BlueprintCallable, Category = "GrapplingHook")
	void SetCable();

	UFUNCTION(BlueprintCallable, Category = "GrapplingHook")
	void SetGrapplingVelocityAndAngle(float DeltaTime);

	UPROPERTY()
	float CableLength = 0.f;

	UPROPERTY()
	FVector HookPoint = FVector(0.f, 0.f, 0.f);

	UPROPERTY()
	float CurrentGrapplingAngularVelocity = 0.f;

	UPROPERTY()
	float CurrentGrapplingAngle = 0.f;

	UPROPERTY()
	float MoveInputX = 0.f;

	UPROPERTY()
	float MoveInputY = 0.f;

	UPROPERTY()
	bool IsGrappling = false;

	UPROPERTY()
	float LastAngle = 0.f;

	UPROPERTY()
	float AngleRotate = 0.f;

	UPROPERTY()
	FVector ReleaseDirection = FVector(0.f, 0.f, 0.f);

	UPROPERTY()
	FVector GrapplingOffset = FVector(0.f, 0.f, 0.f);


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	float GrapplingDamping = 0.99f;

	UPROPERTY(EditAnywhere)
	float GrapplingForce = 0.1f;

	UPROPERTY(EditAnywhere)
	float GrapplingReleaseForce = 250.f;

	UPROPERTY(EditAnywhere)
	float MinCableDistance = 200.f;

	UPROPERTY(EditAnywhere)
	float MaxCableDistance = 500.f;

	UPROPERTY(EditAnywhere)
	float MoreOrLessCablePerFrame = 10.f;

	UPROPERTY(EditAnywhere)
	float StartGrapplingForceFactorWhenAlreadyMoving = 0.001f;

	UPROPERTY()
	TObjectPtr<AGrapplingHook> GrappledPlayerBall;

	UPROPERTY()
	TObjectPtr<AGrapplingHook> GrapplingPlayerBall;
};
