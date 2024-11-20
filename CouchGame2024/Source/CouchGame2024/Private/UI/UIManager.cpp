// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIManager.h"

#include "Blueprint/UserWidget.h"
#include "Events/EventData.h"


// Sets default values
AUIManager::AUIManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AUIManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUIManager::ShowWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	if (!WidgetClass) return;
	
	// Check if the widget is already active
	if (ActiveWidgets.Contains(WidgetClass))
	{
		if (UUserWidget* ExistingWidget = ActiveWidgets[WidgetClass]; ExistingWidget && !ExistingWidget->IsInViewport())
		{
			ExistingWidget->AddToViewport();
		}
		return;
	}

	// Create and show the widget
	if (UUserWidget* NewWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass))
	{
		NewWidget->AddToViewport();
		ActiveWidgets.Add(WidgetClass, NewWidget);
	}
}

void AUIManager::HideWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	if (!WidgetClass || !ActiveWidgets.Contains(WidgetClass)) return;

	if (UUserWidget* WidgetToRemove = ActiveWidgets[WidgetClass])
	{
		WidgetToRemove->RemoveFromParent();
	}
	ActiveWidgets.Remove(WidgetClass);
}

void AUIManager::HideAllWidgets()
{
	for (const auto& Pair : ActiveWidgets)
	{
		if (Pair.Value)
		{
			Pair.Value->RemoveFromParent();
		}
	}
	ActiveWidgets.Empty();
}

void AUIManager::ShowWidgetForEvent(EEventName EventName)
{
	switch (EventName)
	{
	case EEventName::Duck:
		ShowWidget(EventDuckWidget);
		break;
	
	default:
		break;
	}
}

void AUIManager::HideWidgetForEvent(EEventName EventName)
{
	switch (EventName)
	{
	case EEventName::Duck:
		HideWidget(EventDuckWidget);
		break;
	
	default:
		break;
	}
}

