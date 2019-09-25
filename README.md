# ANN-with-genetic-algorithm
It is ANN(artificial neural network) with genetic algorithm. Each AI can learn how to find their forage.

You can build it by Visual Studio 2015 (different version can cause some problem.)
or you can just run x64\Release\opencvtest.exe. 

How to use
-------------
1. build the project or run opencvtest.exe
2. If it is first time to run it, then type 1 and press enter button. If you want to load save file, type 2 and press enter button.
3. Watch movment of AIs. If all of them are died, the program will be closed automatically and make save file.
4. If you want to train AIs more, re-run the program and load the save file.

How does it work
-------------
each AI has 36 neurons as input, 3 neurons as output, 2 hidden layers, 40 hidden neurons at each hidden layers. From 0 to 11 input neurons are used to detect foods. From 12 to 23 input neurons are used to carnivores. From 24 to 35 input neurons are used to detect herbivores.
> NOTE herbivores are blue AI and carnivores are red AI. However, I found it is hard to optimize AI of carnivores. So I blocked to create carnivores. If you want to create carnivores, you should fix the code; you need to delete comment letter on 150, 182, 203 rows and manipulate 11 row and add comment on 149 row.

0 output neuron changes velocity. 1 output neuron changes angular velocity. 2 output neuron is just for beeing reserved. They become died if they do not eat sufficient forage. On the other hand, if they eat forage, they divide by 2 AIs. At that time, their weighting factors become different from their mother's one. Consequently, smarter AI can subsist after a long time.

Result Image
-------------
![Alt text](/picture/gen1.gif "Generation 1")
* generation 1  
![Alt text](/picture/gen3.gif "Generation 3")
* generation 3  
![Alt text](/picture/gen5.gif "Generation 5")
* generation 5  
![Alt text](/picture/gen11.gif "Generation 11")
* generation 11  
![Alt text](/picture/close.gif "Close")
* movement of carnivore

Used library
-------------
opencv

Reference
-------------
https://home.fnal.gov/~souvik/Brain/index.html
the program of regerence link looks so similar with mine. However, it is just derived from same graphic library(opencv).
Actual code is totally different!
