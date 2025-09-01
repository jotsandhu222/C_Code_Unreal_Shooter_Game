// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_MovingPlatform.h"

// Sets default values
ABP_MovingPlatform::ABP_MovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABP_MovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	OriginalLocation = GetActorLocation();
	
}

// Called every frame
void ABP_MovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();
	FVector TargetLocation = OriginalLocation + DistanceShouldTravel;
	float Speed = FVector::Distance(CurrentLocation, TargetLocation)/MoveTime;
	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
	SetActorLocation(NewLocation);
}

