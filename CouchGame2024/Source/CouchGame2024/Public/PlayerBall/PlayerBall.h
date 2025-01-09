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


class AEventPush;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int PlayerIndex = 0;

	
#pragma region Connected / Disconnected

public:
	bool bIsConnected = false;

	void DisconnectPlayer(int InDisconnectValue);

	void ConnectPlayer();
	
#pragma endregion

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> SphereMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereCollision;

	// To detect other balls
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> AttractionSphere;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> GrapplingSphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
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
	
	bool bIsBrakeDirection = false;
	
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

	void SetupPlayerIndexInController(APlayerBallController* InPlayerBallController);
	
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

	UFUNCTION(BlueprintImplementableEvent)
	void PlayPunchFailedGamefeelEffectsBlueprint();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayIsPunchedGamefeelEffectsBlueprint();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayPunchMeshEffect(APlayerBall* InTargetBall);
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayBumpedGamefeelEffectsBlueprint();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayRespawnGamefeelEffectsBlueprint();
	
	UPROPERTY()
	float PunchRadius = 50.f;

	UPROPERTY()
	float PunchRadiusWithStrength = 100.f;
	
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

	UFUNCTION(BlueprintPure)
	bool GetIsImpactingNormal();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayDeathZoneGamefeelEffectsBlueprint();
	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayReleaseTourniquetGamefeelEffectsBlueprint();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayGrapplingGrabGamefeelEffectsBlueprint();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayGrapplingEndGamefeelEffectsBlueprint();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayFreezeCastGamefeelEffectsBlueprint();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayPlayerFreezedGamefeelEffectsBlueprint();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayPlayerUnfreezedGamefeelEffectsBlueprint();

	
	UFUNCTION(BlueprintImplementableEvent)
	void PlayStrengthCastGamefeelEffectsBlueprint();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayStrengthDisapearGamefeelEffectsBlueprint();

	
	UFUNCTION(BlueprintImplementableEvent)
	void PlaySlipperyCastGamefeelEffectsBlueprint();

	UFUNCTION(BlueprintImplementableEvent)
	void PlaySlipperySlipperyDisapearGamefeelEffectsBlueprint();

	UFUNCTION(BlueprintImplementableEvent)
	void PlaySlipperyEffectGamefeelEffectsBlueprint();


	UFUNCTION(BlueprintImplementableEvent)
	void PlayRailEnterGamefeelEffectsBlueprint();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayRailExitGamefeelEffectsBlueprint();
	
public:

	UFUNCTION(BlueprintImplementableEvent)
	void PlaySetProgressStartForceEffect(float NewProgressStartForce);

	UPROPERTY(BlueprintReadWrite)
	bool IsForcedDisabledStartForceEffect = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> StartForceEffectWidget;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnAppearStartForceEffectWidget"))
	void ReceiveOnAppearStartForceEffectWidget();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnSetToNeutralBall"))
	void ReceiveOnSetToNeutralBall();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnUnsetToNeutralBall"))
	void ReceiveOnUnsetToNeutralBall();
	
#pragma endregion

#pragma region Skins
public:
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnChangeSkin"))
	void ReceiveOnChangeSkin();

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
	void ResetPowerUp();
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
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnKill"))
	void ReceiveOnKill(float AnimDuration);
	
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


#pragma region DuckEventReactions

public:
	void ReceiveDuckReaction(int PlayerIndexReceiving, int PlayerIndexLosing);

#pragma endregion

#pragma region PushEventReactions

public:
	void ReceiveImpactPushReaction(int PlayerIndexImpacting, int PlayerIndexImpacting2);

	void ReceivePunchPushReaction(int PlayerIndexPunching, int PlayerIndexPunched);

	UPROPERTY()
	TObjectPtr<AEventPush> EventPush = nullptr;

#pragma endregion 
	
};
