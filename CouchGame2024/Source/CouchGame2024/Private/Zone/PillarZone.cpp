﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Zone/PillarZone.h"

#include "Components/SphereComponent.h"
#include "GrapplingHook/Hookable.h"
#include "Kismet/GameplayStatics.h"
#include "PinballElements/Elements/PillarElement.h"


// Sets default values
APillarZone::APillarZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereComponent->SetupAttachment(RootComponent); // Attache-le au RootComponent ou à un autre composant
	SphereComponent->InitSphereRadius(200.f);
	// Ajuste le rayon de la sphère en fonction de la taille de la zone de collision
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // Active les collisions
	SphereComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic); // Ou un autre type si nécessaire
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap); // Permet l'overlap avec tous les autres acteurs
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APillarZone::OnOverlapBegin); // Déclenche OnOverlapBegin
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &APillarZone::OnOverlapEnd);
}

// Called when the game starts or when spawned
void APillarZone::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnPillar)
	{
		TargetPillar = SpawnPillar;
		SetActorLocation(SpawnPillar->GetActorLocation());
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "MISSING SPAWN PILLAR IN PILLAR ZONE");
	}

	TArray<AActor*> OverlappingActors;
	SphereComponent->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (APillarElement* Pillar = Cast<APillarElement>(Actor))
		{
			Pillar->EnablePillar();
		}
	}

	GetLevelPillars();
}

// Called every frame
void APillarZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentZoneState) {
	case EPillarZoneState::Waiting:
		Wait(DeltaTime);
		break;
	case EPillarZoneState::Moving:
		MoveToPillar(DeltaTime);
		break;
	}
}

void APillarZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	if (APillarElement* OverlappedPillar = Cast<APillarElement>(OtherActor))
	{
		OverlappedPillar->EnablePillar();
	}
}

void APillarZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (APillarElement* OverlappedPillar = Cast<APillarElement>(OtherActor))
	{
		OverlappedPillar->DisablePillar();
	}
}

void APillarZone::GetLevelPillars()
{
	ScenePillars.Reset();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APillarElement::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (APillarElement* Pillar = Cast<APillarElement>(Actor))
		{
			ScenePillars.Add(Pillar);
			UE_LOG(LogTemp, Warning, TEXT("Found Pillar: %s"), *Pillar->GetName());
		}
	}
}

void APillarZone::GetRandomPillar()
{
	if (ScenePillars.Num() <= 1) 
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Not enough pillars to choose from"));
		return;
	}

	APillarElement* ExcludedPillar = TargetPillar; 

	// Filter list without excluded pillar
	TArray<APillarElement*> FilteredList = ScenePillars.FilterByPredicate([ExcludedPillar](const APillarElement* Item)
	{
		return Item != ExcludedPillar;
	});

	// Check list
	if (FilteredList.Num() <= 0)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No valid pillars to choose from!"));
		return;
	}

	// Get random from filtered list
	APillarElement* RandomPillar = FilteredList[FMath::RandRange(0, FilteredList.Num() - 1)];

	if (RandomPillar)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
			FString::Printf(TEXT("New Random Pillar: %s"), *RandomPillar->GetName()));
		
		TargetPillar = RandomPillar;
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Failed to pick a random pillar!"));
	}
}

void APillarZone::MoveToPillar(float DeltaTime)
{
	FVector TargetLocation = TargetPillar->GetActorLocation();
	FVector CurrentLocation = GetActorLocation();  
	FVector Direction = TargetLocation - CurrentLocation;  
	float DistanceToMove = MoveSpeed * DeltaTime;  

	// Get closer to target
	if (Direction.Size() > DistanceToMove)
	{
		Direction.Normalize();  
		FVector NewLocation = CurrentLocation + Direction * DistanceToMove; 
		SetActorLocation(NewLocation);  
	}
	else // End movement
	{
		SetActorLocation(TargetLocation);
		CurrentZoneState = EPillarZoneState::Waiting;
	}
}

void APillarZone::Wait(float DeltaTime)
{
	if (CurrentWaitTime == 0.f)
	{
		CurrentWaitTime = FMath::FRandRange(MinTimeWaitingOnPillar, MaxTimeWaitingOnPillar);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Red,
		                                              FString::Printf(TEXT("Time to wait : %f"), CurrentWaitTime));
	}
	else
	{
		TimeWaited += DeltaTime;

		if (TimeWaited >= CurrentWaitTime)
		{
			GetRandomPillar();
			CurrentZoneState = EPillarZoneState::Moving;
			TimeWaited = 0.f;
			CurrentWaitTime = 0.f;
		}
	}
}
