// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "ItemData.generated.h"

class UItemDefinition;

// Defines item type
UENUM(BlueprintType)
enum class EItemType : uint8
{
	Tool UMETA(DisplayName = "Tool"),
	Consumable UMETA(DisplayName = "Consumable")
};

// Textual Item data
USTRUCT(BlueprintType)
struct FItemText
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;
};


// Base item Definition
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	EItemType ItemType;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FItemText ItemText;

	// Reference to the respective Item Definition
	UPROPERTY(EditAnywhere, Category = "ItemData")
	TSoftObjectPtr<UItemDefinition> ItemBase;
};