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

UCLASS()
class COUCHGAME2024_API UEventData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Event")
	EEventName EventName = EEventName::Undefined;
	
	UPROPERTY(EditAnywhere, Category = "Event")
	float Phase1MinTime = 30.f;
	
	UPROPERTY(EditAnywhere, Category = "Event", meta = (EditCondition = "Phase1MaxTime > Phase1MinTime", Tooltip = "Phase1MaxTime > Phase1MinTime"))
	float Phase1MaxTime = 50.f;

	UPROPERTY(EditAnywhere, Category = "Event")
	float Phase2MinTime = 30.f;

	UPROPERTY(EditAnywhere, Category = "Event", meta = (EditCondition = "Phase2MaxTime > Phase2MinTime", Tooltip = "Phase2MaxTime > Phase2MinTime"))
	float Phase2MaxTime = 50.f;

	UPROPERTY(EditAnywhere, Category = "Event", meta = (EditCondition = "EventRoundTime == Phase1MinTime + Phase2MaxTime", Tooltip = "EventRoundTime = Phase1MinTime + Phase2MaxTime"))
	float EventRoundTime = 80.f;
};
