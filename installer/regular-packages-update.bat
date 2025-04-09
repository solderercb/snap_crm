set "metadata=--unite-metadata"
::set "metadata="
set sourcedir=regular-packages-update-src-files
set repodir=regular-packages-update-mt-repo
rmdir /S /Q %repodir%
C:\Qt\Tools\QtInstallerFramework-mod\4.6_msvc2019_64\bin\repogen.exe %metadata% -p %sourcedir% %repodir%
sed -i "/<PackageUpdate>/i \ <RepositoryUpdate>\r\n  <Repository action=\"add\" url=\"https://github.com/solderercb/snap_crm/releases/download/qtmysqllib-5.15.10.1\" displayname=\"qtmysqllib-update\"/>\r\n </RepositoryUpdate>" %repodir%\Updates.xml
