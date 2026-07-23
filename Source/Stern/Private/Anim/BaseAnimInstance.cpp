// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/BaseAnimInstance.h"
#include "ThirdPersonCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerCharacter = Cast<AThirdPersonCharacter>(GetOwningActor());

	if (OwnerCharacter)
	{
		MovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwnerCharacter || !MovementComponent) return;

	// Velocity
	Velocity = MovementComponent->Velocity;
	Speed = Velocity.Size2D();
	bShouldMove = Speed > 0;

	FRotator ActorRotation = OwnerCharacter->GetActorRotation();
	Direction = UKismetMathLibrary::NormalizedDeltaRotator(Velocity.Rotation(), ActorRotation).Yaw;

	// Aim
	FRotator ControlRotation = OwnerCharacter->GetControlRotation();
	AimPitch = FMath::ClampAngle(
		UKismetMathLibrary::NormalizedDeltaRotator(ControlRotation, ActorRotation).Pitch,
		-90.0f, 90.0f);
	AimYaw = UKismetMathLibrary::NormalizedDeltaRotator(ControlRotation, ActorRotation).Yaw;

	// On Air?
	bIsFalling = MovementComponent->IsFalling();
}

void UBaseAnimInstance::UpdateToolBlendSpace(UBlendSpace* TargetBlendSpace)
{
	ToolBlendSpace = TargetBlendSpace; 
}