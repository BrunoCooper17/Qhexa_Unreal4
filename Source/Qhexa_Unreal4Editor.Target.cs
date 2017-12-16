// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Qhexa_Unreal4EditorTarget : TargetRules
{
	public Qhexa_Unreal4EditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;

        ExtraModuleNames.AddRange(new string[] { "Qhexa_Unreal4" });
    }
}