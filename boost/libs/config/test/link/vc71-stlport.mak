#
# auto generated makefile for VC6+STLPort
#
# usage:
# make
#   brings libraries up to date
# make install
#   brings libraries up to date and copies binaries to your VC6 /lib and /bin directories (recomended)
#

#
# Add additional compiler options here:
#
CXXFLAGS=
#
# Add additional include directories here:
#
INCLUDES=
#
# add additional linker flags here:
#
XLFLAGS=
#
# add additional static-library creation flags here:
#
XSFLAGS=

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF "$(MSVCDIR)" == ""
!ERROR Variable MSVCDIR not set.
!ENDIF

!IF "$(STLPORT_PATH)" == ""
!ERROR Variable STLPORT_PATH not set.
!ENDIF


ALL_HEADER=

all : main_dir liblink_test_vc71-stlport_ms_dir ./vc71-stlport/liblink_test_vc71-stlport_ms.lib ./vc71-stlport/liblink_test_vc71-stlport_ms.exe liblink_test_vc71-stlport_msd_dir ./vc71-stlport/liblink_test_vc71-stlport_msd.lib ./vc71-stlport/liblink_test_vc71-stlport_msd.exe link_test_vc71-stlport_mdd_dir ./vc71-stlport/link_test_vc71-stlport_mdd.lib ./vc71-stlport/link_test_vc71-stlport_mdd.exe link_test_vc71-stlport_md_dir ./vc71-stlport/link_test_vc71-stlport_md.lib ./vc71-stlport/link_test_vc71-stlport_md.exe liblink_test_vc71-stlport_md_dir ./vc71-stlport/liblink_test_vc71-stlport_md.lib ./vc71-stlport/liblink_test_vc71-stlport_md.exe liblink_test_vc71-stlport_mdd_dir ./vc71-stlport/liblink_test_vc71-stlport_mdd.lib ./vc71-stlport/liblink_test_vc71-stlport_mdd.exe link_test_vc71-stlport_mddd_dir ./vc71-stlport/link_test_vc71-stlport_mddd.lib ./vc71-stlport/link_test_vc71-stlport_mddd.exe liblink_test_vc71-stlport_msdd_dir ./vc71-stlport/liblink_test_vc71-stlport_msdd.lib ./vc71-stlport/liblink_test_vc71-stlport_msdd.exe liblink_test_vc71-stlport_mddd_dir ./vc71-stlport/liblink_test_vc71-stlport_mddd.lib ./vc71-stlport/liblink_test_vc71-stlport_mddd.exe

clean :  liblink_test_vc71-stlport_ms_clean liblink_test_vc71-stlport_msd_clean link_test_vc71-stlport_mdd_clean link_test_vc71-stlport_md_clean liblink_test_vc71-stlport_md_clean liblink_test_vc71-stlport_mdd_clean link_test_vc71-stlport_mddd_clean liblink_test_vc71-stlport_msdd_clean liblink_test_vc71-stlport_mddd_clean

install : stlport_check all
	copy vc71-stlport\liblink_test_vc71-stlport_ms.lib "$(MSVCDIR)\lib"
	copy vc71-stlport\liblink_test_vc71-stlport_msd.lib "$(MSVCDIR)\lib"
	copy vc71-stlport\link_test_vc71-stlport_mdd.lib "$(MSVCDIR)\lib"
	copy vc71-stlport\link_test_vc71-stlport_mdd.dll "$(MSVCDIR)\bin"
	copy vc71-stlport\link_test_vc71-stlport_md.lib "$(MSVCDIR)\lib"
	copy vc71-stlport\link_test_vc71-stlport_md.dll "$(MSVCDIR)\bin"
	copy vc71-stlport\liblink_test_vc71-stlport_md.lib "$(MSVCDIR)\lib"
	copy vc71-stlport\liblink_test_vc71-stlport_mdd.lib "$(MSVCDIR)\lib"
	copy vc71-stlport\link_test_vc71-stlport_mddd.lib "$(MSVCDIR)\lib"
	copy vc71-stlport\link_test_vc71-stlport_mddd.dll "$(MSVCDIR)\bin"
	copy vc71-stlport\link_test_vc71-stlport_mddd.pdb "$(MSVCDIR)\lib"
	copy vc71-stlport\liblink_test_vc71-stlport_msdd.lib "$(MSVCDIR)\lib"
	copy vc71-stlport\liblink_test_vc71-stlport_msdd.pdb "$(MSVCDIR)\lib"
	copy vc71-stlport\liblink_test_vc71-stlport_mddd.lib "$(MSVCDIR)\lib"
	copy vc71-stlport\liblink_test_vc71-stlport_mddd.pdb "$(MSVCDIR)\lib"

