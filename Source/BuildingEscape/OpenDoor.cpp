// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Components/AudioComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitalYaw = GetOwner()->GetActorRotation().Yaw;
	TargetYaw = YawChange + InitalYaw;

	FindPressurePlate();
	FindAudioComponent();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > DoorOpenWieght) {
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	} else {
		if ((GetWorld()->GetTimeSeconds() - DoorLastOpened) > DoorCloseDelay) {
			CloseDoor(DeltaTime);
		}
	}	
}

void UOpenDoor::OpenDoor(float DeltaTime) 
{

	float Yaw = GetOwner()->GetActorRotation().Yaw;
	FRotator OpenDoor(0.f, FMath::FInterpTo(Yaw, TargetYaw, DeltaTime, DoorOpenSpeed), 0.f);
	GetOwner()->SetActorRotation(OpenDoor);

	if (!AudioComponent) {return:}
	if (!DoorOpenSoundPlayed) {
		AudioComponent->Play();
		DoorOpenSoundPlayed = true;
		DoorClosedSoundPlayed =  false;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime) 
{
	float Yaw = GetOwner()->GetActorRotation().Yaw;
	FRotator CloseDoor(0.f, FMath::FInterpTo(Yaw, InitalYaw, DeltaTime, DoorCloseSpeed), 0.f);
	GetOwner()->SetActorRotation(CloseDoor);
	
	if (!AudioComponent) {return:}
	if (!DoorClosedSoundPlayed) {
		AudioComponent->Play();
		DoorOpenSoundPlayed = false;
		DoorClosedSoundPlayed =  true;
	}
}

float UOpenDoor::TotalMassOfActors() const 
{
	float TotalMass = 0.f;

	//find all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate){return TotalMass;}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	//add thier masses
	for (AActor* CurrentActor : OverlappingActors) {
		TotalMass += CurrentActor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

void UOpenDoor::FindAudioComponent() 
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent) {
		UE_LOG(LogTemp, Error, TEXT("NO audio component found on %s"), *GetOwner()->GetName())
	}
}

void UOpenDoor::FindPressurePlate() 
{
	if (!PressurePlate) {
		FString ThisActor = GetOwner()->GetName();
		UE_LOG(LogTemp, Error, TEXT("Pressure plate unassigned in %s"), *ThisActor);
	}
}