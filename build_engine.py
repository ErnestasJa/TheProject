import fnmatch
import os, shutil, subprocess

class OS:
    linux = "linux"
    windows = "windows"

lib_folder_mappings = {
    "boost" : "boost"
}

class Helper:
    def __init__(self):
        self.buildPath = "build"
        self.binPath = "bin"
        self.platform = ""
        self.libpath = "libs"
        self.enginePath = "engine"
        self.liboutputpath = ""
        self.fullLibPath = ""
        self.fullLibBuildPath = ""
        self.fullEnginePath = ""
        self.fullBuildPath = ""
        self.fullProjectBinPath = ""
        self.buildcores = "-j9"
        self.projectPath = os.getcwd()

        if os.name == "posix":
            self.platform = OS.linux
        else:
            self.platform = OS.windows

        self.liboutputpath = os.path.join(os.path.join(self.projectPath, self.libpath), self.platform);
        self.fullLibPath = os.path.join(self.projectPath, self.libpath)
        self.fullLibBuildPath = os.path.join(self.fullLibPath, self.buildPath)
        self.fullEnginePath = os.path.join(self.projectPath, self.enginePath)
        self.fullBuildPath = os.path.join(self.projectPath, self.buildPath)
        self.fullProjectBinPath = os.path.join(self.projectPath, self.binPath)


    def get_libs_from_dir(self, dir):
        match = []

        for root, dirnames, filenames in os.walk(dir):
            for filename in fnmatch.filter(filenames, '*.a'):
                if filename != "objects.a":
                    match.append([filename,os.path.join(root, filename)])
                    
        return match

    def get_shared_libs_from_dir(self, dir ):
        match = []
        
        for root, dirnames, filenames in os.walk(dir):
            for filename in fnmatch.filter(filenames, '*.so'):
                match.append([filename,os.path.join(root, filename)])
                    
        return match

    def get_lib_path(self, libFolderName):
        return os.path.join(self.fullLibPath, libFolderName)

    def copy_libs(self):
        os.chdir(self.projectPath)

        matches = []
        shared_matches = []

        directories = [
            self.fullLibBuildPath,
            self.get_lib_path("python"),
            self.get_lib_path("irrKlang-1.5.0"),
            self.get_lib_path("bullet"),
            self.get_lib_path("libccd"),
            os.path.join(self.fullEnginePath, self.buildPath)
        ]
        
        #copy static libs
        for directory in directories:
            matches.extend(self.get_libs_from_dir(directory))

        for f in matches:
            filename = os.path.join(self.fullLibBuildPath, f[0])
            try:
                shutil.move(f[1], filename)
            except:
                pass

        #copy shared libs
        for directory in directories:
            shared_matches.extend(self.get_shared_libs_from_dir(directory))

        dbgPath = os.path.join(self.fullProjectBinPath,"Debug")
        relPath = os.path.join(self.fullProjectBinPath,"Release")
       
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
        fname = os.path.join(self.fullLibBuildPath, "liblist.txt")
        file = open(fname, "w")

        print("Libs found: " + str(len(matches)))

        for f in matches:
            name = f[0]
            file.write(name[3:len(name)-2]+"\n")

        file.close()

        os.chdir(self.projectPath)

    def compile_other_libs(self):
        os.chdir(self.fullLibPath)

        try:
            os.mkdir(self.buildPath)
        except OSError as e:
            print("Caught this little bad boy: " + str(e))
        
        os.chdir(self.buildPath)

        if(self.platform == "linux"):
            subprocess.call('cmake ../ -DCMAKE_BUILD_TYPE=RelWithDebInfo -G "Unix Makefiles"', shell=True)
            subprocess.call('make ' + self.buildcores, shell=True)
        
        os.chdir(self.projectPath)

    def compile_engine(self):
        os.chdir(self.fullEnginePath)

        try:
            os.mkdir(self.buildPath)
        except OSError as e:
            print("Caught this little bad boy: " + str(e))

        os.chdir(self.buildPath)

        if(self.platform == "linux"):
            subprocess.call('cmake ../ -DCMAKE_BUILD_TYPE=RelWithDebInfo -G "Unix Makefiles" -DPROJECT_PATH:STRING="/home/serengeor/Coding/Project"', shell=True)
            subprocess.call('make ' + self.buildcores, shell=True)
       
        os.chdir(self.projectPath)

    def compile_boost_build_sys(self):
        os.chdir(os.path.join(self.fullLibPath, lib_folder_mappings["boost"]))
        subprocess.call('sh bootstrap.sh gcc', shell=True)
        os.chdir(self.projectPath)

    def compile_boost_bcp_tool(self):
        os.chdir(os.path.join(self.fullLibPath, lib_folder_mappings["boost"]))
        subprocess.call('./b2 tools/bcp', shell=True)
        os.chdir(self.projectPath)

    def compile_boost(self):
        os.chdir(os.path.join(self.fullLibPath, lib_folder_mappings["boost"]))
        subprocess.call('./b2 ' + self.buildcores + ' --without-python --build-dir="../../build"  toolset=gcc link=static threading=multi release', shell=True)
        os.chdir(self.projectPath)


