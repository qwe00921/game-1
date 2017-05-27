#!/bin/sh
mcs -codepage:utf8  ./FormMain.cs ./FormMain.Designer.cs ./Program.cs ./Properties/AssemblyInfo.cs ./Properties/Resources.Designer.cs ./Properties/Settings.Designer.cs ./Repack/AndroidApp.cs ./Repack/BaseApp.cs ./Repack/CommandProcess.cs ./Repack/RepackConfig.cs ./Repack/SecretFile.cs ./Repack/SecretKey.cs ./Repack/WinPhoneApp.cs ./UpdateHistory.cs ./UpdateHistory.Designer.cs -r:System.Data -r:System.Drawing -r:System.Windows.Forms -r:./dll/ICSharpCode.SharpZipLib.dll -r:./dll/Newtonsoft.Json.dll