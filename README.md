# ANN-with-genetic-algorithm
It is ANN(artificial neural network) with genetic algorithm. Each AI can learn how to find their forage.

You can build it by Visual Studio 2015 (different version can cause some problem.)
or you can just run x64\Release\opencvtest.exe. 
>NOTE you should include opencv_world330.dll file. I have some trouble to upload dll files.

How to use
-------------
1. build the project or run opencvtest.exe
2. If it is first time to run it, then type 1 and press enter button. If you want to load save file, type 2 and press enter button.
3. Watch movment of AIs. If all of them are died, the program will be closed automatically and make save file.
4. If you want to train AIs more, re-run the program and load the save file.

How does it work
-------------
each AI has 36 neurons as input, 3 neurons as output, 2 hidden layers, 40 hidden neurons at each hidden layers. They become died if they do not eat sufficient forage. On the other hand, if they eat forage, they divide by 2 AIs. At that time, their weighting factors become different from their mother's one. Consequently, smarter AI can subsist after a long time.

Used library
-------------
opencv

Reference
-------------
https://home.fnal.gov/~souvik/Brain/index.html
the program of regerence link looks so similar with mine. However, it is just derived from same graphic library.(opencv)
Actual code is totally different!
