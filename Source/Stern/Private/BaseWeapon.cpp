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

		FTransform SocketTransform = ToolMeshComponent->GetSocketTransform("Muzzle");
		FVector LineStart = SocketTransform.GetLocation();
		FVector ForwardVector = SocketTransform.GetRotation().GetForwardVector();
		FVector LineEnd = LineStart + (ForwardVector * 3000.0f);

		World->LineTraceSingleByChannel(Hit, LineStart, LineEnd, ECC_Visibility);

		FRotator BaseRotator = FRotator(0.f, 0.f, 90.0f);

		if (TracerSystem)
		{
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
				TracerSystem,
				ToolMeshComponent,
				NAME_None,
				LineStart,
				BaseRotator,
				FVector::OneVector,
				EAttachLocation::KeepWorldPosition,
				true,
				ENCPoolMethod::None,
				false,   // bAutoActivate = false — set params first
				true
			);

			if (!NiagaraComp)
			{
				return;
			}

			FVector TargetPosition = Hit.bBlockingHit ? Hit.ImpactPoint : Hit.TraceEnd;

			NiagaraComp->SetVariableVec3(FName("User.MuzzlePosition"), LineStart);
			NiagaraComp->SetVariableBool(FName("User.Trigger"), true);

			UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVector(
				NiagaraComp,
				FName("User.ImpactPositions"),
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