import fnmatch
import os, shutil, subprocess
import subprocess
from tools import easygui

def get_libs_from_dir( dir ):
    match = []
    
    for root, dirnames, filenames in os.walk(dir):
        for filename in fnmatch.filter(filenames, '*.a'):
            if filename != "objects.a":
                match.append([filename,os.path.join(root, filename)])
                
    return match

#INIT VARS
buildDir = "build"
platform = ""

if os.name == "posix":
    platform = "linux"
else:
    platform = "win32"

libpath = os.path.join("libs", platform);
fullLibPath = os.path.join(os.getcwd(), libpath)

#warn if no boost
boostpath = os.path.join(os.getcwd(), "libs/boost")
if (os.path.isdir(boostpath) == False) or (len(os.listdir(boostpath)) == 0):
    easygui.msgbox("You do not have boost sources inside libs folder.\nClose this message box to continue.", title="Boost missing!")

#COMPILE
if os.path.isdir("build") == False:
    os.mkdir("build")

os.chdir("build")

subprocess.call('cmake ../ -DCMAKE_BUILD_TYPE=RelWithDebInfo -G "MinGW Makefiles"', shell=True)
subprocess.call('mingw32-make -j5', shell=True)

os.chdir("..")

#COPY ALL LIBS
matches = []
directories = ["build","libs/boost/lib","libs/freetype"]

for directory in directories:
    matches.extend(get_libs_from_dir(directory))

if os.path.isdir(fullLibPath) == False:
    os.makedirs(fullLibPath)

for f in matches:
    filename = "" + os.path.join(libpath, f[0])
    shutil.copyfile(f[1], filename)

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
