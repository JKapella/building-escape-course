// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;
	void FindAudioComponent();
	void FindPressurePlate();
	
private:
	float InitalYaw;
	float TargetYaw;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;

	UPROPERTY(EditAnywhere)
	float YawChange = 90.f;		
	
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;

	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 0.4f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 3.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 7.f;

	UPROPERTY(EditAnywhere)
	float DoorOpenWieght = 60.f;

	bool DoorOpenSoundPlayed = false;
	bool DoorClosedSoundPlayed = true;
};
