# Design and Implementation of Java-like Garbage Collector for C
A garbage collector for C, works based on the principle of reachability of objects to detect memory leaks.
A C library to catch memory leaks produced by the applications, if any. This Memory Leak Detector(MLD) library which when integrated to your application will provide the facility to catch leaked objects and report them. MLD library will be a generic library and has the ability to parse any application's data structures and manipulate them.

1. Clone the source code as follows: <br />
&nbsp;&nbsp;`$ git clone https://github.com/harishkumar101/MemoryLeakDetector.git` <br />

2. Compile and run the application as follows: <br />
&nbsp;&nbsp;`$ make` <br />
&nbsp;&nbsp;`$ ./garbagecollector` <br />

3. Clean the binaries as follows: <br />
&nbsp;&nbsp;`$ make clean` <br />

4. Output Screenshot:
![Output Screenshot](https://github.com/harishkumar101/MemoryLeakDetector/blob/main/memleakdetector.png?raw=true)
