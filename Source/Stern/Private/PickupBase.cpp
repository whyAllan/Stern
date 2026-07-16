// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupBase.h"
#include "Data/ItemDefinition.h"
#include "Data/ItemData.h" 

// Sets default values
APickupBase::APickupBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	check(PickupMeshComponent != nullptr);

	SetRootComponent(PickupMeshComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	check(SphereComponent != nullptr);

	SphereComponent->SetupAttachment(PickupMeshComponent);
	SphereComponent->SetSphereRadius(32.f);
	SphereComponent->SetGenerateOverlapEvents(true);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

// Called when the game starts or when spawned
void APickupBase::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComponent->OnComponentBeginOverlap.RemoveAll(this);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(
		this,
		&APickupBase::OnSphereBeginOverlap
	);

	InitializePickup();
}


// Called every frame
void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void APickupBase::InitializePickup()
{
	const FSoftObjectPath TablePath = PickupDataTable.ToSoftObjectPath();
	if (!TablePath.IsNull() && !PickupItemID.IsNone())
	{
		UDataTable* LoadedDataTable = PickupDataTable.IsValid()
			? PickupDataTable.Get()
			: PickupDataTable.LoadSynchronous();

		if (!LoadedDataTable)
		{
			return;
		}

		const FItemData* ItemDataRow = LoadedDataTable->FindRow<FItemData>(PickupItemID, PickupItemID.ToString());

		if (!ItemDataRow)
		{
			return;
		}

		UItemDefinition* TempItemDefinition = ItemDataRow->ItemBase.IsValid()
			? ItemDataRow->ItemBase.Get()
			: ItemDataRow->ItemBase.LoadSynchronous();
		if (!TempItemDefinition)
		{
			return;
		}

		ReferenceItem = TempItemDefinition->CreateItemCopy(this);

		// Resolve the item's world mesh from item definition.
		UStaticMesh* LoadedMesh = TempItemDefinition->WorldMesh.IsValid()
			? TempItemDefinition->WorldMesh.Get()
			: TempItemDefinition->WorldMesh.LoadSynchronous();
		if (LoadedMesh)
		{
			PickupMeshComponent->SetStaticMesh(LoadedMesh);
		}
		PickupMeshComponent->SetVisibility(true);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	}
}
// Check if it is the character overlaping the actor
void APickupBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AThirdPersonCharacter* Character = Cast<AThirdPersonCharacter>(OtherActor);
	if (Character != nullptr)
	{
		Character->GiveItem(ReferenceItem);
		PickupMeshComponent->SetVisibility(false);
		PickupMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (bShouldRespawn)
	{
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &APickupBase::InitializePickup, RespawnTime, false);
	}

}

void APickupBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	// Handle parent class property changes
	Super::PostEditChangeProperty(PropertyChangedEvent);

	// If a property was changed, get the name of the changed property. Otherwise use none.
	const FName ChangedPropertyName = PropertyChangedEvent.Property
		? PropertyChangedEvent.Property->GetFName()
		: NAME_None;

	// Verify that the changed property is one that affects item lookup in this class
	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickupBase, PickupItemID) ||
		ChangedPropertyName == GET_MEMBER_NAME_CHECKED(APickupBase, PickupDataTable))
	{
		InitializePickup();
	}

}