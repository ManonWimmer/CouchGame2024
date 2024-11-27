// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUpID.h"
#include "GameFramework/Actor.h"
#include "Rounds/RoundsResetable.h"
#include "PowerUp.generated.h"

class USphereComponent;

UCLASS()
class COUCHGAME2024_API APowerUp : public AActor, public IRoundsResetable
{
	GENERATED_BODY()

public:
	APowerUp();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

#pragma region Components

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereTrigger;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> PowerUpMesh;
 
#pragma endregion 

	UFUNCTION()
	virtual EPowerUpID GetPowerUpID() const;
	
	virtual void TriggerPowerUp();
	virtual void TriggerPowerUp(int Value);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "TriggerPowerUp"))
	void ReceiveTriggerPowerUp();



public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPowerUpCollected);

	FOnPowerUpCollected OnPowerUpCollected;


#pragma region Resetable

public:
	virtual void InitResetable() override;

	virtual bool IsResetable() override;

	virtual void ResetObject() override;

#pragma endregion
	
};
