// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ItemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class PROYECTOEJERCICIO_API UItemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Items")
	static UTexture2D* LoadItemIcon(const FItemData& Item);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Items")
	static bool GetItemByID(UDataTable* TablaItems, FName ItemID, FItemData& OutItem);
};
