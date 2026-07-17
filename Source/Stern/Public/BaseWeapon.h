// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquippableToolBase.h"
#include "Components/StaticMeshComponent.h"
#include "BaseWeapon.generated.h"


/**
 * 
 */
class UNiagaraSystem;
UCLASS()
class STERN_API ABaseWeapon : public AEquippableToolBase
{
	GENERATED_BODY()

public:

	virtual void Use() override;

	virtual void BindInputAction(const UInputAction* InputToBind) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Components")
	UStaticMeshComponent* MagazineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Component")
	UStaticMeshComponent* BoltComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Variables")
	bool HasSilencer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon | Variables")
	UStaticMeshComponent* SilencerComponent;

public:
	ABaseWeapon();
protected:
	virtual void BeginPlay() override;
};
