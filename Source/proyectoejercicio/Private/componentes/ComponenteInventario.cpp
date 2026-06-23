// Fill out your copyright notice in the Description page of Project Settings.


#include "componentes/ComponenteInventario.h"

#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "Framework/PlayerControllerNuevo.h"


// Sets default values for this component's properties
UComponenteInventario::UComponenteInventario()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UComponenteInventario::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UComponenteInventario, Items);
}

// Called when the game starts
void UComponenteInventario::BeginPlay()
{
	Super::BeginPlay();

	
}

void UComponenteInventario::Server_AnadirItem_Implementation(const FItemData& NuevoItem)
{
	for (FItemData& Existente : Items)
	{
		if (Existente.ItemID == NuevoItem.ItemID &&
			Existente.Cantidad < Existente.MaxStack)
		{
			Existente.Cantidad = FMath::Min(
				Existente.Cantidad + NuevoItem.Cantidad,
				Existente.MaxStack);
			OnRep_Items();
			return;
		}
	}
	Items.Add(NuevoItem);
	OnRep_Items();
}
void UComponenteInventario::Server_AnadirTodos_Implementation(const TArray<FItemData>& ListaItems)
{
	for (const FItemData& Item : ListaItems)
		Server_AnadirItem(Item);
}


bool UComponenteInventario::QueryInventory(FName ItemID, FItemData& OutItem) const
{
	for (const FItemData& Item : Items)
	{
		if (Item.ItemID == ItemID)
		{
			OutItem = Item;
			return true;
		}
	}
	return false;
}


// Called every frame
void UComponenteInventario::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UComponenteInventario::OnRep_Items()
{
	OnInventarioActualizado.Broadcast(Items);
	APlayerController* PC = GetOwnerPC();
	if (!PC) return;

	if (APlayerControllerNuevo* MiPC = Cast<APlayerControllerNuevo>(PC))
		MiPC->ActualizarInventario(Items);
}

bool UComponenteInventario::RecogerItem_Implementation(const FItemData& Item)
{
	if (!PuedeRecoger_Implementation(Item)) return false;

	// Solo el servidor modifica el inventario
	if (GetOwner()->HasAuthority())
	{
		Server_AnadirItem(Item);
	}
	return true;
}

bool UComponenteInventario::RecogerTodos_Implementation(const TArray<FItemData>& NuevosItems)
{
	bool bTodos = true;
	for (const FItemData& Item : NuevosItems)
		bTodos &= RecogerItem_Implementation(Item);
	return bTodos;
}

bool UComponenteInventario::PuedeRecoger_Implementation(const FItemData& Item) const
{
	return Items.Num() < 15;
}

bool UComponenteInventario::CraftearItem_Implementation(FName RecetaID)
{
	if (!PuedeCraftear_Implementation(RecetaID)) return false;

	return true;
}

bool UComponenteInventario::PuedeCraftear_Implementation(FName RecetaID) const
{
	
	return false;
}

TArray<FName> UComponenteInventario::ObtenerRecetasDisponibles_Implementation() const
{
	return TArray<FName>();   
}

void UComponenteInventario::Server_QuitarItem_Implementation(FName ItemID, int32 Cantidad)
{
	for (int32 i = 0; i < Items.Num(); i++)
	{
		if (Items[i].ItemID == ItemID)
		{
			Items[i].Cantidad -= Cantidad;

			if (Items[i].Cantidad <= 0)
				Items.RemoveAt(i);

			OnRep_Items();
			return;
		}
	}

}

APlayerController* UComponenteInventario::GetOwnerPC() const
{
	if (APawn* Pawn = Cast<APawn>(GetOwner()))
		return Pawn->GetLocalViewingPlayerController();
	return nullptr;
}


