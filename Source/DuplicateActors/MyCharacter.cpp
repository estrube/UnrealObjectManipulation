// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cam->AttachTo(RootComponent);
	cam->SetRelativeLocation(FVector(0, 0, 40));

	holdingShift = false;
	holdingCtrl = false;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (target)
	{
		if (holdingShift)
		{
			Telekinesis();
		}
		else if (holdingCtrl)
		{
			Levitate();
		}
	}

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("Hori", this, &AMyCharacter::MoveHori);
	InputComponent->BindAxis("Vert", this, &AMyCharacter::MoveVert);

	InputComponent->BindAxis("HoriRot", this, &AMyCharacter::RotHori);
	InputComponent->BindAxis("VertRot", this, &AMyCharacter::RotVert);

	InputComponent->BindAction("Cast", IE_Pressed, this, &AMyCharacter::Ray);
	InputComponent->BindAction("Telekinesis", IE_Pressed, this, &AMyCharacter::SwitchShift);
	InputComponent->BindAction("Telekinesis", IE_Released, this, &AMyCharacter::SwitchShift);
	InputComponent->BindAction("Levitate", IE_Pressed, this, &AMyCharacter::SwitchCtrl);
	InputComponent->BindAction("Levitate", IE_Released, this, &AMyCharacter::SwitchCtrl);

	InputComponent->BindAxis("Resizing", this, &AMyCharacter::Resizing);

}

void AMyCharacter::Resizing(float value)
{
	if (value)
	{
		if (target && holdingShift)
		{
			FVector scale = target->GetActorScale3D();
			if (scale.Size() < 10 && value > 0)
			{
				target->SetActorScale3D(scale * 1.05f);
			}
			if (scale.Size() > 0.5f && value < 0)
			{
				target->SetActorScale3D(scale * 0.95f);
			}
		}
	}
}

void AMyCharacter::SwitchCtrl()
{
	holdingCtrl = !holdingCtrl;
	if (!holdingCtrl)
	{
		if (mesh)
		{
			mesh->SetSimulatePhysics(true);
		}
		mesh = NULL;
		target = NULL;
	}
}

void AMyCharacter::SwitchShift()
{
	holdingShift = !holdingShift;
	if (!holdingShift)
	{
		if (mesh)
		{
			mesh->SetSimulatePhysics(true);
		}
		mesh = NULL;
		target = NULL;
	}
}

void AMyCharacter::Ray()
{
	FVector start = cam->GetComponentLocation();
	FVector forward = cam->GetForwardVector();
	start = FVector(start.X + (forward.X * 100), start.Y + (forward.Y * 100), start.Z + (forward.Z * 100));
	FVector end = start + (forward * 1000);
	FHitResult hit;

	if (GetWorld())
	{
		bool actorHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Pawn, FCollisionQueryParams(), FCollisionResponseParams());
		DrawDebugLine(GetWorld(), start, end, FColor::Red, false, 2.f, 0.f, 10.f);
		if (actorHit && hit.GetActor())
		{
			if (holdingShift)
			{
				target = hit.GetActor();
				diff = target->GetActorLocation() - hit.ImpactPoint;
				mesh = target->FindComponentByClass<UStaticMeshComponent>();
				mesh->SetSimulatePhysics(false);
			}
			else if (holdingCtrl)
			{
				target = hit.GetActor();
				mesh = target->FindComponentByClass<UStaticMeshComponent>();
				mesh->SetSimulatePhysics(false);
			}
			else
			{
				Duplicate(hit.GetActor());
			}
		}
	}
}

void AMyCharacter::Levitate()
{
	FVector pos = target->GetActorLocation();
	pos.Z += 5;
	target->SetActorRelativeLocation(pos);
}

void AMyCharacter::Telekinesis()
{
	FVector forward = cam->GetForwardVector();
	FVector curLoc = target->GetActorLocation();
	FVector loc = cam->GetComponentLocation();

	loc += diff;

	float dist = FVector::Distance(loc, curLoc);

	target->SetActorRelativeLocation(FVector(loc.X + (forward.X * dist), loc.Y + (forward.Y * dist), loc.Z + (forward.Z * dist)));
}

void AMyCharacter::Duplicate(AActor* original)
{
	if (original)
	{
		int randX = FMath::RandRange(-500, 500);
		int randY = FMath::RandRange(-500, 500);

		FActorSpawnParameters spawnParams = FActorSpawnParameters();
		spawnParams.Template = original;
		if (GetWorld())
		{
			GetWorld()->SpawnActor<AActor>(FVector(randX, randY, 0), FRotator(), spawnParams);
		}
	}
}

void AMyCharacter::MoveHori(float value)
{
	if (value)
	{
		AddMovementInput(GetActorRightVector(), value);
	}
}

void AMyCharacter::MoveVert(float value)
{
	if (value)
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

void AMyCharacter::RotHori(float value)
{
	if (value)
	{
		AddActorLocalRotation(FRotator(0, value, 0));
	}
}

void AMyCharacter::RotVert(float value)
{
	if (value)
	{
		float temp = cam->GetRelativeRotation().Pitch + value;

		if (temp < 65 && temp > -65)
		{
			cam->AddLocalRotation(FRotator(value, 0, 0));
		}
		else if (temp > 65 && value < 0)
		{
			cam->AddLocalRotation(FRotator(value, 0, 0));
		}
		else if (temp < -65 && value > 0)
		{
			cam->AddLocalRotation(FRotator(value, 0, 0));
		}
	}
}