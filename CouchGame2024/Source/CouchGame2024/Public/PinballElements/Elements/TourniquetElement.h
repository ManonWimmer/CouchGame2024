// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PinballElements/PinballElement.h"
#include "Rounds/RoundsResetable.h"
#include "TourniquetElement.generated.h"

class UTourniquetElementData;
class APlayerBall;
class USphereComponent;

UCLASS()
class COUCHGAME2024_API ATourniquetElement : public APinballElement, public IRoundsResetable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATourniquetElement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	
	virtual EPinballElementID GetElementID() override;

	virtual void TriggerElement() override;

	virtual void TriggerElementWithPlayer(APlayerBall* InPlayerBall = nullptr) override;
	
#pragma region Components
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereTrigger;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> TourniquetAnchorTurning;

	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> TourniquetAnchorPlayer0;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> TourniquetAnchorPlayer1;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> TourniquetAnchorPlayer2;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> TourniquetAnchorPlayer3;

	

#pragma endregion


#pragma region Turning

public:
	
	void AddBallToTourniquet(APlayerBall* InPlayerBall);

	virtual void InitResetable() override;

	virtual bool IsResetable() override;
	
	virtual void ResetObject() override;
	
public:
	UFUNCTION()
	void ForceExpulsePlayerFromTourniquetWithoutIndex(APlayerBall* InPlayerBall);
	
private:
	UFUNCTION()
	void ExpulsePlayerFromTourniquet(APlayerBall* InPlayerBall, int Index);

	UFUNCTION()
	void IncreaseSpeedTourniquetByVelocity(float InVelocity);
	
	UFUNCTION()
	void HandleTurningEffect(float DeltaTime);

	UFUNCTION()
	void HandleDecreaseSpeedEffect(float DeltaTime);

	UFUNCTION()
	void HandleTimeInTourniquet(float DeltaTime);
	
	UFUNCTION()
	int GetNextEmptyBallPositionTourniquet();

	UFUNCTION()
	USceneComponent* GetTourniquetAnchorFromIndex(int InIndex);

	UFUNCTION()
	int GetPlayerIndexFromTourniquet(APlayerBall* InPlayerBall);
	
	void InitAssociatedBallInTourniquetArray();

	void InitDataTourniquet();
	
	float GetForceExpulseBall();


	UPROPERTY(EditAnywhere)
	TObjectPtr<UTourniquetElementData> TourniquetData;
	
	UPROPERTY()
	TArray<APlayerBall*> AssociatedBallInTourniquet;

	UPROPERTY()
	TArray<float> AssociatedTimeToBallInTourniquet;

	float TurningDuration = 3.f;
	
	float MinTurningTurningSpeed = 0.f;
	
	float MaxTurningTurningSpeed = 1000.f;
	
	float CurrentTurningSpeed = 0.f;

	float TurningSpeedDecceleration = 60.f;


	float MinExpulseForce = 20000.f;

	float MaxExpulseForce = 60000.f;
	

#pragma endregion 
	
};

