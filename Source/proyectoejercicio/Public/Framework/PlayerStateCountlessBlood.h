#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerStateCountlessBlood.generated.h"

/**
 * 
 */
class UComponenteMisiones;

UCLASS()
class PROYECTOEJERCICIO_API APlayerStateCountlessBlood : public APlayerState
{
	GENERATED_BODY()
	
public:
	APlayerStateCountlessBlood();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(ReplicatedUsing=OnRep_Sangre, BlueprintReadOnly, Category = "Experiencia")
	int32 SangreTotal;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	UComponenteMisiones* ComponenteMisiones;

protected:
	UFUNCTION()
	void OnRep_Sangre();
};



