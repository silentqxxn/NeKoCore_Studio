// Fill out your copyright notice in the Description page of Project Settings.


#include "Actores/FogataNueva.h"

#include "componentes/ComponenteCrafteo.h"
#include "Components/SphereComponent.h"
#include "interfaz/InterfazCrafteo.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AFogataNueva::AFogataNueva()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	FuegoFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FuegoFX"));
	FuegoFX->SetupAttachment(Mesh);
	
	RangoCrafteo = CreateDefaultSubobject<USphereComponent>(TEXT("RangoCrafteo"));
	RangoCrafteo->SetupAttachment(Mesh);
	RangoCrafteo->SetSphereRadius(250.f);
	RangoCrafteo->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RangoCrafteo->SetCollisionResponseToAllChannels(ECR_Ignore);
	RangoCrafteo->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RangoCrafteo->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AFogataNueva::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFogataNueva::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (!OtherActor) return;
	if (!OtherActor->Implements<UInterfazCrafteo>()) return;

	if (UComponenteCrafteo* Comp =
		OtherActor->FindComponentByClass<UComponenteCrafteo>())
	{
		Comp->SetFogataCercana(this);
	}
}

void AFogataNueva::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);
	if (!OtherActor) return;
	if (!OtherActor->Implements<UInterfazCrafteo>()) return;

	if (UComponenteCrafteo* Comp =
		OtherActor->FindComponentByClass<UComponenteCrafteo>())
	{
		Comp->SetFogataCercana(nullptr);
	}
}

// Called every frame
void AFogataNueva::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