from tkinter import Tk, Frame, Checkbutton, Button, OptionMenu, Label, W
from tkinter import IntVar, StringVar, RIGHT, LEFT, BOTH

class Launcher(Frame):
  
    def __init__(self, parent):
        Frame.__init__(self, parent)   
         
        self.parent = parent        
        self.initUI()
        self.helper = Helper()
        
    def initUI(self):
        self.parent.title("Lib builder")
        self.pack(fill=BOTH, expand=1)
        self.columnconfigure(0, pad=5)
        self.columnconfigure(1, pad=5)
        self.check_vars = {}
        self.check_buttons = {}
        self.addCheckButtons([
            ["BoostBS", (1,"Build boost build system?")],
            ["BoostBCP", (0,"Build boost bcp tool?")],
            ["Boost", (1,"Build boost?")],
            ["Other", (1,"Build other libraries?")],
            ["Engine", (1,"Build engine!")],
            ["Copy", (1, "Copy all lib.")]
            ])
        
        w = Label(self, text="Thread count:")
        w.grid(row=len(self.check_vars)+1, column=0, padx=8, sticky=W)
    
        self.threadChoices = [ "2", "3", "5", "9", "17" ]
        self.threadChoice = StringVar(self, value=self.threadChoices[3])

        w = OptionMenu(self, self.threadChoice, *self.threadChoices)
        w.grid(row=len(self.check_vars)+1, column=1, sticky=W)

        self.okButton = Button(self, text="Start", command=self.onClick)
        self.okButton.grid(row=len(self.check_vars)+2, column=0, sticky=W)

    def addCheckButtons(self, listception):
        index = 0
        for items in listception:
            key = items[0]
            val = items[1]
            self.rowconfigure(index, pad=3)
            var = IntVar(value=val[0])
            cb = Checkbutton(self, text=val[1], variable=var).grid(row=index, column=0, sticky=W)
            self.check_vars[key]=var
            self.check_buttons[key]=cb
            index+=1

    def onClick(self):
        buildSucceeded = False
        self.buildcores = "-j" + self.threadChoice.get()
        
        for key,val in self.check_vars.items():
            if val.get() == 0:
                continue

            elif key == "Boost":
                self.helper.compile_boost()

            elif key == "Engine":
                self.helper.compile_engine()

            elif key == "BoostBS":
                self.helper.compile_boost_build_sys()

            elif key == "Other":
                self.helper.compile_other_libs()

            elif key == "Copy":
                self.helper.copy_libs()

def main():
    root = Tk()
    root.geometry("250x150+300+300")
    app = Launcher(root)
    root.mainloop()  


if __name__ == '__main__':
    main()  
