RMDIR build /s /q_
MKDIR build
for %%f in (Debug Release) do (
	MKDIR build\%%f
	COPY C:\soft\gmsh-prefix\lib-%%f\*.dll build\%%f\ /Y
)
MKDIR build\RelWithDebInfo
COPY C:\soft\gmsh-prefix\lib-Release\*.dll build\RelWithDebInfo\ /Y
cmake -B build
