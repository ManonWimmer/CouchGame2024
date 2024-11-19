﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/Duck/DuckBank.h"

#include "Components/SphereComponent.h"


// Sets default values
ADuckBank::ADuckBank()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BankMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BankMesh"));
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision")); // overlap player balls
	
	RootComponent = SphereCollision;
	
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ADuckBank::OnBankBeginOverlap);
}

// Called when the game starts or when spawned
void ADuckBank::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADuckBank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADuckBank::OnBankBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