main_dir :
	@if not exist "vc71-stlport\$(NULL)" mkdir vc71-stlport

stlport_check : $(STLPORT_PATH)\stlport\string
	echo


########################################################
#
# section for liblink_test_vc71-stlport_ms.lib
#
########################################################
vc71-stlport/liblink_test_vc71-stlport_ms/link_test.obj: link_test.cpp $(ALL_HEADER)
	cl /c $(INCLUDES) /nologo /MT /W3 /GX /O2 /GB /GF /Gy /I$(STLPORT_PATH)\stlport /I..\..\..\..\ /D_MT /DWIN32 /DNDEBUG /D_MBCS /D_LIB  $(CXXFLAGS) -Y- -Fo./vc71-stlport/liblink_test_vc71-stlport_ms/ -Fdvc71-stlport/liblink_test_vc71-stlport_ms.pdb link_test.cpp

liblink_test_vc71-stlport_ms_dir :
	@if not exist "vc71-stlport\liblink_test_vc71-stlport_ms\$(NULL)" mkdir vc71-stlport\liblink_test_vc71-stlport_ms

liblink_test_vc71-stlport_ms_clean :
	del vc71-stlport\liblink_test_vc71-stlport_ms\*.obj
	del vc71-stlport\liblink_test_vc71-stlport_ms\*.idb
	del vc71-stlport\liblink_test_vc71-stlport_ms\*.exp
	del vc71-stlport\liblink_test_vc71-stlport_ms\*.pch

./vc71-stlport/liblink_test_vc71-stlport_ms.lib : vc71-stlport/liblink_test_vc71-stlport_ms/link_test.obj
	link -lib /nologo /out:vc71-stlport/liblink_test_vc71-stlport_ms.lib $(XSFLAGS)  vc71-stlport/liblink_test_vc71-stlport_ms/link_test.obj

./vc71-stlport/liblink_test_vc71-stlport_ms.exe : main.cpp ./vc71-stlport/liblink_test_vc71-stlport_ms.lib
	cl $(INCLUDES) /nologo /MT /W3 /GX /O2 /GB /GF /Gy /I$(STLPORT_PATH)\stlport /I..\..\..\..\ /D_MT /DWIN32 /DNDEBUG /D_MBCS /D_LIB  $(CXXFLAGS) -o ./vc71-stlport/liblink_test_vc71-stlport_ms.exe main.cpp /link /LIBPATH:./vc71-stlport
   vc71-stlport\liblink_test_vc71-stlport_ms.exe

########################################################
#
# section for liblink_test_vc71-stlport_msd.lib
#
########################################################
vc71-stlport/liblink_test_vc71-stlport_msd/link_test.obj: link_test.cpp $(ALL_HEADER)
	cl /c $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I$(STLPORT_PATH)\stlport /I..\..\..\..\ /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /GX /RTC1  $(CXXFLAGS) -Y- -Fo./vc71-stlport/liblink_test_vc71-stlport_msd/ -Fdvc71-stlport/liblink_test_vc71-stlport_msd.pdb link_test.cpp

liblink_test_vc71-stlport_msd_dir :
	@if not exist "vc71-stlport\liblink_test_vc71-stlport_msd\$(NULL)" mkdir vc71-stlport\liblink_test_vc71-stlport_msd

liblink_test_vc71-stlport_msd_clean :
	del vc71-stlport\liblink_test_vc71-stlport_msd\*.obj
	del vc71-stlport\liblink_test_vc71-stlport_msd\*.idb
	del vc71-stlport\liblink_test_vc71-stlport_msd\*.exp
	del vc71-stlport\liblink_test_vc71-stlport_msd\*.pch

