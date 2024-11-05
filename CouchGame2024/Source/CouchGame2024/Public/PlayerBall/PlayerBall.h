// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerBall.generated.h"


class UPlayerBallBehaviorPowerUp;
class UPlayerBallBehaviorElementReactions;
class UPlayerBallBehaviorGrapple;
class UPlayerBallBehaviorMovements;
class APlayerBallController;
class APinballElement;
class UPlayerBallData;
class UPlayerBallStateMachine;
class USphereComponent;
class UFloatingPawnMovement;
class UStaticMeshComponent;
class UCableComponent;

UCLASS()
class COUCHGAME2024_API APlayerBall : public APawn
{
	GENERATED_BODY()

public:
#pragma region Hit / Overlap
	UFUNCTION()
	void OnCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnAttractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                              const FHitResult& SweepResult);
	UFUNCTION()
	void OnAttractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#pragma endregion
	
	// Sets default values for this pawn's properties
	APlayerBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	int PlayerIndex = 0;


public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<APlayerBallController> PlayerBallController;

#pragma region Setup Data

public:
	UFUNCTION()
	void SetupData();

	TObjectPtr<UPlayerBallData> GetPlayerBallData() const;
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPlayerBallData> PlayerBallData;

#pragma endregion 
	
#pragma region Components
	
public:
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UFloatingPawnMovement> PawnMovement;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> SphereMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereCollision;

	// To detect other balls
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> AttractionSphere;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> GrapplingSphereCollision;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCableComponent> GrapplingCable;
	
#pragma endregion

#pragma  region Behaviors Components

private:
	void InitPlayerBallBehaviors() const;
	
	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPlayerBallBehaviorMovements> BehaviorMovements;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPlayerBallBehaviorGrapple> BehaviorGrapple;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPlayerBallBehaviorElementReactions> BehaviorElementReactions;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPlayerBallBehaviorPowerUp> BehaviorPowerUp;
	

#pragma endregion 
	
#pragma region StateMachine
public:
	void CreateStateMachine();

	void InitStateMachine();

	void TickStateMachine(float DeltaTime) const;

protected:

	TObjectPtr<UPlayerBallStateMachine> StateMachine;
	
	
#pragma endregion

	
private:
	UFUNCTION()
	void BindEventActions();

#pragma region States


#pragma region Punch

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPunchAction, float, PunchValue);
	
	UPROPERTY()
	FOnPunchAction OnPunchAction;

	UFUNCTION()
	void ReceivePunchAction(float InPunchValue);
	
	UPROPERTY()
	float PunchRadius = 50.f;

	UPROPERTY()
	float PunchCooldown = 3.f;

	UPROPERTY()
	float PunchForceMultiplier = 5.f;

	UFUNCTION()
	void HandlePunchCooldown(float DeltaTime);

	float CurrentPunchCooldown = 3.f;
	bool bCanPunch = true;

#pragma endregion
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayImpactEffectsBlueprint();

#pragma region Grapple
	
#pragma region Grappling

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrapplingActionStarted, float, GrapplingValue);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrapplingActionEnded, float, GrapplingValue);
	
	UPROPERTY()
	FOnGrapplingActionStarted OnGrapplingActionStarted;
	UPROPERTY()
	FOnGrapplingActionEnded OnGrapplingActionEnded;
	
	// UFUNCTION()
	// void ReceiveGrapplingAction(float InGrapplingValue);

	UFUNCTION()
	void ReceiveGrapplingActionStarted(float InGrapplingValue);
	UFUNCTION()
	void ReceiveGrapplingActionEnded(float InGrapplingValue);

	UPROPERTY(VisibleAnywhere)
	bool CanGrappling = false;
	
	UPROPERTY()
	float GrapplingValue = 0.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APlayerBall> GrappledPlayerBall = nullptr;	// Ball grappled to this playerBall

	UPROPERTY()
	float CableLength = 0.f;

	UPROPERTY()
	FVector HookPoint = FVector(0.f, 0.f, 0.f);

	UPROPERTY()
	float CurrentGrapplingAngularVelocity = 0.f;

	UPROPERTY()
	float CurrentGrapplingAngle = 0.f;

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

	UPROPERTY()
	float GrapplingDamping = 0.99f;

	UPROPERTY()
	float GrapplingForce = 0.1f;

	UPROPERTY()
	float GrapplingReleaseForce = 250.f;

	UPROPERTY()
	float MinCableDistance = 200.f;

	UPROPERTY()
	float MaxCableDistance = 500.f;

	UPROPERTY()
	float MoreOrLessCablePerFrame = 10.f;

	UPROPERTY()
	float StartGrapplingForceFactorWhenAlreadyMoving = 0.001f;

#pragma endregion

#pragma region Grappled

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrappledActionStarted, float, GrappledValue);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrappledActionEnded, float, GrappledValue);

	
	UPROPERTY()
	FOnGrappledActionStarted OnGrappledActionStarted;
	FOnGrappledActionEnded OnGrappledActionEnded;

	UPROPERTY(VisibleAnywhere)
	bool CanBeGrappled = false;
	
	UFUNCTION()
	void ReceiveGrappledActionStarted(float InGrappledValue);
	void ReceiveGrappledActionEnded(float InGrappledValue);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<APlayerBall> GrapplingPlayerBall = nullptr;	// Ball grappling to this playerBall

#pragma endregion

#pragma region MoreLessGrappling
public:
	UPROPERTY()
	float MoreLessValue;

private:
	UFUNCTION()
	void MoreLessAction(float InMoreLessValue);
	

#pragma endregion
	
#pragma endregion 



#pragma endregion
	
};
