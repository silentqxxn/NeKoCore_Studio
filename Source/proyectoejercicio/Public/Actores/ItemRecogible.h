// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FItemData.h"
#include "GameFramework/Actor.h"
#include "ItemRecogible.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class PROYECTOEJERCICIO_API AItemRecogible : public AActor


{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemRecogible();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshItem;
	
	UPROPERTY(EditDefaultsOnly, Category="Item")
	UDataTable* TablaItems;

	UPROPERTY(EditAnywhere, Category="Item")
	FName ItemID;

	UPROPERTY(EditAnywhere, Category="Item")
	int32 Cantidad = 1;
	

	//FX
 	UFUNCTION(BlueprintImplementableEvent, Category="Item")
	void OnItemRecogido();   
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Interactuar(AActor* Interactor);
};
