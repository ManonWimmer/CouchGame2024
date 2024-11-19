// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBall/PlayerBallState.h"
#include "PlayerBallStatePowerUpHub.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME2024_API UPlayerBallStatePowerUpHub : public UPlayerBallState
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
	void ConsumeCurrentPowerUpPlayerBall();

	
	UFUNCTION()
	void SwitchToFreezePowerUp();

	UFUNCTION()
	void SwitchToStrengthPowerUp();

	UFUNCTION()
	void SwitchToSlipperyPowerUp();
	
};