./vc71-stlport/liblink_test_vc71-stlport_msd.lib : vc71-stlport/liblink_test_vc71-stlport_msd/link_test.obj
	link -lib /nologo /out:vc71-stlport/liblink_test_vc71-stlport_msd.lib $(XSFLAGS)  vc71-stlport/liblink_test_vc71-stlport_msd/link_test.obj

./vc71-stlport/liblink_test_vc71-stlport_msd.exe : main.cpp ./vc71-stlport/liblink_test_vc71-stlport_msd.lib
	cl $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I$(STLPORT_PATH)\stlport /I..\..\..\..\ /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /GX /RTC1  $(CXXFLAGS) -o ./vc71-stlport/liblink_test_vc71-stlport_msd.exe main.cpp /link /LIBPATH:./vc71-stlport
   vc71-stlport\liblink_test_vc71-stlport_msd.exe

########################################################
#
# section for link_test_vc71-stlport_mdd.lib
#
########################################################
vc71-stlport/link_test_vc71-stlport_mdd/link_test.obj: link_test.cpp $(ALL_HEADER)
	cl /c $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I$(STLPORT_PATH)\stlport /I..\..\..\..\ /DBOOST_DYN_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /GX /RTC1  $(CXXFLAGS) -Y- -Fo./vc71-stlport/link_test_vc71-stlport_mdd/ -Fdvc71-stlport/link_test_vc71-stlport_mdd.pdb link_test.cpp

link_test_vc71-stlport_mdd_dir :
	@if not exist "vc71-stlport\link_test_vc71-stlport_mdd\$(NULL)" mkdir vc71-stlport\link_test_vc71-stlport_mdd

link_test_vc71-stlport_mdd_clean :
	del vc71-stlport\link_test_vc71-stlport_mdd\*.obj
	del vc71-stlport\link_test_vc71-stlport_mdd\*.idb
	del vc71-stlport\link_test_vc71-stlport_mdd\*.exp
	del vc71-stlport\link_test_vc71-stlport_mdd\*.pch

./vc71-stlport/link_test_vc71-stlport_mdd.lib : vc71-stlport/link_test_vc71-stlport_mdd/link_test.obj
	link kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"vc71-stlport/link_test_vc71-stlport_mdd.pdb" /debug /machine:I386 /out:"vc71-stlport/link_test_vc71-stlport_mdd.dll" /implib:"vc71-stlport/link_test_vc71-stlport_mdd.lib" /LIBPATH:$(STLPORT_PATH)\lib $(XLFLAGS)  vc71-stlport/link_test_vc71-stlport_mdd/link_test.obj

./vc71-stlport/link_test_vc71-stlport_mdd.exe : main.cpp ./vc71-stlport/link_test_vc71-stlport_mdd.lib
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I$(STLPORT_PATH)\stlport /I..\..\..\..\ /DBOOST_DYN_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /GX /RTC1  $(CXXFLAGS) -o ./vc71-stlport/link_test_vc71-stlport_mdd.exe main.cpp /link /LIBPATH:./vc71-stlport
   vc71-stlport\link_test_vc71-stlport_mdd.exe

########################################################
#
# section for link_test_vc71-stlport_md.lib
#
########################################################
vc71-stlport/link_test_vc71-stlport_md/link_test.obj: link_test.cpp $(ALL_HEADER)
	cl /c $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /I$(STLPORT_PATH)\stlport /Gy /I..\..\..\..\ /DBOOST_DYN_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL  $(CXXFLAGS) -Y- -Fo./vc71-stlport/link_test_vc71-stlport_md/ -Fdvc71-stlport/link_test_vc71-stlport_md.pdb link_test.cpp

link_test_vc71-stlport_md_dir :
	@if not exist "vc71-stlport\link_test_vc71-stlport_md\$(NULL)" mkdir vc71-stlport\link_test_vc71-stlport_md

link_test_vc71-stlport_md_clean :
	del vc71-stlport\link_test_vc71-stlport_md\*.obj
	del vc71-stlport\link_test_vc71-stlport_md\*.idb
	del vc71-stlport\link_test_vc71-stlport_md\*.exp
	del vc71-stlport\link_test_vc71-stlport_md\*.pch

