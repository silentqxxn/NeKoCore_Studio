// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InterfazConsumible.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterfazConsumible : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROYECTOEJERCICIO_API IInterfazConsumible
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interfaces | Consumibles")
	bool ConsumirItem(FName ItemID, float CantidadEfecto);
};
