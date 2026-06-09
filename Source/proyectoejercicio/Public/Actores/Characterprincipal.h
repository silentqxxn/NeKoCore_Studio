// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "interfaz/interfazmonedas.h"
#include "interfaz/interfazparahacerdanio.h"
#include "InputActionValue.h"
#include "Components/SkeletalMeshComponent.h"
#include "interfaz/InterfazAttach.h"
#include "Characterprincipal.generated.h"


class UInputAction;
class UInputMappingContext;
class AWeaponMaster;

UCLASS(Abstract)
class PROYECTOEJERCICIO_API ACharacterprincipal : public ACharacter, public Iinterfazmonedas , public Iinterfazparahacerdanio, public IInterfazAttach
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterprincipal();
	
	virtual void AddMonedas_Implementation(int Moneda) override;
	
	virtual USkeletalMeshComponent* GetSkeletalMesh_Implementation() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventario")
	int Monedas =0;

	
	UFUNCTION(BlueprintCallable)
	void MostrarMensaje();
 
	// Sistema de armas
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_EquipWeapon(AWeaponMaster* Weapon);

	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_DropWeapon();

	UPROPERTY(ReplicatedUsing=OnRep_CurrentWeapon, BlueprintReadOnly, Category="Weapon")
	AWeaponMaster* CurrentWeapon = nullptr;
	
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
	
	void Move(const FInputActionValue& Value);
	void TryInteract();

private:
	void DropCurrentWeapon();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
