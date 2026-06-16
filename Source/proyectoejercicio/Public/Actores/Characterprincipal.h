// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "interfaz/interfazparahacerdanio.h"
#include "InputActionValue.h"
#include "Components/SkeletalMeshComponent.h"
#include "interfaz/InterfazAttach.h"
#include "interfaz/InterfazCrafteo.h"
#include "interfaz/InterfazRecogeItems.h"
#include "componentes/ComponenteArmas.h"
#include "Characterprincipal.generated.h"


class UInputAction;
class UInputMappingContext;
class AWeaponMaster;
class UComponenteInventario; 
struct FItemData;
class UComponenteCrafteo;
class USkeletalMeshComponent;
class UComponenteExperiencia;
struct FExperienciaData;
class UComponenteArmas;

UCLASS(Abstract)
class PROYECTOEJERCICIO_API ACharacterprincipal : public ACharacter, public IInterfazRecogeItems , public Iinterfazparahacerdanio, public IInterfazAttach,public IInterfazCrafteo
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipoActualizado);


public:
	// Sets default values for this character's properties
	ACharacterprincipal();
	

	
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual USkeletalMeshComponent* GetSkeletalMesh_Implementation() override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Componentes")
	UComponenteInventario* CompInventario;
	
	
	virtual bool RecogerItem_Implementation(const FItemData& Item) override;
	virtual bool RecogerTodos_Implementation(const TArray<FItemData>& Items) override;
	virtual bool PuedeRecoger_Implementation(const FItemData& Item) const override;

	virtual bool CraftearItem_Implementation(FName RecetaID) override;
	virtual bool PuedeCraftear_Implementation(FName RecetaID) const override;
	virtual TArray<FName> ObtenerRecetasDisponibles_Implementation() const override;

	
	// Sistema de armas
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	class UComponenteArmas* CompArmas;
	
	UFUNCTION(BlueprintCallable)
	void MostrarMensaje();
	
	UPROPERTY(BlueprintAssignable)
	FOnEquipoActualizado OnEquipoActualizado;
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_EquipItemAttach(AItemMasterAttach* Item);

	UFUNCTION(BlueprintCallable, Category="Movimiento")
	void SetMovimientoBloqueado(bool bBloqueado);
	
	UPROPERTY(ReplicatedUsing=OnRep_CurrentItemAttach, BlueprintReadOnly)
	AItemMasterAttach* CurrentItemAttach = nullptr;

	UFUNCTION()
	void OnRep_CurrentItemAttach();
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Equipamiento")
	AItemMasterAttach* ItemManoDerecha = nullptr;

	UFUNCTION(Server, Reliable)
	void Server_EquipItemGeneric(AItemMasterAttach* NuevoItem);

	UPROPERTY(ReplicatedUsing=OnRep_ItemManoIzquierda, BlueprintReadOnly, Category="Equipamiento")
	AItemMasterAttach* ItemManoIzquierda = nullptr;

	UFUNCTION()
	void OnRep_ItemManoIzquierda();
	
	//craft
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Componentes")
	UComponenteCrafteo* CompCrafteo;

	UFUNCTION(BlueprintCallable)
	void SetItemInteractuable(AActor* Item);
	
	UFUNCTION(BlueprintCallable)
	void ClearItemInteractuable(AActor* Item);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_CurrentWeapon();

	// ---- Enhanced Input ----
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* IMC_Player;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Movimiento;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Interactuar;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Componentes")
	UComponenteExperiencia* CompExperiencia;
	
	void Move(const FInputActionValue& Value);
	void TryInteract();

private:
	void DropCurrentWeapon();
	bool bMovimientoBloqueado = false;
	
	void DesequiparItemAttach(AItemMasterAttach* Item);


	UPROPERTY()
	AActor* ItemInteractuableCercano = nullptr;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	
};
