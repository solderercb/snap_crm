set "metadata=--unite-metadata"
::set "metadata="
set repodir=regular-packages-update-mt-repo
rmdir /S /Q %repodir%
C:\Qt\Tools\QtInstallerFramework-mod\4.6_msvc2019_64\bin\repogen.exe %metadata% -p regular-packages-update-src-files %repodir%