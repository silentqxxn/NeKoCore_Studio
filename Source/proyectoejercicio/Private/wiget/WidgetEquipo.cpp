// Fill out your copyright notice in the Description page of Project Settings.


#include "wiget/WidgetEquipo.h"
#include "Actores/Characterprincipal.h"
#include "Actores/WeaponMaster.h"
#include "componentes/ComponenteArmas.h"
#include "FItemData.h"
#include "wiget/ItemSlotWidget.h"
#include "Actores/ItemMasterAttach.h"

void UWidgetEquipo::Inicializar(ACharacterprincipal* InCharacter)
{
	Character = InCharacter;

	if (Character)
		Character->OnEquipoActualizado.AddDynamic(this, &UWidgetEquipo::Refrescar);

	Refrescar();
}

void UWidgetEquipo::Refrescar()
{
	if (!Character) return;

	UComponenteArmas* CompArmas = Character->FindComponentByClass<UComponenteArmas>();
    
	if (!CompArmas) return; 

	if (CompArmas->ArmaEspalda)
		SlotArma->SetItemData(CompArmas->ArmaEspalda->InfoUI);
	else
		SlotArma->SetItemData(FItemData{});   

	if (CompArmas->ArmaManoDerecha)
		SlotManoDerecha->SetItemData(CompArmas->ArmaManoDerecha->InfoUI);
	else
		SlotManoDerecha->SetItemData(FItemData{});

	if (CompArmas->ArmaManoIzquierda)
		SlotManoIzquierda->SetItemData(CompArmas->ArmaManoIzquierda->InfoUI);
	else
		SlotManoIzquierda->SetItemData(FItemData{});
}