set "metadata=--unite-metadata"
::set "metadata="
set repodir=upload_files
rmdir /S /Q %repodir%
C:\Qt\Tools\QtInstallerFramework-mod\4.6\bin\repogen.exe %metadata% -p new_packages --update %repodir%
