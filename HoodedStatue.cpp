// Fill out your copyright notice in the Description page of Project Settings.


#include "HoodedStatue.h"

// Sets default values
AHoodedStatue::AHoodedStatue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHoodedStatue::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHoodedStatue::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

