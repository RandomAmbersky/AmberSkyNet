cd bin
strip --strip-all AmberSkyNet.exe
upx -9 AmberSkyNet.exe
strip --strip-all plugins/*
upx -9 plugins/*.dll
