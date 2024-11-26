// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DuckBank.generated.h"

class USphereComponent;

UCLASS()
class COUCHGAME2024_API ADuckBank : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADuckBank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> BankMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereCollision;

	UFUNCTION()
	void OnBankBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								 const FHitResult& SweepResult);

	UFUNCTION()
	void Bind();

	UPROPERTY()
	bool bHasBeenBind = false;
	
	UPROPERTY(EditAnywhere)
	int DuckToPoints = 200;
};
