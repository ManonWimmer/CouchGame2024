// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Events/Event.h"
#include "EventPush.generated.h"

class APlayerBall;
class UEventData;
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

	void OnPushEventStarted();
	
	UFUNCTION()
	void OnImpact(int PlayerIndexImpacting, int PlayerIndexImpacting2);

	UFUNCTION()
	void CheckAddTimeLastPushed(float DeltaTime);

	UPROPERTY()
	TMap<int, int> LastPlayerIndexPushedPlayerIndex; // Player Pushed - Last Pushing (-1 if none or time > max)

	UPROPERTY()
	TMap<int, float> TimeSinceLastPlayerIndexPushed; // Player Pushed - Time
	
	UFUNCTION()
	void OnPunch(int PlayerIndexPushing, int PlayerIndexPushed);

	UFUNCTION()
	void CheckAddScoreOnDeath(int PlayerIndexDeath);

	UPROPERTY()
	TObjectPtr<UGlobalScoreSubsystem> ScoreSubsystem = nullptr;
	
	UPROPERTY()
	float TimePushedLimit = 3.0f;
	
	UPROPERTY()
	int LoseScoreOnDeathZone = -1;
	
	UPROPERTY()
	int GainScoreOnPush = 5;

	UFUNCTION()
	void SetEventData(const UEventData* Data);

	UFUNCTION()
	void ChangePlayersMaterialsToPushV2();

	UFUNCTION()
	void ResetPlayersMaterialsToInitial();

	UFUNCTION()
	void GetAllPlayers();

	UPROPERTY()
	TArray<TObjectPtr<APlayerBall>> Players;
	
	UPROPERTY()
	TMap<int, TObjectPtr<UMaterialInterface>> PlayersInitialMaterials;
	
	UPROPERTY()
	TObjectPtr<UMaterialInterface> PushV2Material;
	
#pragma endregion

};
