outFile "matrixgl 2.3.1 Setup.exe"
Name "MatrixGL Installer"
BrandingText "matrixgl is free software"


installDir $WINDIR

LicenseData "COPYING"

Page license
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles


section

   setOutPath $INSTDIR
   file "Matrixgl231.scr"
   file matrixgl_config.exe
   
   setOutPath $INSTDIR\system
   file glut32.dll


   writeUninstaller "$INSTDIR\matrixgluninstall.exe"

   CreateDirectory "$SMPROGRAMS\Matrixgl 2.3.1"
   CreateShortCut "$SMPROGRAMS\Matrixgl 2.3.1\Uninstall matrixgl.lnk" "$INSTDIR\matrixgluninstall.exe"
   MessageBox MB_OK "Sucessfully installed! Right click on the desktop and select properties. Then, click on the screensavers tab and you will see Matrixgl in the list!"


sectionEnd

section "uninstall"
   delete "$INSTDIR\matrixgluninstall.exe"
   delete "$INSTDIR\matrixgl_config"
   delete "$INSTDIR\Matrixgl231.scr"
   delete "$INSTDIR\matrixgl_config.exe"
   delete "$INSTDIR\system\glut32.dll"
   delete "$SMPROGRAMS\Matrixgl 2.3.1\Uninstall matrixgl.lnk"
   RMDIR  "$SMPROGRAMS\Matrixgl 2.3.1"
sectionEnd
