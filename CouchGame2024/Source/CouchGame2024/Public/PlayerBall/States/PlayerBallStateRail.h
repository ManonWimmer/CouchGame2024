// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBall/PlayerBallState.h"
#include "PlayerBallStateRail.generated.h"

class ARailElement;
/**
 * 
 */
UCLASS()
class COUCHGAME2024_API UPlayerBallStateRail : public UPlayerBallState
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

	void EnterRail();

	void HandleRailProgressLocation(float DeltaTime);
	
	void ExitRail();

	void ChangeDirection();

	void CheckForwardCollisionBallRail();

	float DirectionRail = 0.f;

	float CurrentPercent = 0.f;

	float CurrentTimeInRail = 0.f;

	float ProgressRailDuration = 1.f;

	float EndProgressOffset = 0.2;
	
	UPROPERTY()
	TObjectPtr<ARailElement> CurrentRailElement;
};
