// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBall/PlayerBallState.h"
#include "PlayerBallStateLocked.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME2024_API UPlayerBallStateLocked : public UPlayerBallState
{
	GENERATED_BODY()

public:

	virtual EPlayerBallStateID GetStateID() const override;
	
	virtual void StateInit(UPlayerBallStateMachine* InStateMachine) override;

	virtual void StateEnter(EPlayerBallStateID PreviousState) override;

	virtual void StateEnter(EPlayerBallStateID PreviousState, float InFloatParameter) override;

	virtual void StateExit(EPlayerBallStateID NextState) override;

	virtual void StateTick(float DeltaTime) override;


private:

	UFUNCTION()
	void AddStartForceToProgressValue(float InValue);

	UFUNCTION()
	void ImpulseInDirection();
	
	UPROPERTY()
	float CurrentProgressStartForce = 0.f;

	bool bIsLockedSpecial = false;

	float StartingStateMinForce = 30000.f;

	float StartingStateMaxForce = 70000.f;

	float PercentAddByStartingSpam = 0.05f;
	
};
