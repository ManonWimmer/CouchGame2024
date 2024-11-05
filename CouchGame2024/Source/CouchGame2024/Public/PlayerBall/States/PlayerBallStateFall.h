﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBall/PlayerBallState.h"
#include "PlayerBallStateFall.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME2024_API UPlayerBallStateFall : public UPlayerBallState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerBallStateFall();

public:

	virtual EPlayerBallStateID GetStateID() const override;
	
	virtual void StateInit(UPlayerBallStateMachine* InStateMachine) override;

	virtual void StateEnter(EPlayerBallStateID PreviousState) override;

	virtual void StateExit(EPlayerBallStateID NextState) override;

	virtual void StateTick(float DeltaTime) override;

private:
	// UFUNCTION()
	// void FallingMoveX(float DeltaTime) const;
	// UFUNCTION()
	// void FallingMoveY(float DeltaTime) const;
	UFUNCTION()
	void CheckStillFalling();

	UFUNCTION()
	void OnStunned(float StunnedValue);
	
	UFUNCTION()
	void OnPunch(float PunchValue);

	UFUNCTION()
	void OnImpacted(float ImpactedValue);

	UFUNCTION()
	void OnBumped(float BumpedValue);
};
