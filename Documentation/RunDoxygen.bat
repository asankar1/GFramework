ECHO OFF
ECHO "Generating the documentation.."
ECHO "=============================="
call where doxygen > DUMP
if NOT %ERRORLEVEL% == 0 (
	echo "Unable to run doxygen. Install it and add it to system PATH variable!"
	exit 1
)
	
if not exist GFramework.doxygen (
	echo "Unable to locate the configuration file 'GFramework.doxygen'."
	exit 1
	)
call doxygen GFramework.doxygen
if NOT %ERRORLEVEL% == 0 (
	echo "Doxygen error!"
	exit 1
)

SET "prefix=file:///"
SET "pwd=%~dp0"
SET mod_pwd=%pwd:\=/%
SET "postfix=output/html/index.html"

echo %prefix%%mod_pwd%%postfix%
