// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PinballElements/PinballElement.h"
#include "FlipperElement.generated.h"

class UBoxComponent;

UCLASS()
class COUCHGAME2024_API AFlipperElement : public APinballElement
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                           const FHitResult& SweepResult);
	// Sets default values for this actor's properties
	AFlipperElement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

#pragma region Components
public:
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxTrigger;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> FlipperMesh;

#pragma endregion 
	
public:

	virtual EPinballElementID GetElementID() override;

	virtual void TriggerElement() override;

	UFUNCTION(BlueprintCallable)
	void FlipUp();
	
	UFUNCTION(BlueprintCallable)
	void FlipDown();

	UPROPERTY(EditAnywhere)
	FRotator OriginalRotation;
	UPROPERTY(EditAnywhere)
	FRotator FlippedRotation;
	bool bIsFlipped;

	UPROPERTY(EditAnywhere)
	float FlipUpSpeed = 20.f;

	UPROPERTY(EditAnywhere)
	float FlipDownSpeed = 20.f;
};
