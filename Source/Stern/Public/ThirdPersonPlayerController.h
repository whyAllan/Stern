// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ThirdPersonPlayerController.generated.h"


class UInputMappingContext;
class UUserWidget;
/**
 *
 */
UCLASS()
class STERN_API AThirdPersonPlayerController : public APlayerController
{
	GENERATED_BODY()

	protected:
		// Input Mapping Context
		UPROPERTY(EditAnywhere, Category = "input | Input Mappings")
		TArray<UInputMappingContext*>  DefaultMappingContexts;

		/** Gameplay initialization */
		virtual void BeginPlay() override;

		/** Input mapping context setup */
		virtual void SetupInputComponent() override;

		/** Returns true if the player should use UMG touch controls */
		bool ShouldUseTouchControls() const;



	
};
