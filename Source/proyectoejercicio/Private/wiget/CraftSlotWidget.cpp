// Fill out your copyright notice in the Description page of Project Settings.


#include "wiget/CraftSlotWidget.h"
#include "componentes/ComponenteCrafteo.h"
#include "componentes/ComponenteInventario.h"

void UCraftSlotWidget::Configurar(const FRecetaData& Receta, UComponenteCrafteo* InComponente)
{
	RecetaData = Receta;
	Componente = InComponente;

	const bool bPuede = Componente && Componente->PuedeCraftear(Receta.RecetaID);
	OnRecetaActualizada(RecetaData, bPuede);
}

void UCraftSlotWidget::OnCraftearPresionado()
{
	if (!Componente) return;
	Componente->Server_Craftear(RecetaData.RecetaID);
}

FText UCraftSlotWidget::GetIngredientesTexto() const
{
	if (!Componente) return FText::GetEmpty();

	UComponenteInventario* Inv = Componente->GetInventario();
	FString Resultado;

	for (const FIngredienteReceta& Ing : RecetaData.Ingredientes)
	{
		int32 Tengo = 0;
		FItemData Item;
		if (Inv && Inv->QueryInventory(Ing.ItemID, Item))
			Tengo = Item.Cantidad;

		Resultado += FString::Printf(TEXT("%s: %d/%d\n"),
			*Ing.ItemID.ToString(), Tengo, Ing.Cantidad);
	}

	return FText::FromString(Resultado);
}