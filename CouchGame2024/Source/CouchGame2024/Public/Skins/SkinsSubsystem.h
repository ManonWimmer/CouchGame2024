// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SkinsSubsystem.generated.h"

class USkinsData;
/**
 * 
 */
UCLASS()
class COUCHGAME2024_API USkinsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UFUNCTION()
	void InitSkinsSubsystem();
	
	UFUNCTION(BlueprintCallable)
	UMaterialInstance* GetSkinFromIndexAndPlayerIndex(int InPlayerIndex, int InSkinIndex);

	UFUNCTION(BlueprintCallable)
	UMaterialInstance* GetCurrentSkinFromPlayerIndex(int InPlayerIndex);
	
	UFUNCTION(BlueprintCallable)
	UMaterialInstance* GetNextSkinFromPlayerIndex(int InPlayerIndex);

	UFUNCTION()
	UMaterialInstance* GetSkinPlayer1FromIndex(int InSkinIndex);

	UFUNCTION()
	UMaterialInstance* GetSkinPlayer2FromIndex(int InSkinIndex);

	UFUNCTION()
	UMaterialInstance* GetSkinPlayer3FromIndex(int InSkinIndex);

	UFUNCTION()
	UMaterialInstance* GetSkinPlayer4FromIndex(int InSkinIndex);

private:
	bool bIsInitialized = false;
	
	UPROPERTY()
	TObjectPtr<USkinsData> SkinsDataP1;

	UPROPERTY()
	TObjectPtr<USkinsData> SkinsDataP2;

	UPROPERTY()
	TObjectPtr<USkinsData> SkinsDataP3;

	UPROPERTY()
	TObjectPtr<USkinsData> SkinsDataP4;
	
	int P1SkinIndex = 0;
	int P2SkinIndex = 0;
	int P3SkinIndex = 0;
	int P4SkinIndex = 0;
};
