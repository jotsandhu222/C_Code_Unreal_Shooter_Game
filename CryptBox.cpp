// Fill out your copyright notice in the Description page of Project Settings.


#include "CryptBox.h"


UCryptBox::UCryptBox()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCryptBox::BeginPlay()
{
	Super::BeginPlay();
	// ...
	
}

void UCryptBox::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* Actor = GetAcceptableActor();
    if(Actor && !Actor->ActorHasTag("Grabbed"))
    {
        UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
        if(Component)
        {
            Component->SetSimulatePhysics(false);
        }
        Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        Mover->SetShouldMove(true);
    }
    else
    {
        Mover->SetShouldMove(false);
    }

}

AActor* UCryptBox::GetAcceptableActor() const
{
    TArray <AActor* > Actors;
    GetOverlappingActors(Actors);

    for(AActor* Actor : Actors){
        if(Actor)
        {
            if(Actor->ActorHasTag(AcceptableTagName) && !Actor->ActorHasTag("Grabbed"))
            UE_LOG(LogTemp, Error, TEXT("Actor Released %s"), *Actor->GetActorNameOrLabel());
            return Actor;
        }
    }
    return nullptr;
}

void UCryptBox::SetMover(UMover* NewMover)
{
    Mover = NewMover;
}