// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "CollisionQueryParams.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// delete it later
	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * DebugLineLength;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * 200;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
	
	// ...
}


UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle Not Found"));
	}

	return Result;
}

void UGrabber::Grab(){
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Physics Handle Not Found"));
        return;
	}

	FHitResult HitResult;
	bool HasHit = GetGrabblerInReach(HitResult);
	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10, 10, FColor::Green, false, 5);
	DrawDebugSphere(GetWorld(), HitResult.Location, 10, 10, FColor::Yellow, false, 5);

	if(HasHit)
	{
		UPrimitiveComponent* HitComponent = HitResult.GetComponent();
		if (HitComponent == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("No Hit Component"));
            return;
		}
		HitComponent->WakeAllRigidBodies();
		HitResult.GetActor()->Tags.Add("Grabbed");
		PhysicsHandle->GrabComponentAtLocationWithRotation(HitResult.GetComponent(),
		NAME_None,
		HitResult.ImpactPoint,
		GetComponentRotation());
	}
	AActor* HitActor = HitResult.GetActor();
	if (HitActor != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("Actor Grabbed %s"), *HitActor->GetActorNameOrLabel());
	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
	PhysicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
	AActor* GrabbedActor = PhysicsHandle->GetGrabbedComponent()->GetOwner();
	GrabbedActor->Tags.Remove("Grabbed");
	PhysicsHandle->ReleaseComponent();
	}
}

bool UGrabber::GetGrabblerInReach(FHitResult& OutHitResult) const
{
	UPhysicsHandleComponent* PhysicsHandle = GetPhysicsHandle();

	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * DebugLineLength;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 5, 5, FColor::Blue, false, 5);
	FCollisionShape Sphere = FCollisionShape::MakeSphere(5);

    // add ignorance for BP_FirstPerson collision
	AActor* OwnerActor = GetOwner();

    // Create a FCollisionQueryParams object
    FCollisionQueryParams Params(SCENE_QUERY_STAT(Grabber), true);

    // Add the owner actor to the ignored actors list if it's not null
    if (OwnerActor)
    {
        Params.AddIgnoredActor(OwnerActor);
    }
	
	return GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		Sphere,
		Params
	);
}