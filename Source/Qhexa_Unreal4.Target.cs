// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Qhexa_Unreal4Target : TargetRules
{
	public Qhexa_Unreal4Target(TargetInfo Target) : base(Target)
    {
		Type = TargetType.Game;

        ExtraModuleNames.AddRange(new string[] { "Qhexa_Unreal4" });
    }
}