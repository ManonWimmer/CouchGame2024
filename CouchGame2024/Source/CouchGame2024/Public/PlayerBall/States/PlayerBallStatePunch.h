// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBall/PlayerBallState.h"
#include "PlayerBallStatePunch.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME2024_API UPlayerBallStatePunch : public UPlayerBallState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerBallStatePunch();

public:

	virtual EPlayerBallStateID GetStateID() const override;
	
	virtual void StateInit(UPlayerBallStateMachine* InStateMachine) override;

	virtual void StateEnter(EPlayerBallStateID PreviousState) override;
	
	virtual void StateExit(EPlayerBallStateID NextState) override;

	virtual void StateTick(float DeltaTime) override;

private:
	UFUNCTION()
	void PunchPlayerBall();
	UFUNCTION()
	APlayerBall* GetNearestPlayerBallInPunchRadius();

	UFUNCTION()
	void StartPunchCooldown();
	
	UFUNCTION()
	void Move(float DeltaTime);
	//UFUNCTION()
	//void FallingMove(float DeltaTime);

	UFUNCTION()
	void OnImpacted(float ImpactedValue);
	
	UFUNCTION()
	void OnBumped(float BumpedValue);
};
