import fnmatch
import os, shutil, subprocess
import subprocess

#INIT VARS
buildDir = "build"
platform = ""

if os.name == "posix":
    platform = "linux"
else:
    platform = "win32"

#COMPILE
if os.path.isdir("build") == False:
    os.mkdir("build")

os.chdir("build")

subprocess.call('cmake ../ -DCMAKE_BUILD_TYPE=RELWITHDEBINFO -G "MinGW Makefiles"', shell=True)
subprocess.call('mingw32-make -j5', shell=True)

os.chdir("..")

#COPY ALL LIBS
matches = []
for root, dirnames, filenames in os.walk('build'):
  for filename in fnmatch.filter(filenames, '*.a'):
      matches.append([filename,os.path.join(root, filename)])


libpath = os.path.join("libs", platform);

fullLibPath = os.path.join(os.getcwd(), libpath)

if os.path.isdir(fullLibPath) == False:
    os.makedirs(fullLibPath)

for f in matches:
    shutil.copyfile(f[1], os.path.join(libpath, f[0]))    

print("Press [Enter] key to continue...")
s = input('--> ')
