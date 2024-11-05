// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PowerUp/PowerUpID.h"
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
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
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
	
#pragma region Movement
	/*
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

	// Fall -> obsolete
	UPROPERTY()
	float SlowFallForce = 50.f;

	UPROPERTY()
	float AccelerateFallForce = 50.f;

#pragma endregion
	
private:

	UFUNCTION()
	void MoveXAction(float XValue);
	UFUNCTION()	
	void MoveYAction(float YValue);
*/
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

	UFUNCTION()
	void HandlePunchCooldown(float DeltaTime);

	float CurrentPunchCooldown = 3.f;
	bool bCanPunch = true;

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

	UFUNCTION(BlueprintImplementableEvent)
	void PlayImpactEffectsBlueprint();
	
	UPROPERTY()
	FVector NormalImpact = FVector(0, 0, 0);

#pragma endregion

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
	
#pragma region Snapping
public:
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

#pragma region PowerUp

public:
	EPowerUpID GetCurrentPowerUpCarried() const;

	void AssignPowerUpCarried(EPowerUpID PowerUpID);

	UFUNCTION()
	void UsePowerUpAction(float UsePowerUpValue);
	
	UFUNCTION()
	void UsePowerUpCarried();
	
private:
	UPROPERTY(VisibleAnywhere)
	EPowerUpID CurrentPowerUpCarried;
	
#pragma endregion 
	
};
