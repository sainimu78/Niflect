#!/bin/bash

set -e

ProjectName="Niflect"
Arch="$2"
ToolsetAndArch="$1_$Arch"
Platform="Windows"

DstRootDirPath="F:/sainimu78_Storage/Release/$Platform"
SrcBinDirPathDebug="$(pwd)/$ToolsetAndArch/Debug/bin"
SrcBinDirPathRelease="$(pwd)/$ToolsetAndArch/Release/bin"
DstProjectDirPath="$DstRootDirPath/$ProjectName"
DetBuildArchDirPath="$DstProjectDirPath/build/$Platform/$Arch"
DstBuildDirPathDebug="$DetBuildArchDirPath/Debug"
DstBuildDirPathRelease="$DetBuildArchDirPath/Release"
DstBinDirPathDebug="$DstBuildDirPathDebug/bin"
DstBinDirPathRelease="$DstBuildDirPathRelease/bin"
LicenseFilePath="$(pwd)/../../../LICENSE.md"
SrcIncludeDirPath="$(pwd)/../../../Source/$ProjectName/include"
DstIncludeDirPath="$DstProjectDirPath/include"
DstZipFilePath="$DstProjectDirPath.zip"

# Remove destination project directory if it exists
rm -rf "$DstProjectDirPath"

# Uncomment if you don't want to publish Debug version of NiflectGenTool
# cp -u "$SrcBinDirPathRelease/libclang.dll" "$DstBinDirPathDebug/"
# cp -u "$SrcBinDirPathRelease/NiflectGen.dll" "$DstBinDirPathDebug/"
# cp -u "$SrcBinDirPathRelease/NiflectGenCLI.exe" "$DstBinDirPathDebug/"
# cp -u "$SrcBinDirPathDebug/Niflect.*" "$DstBinDirPathDebug/"

# Copy Debug binaries
cp -r "$SrcBinDirPathDebug/"* "$DstBinDirPathDebug/"
rm -f "$DstBinDirPathDebug/"*.exp

# Copy Release binaries
cp -r "$SrcBinDirPathRelease/"* "$DstBinDirPathRelease/"
rm -f "$DstBinDirPathRelease/"*.exp

# Copy license file
cp "$LicenseFilePath" "$DstProjectDirPath/"

# Copy include files
cp -r "$SrcIncludeDirPath/"* "$DstIncludeDirPath/"

# Set the path for 7z
ToolDirPath="$(pwd)"
export PATH="$ToolDirPath:$PATH"

UserName="WishingContributor"
Password="1"
StorageDirPath="http://192.168.245.158/sainimu78_Storage/"
Name7zExe="7za.exe"
Storage7zDirPath="$StorageDirPath/Tool/$Platform/7z"
Dst7zExeFilePath="$ToolDirPath/$Name7zExe"
DstZippedDirPath="$DstRootDirPath/$ZipFileName"

# Download 7z executable
curl -u "$UserName:$Password" -L -o "$Dst7zExeFilePath" "$Storage7zDirPath/$Name7zExe"

# Delete existing zip file
rm -f "$DstZipFilePath"

# Create a zip file from the project directory
"$Dst7zExeFilePath" a "$DstZipFilePath" "$DstProjectDirPath"

# Clean up
rm -rf "$DstProjectDirPath"
rm -f "$Dst7zExeFilePath"