// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <UObject/ObjectMacros.h>

#include "FItemData.h"
#include "Components/SphereComponent.h"
#include "interfaz/InterfazAttach.h"
#include "ItemMasterAttach.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class ACharacterprincipal;

UCLASS()
class PROYECTOEJERCICIO_API AItemMasterAttach : public AActor, public  IInterfazAttach
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemMasterAttach();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipable")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipable")
	USphereComponent* SphereCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipable")
	FName SocketName = "ManoDerechaSocket";
	
		
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Equipable")
	bool bEquipado = false;

	UPROPERTY(ReplicatedUsing = OnRep_OwningCharacter, BlueprintReadOnly, Category = "Equipable")
	ACharacterprincipal* OwningCharacter = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="UI")
	FItemData InfoUI;
	
	UFUNCTION()
	void OnRep_OwningCharacter();

	void EnablePickup();
	void DisablePickup();
	void EnablePickupDelayed(float Delay = 1.0f);

	virtual void Equipar_Implementation(AActor* Interactor) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

private:	
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item", meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* Itemmesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item", meta=(AllowPrivateAccess=true))
	USphereComponent* SphereCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Item", meta=(AllowPrivateAccess=true))
	FName SocketName="";
	*/
	FTimerHandle PickupTimerHandle;
};
