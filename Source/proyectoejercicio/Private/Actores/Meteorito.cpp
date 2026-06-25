// Fill out your copyright notice in the Description page of Project Settings.


#include "Actores/Meteorito.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "interfaz/interfazparahacerdanio.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
AMeteorito::AMeteorito()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true; 

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;
	SphereCollision->InitSphereRadius(20.0f);
	SphereCollision->SetCollisionProfileName(TEXT("BlockAll"));
    
	SphereCollision->OnComponentHit.AddDynamic(this, &AMeteorito::OnHit);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); 

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = SphereCollision;
	ProjectileMovement->InitialSpeed = 1500.f; 
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true; 
	ProjectileMovement->ProjectileGravityScale = 0.3f; 

	InitialLifeSpan = 5.0f;
	
}

// Called when the game starts or when spawned
void AMeteorito::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMeteorito::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMeteorito::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* Lanzador = GetOwner(); 

	if (OtherActor && OtherActor != this && OtherActor != Lanzador)
	{
		if (HasAuthority())
		{
			if (OtherActor->Implements<Uinterfazparahacerdanio>())
			{
				Iinterfazparahacerdanio::Execute_RealizarDanio(OtherActor, DamageAmount);
			}
		}

		if (FX_Impacto)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FX_Impacto, GetActorLocation(), GetActorRotation());
		}

		Destroy();
	}
}