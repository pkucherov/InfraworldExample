// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#define TRACE

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Net.NetworkInformation;
using System.Text;
using System.Text.RegularExpressions;
using UnrealBuildTool;

public class InfraworldRuntime : ModuleRules
{
    private UnrealTargetPlatform Platform;
    private UnrealTargetConfiguration Configuration;

    // name of root folders in the project folder
    private static readonly string GRPC_STRIPPED_FOLDER = "GrpcIncludes";
    private static readonly string GRPC_LIBS_FOLDER = "GrpcLibraries";

    private string INCLUDE_ROOT;
    private string LIB_ROOT;

    public class ModuleDepPaths
    {
        public readonly string[] HeaderPaths;
        public readonly string[] LibraryPaths;

        public ModuleDepPaths(string[] headerPaths, string[] libraryPaths)
        {
            HeaderPaths = headerPaths;
            LibraryPaths = libraryPaths;
        }

        public override string ToString()
        {
            return "Headers:\n" + string.Join("\n", HeaderPaths) + "\nLibs:\n" + string.Join("\n", LibraryPaths);
        }
    }

    [Conditional("DEBUG")]
    [Conditional("TRACE")]
    private void clog(params object[] objects)
    {
        Console.WriteLine(string.Join(", ", objects));
    }

    private IEnumerable<string> FindFilesInDirectory(string dir, string suffix = "")
    {
        List<string> matches = new List<string>();
        if (Directory.Exists(dir))
        {
            string[] files = Directory.GetFiles(dir);
            Regex regex = new Regex(".+\\." + suffix);

            foreach (string file in files)
            {
                if (regex.Match(file).Success)
                    matches.Add(file);
            }
        }

        return matches;
    }

    private string GetConfigurationString()
    {
        return (Configuration == UnrealTargetConfiguration.Shipping) ? "Release" : "Debug";
    }

    public ModuleDepPaths GatherDeps()
    {
        string RootPath = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../"));

        INCLUDE_ROOT = Path.Combine(RootPath, GRPC_STRIPPED_FOLDER);
        LIB_ROOT = Path.Combine(RootPath, GRPC_LIBS_FOLDER);


        List<string> headers = new List<string>();
        List<string> libs = new List<string>();

        string PlatformLibRoot = "";


        if (Platform == UnrealTargetPlatform.Win64)
        {
            PlatformLibRoot = Path.Combine(LIB_ROOT, Platform.ToString());
            libs.AddRange(FindFilesInDirectory(PlatformLibRoot, "lib"));
        }
        else
        {
            PlatformLibRoot = Path.Combine(LIB_ROOT, Platform.ToString());
            libs.AddRange(FindFilesInDirectory(PlatformLibRoot, "a"));
        }

        clog("PlatformLibRoot: " + PlatformLibRoot);

        headers.Add(Path.Combine(INCLUDE_ROOT, "include"));
        headers.Add(Path.Combine(INCLUDE_ROOT, "third_party", "protobuf", "src"));

        return new ModuleDepPaths(headers.ToArray(), libs.ToArray());

    }

    public InfraworldRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
        Definitions.Add("GOOGLE_PROTOBUF_NO_RTTI");

        Platform = Target.Platform;
        Configuration = Target.Configuration;

        ModuleDepPaths moduleDepPaths = GatherDeps();
        Console.WriteLine(moduleDepPaths.ToString());

        PublicIncludePaths.Add("InfraworldRuntime/Public");
        PrivateIncludePaths.Add("InfraworldRuntime/Private");

        PublicIncludePaths.AddRange(moduleDepPaths.HeaderPaths);
        PublicAdditionalLibraries.AddRange(moduleDepPaths.LibraryPaths);

        PublicDependencyModuleNames.AddRange(new string[] { "Core" });

        PrivateDependencyModuleNames.AddRange(new string[] { "CoreUObject", "Engine" });

    }
}
