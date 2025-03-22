# SN-VM-CPU
Virtual machine of Sleptsov Net on CPU using OpenMP using only the MCC file format. Quickly computes the result of a Sleptsov Net using multiple CPU cores.

This code is based on [SN-VM-GPU](https://github.com/tishtri/SN-VM-GPU) and has been modified to run on a CPU using OpenMP.

## How to compile and run

This is how to compile the program using GCC. Requires GCC 14 or higher. The -fopenmp flag is also needed for the parallelisation to work. The -o flag is used to name the executable file.

```
gcc sn-vm-cpu-mcc.c -o sn-vm-cpu-mcc -fopenmp
```

Usage:
```
sn-vm-cpu-mcc [argv1] [argv2] < SN.mcc
  argv1 is the debug level, default is zero and goes upto three with each level giving more debug info. 
  argv2 is the Sleptsov Net steps limit, this limits the number of Sleptsov Net steps (no limit by default).
  SN.mcc is the given Sleptsov Net in the MCC format.
```

The program's output can be redirected to a txt file using `>` or `>>`. `>>` appends to the file, `>` overwrites.
```
sn-vm-cpu-mcc < SN.mcc > output.txt
```

## Results

|MCC File|No Parallelisation|Parallelised|Number of Speedups|
|--------|------------------|------------|------------------|
|d2      |0.000508 s        |0.032883 s  |-64.73            |
|d4      |7.903104 s        |166.627328 s|-21.08            |
|mmul5   |7.931255 s        |1.828004 s  |4.34              |
|mmul6   |32.919470 s       |5.910169 s  |5.57              |
|mmul7   |109.383386 s      |15.639665 s |6.99              |
|mm10    |2805.081585 s     |357.467199 s|7.85              |


## References

1. References for Sleptsov Net Computing (SNC) to read, watch, run, cite, and join. https://dimazaitsev.github.io/snc.html
