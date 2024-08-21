// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "MyCharacter.generated.h"

UCLASS()
class DUPLICATEACTORS_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveHori(float value);
	void MoveVert(float value);

	void RotHori(float value);
	void RotVert(float value);

	void Ray();

	void Duplicate(AActor* original);

	void SwitchShift();
	void Telekinesis();

	void SwitchCtrl();
	void Levitate();

	void Resizing(float value);

	UPROPERTY(EditAnywhere, Category = "Camera")
		UCameraComponent* cam;

	bool holdingShift;
	AActor* target;
	FVector diff;

	bool holdingCtrl;
	UStaticMeshComponent* mesh;
};
