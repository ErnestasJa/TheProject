import fnmatch
import os, shutil, subprocess
import subprocess
from tools import easygui

#INIT VARS
buildDir = "build"
platform = ""

LINUX = "linux"
WINDOWS = "windows"

if os.name == "posix":
    platform = LINUX
else:
    platform = WINDOWS

projectPath = os.getcwd()
libpath = os.path.join("libs", platform);
fullLibPath = os.path.join(projectPath, libpath)

buildcores = ""

choices = ["4", "8"]
choice = easygui.choicebox("Choose number of cores available", "Core choice", choices)

if choice != None :
    if choice == "4":
        buildcores += "-j4"
    elif choice == "8":
        buildcores += "-j8"

if os.path.isdir("build") == False:
    os.mkdir("build")

#COMPILE BOOST
def get_libs_from_dir( dir ):
    match = []
    
    for root, dirnames, filenames in os.walk(dir):
        for filename in fnmatch.filter(filenames, '*.a'):
            if filename != "objects.a":
                match.append([filename,os.path.join(root, filename)])
                
    return match

def get_shared_libs_from_dir( dir ):
    match = []
    
    for root, dirnames, filenames in os.walk(dir):
        for filename in fnmatch.filter(filenames, '*.dll'):
            match.append([filename,os.path.join(root, filename)])
                
    return match

def compile_boost_windows():
    #warn if no boost
    boostpath = os.path.join(os.getcwd(), "libs/boost")

    if (os.path.isdir(boostpath) == False) or (len(os.listdir(boostpath)) == 0):
        easygui.msgbox("You do not have boost sources inside libs folder.\nClose this message box to continue.", title="Boost missing!")
    else:
        restore_path = os.getcwd()
        os.chdir(boostpath)

        if not os.path.isfile("b2.exe"):
            if easygui.boolbox(msg='Would you like to compile boost build system?', title='Compile boost', choices=('Yes', 'No'), image=None):
                print("Calling boost: bootstrap gcc")
                subprocess.call('bootstrap gcc', shell=True)

        if easygui.boolbox(msg='Would you like to compile boost tool: bcp?', title='Compile boost', choices=('Yes', 'No'), image=None):
                subprocess.call('b2 tools/bcp', shell=True)

        if easygui.boolbox(msg='Would you like to compile boost.python?', title='Compile boost', choices=('Yes', 'No'), image=None):
            buildstr = 'b2 ' + buildcores + ' --with-python --build-dir="../../build"  toolset=gcc cxxflags="-fPIC" link=shared threading=multi debug'
            print("Calling boost: " + buildstr)
            subprocess.call(buildstr, shell=True)

        if easygui.boolbox(msg='Would you like to compile boost?', title='Compile boost', choices=('Yes', 'No'), image=None):
            buildstr = 'b2 ' + buildcores + ' --without-python --build-dir="../../build"  toolset=gcc cxxflags="-fPIC" link=static threading=multi release'
            print("Calling boost: " + buildstr)
            subprocess.call(buildstr, shell=True)
            
        os.chdir(restore_path)

def compile_boost_linux():
    #warn if no boost
    boostpath = os.path.join(os.getcwd(), "libs/boost")

    if (os.path.isdir(boostpath) == False) or (len(os.listdir(boostpath)) == 0):
        easygui.msgbox("You do not have boost sources inside libs folder.\nClose this message box to continue.", title="Boost missing!")
    else:
        restore_path = os.getcwd()
        os.chdir(boostpath)

        if not os.path.isfile( "b2" ):
            if easygui.boolbox(msg='Would you like to compile boost build system?', title='Compile boost', choices=('Yes', 'No'), image=None):
                print("Calling boost: bootstrap gcc")
                subprocess.call('sh bootstrap.sh gcc', shell=True)

        if easygui.boolbox(msg='Would you like to compile boost tool: bcp?', title='Compile boost', choices=('Yes', 'No'), image=None):
            subprocess.call('./b2 tools/bcp', shell=True)

        if easygui.boolbox(msg='Would you like to compile boost.python?', title='Compile boost', choices=('Yes', 'No'), image=None):
            buildstr = './b2 ' + buildcores + ' --with-python --build-dir="../../build"  toolset=gcc cxxflags="-fPIC" link=shared threading=multi debug'
            print("Calling boost: " + buildstr)
            subprocess.call(buildstr, shell=True)

        if easygui.boolbox(msg='Would you like to compile boost?', title='Compile boost', choices=('Yes', 'No'), image=None):
            buildstr = './b2 ' + buildcores + ' --without-python --build-dir="../../build"  toolset=gcc link=static threading=multi release'
            print("Calling boost: " + buildstr)
            subprocess.call(buildstr, shell=True)
            
        os.chdir(restore_path)

if( platform == LINUX ):
    compile_boost_linux()
else:
    compile_boost_windows()

#COMPILE
if easygui.boolbox(msg='Would you like to compile other libraries?', title='Compile other libraries', choices=('Yes', 'No'), image=None):
    os.chdir("build")

    if(platform == "linux"):
        subprocess.call('cmake ../ -DCMAKE_BUILD_TYPE=RelWithDebInfo -G "Unix Makefiles"', shell=True)
        subprocess.call('make ' + buildcores, shell=True)
    else:
        subprocess.call('cmake ../ -DCMAKE_BUILD_TYPE=RelWithDebInfo -G "MinGW Makefiles"', shell=True)
        subprocess.call('mingw32-make ' + buildcores, shell=True)

    os.chdir("..")

#COPY ALL LIBS
if easygui.boolbox(msg='Would you like to copy all libraries?', title='Compile boost', choices=('Yes', 'No'), image=None):
    matches = []
    shared_matches = []
    
    directories = ["build", "libs/python", "libs/irrKlang-1.5.0"]

    #copy static libs
    for directory in directories:
        matches.extend(get_libs_from_dir(directory))
    
    if os.path.isdir(fullLibPath) == False:
        os.makedirs(fullLibPath)

    for f in matches:
        filename = os.path.join(libpath, f[0])
        shutil.copyfile(f[1], filename)

    #copy shared libs
    for directory in directories:
        shared_matches.extend(get_shared_libs_from_dir(directory))

    dbgPath = os.path.join(projectPath,"bin/Debug")
    relPath = os.path.join(projectPath,"bin/Release")
   
    if os.path.isdir(dbgPath) == False:
        os.makedirs(dbgPath)
   
    if os.path.isdir(relPath) == False:
        os.makedirs(relPath)
        
    for f in shared_matches:
        dbg_filename = os.path.join(dbgPath, f[0])
        rel_filename = os.path.join(relPath, f[0])
        shutil.copyfile(f[1], dbg_filename)
        shutil.copyfile(f[1], rel_filename)
        

    #Write lib names to file for easy config
    fname = os.path.join(libpath, "liblist.txt")
    file = open(fname, "w")

    print("Libs found: " + str(len(matches)))

    for f in matches:
        name = f[0]
        file.write(name[3:len(name)-2]+"\n")

    file.close()

    print("Press [Enter] key to continue...")
    s = input('--> ')
