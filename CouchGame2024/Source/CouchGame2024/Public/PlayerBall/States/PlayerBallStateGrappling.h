// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBall/PlayerBallState.h"
#include "PlayerBallStateGrappling.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME2024_API UPlayerBallStateGrappling : public UPlayerBallState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerBallStateGrappling();

public:

	virtual EPlayerBallStateID GetStateID() const override;
	
	virtual void StateInit(UPlayerBallStateMachine* InStateMachine) override;

	virtual void StateEnter(EPlayerBallStateID PreviousState) override;

	virtual void StateExit(EPlayerBallStateID NextState) override;

	virtual void StateTick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnEndGrappling(float InGrapplingValue);

	UFUNCTION()
	void OnStunned(float StunnedValue);

	UFUNCTION()
	void OnImpacted(float ImpactedValue);
};
