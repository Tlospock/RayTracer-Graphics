REM Batch file to launch ray tracer on specified file
set input_file=scene01.yaml
echo input_file
bin\debug\GraphicsRayTracer.exe %input_file% output.png
pause