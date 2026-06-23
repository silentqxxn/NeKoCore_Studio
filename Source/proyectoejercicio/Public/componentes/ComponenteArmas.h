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

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Armas")
	AWeaponMaster* ArmaManoDerecha = nullptr;     // melee

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Armas")
	AWeaponMaster* ArmaManoIzquierda = nullptr;   // escudo

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Armas")
	AWeaponMaster* ArmaEspalda = nullptr;         // distancia


	UFUNCTION(BlueprintCallable, Server, Reliable, Category="Armas")
	void Server_RecogerArma(AWeaponMaster* Arma);


	UFUNCTION(BlueprintCallable, Server, Reliable, Category="Armas")
	void Server_SoltarEquipada(EEquipableSlot Slot);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AttachVisual(AWeaponMaster* Arma, FName Socket);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DetachVisual(AWeaponMaster* Arma);
	
private:
	AWeaponMaster*& GetSlotEquipado(EEquipableSlot Slot);

	void AttacharArma(AWeaponMaster* Arma, FName Socket);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;
};
