# SHELDON
## This repo holds my code for image processing event sheldon of KTJ'15
##### Compile prerequisites
* opencv 3
* gcc
* tesseract
* leptonica


#### To run 

g++ -ggdb `pkg-config --cflags opencv` -o `basename KTJ1.cpp .cpp` KTJ1.cpp `pkg-config --libs opencv tesseract`
