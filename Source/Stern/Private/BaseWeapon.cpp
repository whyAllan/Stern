// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "EnhancedInputComponent.h"
#include "ThirdPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"


// Shoot
void ABaseWeapon::Use()
{
	UWorld* const World = GetWorld();
	if (World != nullptr )
	{
		FHitResult Hit;
		FRotator SpawnRotation = ToolMeshComponent->GetSocketRotation("Muzzle");

		FVector LineStart = ToolMeshComponent->GetSocketLocation("Muzzle");

		FVector LineEnd = LineStart + (OwningCharacter->GetFollowCamera()->GetForwardVector() * 3000);

		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, LineStart, LineEnd, ECC_Visibility);

		if (TracerSystem)
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
				TracerSystem,
				ToolMeshComponent,
				FName("Muzzle"),
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				true,   // auto destroy
				false
			);

			if (!NiagaraComp)
			{
				return;
			}

			FVector TargetPosition = Hit.bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd;
			// Vector user param (MuzzlePosition)
			NiagaraComp->SetVariableVec3(FName("MuzzlePosition"), LineStart);

			// Bool user param (Trigger)
			NiagaraComp->SetVariableBool(FName("Trigger"), true);

			// Data Interface Array Float3 (ImpactPosition) - separate function library
			UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(
				NiagaraComp,
				FName("ImpactPosition"),
				TArray<FVector>{ TargetPosition }
			);

			NiagaraComp->Activate();


		}
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