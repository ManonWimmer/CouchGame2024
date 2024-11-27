// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBall/PlayerBallState.h"
#include "PlayerBallStateGrappling.generated.h"


class APillarElement;
class UGlobalScoreSubsystem;

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
	float RotationDirection = 0.f;
	
	UFUNCTION()
	void OnEndGrappling(float InGrapplingValue);

	UFUNCTION()
	void OnStunned(float StunnedValue);

	UFUNCTION()
	void OnImpacted(float ImpactedValue);

	UFUNCTION()
	void SetCable();

	UFUNCTION()
	void SetGrapplingVelocityAndAnglePillar(float DeltaTime);
	
	UFUNCTION()
	void SetGrapplingVelocityAndAngleNotPillar(float DeltaTime);

	UFUNCTION()
	bool DetectWalls();
	
	UPROPERTY()
	FVector LastLocation = FVector::ZeroVector;

	UPROPERTY()
	float TempGrapplingAngularVelocity = 0.f;

	UPROPERTY()
	float TempGrapplingAngle = 0.f;

	UPROPERTY()
	float StartAngle = 0.f;

	UFUNCTION()
	void GainPillarPoints();

	UPROPERTY()
	float CurrentTimeOnPillar = 0.f;

	UPROPERTY()
	float ExitTimePillarTricked = 0.f;

	UPROPERTY()
	UGlobalScoreSubsystem* ScoreSubsystem;

	UPROPERTY()
	TObjectPtr<APillarElement> Pillar = nullptr;
};
