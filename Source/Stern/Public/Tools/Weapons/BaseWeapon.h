// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tools/EquippableToolBase.h"
#include "Components/StaticMeshComponent.h"
#include "BaseWeapon.generated.h"


/**
 * 
 */
class UNiagaraSystem;
class UNiagaraComponent;
class USceneComponent;

UCLASS()
class STERN_API ABaseWeapon : public AEquippableToolBase
{
	GENERATED_BODY()

public:

	virtual void Use() override;

	virtual void BindInputAction(const UInputAction* InputToBind) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	UNiagaraSystem* MuzzleFlashSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components | Effects")
	UNiagaraComponent* MuzzleFlashComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MagazineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BoltComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	bool HasSilencer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Variables")
	UStaticMeshComponent* SilencerComponent;


public:
	ABaseWeapon();
protected:
	virtual void BeginPlay() override;
};
