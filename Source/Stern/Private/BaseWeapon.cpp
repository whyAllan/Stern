// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "EnhancedInputComponent.h"
#include "ThirdPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"




ABaseWeapon::ABaseWeapon(){

	MagazineComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagazineComponent"));
	check(MagazineComponent != nullptr);
	MagazineComponent->SetCollisionProfileName(TEXT("NoCollision"));
	
	BoltComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoltComponent"));
	check(BoltComponent != nullptr);
	BoltComponent->SetCollisionProfileName(TEXT("NoCollision"));

	SilencerComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SilencerComponent"));
	check(SilencerComponent != nullptr);
	SilencerComponent->SetCollisionProfileName(TEXT("NoCollision"));
	SilencerComponent->SetVisibility(false); 
	SilencerComponent->SetComponentTickEnabled(false);

	MagazineComponent->SetupAttachment(ToolMeshComponent);
	BoltComponent->SetupAttachment(ToolMeshComponent);
	SilencerComponent->SetupAttachment(ToolMeshComponent);
}


// Shoot
void ABaseWeapon::Use()
{
	UWorld* const World = GetWorld();
	if (World != nullptr )
	{
		FHitResult Hit;

		FTransform SocketTransform = ToolMeshComponent->GetSocketTransform("Muzzle");
		FVector LineStart = SocketTransform.GetLocation();
		FVector ForwardVector = SocketTransform.GetRotation().GetForwardVector();
		FVector LineEnd = LineStart + (ForwardVector * 3000.0f);

		World->LineTraceSingleByChannel(Hit, LineStart, LineEnd, ECC_Visibility);
		
	}
}


// Bind input
void ABaseWeapon::BindInputAction(const UInputAction* InputToBind)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(OwningCharacter->GetController()))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(InputToBind, ETriggerEvent::Triggered, this, &ABaseWeapon::Use);
		}
	}
}


void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasSilencer)
	{
		SilencerComponent->SetVisibility(true);
		SilencerComponent->SetComponentTickEnabled(true);
	}
}