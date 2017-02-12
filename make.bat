SET devcpppath=c:\dev-cpp
SET eatname=trainer.exe
del .\eat\*.o
del .\eat\*.layout
del .\eat\*.res
del .\eat\exy_app_temp_private.rc
del .\eat\Makefile.win
cd eat
%devcpppath%\bin\windres res.rc -O coff -o res.res
cd ..
%devcpppath%\bin\g++ main.cpp application.cpp .\eat\exy_button.cpp .\eat\exy_combo.cpp .\eat\exy_edit.cpp .\eat\exy_listbox.cpp .\eat\exy_rbutton.cpp .\eat\exy_window.cpp .\eat\mem_functions.cpp .\eat\exy_find.cpp .\eat\exy_send.cpp .\eat\exy_strutils.cpp .\eat\res.res -o %eatname% -masm=intel -L"%devcpppath%\lib" -mwindows .\eat\ufmod.obj -lwinmm -lwsock32 ./eat/zlib/libz.a
%devcpppath%\bin\strip %eatname%
.\eat\upx.exe %eatname%
pause