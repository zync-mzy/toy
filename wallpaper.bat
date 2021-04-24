@echo off

::进入到保存Windows10开机壁纸的文件路径下
cd %USERPROFILE%\AppData\Local\Packages\Microsoft.Windows.ContentDeliveryManager_cw5n1h2txyewy\LocalState\Assets

::创建BootWallpaper文件夹
md %USERPROFILE%\Pictures\BootWallpaper

::将壁纸文件复制到BootWallpaper文件夹
for /f %%i in ('dir /b *') do (
    if %%~zi GTR 300000 copy %%i %USERPROFILE%\Pictures\BootWallpaper
)
::copy *.* %USERPROFILE%\Pictures\BootWallpaper

::进入到BootWallpaper文件路径下
cd %USERPROFILE%\Pictures\BootWallpaper


::设置本地为延迟扩展
setlocal enabledelayedexpansion
for /f %%i in ('dir /b *') do (
    ::将BootWallpaper文件夹下的所有文件重命名为.png形式
    ren "%%i" "%%i.png"
)

pause
