// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InterfazAttach.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInterfazAttach : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROYECTOEJERCICIO_API IInterfazAttach
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interfaz player")
	USkeletalMeshComponent* GetSkeletalMesh();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interfaz player")
	void Equipar(AActor* Interactor);
};
