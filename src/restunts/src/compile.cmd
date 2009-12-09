@echo off
for %%x in (*.asm) do echo %%x & c:\tasm\bin\tasm %%~sx /m2 || goto error
:error
