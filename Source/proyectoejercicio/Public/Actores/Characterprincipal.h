// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "interfaz/interfazparahacerdanio.h"
#include "InputActionValue.h"
#include "Components/SkeletalMeshComponent.h"
#include "interfaz/InterfazAttach.h"
#include "interfaz/InterfazCrafteo.h"
#include "Camera/CameraComponent.h"
#include "interfaz/InterfazRecogeItems.h"
#include "Components/DecalComponent.h"
#include "interfaz/InterfazConsumible.h"
#include "componentes/ComponenteEstadisticas.h"
#include "componentes/ComponenteArmas.h"
#include "Characterprincipal.generated.h"


class UInputAction;
class UInputMappingContext;
class AWeaponMaster;
class UComponenteInventario; 
class UComponenteEstadisticas;
struct FItemData;
class UComponenteCrafteo;
class USkeletalMeshComponent;
class UComponenteExperiencia;
struct FExperienciaData;
class UComponenteArmas;

UCLASS(Abstract)
class PROYECTOEJERCICIO_API ACharacterprincipal : public ACharacter, public IInterfazRecogeItems , public Iinterfazparahacerdanio, public IInterfazAttach,public IInterfazCrafteo, public IInterfazConsumible
{
	GENERATED_BODY()
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipoActualizado);


public:
	// Sets default values for this character's properties
	ACharacterprincipal();
	

	
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual USkeletalMeshComponent* GetSkeletalMesh_Implementation() override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Componentes")
	UComponenteInventario* CompInventario;
	
	
	virtual bool RecogerItem_Implementation(const FItemData& Item) override;
	virtual bool RecogerTodos_Implementation(const TArray<FItemData>& Items) override;
	virtual bool PuedeRecoger_Implementation(const FItemData& Item) const override;

	virtual bool CraftearItem_Implementation(FName RecetaID) override;
	virtual bool PuedeCraftear_Implementation(FName RecetaID) const override;
	virtual TArray<FName> ObtenerRecetasDisponibles_Implementation() const override;

	
	// Sistema de armas
		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	class UComponenteArmas* CompArmas;
	
	UFUNCTION(BlueprintCallable)
	void MostrarMensaje();
	
	UPROPERTY(BlueprintAssignable)
	FOnEquipoActualizado OnEquipoActualizado;
	
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void Server_EquipItemAttach(AItemMasterAttach* Item);

	UFUNCTION(BlueprintCallable, Category="Movimiento")
	void SetMovimientoBloqueado(bool bBloqueado);
	
	UPROPERTY(ReplicatedUsing=OnRep_CurrentItemAttach, BlueprintReadOnly)
	AItemMasterAttach* CurrentItemAttach = nullptr;

	UFUNCTION()
	void OnRep_CurrentItemAttach();
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Equipamiento")
	AItemMasterAttach* ItemManoDerecha = nullptr;

	UFUNCTION(Server, Reliable)
	void Server_EquipItemGeneric(AItemMasterAttach* NuevoItem);

	UPROPERTY(ReplicatedUsing=OnRep_ItemManoIzquierda, BlueprintReadOnly, Category="Equipamiento")
	AItemMasterAttach* ItemManoIzquierda = nullptr;

	UFUNCTION()
	void OnRep_ItemManoIzquierda();
	
	//craft
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Componentes")
	UComponenteCrafteo* CompCrafteo;

	UFUNCTION(BlueprintCallable)
	void SetItemInteractuable(AActor* Item);
	
	UFUNCTION(BlueprintCallable)
	void ClearItemInteractuable(AActor* Item);

	UFUNCTION(BlueprintImplementableEvent, Category = "Combate")
	void EjecutarDisparoRanged(FRotator Direccion);
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_CurrentWeapon();

	// ---- Enhanced Input ----
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* IMC_Player;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Movimiento;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Interactuar;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Correr;
	
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Saltar;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Dash;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* IA_Curar;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movimiento")
	float MultiplicadorCorrer = 1.5f;
	
	void EmpezarACorrer();
	void DejarDeCorrer();
	
	UFUNCTION(Server, Reliable)
	void Server_SetVelocidadMax(float NuevaVelocidad);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Componentes")
	UComponenteExperiencia* CompExperiencia;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input") 
	UInputAction* ActionAtaqueRanged;
	
	void Move(const FInputActionValue& Value);
	void TryInteract();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Componentes")
	class UComponenteEstadisticas* CompEstadisticas;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Puntería")
	UDecalComponent* IndicadorRanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puntería")
	UMaterialInterface* MaterialFlechaPunteria;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puntería")
	float RangoFlecha = 800.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FX al Correr")
	class UNiagaraComponent* EfectoCorrerNiagara;

	UFUNCTION(Server, Reliable)
	void Server_SetEfectoCorrer(bool bActivo);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetEfectoCorrer(bool bActivo);

	void IniciarPunteriaRanged();
	void FinalizarYAtaqueRanged();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX al Correr")
	float FOVNormal = 90.0f; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX al Correr")
	float FOVCorrer = 110.0f; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX al Correr")
	float VelocidadTransicionFOV = 10.0f; 
	float TargetFOV;

	FTimerHandle TimerTransicionCamara;
	
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX al saltar")
	float DistanciaNormal = 400.0f; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX al saltar")
	float DistanciaSalto = 550.0f;  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX al saltar")
	float VelocidadTransicionDistancia = 5.0f; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX al saltar")
	float FOVSalto = 100.0f;
	
	float TargetDistancia;
	void ActualizarCamara();

	int32 SaltosRealizados = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Salto")
	int32 MaximoDeSaltos = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Salto")
	float TiempoCoyote = 0.2f;

	bool bCoyoteTimeActivo = false;

	FTimerHandle TimerCoyote;

	void IntentarSaltar();
	void IntentarDetenerSalto();

	void FinalizarCoyoteTime();
//Dash
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float FuerzaDash = 2500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float CooldownDash = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float DuracionDash = 0.2f; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash")
	float FOVDash = 120.0f;
	FTimerHandle TimerDuracionDash;
	
	float FriccionOriginal; 

	void FinalizarDash();
	
	bool bPuedeDashear = true;
	
	FTimerHandle TimerCooldownDash;
	
	void EjecutarDash();
	void ResetearDash();
	
	UFUNCTION(Server, Reliable)
	void Server_EjecutarDash(FVector FuerzaEmpuje);
//Curarse
	void PresionTeclaCuracion();
	
	UFUNCTION(Server, Reliable)
	void Server_ProcesarConsumoInterface(FName ItemID, float CantidadEfecto);
	
private:
	void DropCurrentWeapon();
	bool bMovimientoBloqueado = false;
	
	void DesequiparItemAttach(AItemMasterAttach* Item);


	UPROPERTY()
	AActor* ItemInteractuableCercano = nullptr;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual bool ConsumirItem_Implementation(FName ItemID, float CantidadEfecto) override;
	
	

	// Called to bind functionality to input
	
};