./vc71-stlport/link_test_vc71-stlport_md.lib : vc71-stlport/link_test_vc71-stlport_md/link_test.obj
	link kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"vc71-stlport/link_test_vc71-stlport_md.pdb" /debug /machine:I386 /out:"vc71-stlport/link_test_vc71-stlport_md.dll" /implib:"vc71-stlport/link_test_vc71-stlport_md.lib" /LIBPATH:$(STLPORT_PATH)\lib $(XLFLAGS)  vc71-stlport/link_test_vc71-stlport_md/link_test.obj

./vc71-stlport/link_test_vc71-stlport_md.exe : main.cpp ./vc71-stlport/link_test_vc71-stlport_md.lib
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /I$(STLPORT_PATH)\stlport /Gy /I..\..\..\..\ /DBOOST_DYN_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL  $(CXXFLAGS) -o ./vc71-stlport/link_test_vc71-stlport_md.exe main.cpp /link /LIBPATH:./vc71-stlport
   vc71-stlport\link_test_vc71-stlport_md.exe

########################################################
#
# section for liblink_test_vc71-stlport_md.lib
#
########################################################
vc71-stlport/liblink_test_vc71-stlport_md/link_test.obj: link_test.cpp $(ALL_HEADER)
	cl /c $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I$(STLPORT_PATH)\stlport /I..\..\..\..\ /DBOOST_REGEX_STATIC_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL  $(CXXFLAGS) -Y- -Fo./vc71-stlport/liblink_test_vc71-stlport_md/ -Fdvc71-stlport/liblink_test_vc71-stlport_md.pdb link_test.cpp

liblink_test_vc71-stlport_md_dir :
	@if not exist "vc71-stlport\liblink_test_vc71-stlport_md\$(NULL)" mkdir vc71-stlport\liblink_test_vc71-stlport_md

liblink_test_vc71-stlport_md_clean :
	del vc71-stlport\liblink_test_vc71-stlport_md\*.obj
	del vc71-stlport\liblink_test_vc71-stlport_md\*.idb
	del vc71-stlport\liblink_test_vc71-stlport_md\*.exp
	del vc71-stlport\liblink_test_vc71-stlport_md\*.pch

./vc71-stlport/liblink_test_vc71-stlport_md.lib : vc71-stlport/liblink_test_vc71-stlport_md/link_test.obj
	link -lib /nologo /out:vc71-stlport/liblink_test_vc71-stlport_md.lib $(XSFLAGS)  vc71-stlport/liblink_test_vc71-stlport_md/link_test.obj

./vc71-stlport/liblink_test_vc71-stlport_md.exe : main.cpp ./vc71-stlport/liblink_test_vc71-stlport_md.lib
	cl $(INCLUDES) /nologo /MD /W3 /GX /O2 /GB /GF /Gy /I$(STLPORT_PATH)\stlport /I..\..\..\..\ /DBOOST_REGEX_STATIC_LINK /DNDEBUG /DWIN32 /D_WINDOWS /D_MBCS /D_USRDLL  $(CXXFLAGS) -o ./vc71-stlport/liblink_test_vc71-stlport_md.exe main.cpp /link /LIBPATH:./vc71-stlport
   vc71-stlport\liblink_test_vc71-stlport_md.exe

########################################################
#
# section for liblink_test_vc71-stlport_mdd.lib
#
########################################################
vc71-stlport/liblink_test_vc71-stlport_mdd/link_test.obj: link_test.cpp $(ALL_HEADER)
	cl /c $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I$(STLPORT_PATH)\stlport /I..\..\..\..\ /DBOOST_REGEX_STATIC_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /GX /RTC1  $(CXXFLAGS) -Y- -Fo./vc71-stlport/liblink_test_vc71-stlport_mdd/ -Fdvc71-stlport/liblink_test_vc71-stlport_mdd.pdb link_test.cpp

liblink_test_vc71-stlport_mdd_dir :
	@if not exist "vc71-stlport\liblink_test_vc71-stlport_mdd\$(NULL)" mkdir vc71-stlport\liblink_test_vc71-stlport_mdd

