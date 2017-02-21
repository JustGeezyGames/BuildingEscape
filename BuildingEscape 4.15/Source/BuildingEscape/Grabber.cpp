// Copyright JustGeezy Games 2017

#include "BuildingEscape.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandle();
	SetupInput();
}

//Look for attached Input Component
void UGrabber::SetupInput()
{
	Input = GetOwner()->FindComponentByClass<UInputComponent>();
	if (Input)
	{
		Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabber::Released);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Input component found on %s"), *GetOwner()->GetName())
	}
}

//Look for attached Physics Handle
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle component found on %s"), *GetOwner()->GetName())
	}
}

void UGrabber::Grab()
{
	//Line trace to see if you reach any actors with physics body collision channel set
	FHitResult ComponentToGrab = GetFirstPhysicsBodyInReach();
	AActor* ActorHit = ComponentToGrab.GetActor();

	//If we hit something then attach a physics handle
	if (ActorHit)
	{
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponent(ComponentToGrab.GetComponent(), NAME_None, ComponentToGrab.GetComponent()->GetOwner()->GetActorLocation(), true);
	}
}

void UGrabber::Released()
{
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetReachEnd());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	//Setup query parameters
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(HitResult, GetReachStart(), GetReachEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	// See what we hit
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("You are at %s"), *ActorHit->GetName())
	}	
	return HitResult;
}

FVector UGrabber::GetReachStart()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachEnd()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

