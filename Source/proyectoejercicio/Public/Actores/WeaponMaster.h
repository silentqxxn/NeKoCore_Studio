// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "EEquipableSlot.h"
#include "GameFramework/Actor.h"
#include "EWeaponType.h"
#include "FItemData.h"
#include "interfaz/InterfazAttach.h"
#include "Actores/ItemMasterAttach.h"
#include "WeaponMaster.generated.h"

class ACharacterprincipal;
class USphereComponent;
class USkeletalMeshComponent;

UCLASS()
class PROYECTOEJERCICIO_API AWeaponMaster : public AItemMasterAttach 
{
	GENERATED_BODY()

public:
	AWeaponMaster();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats Arma")
	float Damage = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Stats Arma")
	float Range = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Arma")
	EEquipableSlot SlotEquipable = EEquipableSlot::ManoDerecha;

	// Socket donde se attachea cuando está guardada (reserva)
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	//FName SocketGuardado = "SpineSocket";
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	//FItemData InfoUI;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Arma")
	int32 MunicionActual = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arma")
	int32 CapacidadCargador = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arma")
	FName ID_Municion;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats Arma")
	float ProbabilidadCritico = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats Arma")
	float MultiplicadorCritico = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats Arma")
	float CadenciaAtaque = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats Arma")
	float DefensaDeEscudo = 5.0f;
	
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};