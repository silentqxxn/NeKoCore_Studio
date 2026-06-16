// Fill out your copyright notice in the Description page of Project Settings.


#include "Framework/GameModeCountlessBlood.h"

#include "Actores/Characterprincipal.h"
#include "Framework/GameStateCountlessBlood.h"
#include "Framework/PlayerControllerNuevo.h"
#include "Framework/PlayerStateCountlessBlood.h"


AGameModeCountlessBlood::AGameModeCountlessBlood()
{
	DefaultPawnClass = ACharacterprincipal::StaticClass();
	PlayerControllerClass = APlayerControllerNuevo::StaticClass();

	GameStateClass = AGameStateCountlessBlood::StaticClass();
	PlayerStateClass = APlayerStateCountlessBlood ::StaticClass();
}
