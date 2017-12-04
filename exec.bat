REM Batch file to launch ray tracer on specified file
REM change the input_file value to change file
set input_file=scene01.yaml
echo input_file
bin\debug\GraphicsRayTracer.exe %input_file% output.png

bin\debug\GraphicsRayTracer.exe scene02.yaml scene02.png
bin\debug\GraphicsRayTracer.exe scene01.yaml scene01.png
bin\debug\GraphicsRayTracer.exe scene01-phong.yaml scene01-phong.png
bin\debug\GraphicsRayTracer.exe scene01-zbuffer.yaml scene01-zbuffer.png
bin\debug\GraphicsRayTracer.exe scene01-normal.yaml scene01-normal.png
bin\debug\GraphicsRayTracer.exe scene01-plane.yaml scene01-plane.png
bin\debug\GraphicsRayTracer.exe scene01-triangle.yaml scene01-triangle.png
bin\debug\GraphicsRayTracer.exe scene01-shadow.yaml scene01-shadow.png

REM Open the output file
output.png