set path=D:\HenDuoDongXi\perl\c\bin;%path%
cd DefaultBuild
cmake --build ./Debug -j 1024 --config Debug
cmake --build ./Release -j 1024 --config Release
cd ..