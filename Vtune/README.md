# Intel Vtune Assignment

## Problem Statement

### Part1

- Run the performance snapshot analysis over the sample matrix multiplication code provided. And report the IPC, bad speculation, logical core utilization.
- Now, run hotspot detection on the same code and report top hotspot function along with percentage of CPU time.
- Follow up, with the memory access analysis and report percentage of L2-bound, L3-bound and LLC-bound memory accesses. Here L2-bound memory accesses refer to accesses that where they where stalled on L2 i.e, a L1 miss happened but the data was present in L2.
- Next, run the microarchitecture exploration tool and report the number of instructions retired, average CPU frequency, effective logical core utilization and explain the difference with the logical core utilization found in performance snapshot analysis.
- For each of these parts, you need to show screenshots to validate your numbers and briefly explain what these metrics are and what information do you gain from them. 

### Part2

#### Intro
- You are givent the c code for a [password cracking](cracker.c) application which you are required to optimize.
- **The encryption/checksum algorithms are awkward,insecure and is not an example of a practical application. This is done intentionally to allow significant scope for optimization**

#### Compilation
A [Makefile](Makefile) is provided in the same folder with the required optimization flags you will be evaluated on. The following command can be used to compile the application generating the binary cracker.o
```
make
``` 
- The number of passwords that are tested can be configured via the compile time parameter ITER. This is provided in the code in the following manner:
```
#define ITER 10000
```

#### Running Instructions
No other parameters or arguments are required to run the password cracker. The command used to execute it is as follows:
```
./cracker.o
```

#### Output Format
```
= = = VTune profiling demo = = =
=================================
memory malloc		+OK
initializing from		built-in data
memory malloc		+OK
max iter		100000
===
try search... wait!
Password per sec:       250 
```
The beginning section of the output can be used to verify if the initialization process completes successfully along with the value of the parameter ITER. The application in addition prints the number of passwords cracker per second. Note that this value is empirical and is affected by system noise, value of ITER and the resolution of the clock in use.

#### Objective
You are expected to use the vtune tool to detect hotspots and remove them preserving the algorithm used. The number of passwords cracked per second will be used to determine the grade.

#### Constraints
- You are not allowed to use the c library functions strlen and memcmp and are needed to implement a custom optimized version of the same in the provided my_strlen and my_memcmp functions provided. In addition assume that the lengths of ciphertext and plaintext are also unknown and need to be computed during runtime.
- Do not modify the decryption scheme as well as the representation used to hold the required keys and password. In addition you are not expected to modify the order in which passwords are generated as well as the way the keys are held. i.e, the value of any global data structure must be preserved at the end of execution.
- Do not create any new global data structures.
- Do not modify any line of code following the marker `// DO NOT MODIFY ANY CODE BELOW`
- Note that any optimizations not following the flowchart will be directly awarded 0.
- If you are unsure regarding whether an optimization violates the constraints/flowchart/intent then ask the TAs preferably during the lab hours. 

#### Flowchart
![](lab5_vtune.svg?raw=true)

#### Pseudo-code for the decrypt function
Note that the function sub_func is a substitution function that takes in an integer to return another integer as per a mapping. This mapping must not be changed. Further the password is treated as a 2-D matrix while the key as an array.
```
Decrypt:
    plaintext := ciphertext
    for j := range 0 to PASS_Y_LIM
    do
        for i := range 0 to PASS_X_LIM
        do
            ke := key[i*PASS_Y_LIM + j]
            pr := (ke.li ^ ke.ki ^ ke.zi) & 0xf
            mask := ke.a[(pr + 1) >> 4]
            if pr eq 15
            then
                pr := 31
            end
            sub_res := call sub_func (pr & mask)
            plaintext[(i*PASS_Y_LIM + j) mod len(plaintext)] := plaintext[(i*PASS_Y_LIM + j) mod len(plaintext)] ^ sub_res ^ pswd[i][j] 
        end
    end
```

#### Report
- Show a screenshot of the final output obtained along with a presentation of the series of optimizations performed and the intuition/reason behind them in the video. You will have to defend every optimization performed.
- Show a screenshot showing the effect of each optimization performed and the resultant performance improvement over baseline in the report as well. Ex. [slide 9](https://www.cse.iitb.ac.in/~biswa/courses/CS305/lectures/L34.pdf)
- **Pro Tip**: Always try to optimize the common case and the largest hotspot first.
- **Pro Tip**: We can not time a single instruction in a pipelined processor, so whenever a hotspot occurs search it's vicinity as well.

## Resources
- [Installation and basic usage of Intel Vtune](https://github.com/CS232-Labs/Lab-5---Resources/tree/main/intel-vtune)
- [Stable benchmarking of binaries in modern machines](https://easyperf.net/blog/2019/08/02/Perf-measurement-environment-on-Linux)
