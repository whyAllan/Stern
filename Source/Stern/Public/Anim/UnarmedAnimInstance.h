// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UnarmedAnimInstance.generated.h"

class AThirdPersonCharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class STERN_API UUnarmedAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:

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

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bShouldMove;

private:

	UPROPERTY()
	TObjectPtr<AThirdPersonCharacter> OwnerCharacter;
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MovementComponent;
};
