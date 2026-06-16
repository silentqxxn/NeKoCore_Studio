// Fill out your copyright notice in the Description page of Project Settings.


#include "wiget/WidgetCrafteo.h"
#include "componentes/ComponenteCrafteo.h"
#include "Components/WrapBox.h"
#include "wiget/CraftSlotWidget.h"
void UWidgetCrafteo::Inicializar(UComponenteCrafteo* InComponente)
{
	CompCrafteo = InComponente;
	Refrescar();
}

void UWidgetCrafteo::Refrescar()
{
	if (!ContenedorRecetas || !ClaseSlot || !CompCrafteo) return;

	ContenedorRecetas->ClearChildren();

	for (const FRecetaData& Receta : CompCrafteo->ObtenerTodasLasRecetas())
	{
		UCraftSlotWidget* NuevoSlot = CreateWidget<UCraftSlotWidget>(this, ClaseSlot);
		if (!NuevoSlot) continue;

		NuevoSlot->Configurar(Receta, CompCrafteo);
		ContenedorRecetas->AddChildToWrapBox(NuevoSlot);
	}
}