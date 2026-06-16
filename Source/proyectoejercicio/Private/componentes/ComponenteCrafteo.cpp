// Fill out your copyright notice in the Description page of Project Settings.


#include "componentes/ComponenteCrafteo.h"
#include "componentes/ComponenteInventario.h"
#include "wiget/CraftSlotWidget.h"
#include "FRecetaData.h"
#include "FItemData.h"
#include "ItemLibrary.h"
#include "Actores/FogataNueva.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UComponenteCrafteo::UComponenteCrafteo()
{
	
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(false);

}
// Called when the game starts
void UComponenteCrafteo::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
UComponenteInventario* UComponenteCrafteo::GetInventario() const
{
    return GetOwner()
        ? GetOwner()->FindComponentByClass<UComponenteInventario>(): nullptr;
}

void UComponenteCrafteo::SetFogataCercana(AFogataNueva* Fogata)
{
    FogataActual = Fogata;

}

bool UComponenteCrafteo::BuscarReceta(FName RecetaID, FRecetaData& OutReceta) const
{
    if (!TablaRecetas) return false;

    TArray<FRecetaData*> Filas;
    TablaRecetas->GetAllRows<FRecetaData>(TEXT("BuscarReceta"), Filas);

    for (FRecetaData* Fila : Filas)
    {
        if (Fila && Fila->RecetaID == RecetaID)
        {
            OutReceta = *Fila;
            return true;
        }
    }
    return false;
}


bool UComponenteCrafteo::PuedeCraftear(FName RecetaID) const
{
    FRecetaData Receta;
    if (!BuscarReceta(RecetaID, Receta)) return false;

    UComponenteInventario* Inv = GetInventario();
    if (!Inv) return false;

    for (const FIngredienteReceta& Ingrediente : Receta.Ingredientes)
    {
        FItemData ItemEnInventario;
        if (!Inv->QueryInventory(Ingrediente.ItemID, ItemEnInventario))
            return false;   // no tiene el ítem

        if (ItemEnInventario.Cantidad < Ingrediente.Cantidad)
            return false;   // no tiene suficiente cantidad
    }

    return true;
}

TArray<FName> UComponenteCrafteo::ObtenerRecetasDisponibles() const
{
    TArray<FName> Disponibles;
    if (!TablaRecetas) return Disponibles;

    TArray<FRecetaData*> Filas;
    TablaRecetas->GetAllRows<FRecetaData>(TEXT("ObtenerRecetasDisponibles"), Filas);
    
    for (FRecetaData* Fila : Filas)
    {
        if (Fila && PuedeCraftear(Fila->RecetaID))
            Disponibles.Add(Fila->RecetaID);
    }
    return Disponibles;
}

TArray<FRecetaData> UComponenteCrafteo::ObtenerTodasLasRecetas() const
{
    TArray<FRecetaData> Resultado;
    if (!TablaRecetas) return Resultado;

    TArray<FRecetaData*> Filas;
    TablaRecetas->GetAllRows<FRecetaData>(TEXT("ObtenerTodasLasRecetas"), Filas);

    for (FRecetaData* Fila : Filas)
        Resultado.Add(*Fila);

    return Resultado;
}

void UComponenteCrafteo::Server_Craftear_Implementation(FName RecetaID)
{
    if (!FogataActual) return;
    if (!PuedeCraftear(RecetaID)) return;

    FRecetaData Receta;
    if (!BuscarReceta(RecetaID, Receta)) return;

    UComponenteInventario* Inv = GetInventario();
    if (!Inv) return;

    for (const FIngredienteReceta& Ingrediente : Receta.Ingredientes)
        Inv->Server_QuitarItem(Ingrediente.ItemID, Ingrediente.Cantidad);

    FItemData ItemResultado;
  if (UItemLibrary::GetItemByID(TablaItems, Receta.ItemResultadoID, ItemResultado))
    {
        ItemResultado.Cantidad = Receta.CantidadResultado;
        Inv->Server_AnadirItem(ItemResultado);
    }
}




// Called every frame
void UComponenteCrafteo::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

