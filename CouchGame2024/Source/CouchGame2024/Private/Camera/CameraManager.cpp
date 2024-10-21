// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraManager.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ACameraManager::ACameraManager(): MainCamera(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACameraManager::BeginPlay()
{
	Super::BeginPlay();

	GetAllPlayers();
	GetMainCamera();
}

// Called every frame
void ACameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACameraManager::GetAllPlayers()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlayerClass, FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
		                                              FString::Printf(TEXT("Found actor : %s"), *Actor->GetName()));

		Players.Add(Actor);
	}
}

void ACameraManager::GetMainCamera()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Get Main Camera"));
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CameraClass, FoundActors);

	if (FoundActors.Num() == 1)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
													  FString::Printf(TEXT("Found camera : %s"), *FoundActors[0]->GetName()));
		MainCamera = FoundActors[0];
	}
	else if (FoundActors.Num() == 0)
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No main camera"));
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Too many main cameras"));
	}
}
