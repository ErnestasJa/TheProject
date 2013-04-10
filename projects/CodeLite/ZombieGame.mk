##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=ZombieGame
ConfigurationName      :=Debug
WorkspacePath          := "/home/serengeor/.codelite"
ProjectPath            := "/home/serengeor/Coding/ZombieGameProjectDevelopment/projects/CodeLite"
IntermediateDirectory  :=../../../Build/Debug/
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=
Date                   :=03/11/13
CodeLitePath           :="/home/serengeor/.codelite"
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="ZombieGame.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)../../src $(IncludeSwitch)../../libs/glfw/include $(IncludeSwitch)../../libs/bullet/src $(IncludeSwitch)../../libs/glm $(IncludeSwitch)../../libs/glxw/include 
IncludePCH             :=  -include ../../src/precomp.h 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)dl $(LibrarySwitch)pthread $(LibrarySwitch)Xext $(LibrarySwitch)X11 $(LibrarySwitch)GL $(LibrarySwitch)Xrandr $(LibrarySwitch)glfw $(LibrarySwitch)physfs 
ArLibs                 :=  "dl" "pthread" "Xext" "X11" "GL" "Xrandr" "libglfw.a" "libphysfs.a" 
LibPath                := $(LibraryPathSwitch). $(LibraryPathSwitch)../../libs/glfw/lib/x11 $(LibraryPathSwitch)../../libs/physfs 

##
## Common variables
## AR, CXX, CC, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_precomp$(ObjectSuffix) $(IntermediateDirectory)/src_main$(ObjectSuffix) $(IntermediateDirectory)/src_glxw$(ObjectSuffix) $(IntermediateDirectory)/src_glxw_glx$(ObjectSuffix) 

Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@test -d ../../../Build/Debug/ || $(MakeDirCommand) ../../../Build/Debug/

PreBuild:

# PreCompiled Header
../../src/precomp.h.gch: ../../src/precomp.h
	$(CXX) $(SourceSwitch) ../../src/precomp.h $(CXXFLAGS) $(IncludePath)



##
## Objects
##
$(IntermediateDirectory)/src_precomp$(ObjectSuffix): ../../src/precomp.cpp $(IntermediateDirectory)/src_precomp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/serengeor/Coding/ZombieGameProjectDevelopment/src/precomp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_precomp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_precomp$(DependSuffix): ../../src/precomp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_precomp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_precomp$(DependSuffix) -MM "../../src/precomp.cpp"

$(IntermediateDirectory)/src_precomp$(PreprocessSuffix): ../../src/precomp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_precomp$(PreprocessSuffix) "../../src/precomp.cpp"

$(IntermediateDirectory)/src_main$(ObjectSuffix): ../../src/main.cpp $(IntermediateDirectory)/src_main$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/serengeor/Coding/ZombieGameProjectDevelopment/src/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_main$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_main$(DependSuffix): ../../src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_main$(ObjectSuffix) -MF$(IntermediateDirectory)/src_main$(DependSuffix) -MM "../../src/main.cpp"

$(IntermediateDirectory)/src_main$(PreprocessSuffix): ../../src/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_main$(PreprocessSuffix) "../../src/main.cpp"

$(IntermediateDirectory)/src_glxw$(ObjectSuffix): ../../libs/glxw/src/glxw.c $(IntermediateDirectory)/src_glxw$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/serengeor/Coding/ZombieGameProjectDevelopment/libs/glxw/src/glxw.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_glxw$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_glxw$(DependSuffix): ../../libs/glxw/src/glxw.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_glxw$(ObjectSuffix) -MF$(IntermediateDirectory)/src_glxw$(DependSuffix) -MM "../../libs/glxw/src/glxw.c"

$(IntermediateDirectory)/src_glxw$(PreprocessSuffix): ../../libs/glxw/src/glxw.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_glxw$(PreprocessSuffix) "../../libs/glxw/src/glxw.c"

$(IntermediateDirectory)/src_glxw_glx$(ObjectSuffix): ../../libs/glxw/src/glxw_glx.c $(IntermediateDirectory)/src_glxw_glx$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/serengeor/Coding/ZombieGameProjectDevelopment/libs/glxw/src/glxw_glx.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_glxw_glx$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_glxw_glx$(DependSuffix): ../../libs/glxw/src/glxw_glx.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_glxw_glx$(ObjectSuffix) -MF$(IntermediateDirectory)/src_glxw_glx$(DependSuffix) -MM "../../libs/glxw/src/glxw_glx.c"

$(IntermediateDirectory)/src_glxw_glx$(PreprocessSuffix): ../../libs/glxw/src/glxw_glx.c
	@$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_glxw_glx$(PreprocessSuffix) "../../libs/glxw/src/glxw_glx.c"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) $(IntermediateDirectory)/src_precomp$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_precomp$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_precomp$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_main$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_main$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_main$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_glxw$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_glxw$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_glxw$(PreprocessSuffix)
	$(RM) $(IntermediateDirectory)/src_glxw_glx$(ObjectSuffix)
	$(RM) $(IntermediateDirectory)/src_glxw_glx$(DependSuffix)
	$(RM) $(IntermediateDirectory)/src_glxw_glx$(PreprocessSuffix)
	$(RM) $(OutputFile)
	$(RM) "../../../../.codelite/.build-debug/ZombieGame"
	$(RM) ../../src/precomp.h.gch


