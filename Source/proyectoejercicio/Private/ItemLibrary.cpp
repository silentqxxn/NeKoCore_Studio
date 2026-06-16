// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemLibrary.h"
#include "FItemData.h"

UTexture2D* UItemLibrary::LoadItemIcon(const FItemData& Item)
{
	if (Item.Icono.IsNull()) return nullptr;
	return Item.Icono.LoadSynchronous();
}

bool UItemLibrary::GetItemByID(UDataTable* TablaItems, FName ItemID, FItemData& OutItem)
{
	if (!TablaItems) return false;

	TArray<FItemData*> Filas;
	TablaItems->GetAllRows<FItemData>(TEXT("GetItemByID"), Filas);

	for (FItemData* Fila : Filas)
	{
		if (Fila && Fila->ItemID == ItemID)
		{
			OutItem = *Fila;
			return true;
		}
	}

	return false;
}