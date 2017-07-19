@echo off

SET CL_FLAGS=-W2 -Od -Zi
SET CL_FLAGS=%CL_FLAGS% -FAcs -DDEBUG=1 -wd4806
rem SET CL_FLAGS=%CL_FLAGS% -DCONSOLE_PRINT=1 


rem SET LINK_FLAGS=mgrs.lib
SET LINK_FLAGS=

IF NOT EXIST ..\..\build mkdir ..\..\build
pushd ..\..\build
rem cl %CL_FLAGS% ..\code\harness.cpp /link %LINK_FLAGS% 

cl %CL_FLAGS% ..\Harness\code\harness.cpp 
popd
