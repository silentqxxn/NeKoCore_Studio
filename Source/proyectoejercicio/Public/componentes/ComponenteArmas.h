// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComponenteArmas.generated.h"

class AWeaponMaster;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROYECTOEJERCICIO_API UComponenteArmas : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UComponenteArmas();
	
	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// ── Equipadas (activas) ──────────────────────────────────
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Armas")
	AWeaponMaster* ArmaManoDerecha = nullptr;     // melee

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Armas")
	AWeaponMaster* ArmaManoIzquierda = nullptr;   // escudo

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Armas")
	AWeaponMaster* ArmaEspalda = nullptr;         // distancia

	/*// ── Guardadas (reserva) ───────────────────────────────────
	UPROPERTY(Replicated, BlueprintReadOnly, Category="Armas")
	AWeaponMaster* ArmaManoDerechaGuardada = nullptr;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Armas")
	AWeaponMaster* ArmaManoIzquierdaGuardada = nullptr;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Armas")
	AWeaponMaster* ArmaEspaldaGuardada = nullptr;
*/
	UFUNCTION(BlueprintCallable, Server, Reliable, Category="Armas")
	void Server_RecogerArma(AWeaponMaster* Arma);

	//UFUNCTION(BlueprintCallable, Server, Reliable, Category="Armas")
	//void Server_IntercambiarArma(EEquipableSlot Slot);

	UFUNCTION(BlueprintCallable, Server, Reliable, Category="Armas")
	void Server_SoltarEquipada(EEquipableSlot Slot);
	
	
private:
	AWeaponMaster*& GetSlotEquipado(EEquipableSlot Slot);
//	AWeaponMaster*& GetSlotGuardado(EEquipableSlot Slot);

	void AttacharArma(AWeaponMaster* Arma, FName Socket);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
