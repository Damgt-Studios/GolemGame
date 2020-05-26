@echo "Copying necessary data"

xcopy /E %cd%\files %cd%\x64\Debug\Game\AppX\files
xcopy /E %cd%\files %cd%\x64\Release\Game\AppX\files
xcopy /Y /R %cd%\api\studio\lib\x64\fmodstudioL.dll %cd%\x64\Debug\Game\AppX
xcopy /Y /R %cd%\api\core\lib\x64\fmodL.dll %cd%\x64\Debug\Game\AppX
xcopy /Y /R %cd%\api\studio\lib\x64\fmodstudio.dll %cd%\x64\Release\Game\AppX
xcopy /Y /R %cd%\api\core\lib\x64\fmod.dll %cd%\x64\Release\Game\AppX

@pause