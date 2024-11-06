// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBallStateID.h"
#include "Components/ActorComponent.h"
#include "PlayerBallState.generated.h"


class APlayerBallController;
class APlayerBall;
class UPlayerBallStateMachine;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class COUCHGAME2024_API UPlayerBallState : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerBallState();

	virtual EPlayerBallStateID GetStateID() const;

	virtual void StateInit(UPlayerBallStateMachine* InStateMachine);
	
	virtual void StateEnter(EPlayerBallStateID PreviousState);

	virtual void StateEnter(EPlayerBallStateID PreviousState, float InFloatParameter);

	virtual void StateExit(EPlayerBallStateID NextState);
	
	virtual void StateTick(float DeltaTime);

protected:
	UPROPERTY()
	TObjectPtr<APlayerBall> Pawn;

	UPROPERTY()
	TObjectPtr<APlayerBallController> Controller;

	UPROPERTY()
	TObjectPtr<UPlayerBallStateMachine> StateMachine;
};