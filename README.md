# ANN-with-genetic-algorithm
It is ANN(artificial neural network) with a genetic algorithm. Each AI can learn how to find their forage.

You can build it by Visual Studio 2015 (different versions can cause some problems.)
or you can just run x64\Release\opencvtest.exe. 

How to use
-------------
> If you do not have Visual Studio, then you need to install Visual C++ Redistributable. Here is the link (https://www.microsoft.com/en-pk/download/details.aspx?id=48145)
1. build the project or run opencvtest.exe
> Because I uploaded dll files by Git LFS. You can not download dll files by "Download Zip" button. It will make some error having to do with opencv_world330.dll. Therefore, you need to download opencv_world330.dll file separately or you can just download it from the official website (https://opencv.org/)
2. If it is the first time to run it, then type 1 and press the enter button. If you want to load the save file, type 2 and press the enter button.
3. Watch the movement of AIs. If all of them die, the program will be closed automatically and make a save file.
4. If you want to train AIs more, re-run the program and load the save file.

How does it work
-------------
each AI has 36 neurons as input, 3 neurons as output, 2 hidden layers, 40 hidden neurons at each hidden layer. From 0 to 11 input neurons are used to detect foods. From 12 to 23 input neurons are used to carnivores. From 24 to 35 input neurons are used to detect herbivores.
> NOTE herbivores are blue AI and carnivores are red AI. However, I found it is hard to optimize AI of carnivores. So I blocked to create carnivores. If you want to create carnivores, you should fix the code; you need to delete comment letter on 150, 182, 203 rows and manipulate 11 row and add comment on 149 row.

0 output neuron changes velocity. 1 output neuron changes angular velocity. 2 output neuron is just for being reserved. They become died if they do not eat sufficient forage. On the other hand, if they eat forage, they divide by 2 AIs. At that time, their weighting factors become different from their mother's. Consequently, smarter AI can subsist after a long time.

Developement period
-------------
I made the fundamental structure of ANN in March 2017. Then I used the structure to make this program in October 2017. However, I am not sure about the accurate period of development.

Result Image
-------------
![Alt text](/picture/new/gen1.gif "Generation 1")
* generation 1  
#
![Alt text](/picture/new/gen3.gif "Generation 3")
* generation 3  
#
![Alt text](/picture/new/gen5.gif "Generation 5")
* generation 5  
#
![Alt text](/picture/new/gen11.gif "Generation 11")
* generation 11
#
![Alt text](/picture/new/gen16.gif "Generation 16")
* generation 16
#
![Alt text](/picture/new/gen17.gif "Generation 17")
* generation 17 after a long time. At generation 17, they did not become extinct.
#
![Alt text](/picture/new/close1.gif "Close")
* movement of herbivores 1
#
![Alt text](/picture/new/close2.gif "Close")
* movement of herbivores 2

Used library
-------------
opencv

Reference
-------------
https://home.fnal.gov/~souvik/Brain/index.html
the program of reference link looks so similar to mine. However, it is just derived from the same graphic library(OpenCV).
Actual code is totally different!
