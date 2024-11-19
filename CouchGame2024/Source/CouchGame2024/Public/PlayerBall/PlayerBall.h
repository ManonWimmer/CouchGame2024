// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Killable.h"
#include "LockableInput.h"
#include "PlayerBallStateID.h"
#include "Camera/CameraFollowTarget.h"
#include "GameFramework/Pawn.h"
#include "Rounds/RoundsResetable.h"
#include "PlayerBall.generated.h"


class UPlayerPowerUpData;
class UWidgetComponent;
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
class COUCHGAME2024_API APlayerBall : public APawn, public IRoundsResetable, public ILockableInput, public IKillable, public ICameraFollowTarget
{
	GENERATED_BODY()

public:
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

	TObjectPtr<UPlayerPowerUpData> GetPlayerPowerUpData() const;
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPlayerBallData> PlayerBallData;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPlayerPowerUpData> PlayerPowerUpData;

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

	UPROPERTY(EditAnywhere)
	TArray<EPlayerBallStateID> PlayerStates;
	
protected:

	UPROPERTY()
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

	UFUNCTION(BlueprintImplementableEvent)
	void PlayPunchGamefeelEffectsBlueprint();
	
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

public:

	UFUNCTION(BlueprintImplementableEvent)
	void PlayImpactGamefeelEffectsBlueprint();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayDeathZoneGamefeelEffectsBlueprint();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayReleaseTourniquetGamefeelEffectsBlueprint();
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void PlaySetProgressStartForceEffect(float NewProgressStartForce);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> StartForceEffectWidget;
	
#pragma endregion

#pragma region IRoundsResetable

private:
	virtual void InitResetable() override;
	
public:
	virtual bool IsResetable() override;

	virtual void ResetObject() override;
	
	void ResetState();
	void ResetMovement();
	void ResetGrapple();
	void ResetCooldown();
	void ResetPosition();
	void ResetPhysics();

#pragma endregion

#pragma region ILockableInput

public:
	virtual void InitLockableInput() override;
	
	virtual bool IsLockableInput() override;

	virtual void LockInput() override;

	virtual void UnlockInput() override;

	virtual void LockButOnlySpecialInput() override;

	virtual int GetLockableInputIndex() override;

	virtual bool IsLocked() override;

private:
	bool bIsLocked = false;

	bool bIsLockedButSpecial = false;
	
#pragma endregion

#pragma region IKillable

public:

	virtual void Kill() override;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathReaction, float, DeathValue);

	FOnDeathReaction OnDeathReaction;

	UFUNCTION(CallInEditor)
	void TestCallRespawn();
	
	virtual void Respawn() override;

	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRespawnAction, float, RespawnReaction);

	FOnRespawnAction OnRespawnAction;
	
	bool bIsDead = false;
	float DeathDurationBeforeRespawn = 1.f;
	
#pragma endregion 

#pragma region ICameraFollowTarget

public:
	virtual void InitFollowTarget() override;
	
	virtual FVector GetFollowPosition() const override;
	
	virtual bool IsFollowable() const override;


#pragma endregion 

	
};
