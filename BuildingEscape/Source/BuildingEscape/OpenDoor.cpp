// Copyright Jason Li 2017

#include "OpenDoor.h"
#include "Gameframework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT

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
	Owner = GetOwner();
	if (!PressurePlate) {
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *Owner->GetName())
	}
}

void UOpenDoor::OpenDoor()
{
	// Set the door rotation
	//Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
	OnOpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	// Set the door rotation
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll TriggerVolume every frame
	if (GetTotalMassOfActorOnPlate() > 40.f) // TODO make into a parameter
	{
		OpenDoor();
		DoorLastOpenTime = GetWorld()->GetTimeSeconds();
	}

	// Check if it's time to close the door
	if ((GetWorld()->GetRealTimeSeconds() - DoorLastOpenTime)>DoorCloseDelay) {
		CloseDoor();
	}

}

float UOpenDoor::GetTotalMassOfActorOnPlate() {
	float TotalMass = 0.f;

	//Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (const auto* Actor : OverlappingActors) {
		UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *(Actor->GetName()))
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}
