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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	float Damage = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	float Range = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	EEquipableSlot SlotEquipable = EEquipableSlot::ManoDerecha;

	// Socket donde se attachea cuando está guardada (reserva)
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	//FName SocketGuardado = "SpineSocket";
	
	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	//FItemData InfoUI;
	
protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;
};