echo up DataCfg

@set BaseFrameDir=..\ArkGameFrame

xcopy %BaseFrameDir%\Dep\*.* Dep\ /e/y
xcopy %BaseFrameDir%\Doc\*.* Doc\  /e/y
xcopy %BaseFrameDir%\Frame\*.* Frame\ /e/y
xcopy %BaseFrameDir%\Bin\*.* Bin\ /e/y

echo complete up 
pause