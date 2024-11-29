// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EventData.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EEventName: uint8
{
	Undefined,
	Duck,
	Zones,
	Mole,
	Push
};

UCLASS(BlueprintType)
class COUCHGAME2024_API UEventData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	EEventName EventName = EEventName::Undefined;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	FString EventDescription = "Event Description";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	FName EventTag = "EventTag";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	float Phase1MinTime = 30.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event", meta = (EditCondition = "Phase1MaxTime > Phase1MinTime", Tooltip = "Phase1MaxTime > Phase1MinTime"))
	float Phase1MaxTime = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	float Phase2MinTime = 30.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event", meta = (EditCondition = "Phase2MaxTime > Phase2MinTime", Tooltip = "Phase2MaxTime > Phase2MinTime"))
	float Phase2MaxTime = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event", meta = (EditCondition = "EventRoundTime == Phase1MinTime + Phase2MaxTime", Tooltip = "EventRoundTime = Phase1MinTime + Phase2MaxTime"))
	float EventRoundTime = 80.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event", meta = (EditCondition = "EventName == EEventName::Mole", Tooltip = "Que pour l'event Mole"))
	int MoleToPoints = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event", meta = (EditCondition = "EventName == EEventName::Duck", Tooltip = "Que pour l'event Duck"))
	int DuckToPoints = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event", meta = (EditCondition = "EventName == EEventName::Push", Tooltip = "Que pour l'event Push"))
	float TimePushedLimit = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event", meta = (EditCondition = "EventName == EEventName::Push", Tooltip = "Que pour l'event Push"))
	int LoseScoreOnDeathZone = -1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event", meta = (EditCondition = "EventName == EEventName::Push", Tooltip = "Que pour l'event Push"))
	int GainScoreOnPush = 5;
};
