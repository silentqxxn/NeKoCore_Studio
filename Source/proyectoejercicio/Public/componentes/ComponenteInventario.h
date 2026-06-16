// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FItemData.h"
#include "interfaz/InterfazCrafteo.h"
#include "interfaz/InterfazRecogeItems.h"
#include "ComponenteInventario.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventarioActualizado, const TArray<FItemData>&, Items);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTOEJERCICIO_API UComponenteInventario : public UActorComponent, public IInterfazRecogeItems, public IInterfazCrafteo
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UComponenteInventario();
	
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing=OnRep_Items, BlueprintReadOnly)
	TArray<FItemData> Items;

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_AnadirItem(const FItemData& NuevoItem);

	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_AnadirTodos(const TArray<FItemData>& ListaItems);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool QueryInventory(FName ItemID, FItemData& OutItem) const;
	
	UPROPERTY(BlueprintAssignable)
	FOnInventarioActualizado OnInventarioActualizado;

	virtual bool RecogerItem_Implementation(const FItemData& Item) override;
	virtual bool RecogerTodos_Implementation(const TArray<FItemData>& Items) override;
	virtual bool PuedeRecoger_Implementation(const FItemData& Item) const override;
	
	virtual bool CraftearItem_Implementation(FName RecetaID) override;
	virtual bool PuedeCraftear_Implementation(FName RecetaID) const override;
	virtual TArray<FName> ObtenerRecetasDisponibles_Implementation() const override;
	
	
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Server_QuitarItem(FName ItemID, int32 Cantidad);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	
	
private:
	
	APlayerController* GetOwnerPC() const;
	
	UFUNCTION()
	void OnRep_Items();

};
