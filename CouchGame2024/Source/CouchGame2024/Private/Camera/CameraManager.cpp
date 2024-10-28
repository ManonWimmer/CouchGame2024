// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraManager.h"

#include "Camera/CameraActor.h"
#include "GeometryCollection/GeometryCollectionAlgo.h"
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
	GetDebugSphere();
}

// Called every frame
void ACameraManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CalculateCameraLocations();
	MoveCamera(DeltaTime);
	MoveDebugSphere();
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

void ACameraManager::CalculateCameraLocations()
{
	FVector TotalPlayersPositions = FVector::ZeroVector;

	for (AActor* Player : Players)
	{
		TotalPlayersPositions += Player->GetActorLocation();
	}
	AveragePlayerPositions = TotalPlayersPositions / Players.Num();
	
	FRotator CameraRotation = MainCamera->GetActorRotation();
	
	float CameraPitchRadians = FMath::DegreesToRadians(CameraRotation.Pitch);
	
	float OffsetZ = -FMath::Sin(CameraPitchRadians) * CameraDistance * CameraPitchMultiplier; 
	float OffsetX = FMath::Cos(CameraPitchRadians) * CameraDistance; 
	
	FVector NewCameraLocation = AveragePlayerPositions - MainCamera->GetActorForwardVector() * OffsetX + FVector(0.0f, 0.0f, OffsetZ);
	
	MainCamera->SetActorLocation(NewCameraLocation);
}



void ACameraManager::MoveCamera(float DeltaTime)
{
	//NewCameraLocation = FVector(MainCamera->GetActorLocation().X, AveragePlayerPositions.Y, AveragePlayerPositions.Z);
	//MainCamera->SetActorLocation(NewCameraLocation);
}

void ACameraManager::GetDebugSphere()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Get Debug Sphere"));
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), DebugSphereClass, FoundActors);

	if (FoundActors.Num() == 1)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
													  FString::Printf(TEXT("Found debug sphere : %s"), *FoundActors[0]->GetName()));
		DebugSphere = FoundActors[0];
	}
	else if (FoundActors.Num() == 0)
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No debug sphere"));
		else
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Too many debug spheres"));
		}
}

void ACameraManager::MoveDebugSphere()
{
	if (DebugSphere == nullptr) return;
	
	DebugSphere->SetActorLocation(AveragePlayerPositions);
}
