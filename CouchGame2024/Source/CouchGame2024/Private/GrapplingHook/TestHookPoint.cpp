// Fill out your copyright notice in the Description page of Project Settings.


#include "GrapplingHook/TestHookPoint.h"


// Sets default values
ATestHookPoint::ATestHookPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATestHookPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestHookPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ATestHookPoint::GetHookPosition()
{
	return GetActorLocation();
}

bool ATestHookPoint::IsHookable()
{
	return true;
}

bool ATestHookPoint::IsPillar()
{
	return true;
}

