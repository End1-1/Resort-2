set path=%PATH%;C:\Qt\MVSC\5.12.2\msvc2017\bin;C:\Qt\MVSC\Tools\QtCreator\bin;C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.26.28801\bin\Hostx64\x64

cd C:\Projects\Resort2\Restaurant2
lupdate -no-obsolete Restaurant.pro -ts Restaurant.am.ts
cd C:\Projects\Resort2\Resort
lupdate -no-obsolete Resort.pro -ts Resort.ts

