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

	//Look for attached Input Component
	Input = GetOwner()->FindComponentByClass<UInputComponent>();
	if (Input)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component found on %s!"), *GetOwner()->GetName())
		//Bind the input axis
		Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabber::Released);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Input component found on %s"), *GetOwner()->GetName())
	}
}

void FindPhysicsHandle()
{
	//Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		//Physics handle  is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No Physics Handle component found on %s"), *GetOwner()->GetName())
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Key Pressed"))
}

void UGrabber::Released()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab Key Released"))
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

	//FString PVPLoc = PlayerViewPointLocation.ToString();
	//FString PVPRot = PlayerViewPointRotation.ToString();
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s Rotation: %s"), *PVPLoc, *PVPRot)

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	//Draw a red trace in the world
	DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(255, 0, 0), false, 0, 0, 10.0f);

	//Setup query parameters
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	// Line-trace (AKA Ray-cast) out to reach distance
	
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByObjectType(HitResult, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	// See what we hit
	AActor* ActorHit = HitResult.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("You are at %s"), *ActorHit->GetName())
	}

}

