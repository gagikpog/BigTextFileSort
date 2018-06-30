@echo off
set /a i = 5
:s
type input.txt >> temp.txt
type temp.txt >> input.txt

set /a i = i-1
echo %i%

if not %i%==0 (goto s)