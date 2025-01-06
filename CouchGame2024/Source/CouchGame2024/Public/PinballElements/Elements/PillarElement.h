// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrapplingHook/Hookable.h"
#include "PinballElements/PinballElement.h"
#include "PillarElement.generated.h"

class APillarZone;
class UPlayerBallStateMachine;

UCLASS()
class COUCHGAME2024_API APillarElement : public APinballElement, public IHookable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APillarElement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TriggerElement() override;
	
	virtual EPinballElementID GetElementID() override;

#pragma region Grappling Interface
public:
	virtual FVector GetHookPosition() override;

	virtual bool IsHookable() override;

	virtual bool IsPillar() override;
#pragma endregion

	UFUNCTION()
	void DisablePillar();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDisablePillar"))
	void ReceiveOnDisablePillar();
	
	UFUNCTION()
	void EnablePillar(bool Tricked, APillarZone* Zone);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnEnablePillar"))
	void ReceiveOnEnablePillar();

	UFUNCTION()
	void DisableTrickedZone();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnDisableTrickedZone"))
	void ReceiveOnDisableTrickedZone();

	UFUNCTION()
	void OnStartedGrabbed();

	UFUNCTION()
	void OnStoppedGrabbed();
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnStartGainPoint"))
	void ReceiveOnStartGainPoint();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnEndGainPoint"))
	void ReceiveOnEndGainPoint();
	
	UPROPERTY()
	bool bIsHookable = true;

	UPROPERTY()
	bool bIsTricked = false;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> Mesh;
	
	UPROPERTY()
	TObjectPtr<UMaterialInterface> DisableMaterial;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> EnableMaterial;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> TrickedMaterial;

	UPROPERTY()
	TArray<TObjectPtr<UPlayerBallStateMachine>> PlayerStateMachinesOnPillar;

	UPROPERTY()
	APillarZone* PillarZone = nullptr;

	UPROPERTY()
	bool bZoneDisabled = false;

	UPROPERTY()
	int NbrPlayersGrappling = 0;

	UFUNCTION(BlueprintPure)
	int GetNumberOfPlayersGrappling() const;

	/*
	UFUNCTION()
	void AddPlayerGrappling();

	UFUNCTION()
	void RemovePlayerGrappling();
	*/
};
