import os, shutil, subprocess

subprocess.call('pexports python34.dll > python34.def', shell=True)
subprocess.call('dlltool --dllname python34.dll --def python34.def --output-lib libpython34.a', shell=True)
