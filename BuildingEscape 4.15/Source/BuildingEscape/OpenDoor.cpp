// Copyright JustGeezy Games 2017

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
	
}

void UOpenDoor::OpenDoor()
{
	FRotator DoorOpen = FRotator(0.f, OpenAngle, 0.f);


	Owner->SetActorRotation(DoorOpen);
}

void UOpenDoor::CloseDoor()
{
	FRotator DoorClose = FRotator(0.f, 0.f, 0.f);
	
	Owner->SetActorRotation(DoorClose);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Poll the trigger volume
	if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		OpenDoor();
	}
/*	else if (PressurePlate->IsOverlappingActor(ActorThatOpens))
	{
		CloseDoor();
	}*/
}

