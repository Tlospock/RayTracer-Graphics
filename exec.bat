REM Batch file to launch ray tracer on specified file
REM change the input_file value to change file
set input_file=scene01-texture-ss-reflect-lights-shadows-OBJModel.yaml
echo input_file
bin\debug\GraphicsRayTracer.exe %input_file% output.png

REM bin\debug\GraphicsRayTracer.exe scene01-texture-ss-reflect-lights-shadows.yaml sceneTexture.png
REM bin\debug\GraphicsRayTracer.exe scene01.yaml scene01.png
REM bin\debug\GraphicsRayTracer.exe scene01-phong.yaml scene01-phong.png
REM bin\debug\GraphicsRayTracer.exe scene01-zbuffer.yaml scene01-zbuffer.png
REM bin\debug\GraphicsRayTracer.exe scene01-normal.yaml scene01-normal.png
REM bin\debug\GraphicsRayTracer.exe scene01-plane.yaml scene01-plane.png
REM bin\debug\GraphicsRayTracer.exe scene01-triangle.yaml scene01-triangle.png
REM bin\debug\GraphicsRayTracer.exe scene01-shadow.yaml scene01-shadow.png

REM Open the output file
output.png
REM pause