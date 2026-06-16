// Fill out your copyright notice in the Description page of Project Settings.


#include "wiget/ItemSlotWidget.h"

void UItemSlotWidget::SetItemData(const FItemData& NuevoItem)
{
	ItemData = NuevoItem;
	OnItemDataActualizado(ItemData);
}
