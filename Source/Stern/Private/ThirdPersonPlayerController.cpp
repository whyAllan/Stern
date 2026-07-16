// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"


void AThirdPersonPlayerController::BeginPlay()
{
	Super::BeginPlay();

}

void AThirdPersonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add input Mapping context
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Contexts
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}
}