// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"

#include "TagManagerTool.generated.h"

UCLASS(Blueprintable)
class COUCHGAME2024_API UATagManagerTool : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UATagManagerTool();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tags")
	TArray<FName> AvailableTags;
	
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Tag Manager")
	void AssignTag(FName TagName);

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Tag Manager")
	void RemoveTag(FName TagName);
	
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Tag Manager")
	void ShowObjectsWithTag(FName TagName);
	
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Tag Manager")
	void HideObjectsWithTag(FName TagName);

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Tag Manager")
	void HideOtherTags(FName TagName);

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Tag Manager")
	void SetMissingTagsToConstant();
	
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Tag Manager")
	void ShowAll();

	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Tag Manager")
	void RemoveAllTags();

private:
	// Trouve tous les acteurs dans le niveau avec un tag donné
	TArray<AActor*> GetActorsWithTag(FName TagName);

	FName HasTagInList(AActor* Actor);



	
};
