// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"

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
	TargetEndYaw = YawChange + InitalYaw;

	UE_LOG(LogTemp, Warning, TEXT("Target Yaw Of Door Is %f"), InitalYaw);
	UE_LOG(LogTemp, Warning, TEXT("Target Yaw Of Door Is %f"), YawChange);
	UE_LOG(LogTemp, Warning, TEXT("Target Yaw Of Door Is %f"), TargetEndYaw);

}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float Yaw = GetOwner()->GetActorRotation().Yaw;
	UE_LOG(LogTemp, Warning, TEXT("Target Yaw Of Door Is %f"), TargetEndYaw);

	FRotator OpenDoor(0.f, FMath::FInterpTo(Yaw, TargetEndYaw, DeltaTime, 2), 0.f);
	GetOwner()->SetActorRotation(OpenDoor);

	// FRotator OpenDoor(0.f, 90.f, 0.f);
	// GetOwner()->SetActorRotation(OpenDoor);
}