liblink_test_vc71-stlport_mdd_clean :
	del vc71-stlport\liblink_test_vc71-stlport_mdd\*.obj
	del vc71-stlport\liblink_test_vc71-stlport_mdd\*.idb
	del vc71-stlport\liblink_test_vc71-stlport_mdd\*.exp
	del vc71-stlport\liblink_test_vc71-stlport_mdd\*.pch

./vc71-stlport/liblink_test_vc71-stlport_mdd.lib : vc71-stlport/liblink_test_vc71-stlport_mdd/link_test.obj
	link -lib /nologo /out:vc71-stlport/liblink_test_vc71-stlport_mdd.lib $(XSFLAGS)  vc71-stlport/liblink_test_vc71-stlport_mdd/link_test.obj

./vc71-stlport/liblink_test_vc71-stlport_mdd.exe : main.cpp ./vc71-stlport/liblink_test_vc71-stlport_mdd.lib
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I$(STLPORT_PATH)\stlport /I..\..\..\..\ /DBOOST_REGEX_STATIC_LINK /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /GX /RTC1  $(CXXFLAGS) -o ./vc71-stlport/liblink_test_vc71-stlport_mdd.exe main.cpp /link /LIBPATH:./vc71-stlport
   vc71-stlport\liblink_test_vc71-stlport_mdd.exe

########################################################
#
# section for link_test_vc71-stlport_mddd.lib
#
########################################################
vc71-stlport/link_test_vc71-stlport_mddd/link_test.obj: link_test.cpp $(ALL_HEADER)
	cl /c $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I$(STLPORT_PATH)\stlport /I..\..\..\..\ /DBOOST_DYN_LINK /D__STL_DEBUG /D_STLP_DEBUG /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /GX /RTC1  $(CXXFLAGS) -Y- -Fo./vc71-stlport/link_test_vc71-stlport_mddd/ -Fdvc71-stlport/link_test_vc71-stlport_mddd.pdb link_test.cpp

link_test_vc71-stlport_mddd_dir :
	@if not exist "vc71-stlport\link_test_vc71-stlport_mddd\$(NULL)" mkdir vc71-stlport\link_test_vc71-stlport_mddd

link_test_vc71-stlport_mddd_clean :
	del vc71-stlport\link_test_vc71-stlport_mddd\*.obj
	del vc71-stlport\link_test_vc71-stlport_mddd\*.idb
	del vc71-stlport\link_test_vc71-stlport_mddd\*.exp
	del vc71-stlport\link_test_vc71-stlport_mddd\*.pch

./vc71-stlport/link_test_vc71-stlport_mddd.lib : vc71-stlport/link_test_vc71-stlport_mddd/link_test.obj
	link kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /pdb:"vc71-stlport/link_test_vc71-stlport_mddd.pdb" /debug /machine:I386 /out:"vc71-stlport/link_test_vc71-stlport_mddd.dll" /implib:"vc71-stlport/link_test_vc71-stlport_mddd.lib" /LIBPATH:$(STLPORT_PATH)\lib $(XLFLAGS)  vc71-stlport/link_test_vc71-stlport_mddd/link_test.obj

./vc71-stlport/link_test_vc71-stlport_mddd.exe : main.cpp ./vc71-stlport/link_test_vc71-stlport_mddd.lib
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I$(STLPORT_PATH)\stlport /I..\..\..\..\ /DBOOST_DYN_LINK /D__STL_DEBUG /D_STLP_DEBUG /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /GX /RTC1  $(CXXFLAGS) -o ./vc71-stlport/link_test_vc71-stlport_mddd.exe main.cpp /link /LIBPATH:./vc71-stlport
   vc71-stlport\link_test_vc71-stlport_mddd.exe

########################################################
#
# section for liblink_test_vc71-stlport_msdd.lib
#
########################################################
vc71-stlport/liblink_test_vc71-stlport_msdd/link_test.obj: link_test.cpp $(ALL_HEADER)
	cl /c $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I$(STLPORT_PATH)\stlport /I..\..\..\..\ /D__STL_DEBUG /D_STLP_DEBUG /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /GX /RTC1  $(CXXFLAGS) -Y- -Fo./vc71-stlport/liblink_test_vc71-stlport_msdd/ -Fdvc71-stlport/liblink_test_vc71-stlport_msdd.pdb link_test.cpp

