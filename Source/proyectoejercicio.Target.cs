// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class proyectoejercicioTarget : TargetRules
{
    public proyectoejercicioTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;

        // 1. Actualizado a V6 para UE 5.7
        DefaultBuildSettings = BuildSettingsVersion.V6;

        // 2. Actualizado al orden de inclusión de la 5.7
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;

        ExtraModuleNames.Add("proyectoejercicio");
    }
}
