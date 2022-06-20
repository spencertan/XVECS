call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
cl /W4 /WX /Za /nologo /EHa /std:c++latest /Femain.exe DescriptorTest.cpp
echo Compiled Successfully!
echo Test Running...
main.exe
echo Test Successfully!
PAUSE..