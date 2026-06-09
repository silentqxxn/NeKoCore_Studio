// Fill out your copyright notice in the Description page of Project Settings.


#include "Actores/ItemMasterAttach.h"

#include "interfaz/InterfazAttach.h"

// Sets default values
AItemMasterAttach::AItemMasterAttach()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicatingMovement(true);
	
	Itemmesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMesh"); SetRootComponent(Itemmesh);
	Itemmesh ->SetSimulatePhysics(false);
	Itemmesh->SetEnableGravity(false);

	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	SphereCollision->SetupAttachment(Itemmesh);
	SphereCollision->SetSphereRadius(100.0f);

}

// Called when the game starts or when spawned
void AItemMasterAttach::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemMasterAttach::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemMasterAttach::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	if (!HasAuthority()) return;
	if (!OtherActor) return;

	if (OtherActor->Implements<UInterfazAttach>())
	{
		USkeletalMeshComponent*SkeletalMeshComponent= IInterfazAttach::Execute_GetSkeletalMesh(OtherActor);
		if (!SkeletalMeshComponent) return;
		AttachToComponent(SkeletalMeshComponent,FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	}

}

