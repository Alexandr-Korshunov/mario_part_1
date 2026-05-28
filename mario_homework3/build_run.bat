@echo off
chcp 1251 > log
del log 

set MAIN="main.cpp GameObject.cpp Mario.cpp MovingObject.cpp Enemy.cpp Brick.cpp Level.cpp Map.cpp Coin.cpp"
set EXE=mario.exe
set CHARSET="-finput-charset=utf-8 -fexec-charset=windows-1251"

if exist %EXE% del %EXE%

g++ "%CHARSET%" "%MAIN%" -o %EXE%

%EXE%