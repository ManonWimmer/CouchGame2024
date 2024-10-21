// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerBall.generated.h"

class USphereComponent;
class UFloatingPawnMovement;
class UStaticMeshComponent;

UCLASS()
class COUCHGAME2024_API APlayerBall : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerBall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere)
	TObjectPtr<UFloatingPawnMovement> PawnMovement;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> SphereMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;
	
private:
	UFUNCTION(BlueprintCallable)
	void BindEventActions();

	UFUNCTION(BlueprintCallable)
	void MoveXAction(float XValue);

	UFUNCTION(BlueprintCallable)	
	void MoveYAction(float YValue);

	UFUNCTION(BlueprintCallable)
	void UpdateRotation();
	
};
