// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UInputMappingContext;

class UItemDefinition;
class AEquippableToolBase;
class UEquippableToolDefinition;
class UInventoryComponent;

UCLASS(Abstract)
class STERN_API AThirdPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimBlueprint* AnimInstance;

	UPROPERTY(VIsibleAnywhere, Category = "Inventory")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tools")
	TObjectPtr<AEquippableToolBase> EquippedTool;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> ThirdPersonContext;
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* InteractAction;


public:
	UFUNCTION(Category = "Input")
	virtual void DoMove(float Right, float Forward);

	UFUNCTION(Category = "input")
	virtual void DoLook(float Yaw, float Pitch);

	UFUNCTION(Category = "Input")
	virtual void DoJumpStart();

	UFUNCTION(Category = "Input")
	virtual void DoJumpEnd();

	UFUNCTION(Category = "Input")
	virtual void DoAim(bool bIsAiming);

	UFUNCTION(Category = "Input")
	virtual void DoShoot(bool bIsShooting);

	UFUNCTION(Category = "Input")
	virtual void DoInteract(bool bIsInteracting);

public:
	UFUNCTION()
	void GiveItem(UItemDefinition* ItemDefinition);

	UFUNCTION()
	bool IsToolAlreadyOwned(UEquippableToolDefinition* ToolDefinition);

	UFUNCTION()
	void AttachTool(UEquippableToolDefinition* ToolDefinition);


protected:

	void Move(const FInputActionValue& Value);//** Move Input */
	void Look(const FInputActionValue& Value);//** Look Input */
	void Aim(const FInputActionValue& Value);//** Aim Input */
	void Shoot(const FInputActionValue& Value);//** Shoot Input */
	void Interact(const FInputActionValue& Value);//** Interact Input */


public:
	// Sets default values for this character's properties
	AThirdPersonCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
