#include "Actores/ItemRecogible.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "interfaz/InterfazRecogeItems.h"
#include "FItemData.h"
#include <Actores/Characterprincipal.h>

#include "ItemLibrary.h"
#include "interfaz/InterfazRecogeItems.h"

// Sets default values
AItemRecogible::AItemRecogible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SphereCollision = CreateDefaultSubobject <USphereComponent> (TEXT("SphereCollision"));
	RootComponent = SphereCollision;

	MeshItem = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("SphereMesh"));
	MeshItem->SetupAttachment(RootComponent);
	MeshItem->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereCollision->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void AItemRecogible::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemRecogible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemRecogible::Interactuar(AActor* Interactor)
{
	if (!Interactor) return;

	
	if (!Interactor->Implements<UInterfazRecogeItems>()) return;

	

	if (!TablaItems || ItemID.IsNone()) return;

	FItemData Item;
	const bool bEncontrado = UItemLibrary::GetItemByID(TablaItems, ItemID, Item);


	if (!bEncontrado) return;

	Item.Cantidad = Cantidad;

	const bool bRecogido = IInterfazRecogeItems::Execute_RecogerItem(Interactor, Item);

	if (bRecogido)
	{
		OnItemRecogido();
		Destroy();
	}
}


void AItemRecogible::NotifyActorBeginOverlap(AActor* OtherActor)
{
	
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (!HasAuthority()) return;
	if (!OtherActor || !OtherActor->Implements<UInterfazRecogeItems>()) return;
	
	Interactuar(OtherActor);
}
