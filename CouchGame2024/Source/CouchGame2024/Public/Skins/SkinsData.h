// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SkinsData.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME2024_API USkinsData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skins")
	TArray<TSoftObjectPtr<UMaterialInstance>> SkinsMaterials;
};
