// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InventoryComponent.h"
#include "Data/ItemDefinition.h"
#include "EquippableToolBase.h"
#include "EquippableToolDefinition.h"

// Sets default values
AThirdPersonCharacter::AThirdPersonCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 100.f);

	// Set Control Rotation
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 300.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;


	// Configure Camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraComponent"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	// Configure Follow Camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//Inventory
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

}

// Called when the game starts or when spawned
void AThirdPersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine != nullptr);

	GetMesh()->SetAnimInstanceClass(AnimInstance->GeneratedClass);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ThirdPersonContext, 0);
		}
	}


}

// Called every frame
void AThirdPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AThirdPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::Look);

		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::Aim);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AThirdPersonCharacter::Shoot);


	}



}

void AThirdPersonCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AThirdPersonCharacter::Aim(const FInputActionValue& Value)
{
	bool bIsAiming = Value.Get<bool>();

	DoAim(bIsAiming);
}

void AThirdPersonCharacter::Shoot(const FInputActionValue& Value)
{
	bool bIsShooting = Value.Get<bool>();

	DoShoot(bIsShooting);
}

void AThirdPersonCharacter::Interact(const FInputActionValue& Value)
{
	bool bIsInteracting = Value.Get<bool>();

	 DoInteract(bIsInteracting);
}


void AThirdPersonCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook( LookAxisVector.X, - LookAxisVector.Y);
}

void AThirdPersonCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AThirdPersonCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AThirdPersonCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AThirdPersonCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AThirdPersonCharacter::DoAim(bool bIsAiming)
{
}

void AThirdPersonCharacter::DoShoot(bool bIsShooting)
{
	if (EquippedTool != nullptr)
	{
		EquippedTool->Use();
	}
}


void AThirdPersonCharacter::DoInteract(bool bIsInteracting)
{
}
void AThirdPersonCharacter::GiveItem(UItemDefinition* ItemDefinition)
{
	
	switch (ItemDefinition->ItemType)
	{
	case EItemType::Tool:
	{
		UEquippableToolDefinition* ToolDefinition = Cast<UEquippableToolDefinition>(ItemDefinition);

		if (ToolDefinition != nullptr)
		{
			AttachTool(ToolDefinition);
		}
		else
		{

		}
		break;
	}

	case EItemType::Consumable:
	{
		break;
	}
	default:
	{
		break;
	}
	}
}

bool AThirdPersonCharacter::IsToolAlreadyOwned(UEquippableToolDefinition* ToolDefinition)
{
 for (UEquippableToolDefinition* InventoryItem : InventoryComponent->ToolInventory)
 {
	 if (ToolDefinition->ID == InventoryItem->ID)
	 {
		 return true;
	 }
 }
 return false;
}

void AThirdPersonCharacter::AttachTool(UEquippableToolDefinition* ToolDefinition)
{
	if(!IsToolAlreadyOwned(ToolDefinition))
	{
		AEquippableToolBase* ToolToEquip = GetWorld()->SpawnActor<AEquippableToolBase>(ToolDefinition->ToolAsset, this->GetActorTransform());
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

		ToolToEquip->AttachToActor(this, AttachmentRules);
		ToolToEquip->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("HandGrip_R")));

		GetMesh()->SetAnimInstanceClass(ToolToEquip->ThirdPersonToolAnim->GeneratedClass);
		
		//Add to to inventory 
		InventoryComponent->ToolInventory.Add(ToolDefinition);
		ToolToEquip->OwningCharacter = this;
		EquippedTool = ToolToEquip;

	}
}


