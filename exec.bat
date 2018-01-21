REM Batch file to launch ray tracer on specified file
REM change the input_file value to change file
set input_file=scene01-texture-ss-reflect-lights-shadows-OBJModel.yaml
echo input_file
bin\debug\GraphicsRayTracer.exe %input_file% output.png

REM Open the output file
output.png
pause