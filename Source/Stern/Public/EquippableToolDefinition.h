// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/ItemDefinition.h"
#include "EquippableToolDefinition.generated.h"


class AEquippableToolBase;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class STERN_API UEquippableToolDefinition : public UItemDefinition
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AEquippableToolBase> ToolAsset;

	virtual UEquippableToolDefinition* CreateItemCopy(UObject* Outer) const override;
};
