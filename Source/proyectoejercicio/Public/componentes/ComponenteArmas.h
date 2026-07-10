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

	UPROPERTY(ReplicatedUsing = OnRep_ArmaManoDerecha, BlueprintReadWrite, EditAnywhere, Category = "Armas")
	AWeaponMaster* ArmaManoDerecha;

	UPROPERTY(ReplicatedUsing = OnRep_ArmaManoIzquierda, BlueprintReadWrite, EditAnywhere, Category = "Armas")
	AWeaponMaster* ArmaManoIzquierda;

	UPROPERTY(ReplicatedUsing = OnRep_ArmaEspalda, BlueprintReadWrite, EditAnywhere, Category = "Armas")
	AWeaponMaster* ArmaEspalda;
	

	UFUNCTION(BlueprintCallable, Server, Reliable, Category="Armas")
	void Server_RecogerArma(AWeaponMaster* Arma);


	UFUNCTION(BlueprintCallable, Server, Reliable, Category="Armas")
	void Server_SoltarEquipada(EEquipableSlot Slot);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AttachVisual(AWeaponMaster* Arma, FName Socket);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DetachVisual(AWeaponMaster* Arma);
	UFUNCTION()
	void OnRep_ArmaManoDerecha(AWeaponMaster* ArmaVieja);

	UFUNCTION()
	void OnRep_ArmaManoIzquierda(AWeaponMaster* ArmaVieja);

	UFUNCTION()
	void OnRep_ArmaEspalda(AWeaponMaster* ArmaVieja);
	
private:
	AWeaponMaster*& GetSlotEquipado(EEquipableSlot Slot);
	void SetupArmaAdjuntada(AWeaponMaster* Arma);
	void SetupArmaDropeada(AWeaponMaster* Arma);
	void AttacharArma(AWeaponMaster* Arma, FName Socket);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;
};
