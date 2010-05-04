::---------------------------------------------------------------------------------------------------------------------------------
:: Copyright (C) 2008 - 2009 Cristian Adam
::---------------------------------------------------------------------------------------------------------------------------------

set COMPILER=VS2005

:: Get revision number
svn info http://svn.xiph.org/trunk/oggdsf | findstr Revision > revision_text
set /p SVN_REVISION_FULL=<revision_text
set SVN_REVISION=%SVN_REVISION_FULL:~-5%
del revision_text

set PRODUCT_VERSION=0.83.%SVN_REVISION%
set OGGCODECS_ROOT_DIR=..\..\..

@set FILTERS=dsfFLACEncoder dsfNativeFLACSource dsfFLACDecoder
@set FILTERS=%FILTERS% dsfTheoraEncoder dsfTheoraDecoder
@set FILTERS=%FILTERS% dsfSpeexEncoder dsfSpeexDecoder
@set FILTERS=%FILTERS% dsfVorbisEncoder dsfVorbisDecoder
@set FILTERS=%FILTERS% dsfCMMLDecoder dsfCMMLRawSource
@set FILTERS=%FILTERS% dsfOggMux dsfAnxMux dsfOggDemux2
@set FILTERS=%FILTERS% wmpinfo AxPlayer

:: First make the x86 installer
set X64=
call:copy_binaries
call:make_installer

:: Then the x64 installer
set X64=true
call:copy_binaries
call:make_installer

goto:eof

::---------------------------------------------------------------------------------------------------------------------------------

:make_installer

signtool sign /a /t http://time.certum.pl/ bin\AxPlayer.dll

"%ProgramFiles%\nsis\unicode\makensis.exe" oggcodecs_release.nsi 

if [%X64%] == [] (
	set SUFFIX=win32
) else (
	set SUFFIX=x64
)

signtool sign /a /t http://time.certum.pl/ oggcodecs_%PRODUCT_VERSION%-%SUFFIX%.exe 
"%ProgramFiles%\7-zip\7z.exe" a oggcodecs_%PRODUCT_VERSION%_pdbs-%SUFFIX%.7z pdb\*

goto:eof
::---------------------------------------------------------------------------------------------------------------------------------

:copy_binaries

if [%X64%] == [] (
	@set PLATFORM=win32
) else (
	@set PLATFORM=x64
)

rmdir /s /q bin
mkdir bin

for %%i in (%FILTERS%) do (
copy "%OGGCODECS_ROOT_DIR%\sln\oggdsf_%COMPILER%\%PLATFORM%\Release\%%i.dll" bin\
)

rmdir /s /q pdb
mkdir pdb

for %%i in (%FILTERS%) do (
copy "%OGGCODECS_ROOT_DIR%\sln\oggdsf_%COMPILER%\%PLATFORM%\Release\%%i.pdb" pdb\
)

goto:eof
