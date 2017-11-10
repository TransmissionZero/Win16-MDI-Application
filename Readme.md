# Win16 MDI Application

## Table of Contents

- [Introduction](#introduction)
- [Building the Application](#building-the-application)
- [Smart Callbacks](#smart-callbacks)
- [Common Dialogs](#common-dialogs)
- [Terms of Use](#terms-of-use)
- [Problems?](#problems)
- [Changelog](#changelog)

## Introduction

This application is an example Win16 MDI GUI application. The compiled application is fully functional, with the
exception that opening and saving files does not read or write the files, and the MDI child windows do not have any code
to display documents. However, by adding this functionality you can quickly create a functioning MDI application.

The application can be built using Visual C++ 1.5x. You can
[download Visual C++ 1.52](https://my.visualstudio.com/Downloads?pid=140) if you have a Visual Studio Subscription. If
you don't have a subscription, you can download the
[Windows Server 2003 DDK](http://download.microsoft.com/download/9/0/f/90f019ac-8243-48d3-91cf-81fc4093ecfd/1830_usa_ddk.iso)
which contains the 16 bit command line tools (most of which are from Visual C++ 1.52). The 16 bit binaries can be found
in "bin\bin16" of the installed DDK, include files in "inc\inc16", and lib files in "lib\lib16". A number of the
binaries come in two versions, one with and one without a "16" suffix, e.g. "cl.exe" and "cl16.exe". Despite the
different filenames, the file contents are identical.

With a few small changes to the Makefile, the application will build with Microsoft C / C++ 7.0 and the Windows 3.1 SDK.
It won't build  with the Windows 3.0 SDK because the headers don't define some of the required types, and it does not
have the "ver.h" header which is required for the version information resource.

## Building the Application

To build the application with the Microsoft Visual C++ 1.5x GUI, go to "project", "open", then open "Win16MDI.mak". To
choose a debug or release build, go to "options" then "project". The project can be built using the "build" or "rebuild"
toolbar items, or via the "project" menu.

To build the application from the command line with Visual C++ 1.5x, launch a command prompt, run the "MSVCVARS.BAT"
file which can be found in the Visual C++ "bin" directory, and then navigate to the directory containing the Makefile.
Run "nmake /f Win16MDI.mak" to build. This also works with the Windows Server 2003 DDK, but instead of running
"MSVCVARS.BAT", you will need to add the "bin16" directory to your "%PATH%" environment variable, "inc16" to
"%INCLUDE%", and "lib16" to "%LIB%".

## Smart Callbacks

This application makes use of so-called "smart callbacks". This means the data segment register is loaded from the
stack segment register on entry to any callback function which is called by Windows. The result of this is that it is
not necessary to list the callback functions in the application's module definition file, and it is not necessary to use
"MakeProcInstance" to create a thunk when (e.g.) displaying a dialog box. In Visual C++ this is achieved by marking
callback functions with the "__export" modifier, and using the "/GA /GEs /GEm" compiler options (optimise far function
epilogs, load DS from SS, and increment BP on entry to the callback--required only for real mode stack walking support).
You can also use the "/GEf" option, which means all far functions are given the code to load DS from SS regardless of
whether they are callback functions. This is slightly less efficient, but does mean that callback functions don't
require the "__export" modifier.

Refer to the [Windows 1 Application](https://github.com/TransmissionZero/Windows-1-Example-Application) for an example
application which uses "MakeProcInstance" rather than smart callbacks.

## Common Dialogs

This application uses the Common Dialogs library (COMMDLG.DLL), which provides the standard open and save file dialogs.
This library comes with Windows 3.1 but doesn't come with Windows 3.0. The DLL is redistributable though, so you should
include a copy of the DLL with the application. Note that the DLL runs in protected mode only, so the application
executable has been marked as requiring protected mode. Nothing else in the application requires protected mode.

## Terms of Use

Refer to "License.txt" for terms of use.

## Problems?

If you have any problems or questions, please ensure you have read this readme. If you are still having trouble, you can
[get in contact](http://www.transmissionzero.co.uk/contact/).

## Changelog
1. 2017-11-10: Version 1.0
  - Initial release.

Transmission Zero
2017-11-10
