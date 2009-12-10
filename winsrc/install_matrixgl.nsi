outFile "matrixgl install.exe"
Name "MatrixGL Installer"
BrandingText " "

installDir $WINDIR

LicenseData "COPYING"

Page license
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles


section

   setOutPath $INSTDIR
   file matrixgl.scr
   file matrixgl_config.exe
   
   setOutPath $INSTDIR\system
   file glut32.dll


   writeUninstaller "$INSTDIR\matrixgluninstall.exe"

   WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MatrixGL" "Display Name" "MatrixGL Screensaver"
   WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MatrixGL" "UninstallString" "$\"$INSTDIR\matrixgluninstall.exe$\""
   WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MatrixGL" "DisplayVersion" "2.2.6"
   WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MatrixGL" "NoRepair" "1"
   WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MatrixGL" "NoModify" "1"              

sectionEnd

section "uninstall"
 
    delete "$INSTDIR\matrixgluninstall.exe"
   delete "$INSTDIR\matrixgl_config"
   delete "$INSTDIR\matrixgl.scr"
   delete "$INSTDIR\matrixgl_config.exe"
   delete "$INSTDIR\system\glut32.dll"
   DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\MatrixGL"
sectionEnd
