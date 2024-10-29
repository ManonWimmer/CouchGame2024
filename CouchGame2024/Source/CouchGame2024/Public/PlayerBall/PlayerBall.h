// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/PrimitiveComponent.h"
#include "PlayerBall.generated.h"


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
	UFUNCTION()
	void OnCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnAttractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                              const FHitResult& SweepResult);
	UFUNCTION()
	void OnAttractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
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



#pragma region Setup Data

public:
	UFUNCTION()
	void SetupData();

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
	
#pragma region Movement
	
public:

	UFUNCTION(BlueprintCallable)
	bool IsGrounded();
	
	UPROPERTY()
	float MoveXValue;
	UPROPERTY()
	float MoveYValue;

	// roll Datas
	UPROPERTY()
	float AngularRollForce = 2000.f;
	UPROPERTY()
	float BraqueDirectionForceMultiplier = 1.f;

#pragma region Fall (obsolete)
/*
	// Fall -> obsolete
	UPROPERTY()
	float SlowFallForce = 50.f;

	UPROPERTY()
	float AccelerateFallForce = 50.f;
*/
#pragma endregion
	
private:

	UFUNCTION()
	void MoveXAction(float XValue);
	UFUNCTION()	
	void MoveYAction(float YValue);

#pragma endregion

#pragma region Stun

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStunedAction, float, StunValue);
	
	UPROPERTY()
	FOnStunedAction OnStunnedAction;

	UFUNCTION(BlueprintCallable)
	void ReceiveStunnedAction(float InStunnedDurationValue);
	
	UPROPERTY()
	float PunchStunCooldown = 3.f;

#pragma	endregion

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

#pragma endregion

#pragma region Impact

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnImpactAction, float, ImpactValue);
	
	UPROPERTY()
	FOnImpactAction OnImpactAction;

	UPROPERTY()
	TObjectPtr<APlayerBall> ImpactedPlayerBall;

	UPROPERTY()
	float ImpactForceMultiplier = 30000.f;

	UPROPERTY()
	float ImpactMinTotalForce = 50.f;

	UPROPERTY()
	float ImpactStunCooldown = 1.f;
	
	UFUNCTION()
	void ReceiveImpactAction(float ImpactValue, const FVector &InNormalImpact);

	UPROPERTY()
	FVector NormalImpact = FVector(0, 0, 0);

#pragma endregion

#pragma region Reaction Pinball Elements

public:
	TObjectPtr<APinballElement> HitPinballElement;

	
#pragma region Bumper Reaction
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBumperReaction, float, BumperReactionValue);

	FOnBumperReaction OnBumperReaction;

	UPROPERTY()
	float BumpedForceMultiplier = 30000.f;

	UPROPERTY()
	float BumpedHitLagCooldown = 0.2f;
	
	UPROPERTY()
	FVector NormalBump = FVector(0, 0, 0);
	
private:
	UFUNCTION()
	void ReceiveBumperReaction(APinballElement* Element, const FVector &InNormalBump);

#pragma endregion 

	
#pragma endregion

#pragma region Grappling

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrapplingAction, float, GrapplingValue);
	
	UPROPERTY()
	FOnGrapplingAction OnGrapplingAction;
	
	UFUNCTION()
	void ReceiveGrapplingAction(float InGrapplingValue);

	UPROPERTY()
	float GrapplingValue = 0.f;

	UPROPERTY()
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

#pragma endregion

#pragma region Grappled

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrappledAction, float, GrappledValue);
	
	UPROPERTY()
	FOnGrappledAction OnGrappledAction;
	
	UFUNCTION()
	void ReceiveGrappledAction(float InGrappledValue);

	UPROPERTY()
	TObjectPtr<APlayerBall> GrapplingPlayerBall = nullptr;	// Ball grappling to this playerBall

#pragma endregion

#pragma region Snapping

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReceiveSnappingAction, float, SnappingValue);

	UPROPERTY()
	FOnReceiveSnappingAction OnReceiveSnappingAction;

	UFUNCTION()
	void ReceiveSnappingAction(float SnappingValue);
	
	UPROPERTY()
	TObjectPtr<APlayerBall> SnappingPlayerBall;

	float SnapAngularForce = 3000.f;

	float SnapControlMoveRollDivider = 2.f;

	float MinVelocityToSnap = 200.f;

#pragma endregion 

	
};
