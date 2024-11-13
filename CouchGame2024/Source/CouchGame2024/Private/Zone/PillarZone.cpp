// Fill out your copyright notice in the Description page of Project Settings.


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
	SphereComponent->SetupAttachment(RootComponent);  // Attache-le au RootComponent ou à un autre composant
	SphereComponent->InitSphereRadius(200.f);  // Ajuste le rayon de la sphère en fonction de la taille de la zone de collision
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // Active les collisions
	SphereComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);  // Ou un autre type si nécessaire
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);  // Permet l'overlap avec tous les autres acteurs
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APillarZone::OnOverlapBegin);  // Déclenche OnOverlapBegin
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &APillarZone::OnOverlapEnd);
}

// Called when the game starts or when spawned
void APillarZone::BeginPlay()
{
	Super::BeginPlay();

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
}

void APillarZone::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APillarElement* OverlappedPillar = Cast<APillarElement>(OtherActor))
	{
		OverlappedPillar->EnablePillar();
	}
}

void APillarZone::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if (APillarElement* OverlappedPillar = Cast<APillarElement>(OverlappedComponent))
	{
		OverlappedPillar->DisablePillar();
	}
}

void APillarZone::GetLevelPillars()
{
	ScenePillarZones.Reset();
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APillarElement::StaticClass(), FoundActors);
	
	for (AActor* Actor : FoundActors)
	{
		if (APillarElement* Pillar = Cast<APillarElement>(Actor))
		{
			ScenePillarZones.Add(Pillar);
			UE_LOG(LogTemp, Warning, TEXT("Found Pillar: %s"), *Pillar->GetName());
		}
	}
}

void APillarZone::SwitchPillar()
{
}

void APillarZone::GetRandomPillar()
{
}

void APillarZone::MoveToPillar()
{
}

void APillarZone::Wait()
{
}

