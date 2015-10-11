#!/usr/bin/python

import fnmatch
import os, shutil, subprocess
from tkinter import Tk, Frame, Checkbutton, Button, W
from tkinter import IntVar, RIGHT, LEFT, BOTH


class Launcher(Frame):
  
    def __init__(self, parent):
        Frame.__init__(self, parent)   
         
        self.parent = parent        
        self.initUI()
        
    def initUI(self):
        self.parent.title("Launcher")

        self.pack(fill=BOTH, expand=1)
        self.should_build = IntVar()
        self.should_run = IntVar()

        self.columnconfigure(0, pad=5)

        self.rowconfigure(0, pad=3)
        self.rowconfigure(1, pad=3)
        self.rowconfigure(2, pad=3)
        self.rowconfigure(3, pad=3)
        self.rowconfigure(4, pad=3)
        
        self.cb_build = Checkbutton(self, text="Build app",
            variable=self.should_build)
        self.cb_build.select()
        self.cb_build.grid(row=0, column=0, sticky=W)
        
        self.cb_run = Checkbutton(self, text="Run after build",
            variable=self.should_run)
        self.cb_run.select()
        self.cb_run.grid(row=1, column=0, sticky=W)

        self.okButton = Button(self, text="Start", command=self.onClick)
        self.okButton.grid(row=2, column=0, sticky=W)
        
    def onClick(self):
        buildSucceeded = False
        
        if self.should_build.get():
            buildSucceeded = self.startBuild()
            
        if not self.should_build.get() or buildSucceeded:
            if self.should_run.get():
                self.runApplication()


    def startBuild(self):
        buildSucceeded = False
        
        restore_path = os.getcwd()

        if os.path.isdir("bin") == False:
            os.mkdir("bin")

        os.chdir("cmake")
        
        #Remove hardcoded path! :D
        ret = subprocess.call('cmake ../cmake -G "Unix Makefiles" -Wno-dev -DPROJECT_PATH:STRING="/home/serengeor/Coding/Project"', shell=True)
        ret2 = subprocess.call('make -j9', shell=True)
        
        buildSucceeded = (ret == 0 and ret2 == 0)
        
        if buildSucceeded:
            shutil.move(os.path.abspath("VoxelOctree"), "../bin/VoxelOctree")

        os.chdir(restore_path)

        return buildSucceeded

    def runApplication(self):
        restore_path = os.getcwd()
        
        #os.chdir("bin")

        subprocess.call('./run.sh 1', shell=True)

        #os.chdir(restore_path)

def main():
    root = Tk()
    root.geometry("150x80+300+300")
    root.resizable(0,0)
    app = Launcher(root)
    root.mainloop()  


if __name__ == '__main__':
    main()  
