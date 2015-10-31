#ifndef PYTHON_MANAGER_H
#define PYTHON_MANAGER_H

#include "VoxelOctreeApp.h"
#include "utility/Logger.h"

class PythonManager
{
public:
	static bool RunScript(const std::string & script);
	static Path GetPythonScriptLoadPath();
	static bool Init();
private:

};


Path PythonManager::GetPythonScriptLoadPath()
{
	///Write directory should be our application directory, unless setting it failed.
	Path applicationPath = GetContext().GetFileSystem()->GetWriteDirectory();
	applicationPath.append("python");
	return applicationPath;
}

bool PythonManager::RunScript(const std::string & script)
{
	char save_error_type[1024], save_error_info[1024];
  	PyObject *errobj, *errdata, *errtraceback, *pystring;

 	int retval = PyRun_SimpleString(script.c_str());

	if(retval==-1)
	{
		GetContext().GetLogger()->log(LOG_LOG, "Failed to run python script..");
	}

	return true;
}

bool PythonManager::Init()
{
	Path pythonLoadPath = GetPythonScriptLoadPath();
	
	std::string scriptLoadString = "script_path = '" + pythonLoadPath.generic_string() + "'";
	std::string initString = "import sys, os\n";
	initString += scriptLoadString + "\n";
	initString += "sys.path.append(script_path)\n";
	initString += "print('Python search path: ' + str(sys.path))\n";

	PythonManager::RunScript(initString);
}

#endif