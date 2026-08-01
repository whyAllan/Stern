// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim/BaseAnimInstance.h"
#include "ThirdPersonCharacter.h"
#include "EquippableToolBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkinnedMeshComponent.h"
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

	//Has Tool?
	EquippedTool = OwnerCharacter->GetEquippedTool();
	
	if (EquippedTool)
	{
		if (EquippedTool->bNeedBothHands)
		{
			LeftHandAlpha = 1;


			FTransform WorldLeftHandSceneTransform = EquippedTool->LeftHandTransformComponent->GetComponentTransform();
			FVector BoneSpaceLocation;
			FRotator BoneSpaceRotation;

			OwnerCharacter->GetMesh()->TransformToBoneSpace(
				TEXT("hand_r"),
				WorldLeftHandSceneTransform.GetLocation(),
				WorldLeftHandSceneTransform.Rotator(),
				BoneSpaceLocation,
				BoneSpaceRotation
			);

			LeftHandTransform = FTransform(BoneSpaceRotation, BoneSpaceLocation, WorldLeftHandSceneTransform.GetScale3D());
		}
		else {
			LeftHandAlpha = 0;
		}
	}

}

void UBaseAnimInstance::UpdateToolBlendSpace(UBlendSpace* TargetBlendSpace)
{
	ToolBlendSpace = TargetBlendSpace; 
}