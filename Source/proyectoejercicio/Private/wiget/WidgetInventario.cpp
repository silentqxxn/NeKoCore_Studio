// Fill out your copyright notice in the Description page of Project Settings.


#include "wiget/WidgetInventario.h"
#include "Components/WrapBox.h"
#include "wiget/ItemSlotWidget.h"


void UWidgetInventario::RefrescarLista(const TArray<FItemData>& Items)
{
	if (!ContenedorItems) return;
	if (!ClaseSlot) return;

	ContenedorItems->ClearChildren();

	for (const FItemData& Item : Items)
	{
		UItemSlotWidget*  NuevoSlot= CreateWidget<UItemSlotWidget>(this, ClaseSlot);
		if (!NuevoSlot) continue;

		NuevoSlot->SetItemData(Item);
		ContenedorItems->AddChildToWrapBox(NuevoSlot);
	}
}
