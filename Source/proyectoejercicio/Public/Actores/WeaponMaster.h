// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponMaster.generated.h"

class ACharacterprincipal;
class USphereComponent;
class USkeletalMeshComponent;

UCLASS()
class PROYECTOEJERCICIO_API AWeaponMaster : public AActor
{
	GENERATED_BODY()

public:
	AWeaponMaster();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Stats")
	float Damage = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon|Stats")
	float Range = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon")
	FName SocketName = "EspadaSocket";

	void EnablePickup();
	void DisablePickup();
	void EnablePickupDelayed(float Delay = 1.0f);

	
protected:
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

private:
	
	FTimerHandle PickupTimerHandle;

	UPROPERTY()
	USphereComponent* PickupCollision;

};