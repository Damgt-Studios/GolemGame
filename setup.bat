@echo "Copying necessary data"

xcopy /E %cd%\files %cd%\x64\Debug\Game\AppX\files
xcopy /E %cd%\files %cd%\x64\Release\Game\AppX\files


@pause