// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBall/PlayerBallState.h"
#include "PlayerBallStateStun.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME2024_API UPlayerBallStateStun : public UPlayerBallState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerBallStateStun();

public:

	virtual EPlayerBallStateID GetStateID() const override;
	
	virtual void StateInit(UPlayerBallStateMachine* InStateMachine) override;

	virtual void StateEnter(EPlayerBallStateID PreviousState) override;

	virtual void StateEnter(EPlayerBallStateID PreviousState, float InFloatParameter) override;
	
	virtual void StateExit(EPlayerBallStateID NextState) override;

	virtual void StateTick(float DeltaTime) override;

private:
	UPROPERTY()
	float CurrentStunRemaining = 0.f;


	UFUNCTION()
	void DecreaseCooldownStun(float DeltaTime);


	UFUNCTION()
	void OnImpacted(float ImpactedValue);

	UFUNCTION()
	void OnBumped(float BumpedValue);

	UFUNCTION()
	void OnSnapped(float InSnapValue);

	UFUNCTION()
	void OnGrappled(float GrappledValue);

	UFUNCTION()
	void OnDeath(float DeathValue);
};
