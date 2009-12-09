@echo off
setlocal ENABLEDELAYEDEXPANSION 
set filenames=
for %%x in (*.obj) do set filenames=!filenames! %%x
echo %filenames% > linkfile
c:\tasm\bin\tlink @linkfile
