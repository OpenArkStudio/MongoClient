
echo up DataCfg

@set BaseFrameDir=..\ArkGameFrame

xcopy Dep\*.* %BaseFrameDir%\Dep\ /e/y
xcopy Doc\*.* %BaseFrameDir%\Doc\ /e/y
xcopy Plugins\NFLuaScriptPlugin\*.* %BaseFrameDir%\Plugins\NFLuaScriptPlugin\ /e/y

echo complete up 
pause