liblink_test_vc71-stlport_msdd_dir :
	@if not exist "vc71-stlport\liblink_test_vc71-stlport_msdd\$(NULL)" mkdir vc71-stlport\liblink_test_vc71-stlport_msdd

liblink_test_vc71-stlport_msdd_clean :
	del vc71-stlport\liblink_test_vc71-stlport_msdd\*.obj
	del vc71-stlport\liblink_test_vc71-stlport_msdd\*.idb
	del vc71-stlport\liblink_test_vc71-stlport_msdd\*.exp
	del vc71-stlport\liblink_test_vc71-stlport_msdd\*.pch

./vc71-stlport/liblink_test_vc71-stlport_msdd.lib : vc71-stlport/liblink_test_vc71-stlport_msdd/link_test.obj
	link -lib /nologo /out:vc71-stlport/liblink_test_vc71-stlport_msdd.lib $(XSFLAGS)  vc71-stlport/liblink_test_vc71-stlport_msdd/link_test.obj

./vc71-stlport/liblink_test_vc71-stlport_msdd.exe : main.cpp ./vc71-stlport/liblink_test_vc71-stlport_msdd.lib
	cl $(INCLUDES) /nologo /MTd /W3 /Gm /GX /Zi /Od /I$(STLPORT_PATH)\stlport /I..\..\..\..\ /D__STL_DEBUG /D_STLP_DEBUG /DWIN32 /D_MT /D_DEBUG /D_MBCS /D_LIB /GX /RTC1  $(CXXFLAGS) -o ./vc71-stlport/liblink_test_vc71-stlport_msdd.exe main.cpp /link /LIBPATH:./vc71-stlport
   vc71-stlport\liblink_test_vc71-stlport_msdd.exe

########################################################
#
# section for liblink_test_vc71-stlport_mddd.lib
#
########################################################
vc71-stlport/liblink_test_vc71-stlport_mddd/link_test.obj: link_test.cpp $(ALL_HEADER)
	cl /c $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I$(STLPORT_PATH)\stlport /I..\..\..\..\ /DBOOST_REGEX_STATIC_LINK /D__STL_DEBUG /D_STLP_DEBUG /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /GX /RTC1  $(CXXFLAGS) -Y- -Fo./vc71-stlport/liblink_test_vc71-stlport_mddd/ -Fdvc71-stlport/liblink_test_vc71-stlport_mddd.pdb link_test.cpp

liblink_test_vc71-stlport_mddd_dir :
	@if not exist "vc71-stlport\liblink_test_vc71-stlport_mddd\$(NULL)" mkdir vc71-stlport\liblink_test_vc71-stlport_mddd

liblink_test_vc71-stlport_mddd_clean :
	del vc71-stlport\liblink_test_vc71-stlport_mddd\*.obj
	del vc71-stlport\liblink_test_vc71-stlport_mddd\*.idb
	del vc71-stlport\liblink_test_vc71-stlport_mddd\*.exp
	del vc71-stlport\liblink_test_vc71-stlport_mddd\*.pch

./vc71-stlport/liblink_test_vc71-stlport_mddd.lib : vc71-stlport/liblink_test_vc71-stlport_mddd/link_test.obj
	link -lib /nologo /out:vc71-stlport/liblink_test_vc71-stlport_mddd.lib $(XSFLAGS)  vc71-stlport/liblink_test_vc71-stlport_mddd/link_test.obj

./vc71-stlport/liblink_test_vc71-stlport_mddd.exe : main.cpp ./vc71-stlport/liblink_test_vc71-stlport_mddd.lib
	cl $(INCLUDES) /nologo /MDd /W3 /Gm /GX /Zi /Od /I$(STLPORT_PATH)\stlport /I..\..\..\..\ /DBOOST_REGEX_STATIC_LINK /D__STL_DEBUG /D_STLP_DEBUG /D_DEBUG /DWIN32 /D_WINDOWS /D_MBCS /DUSRDLL /GX /RTC1  $(CXXFLAGS) -o ./vc71-stlport/liblink_test_vc71-stlport_mddd.exe main.cpp /link /LIBPATH:./vc71-stlport
   vc71-stlport\liblink_test_vc71-stlport_mddd.exe

