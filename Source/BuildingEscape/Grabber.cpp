// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

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
	BindInputActions();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent) {

		FVector PlayerViewpointVector;
		FRotator PlayerViewpointRotator;

		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			OUT PlayerViewpointVector, 
			OUT PlayerViewpointRotator);

		FVector LineTraceEnd = PlayerViewpointVector + (PlayerViewpointRotator.Vector() * Reach);

		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

void UGrabber::Grab() {

	UE_LOG(LogTemp, Error, TEXT("GRAB!"));

	FVector PlayerViewpointVector;
	FRotator PlayerViewpointRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointVector, 
		OUT PlayerViewpointRotator);

	FVector LineTraceEnd = PlayerViewpointVector + (PlayerViewpointRotator.Vector() * Reach);

	FHitResult RaycastHitResult = RaycastToFirstPhysicsBodyInRange();
	UPrimitiveComponent* ComponentToGrab = RaycastHitResult.GetComponent();

	if (RaycastHitResult.GetActor()) {
		PhysicsHandle->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			LineTraceEnd
		);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Error, TEXT("RELEASE!"));

	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsHandle() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("No PhysicsHandleComponent on object %s"), *GetOwner()->GetName());
	}
}

void UGrabber::BindInputActions() {
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Input Component Found on %s"), *GetOwner()->GetName());
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

FHitResult UGrabber::RaycastToFirstPhysicsBodyInRange() const {
	
	FVector PlayerViewpointVector;
	FRotator PlayerViewpointRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewpointVector, 
		OUT PlayerViewpointRotator);

	FVector LineTraceEnd = PlayerViewpointVector + (PlayerViewpointRotator.Vector() * Reach);

	FHitResult Hit;
	FCollisionQueryParams TraceParams(
		FName(TEXT("")),
		false,
		GetOwner()
	);
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewpointVector,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	AActor* ActorHit = Hit.GetActor();

	return Hit;
}

void UGrabber::DrawDebugLine() {
	// 	DrawDebugLine(
	// 	GetWorld(), 
	// 	PlayerViewpointVector,
	// 	LineTraceEnd,
	// 	FColor(0, 255, 0),
	// 	false,
	// 	0.f,
	// 	0,
	// 	5.f 
	// );
}