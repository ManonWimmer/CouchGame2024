﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UIManager.generated.h"

class AHUD;

UCLASS()
class COUCHGAME2024_API AUIManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUIManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Show a widget
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowWidget(TSubclassOf<UUserWidget> WidgetClass);

	// Hide a widget
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideWidget(TSubclassOf<UUserWidget> WidgetClass);

	// Hide all widgets
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideAllWidgets();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowWidgetForEvent(EEventName EventName);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HideWidgetForEvent(EEventName EventName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> EventDuckWidget;

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void ShowDuckCounter();

	UFUNCTION(BlueprintNativeEvent, Category = "UI")
	void HideDuckCounter();
	

private:
	// Map of active widgets
	UPROPERTY()
	TMap<TSubclassOf<UUserWidget>, UUserWidget*> ActiveWidgets;
};
