@echo off

cls
del backgrounds\*.img
del backgrounds\*.map
del backgrounds\*.pal
del h\*.h
grit.exe bmp\*.bmp -g -gT00FF00 -m -mRtf -mLs -p -ftb

echo grit.exe bmp\*.bmp -g -gT00FF00 -p -ftb
for /f "delims==" %%F in ('dir /b *.bin') do ren "%%~nxF" "%%~nF"
move *.img backgrounds
move *.map backgrounds
move *.pal backgrounds
move *.h h

echo cleanup...
del /q tiles\Chomp_LevelUp_BG\*.bin
del /q tiles\Chomp_NewForm_BG\*.bin
del /q tiles\TheEnd_Bot\*.bin
del /q tiles\Title_Logos\*.bin

echo extracting tiles...
for %%f in (backgrounds\*.img) do TilesRipper.exe %%f %%~nf

echo encoding...
rle -e tiles\Chomp_LevelUp_BG\*.bin
rle -e tiles\Chomp_NewForm_BG\*.bin
rle -e tiles\TheEnd_Bot\*.bin
rle -e tiles\Title_Logos\*.bin

echo joining tiles and creating custom formats...
for %%f in (backgrounds\*.img) do TilesJoiner.exe %%~nf 

echo all done!
pause