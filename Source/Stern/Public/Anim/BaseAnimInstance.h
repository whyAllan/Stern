// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/BlendSpace.h"
#include "Animation/AnimSequence.h"
#include "BaseAnimInstance.generated.h"


class AThirdPersonCharacter;
class AEquippableToolBase;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class STERN_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UBlendSpace> ToolBlendSpace;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimSequence> IdleAnim;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	TObjectPtr<UAnimSequence> AimAnim;


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tools")
	TObjectPtr<AEquippableToolBase> EquippedTool;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	float AimPitch;

	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	float AimYaw;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsInCover;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsCrouching;

	UPROPERTY(BlueprintReadOnly, Category = "Variable")
	bool bShouldMove;

	UPROPERTY(BlueprintReadOnly, Category = "Variable")
	float LeftHandAlpha;

	UPROPERTY(BlueprintReadOnly, Category = "Variable")
	FTransform LeftHandTransform;


protected:

	UPROPERTY()
	TObjectPtr<AThirdPersonCharacter> OwnerCharacter;
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MovementComponent;

public:

	UFUNCTION()
	void UpdateToolBlendSpace(UBlendSpace* TargetBlendSpace);
};
