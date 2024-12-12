// Fill out your copyright notice in the Description page of Project Settings.


#include "PinballElements/Elements/PillarElement.h"

#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "Zone/PillarZone.h"


// Sets default values
APillarElement::APillarElement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	DisableMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Project/Assets/Art/03_Mat/Pillar/M_PillarDisable.M_PillarDisable"));
	EnableMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Project/Assets/Art/03_Mat/Pillar/M_PillarEnable.M_PillarEnable"));
	TrickedMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Project/Assets/Art/03_Mat/Pillar/M_PillarTricked.M_PillarTricked"));
}

// Called when the game starts or when spawned
void APillarElement::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh = Cast<UStaticMeshComponent>(GetRootComponent());

	DisablePillar();
}

// Called every frame
void APillarElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APillarElement::TriggerElement()
{
	Super::TriggerElement();
}

EPinballElementID APillarElement::GetElementID()
{
	return EPinballElementID::Pillar;
}

FVector APillarElement::GetHookPosition()
{
	return GetActorLocation();
}

bool APillarElement::IsHookable()
{
	return bIsHookable;
}

bool APillarElement::IsPillar()
{
	return true;
}

void APillarElement::DisablePillar()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(0, 3, FColor::Red, "Disabled");
	bIsHookable = false;

	if (PillarZone != nullptr)
	{
		PillarZone->OnReceiveAssociatedPillarUnGrabbed();
	}
	
	PillarZone = nullptr;
	
	if (Mesh)
	{
		if (DisableMaterial)
		{
			Mesh->SetMaterial(0, DisableMaterial);
		}
		else
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(0, 3, FColor::Red, "pas material");
		}
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(0, 3, FColor::Red, "pas mesh");
	}

	if (PlayerStateMachineOnPillar)
	{
		PlayerStateMachineOnPillar->ChangeState(EPlayerBallStateID::Idle);
		PlayerStateMachineOnPillar = nullptr;
	}

	/*
	if (!bIsTricked)
	{
		ReceiveOnDisablePillar();
	}
	*/

	ReceiveOnDisablePillar();
}

void APillarElement::EnablePillar(bool Tricked, APillarZone* Zone)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(0, 3, FColor::Red, "Enabled");
	bIsHookable = true;
	bIsTricked = Tricked;
	bZoneDisabled = false;

	PillarZone = Zone;
	
	if (Mesh)
	{
		if (EnableMaterial)
		{
			if (!bIsTricked)
				Mesh->SetMaterial(0, EnableMaterial);
			else
				Mesh->SetMaterial(0, TrickedMaterial);
		}
		else
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(0, 3, FColor::Red, "pas material");
		}
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(0, 3, FColor::Red, "pas mesh");
	}

	ReceiveOnEnablePillar();
}

void APillarElement::DisableTrickedZone()
{
	bIsHookable = false;
	if (PillarZone == nullptr) return;
	if (bZoneDisabled) return;

	UE_LOG(LogTemp, Warning, TEXT("DISABLE TRICKED ZONE"));
	
	PillarZone->HideZone();
	bZoneDisabled = true;
	bIsTricked = false;

	ReceiveOnDisableTrickedZone();
}

void APillarElement::OnStartedGrabbed()
{
	if (PillarZone == nullptr) return;

	PillarZone->OnReceiveAssociatedPillarGrabbed();
}

void APillarElement::OnStoppedGrabbed()
{
	if (PillarZone == nullptr) return;

	if (GetNumberOfPlayersGrappling() <= 0)
	{
		PillarZone->OnReceiveAssociatedPillarUnGrabbed();
	}
}

int APillarElement::GetNumberOfPlayersGrappling() const
{
	return NbrPlayersGrappling;
}

