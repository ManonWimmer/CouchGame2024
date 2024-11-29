// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Events/Event.h"
#include "EventPush.generated.h"

class UGlobalScoreSubsystem;

UCLASS()
class COUCHGAME2024_API AEventPush : public AEvent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEventPush();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region Event

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPushStartedEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPushEndedEvent);

	UPROPERTY(BlueprintAssignable)
	FPushStartedEvent OnPushStartedEvent;

	UPROPERTY(BlueprintAssignable)
	FPushEndedEvent OnPushEndedEvent;
	
	virtual void SetupEventPhase1() override;

	virtual void TriggerEventPhase1() override;

	virtual void TriggerEventPhase2() override;
	
	virtual void TickPhase1() override;

	virtual void TickPhase2() override;
	
	virtual void EndEvent() override;

	UFUNCTION()
	void OnImpact(int PlayerIndexImpacting, int PlayerIndexImpacting2);

	UFUNCTION()
	void CheckAddTimeLastPushed(float DeltaTime);

	UPROPERTY()
	TMap<int, int> LastPlayerIndexImpactingPlayerIndex; // Player - Last Impacting (-1 if none or time > max)

	UPROPERTY()
	TMap<int, float> TimeSinceLastPlayerIndexImpacted; // Player Impacted - Time
	
	UFUNCTION()
	void OnPunch(int PlayerIndexPushing, int PlayerIndexPushed);

	UPROPERTY()
	TMap<int, int> LastPlayerIndexPunchingPlayerIndex; // Player Pushed - Last Pushing (-1 if none or time > max)

	UPROPERTY()
	TMap<int, float> TimeSinceLastPlayerIndexPunched; // Player Pushed - Time

	UFUNCTION()
	void CheckAddScoreOnDeath(int PlayerIndexDeath);

	UPROPERTY()
	float TimePushedLimit = 3.0f;

	UPROPERTY()
	TObjectPtr<UGlobalScoreSubsystem> ScoreSubsystem = nullptr;

	UPROPERTY()
	int GainScoreOnPush = 5;
	
#pragma endregion

};
