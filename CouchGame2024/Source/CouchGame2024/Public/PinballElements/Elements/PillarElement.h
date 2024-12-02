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

	UFUNCTION()
	void EnablePillar(bool Tricked, const APillarZone* Zone);

	UFUNCTION()
	void DisableTrickedZone();
	
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
	TObjectPtr<UPlayerBallStateMachine> PlayerStateMachineOnPillar = nullptr;

	UPROPERTY()
	const APillarZone* PillarZone = nullptr;

	UPROPERTY()
	bool bZoneDisabled = false;

	UPROPERTY()
	int NbrPlayersGrappling = 0;

	/*
	UFUNCTION()
	void AddPlayerGrappling();

	UFUNCTION()
	void RemovePlayerGrappling();
	*/
};
