REM Batch file to launch ray tracer on specified file
REM change the input_file value to change file
set input_file=scene02.yaml
echo input_file
bin\debug\GraphicsRayTracer.exe %input_file% output.png
bin\debug\GraphicsRayTracer.exe scene01.yaml scene01.png
bin\debug\GraphicsRayTracer.exe scene02.yaml scene02.png
pause