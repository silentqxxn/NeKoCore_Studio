// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/PlayerControllerNuevo.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "componentes/ComponenteExperiencia.h"
#include "InputMappingContext.h"
#include "FItemData.h"
#include "Actores/Characterprincipal.h"
#include "componentes/ComponenteCrafteo.h"
#include "UObject/ConstructorHelpers.h"
#include "Componentes/ComponenteInventario.h"
#include "wiget/WidgetCrafteo.h"
#include "wiget/WidgetInventario.h"




void APlayerControllerNuevo::BeginPlay()

{
	Super::BeginPlay();
    if (IsLocalPlayerController())
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
        {
            if (IMC_CountlessBlood)
                Subsystem->AddMappingContext(IMC_CountlessBlood, 0);
        }
    }
    SuscribirEventosInventario();

    
    if (ClaseWidgetStats)
    {
        WidgetStats = CreateWidget<UUserWidget>(this, ClaseWidgetStats);
        WidgetStats->AddToViewport(0);
    }
   }

void APlayerControllerNuevo::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (UEnhancedInputComponent* EI =
       Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (IA_Inventario)
        {
            EI->BindAction(IA_Inventario, ETriggerEvent::Started,
                this, &APlayerControllerNuevo::ToggleInventario);
        }
    }
    
}

APlayerControllerNuevo::APlayerControllerNuevo()
{static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_Asset(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/imputs/IMC_CountlessBlood.IMC_CountlessBlood'")); 
    if (IMC_Asset.Succeeded())
    {
        IMC_CountlessBlood = IMC_Asset.Object;
    }
}

void APlayerControllerNuevo::MostrarInventario()
{
    UUserWidget* W = ObtenerOCrearWidget(ClaseWidgetInventario, WidgetInventario);
    if (!W) return;

    W->SetVisibility(ESlateVisibility::Visible);
    AbrirUI();

    if (APawn* OwnerPawn = GetPawn())
    {
        if (UComponenteInventario* CI =
            OwnerPawn->FindComponentByClass<UComponenteInventario>())
        {
            ActualizarInventario(CI->Items);
        }
    }
}

void APlayerControllerNuevo::OcultarInventario()
{
    if (WidgetInventario)
        WidgetInventario->SetVisibility(ESlateVisibility::Collapsed);

    CerrarUI();
}

void APlayerControllerNuevo::ActualizarInventario(const TArray<FItemData>& Items)
{
    //if (!WidgetInventario) return;
    //if (WidgetInventario->GetVisibility() == ESlateVisibility::Collapsed) return;
    
    if (UWidgetInventario* InvWidget = Cast<UWidgetInventario>(WidgetInventario))
    {
        if (WidgetInventario->GetVisibility() == ESlateVisibility::Visible)
            InvWidget->RefrescarLista(Items);
    }

    if (WidgetCrafteo && WidgetCrafteo->GetVisibility() == ESlateVisibility::Visible)
    {
        if (UWidgetCrafteo* CraftWidget = Cast<UWidgetCrafteo>(WidgetCrafteo))
            CraftWidget->Refrescar();
    }

}

// crafteo 
void APlayerControllerNuevo::MostrarCrafteo()
{
    UUserWidget* W = ObtenerOCrearWidget(ClaseWidgetCrafteo, WidgetCrafteo);
    if (!W) return;

    W->SetVisibility(ESlateVisibility::Visible);
    AbrirUI();

    if (APawn* OwnerPawn = GetPawn())
    {
        if (UComponenteCrafteo* CC = OwnerPawn->FindComponentByClass<UComponenteCrafteo>())
        {
            if (UWidgetCrafteo* CraftWidget = Cast<UWidgetCrafteo>(WidgetCrafteo))
                CraftWidget->Inicializar(CC);
        }
        if (ACharacterprincipal* MiPersonaje = Cast<ACharacterprincipal>(OwnerPawn))
            MiPersonaje->SetMovimientoBloqueado(true);
    }
}

void APlayerControllerNuevo::OcultarCrafteo()
{
    if (WidgetCrafteo)
        WidgetCrafteo->SetVisibility(ESlateVisibility::Collapsed);

    CerrarUI();
    
    if (APawn* OwnerPawn = GetPawn())
    {
        if (ACharacterprincipal* MiPersonaje = Cast<ACharacterprincipal>(OwnerPawn))
            MiPersonaje->SetMovimientoBloqueado(false);
    }
}
// Stats
void APlayerControllerNuevo::ActualizarStats()
{
    if (!WidgetStats) return;
}

void APlayerControllerNuevo::ActualizarBarraXP(int32 XPActual, int32 XPMaximo)
{
}

void APlayerControllerNuevo::MostrarSubidaNivel(int32 NivelAnterior, int32 NivelNuevo)
{
}


UUserWidget* APlayerControllerNuevo::ObtenerOCrearWidget(TSubclassOf<UUserWidget> Clase,UUserWidget*& Cache)
{
    if (!Clase) return nullptr;
    if (!Cache)
    {
        Cache = CreateWidget<UUserWidget>(this, Clase);
        Cache->AddToViewport(1); 
    }
    return Cache;
}

void APlayerControllerNuevo::AbrirUI()
{
    SetShowMouseCursor(true);
    SetInputMode(FInputModeGameAndUI());
}

void APlayerControllerNuevo::CerrarUI()
{
    SetShowMouseCursor(false);
    SetInputMode(FInputModeGameOnly());
}

void APlayerControllerNuevo::ToggleInventario()
{
    UUserWidget* W = ObtenerOCrearWidget(ClaseWidgetInventario, WidgetInventario);
    if (!W) return;

    const bool bAbierto = WidgetInventario->GetVisibility() == ESlateVisibility::Visible;

    bAbierto ? OcultarInventario() : MostrarInventario();
}

void APlayerControllerNuevo::ToggleCrafteo()
{
    UUserWidget* W = ObtenerOCrearWidget(ClaseWidgetCrafteo, WidgetCrafteo);
    if (!W) return;

    const bool bAbierto =
        WidgetCrafteo->GetVisibility() == ESlateVisibility::Visible;

    if (bAbierto)
    {
        OcultarCrafteo();
    }
    else
    {
        MostrarCrafteo();   
    }
}

void APlayerControllerNuevo::SuscribirEventosInventario()
{
    APawn* OwnerPawn = GetPawn();

    if (!OwnerPawn)
    {
        GetWorld()->GetTimerManager().SetTimerForNextTick(this, &APlayerControllerNuevo::SuscribirEventosInventario);
        return;
    }

    if (UComponenteInventario* CI =OwnerPawn->FindComponentByClass<UComponenteInventario>())
    {
        CI->OnInventarioActualizado.AddDynamic(this, &APlayerControllerNuevo::ActualizarInventario);
    }
/*
    if (UComponenteExperiencia* CE =OwnerPawn->FindComponentByClass<UComponenteExperiencia>())
    {
        CE->OnXPCambia.AddDynamic(this, &APlayerControllerNuevo::ActualizarBarraXP);
        CE->OnSubeNivel.AddDynamic(this, &APlayerControllerNuevo::MostrarSubidaNivel);
    }
    */
}
