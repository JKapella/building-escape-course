// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

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

	InitalYaw = GetOwner()->GetActorRotation().Yaw;
	TargetYaw = YawChange + InitalYaw;

	if (!PressurePlate) {
		FString ThisActor = GetOwner()->GetName();
		UE_LOG(LogTemp, Error, TEXT("Pressure plate unassigned in %s"), *ThisActor);
	}

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens)) {
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	} else {
		if ((GetWorld()->GetTimeSeconds() - DoorLastOpened) > DoorCloseDelay) {
			CloseDoor(DeltaTime);
		}
	}

	
}

void UOpenDoor::OpenDoor(float DeltaTime) {

	float Yaw = GetOwner()->GetActorRotation().Yaw;
	FRotator OpenDoor(0.f, FMath::FInterpTo(Yaw, TargetYaw, DeltaTime, DoorOpenSpeed), 0.f);
	GetOwner()->SetActorRotation(OpenDoor);
}

void UOpenDoor::CloseDoor(float DeltaTime) {
	float Yaw = GetOwner()->GetActorRotation().Yaw;
	FRotator CloseDoor(0.f, FMath::FInterpTo(Yaw, InitalYaw, DeltaTime, DoorCloseSpeed), 0.f);
	GetOwner()->SetActorRotation(CloseDoor);
}
