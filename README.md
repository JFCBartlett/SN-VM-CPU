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

|MCC File|No Parallelisation|Parallelised (with reduction)|Number of Speedups|without reduction|Reduction Speedup|
|--------|------------------|-----------------------------|------------------|-----------------|-----------------|
|d2      |0.000508 s        |0.032883 s                   |-64.73            |0.029633 s       |-0.11            |
|d3      |0.018005 s        |0.554946 s                   |-30.82            |0.551388 s       |-0.01            |
|d4      |7.903104 s        |166.627328 s                 |-21.08            |168.613750 s     |0.01             |
|mm5     |7.931255 s        |1.828004 s                   |4.34              |1.998270 s       |0.09             |
|mm6     |32.919470 s       |5.910169 s                   |5.57              |6.916206 s       |0.17             |
|mm7     |109.383386 s      |15.639665 s                  |6.99              |20.957118 s      |0.34             |
|mm10    |2805.081585 s     |357.467199 s                 |7.85              |415.732053 s     |0.16             |

The parallel version is slower for the dn nets since they are only able to be computed sequentially. The reason they are far slower is since several parallel sections are opened and closed and this likely why the difference between parallel and sequential reduces as the net gets larger.

The parallel version is faster for the mmn nets as they can be calculated in parallel which makes the parallel version quicker. This becomes more prevelant as the nets increase in size.

<p align="middle">
  <img src="https://github.com/user-attachments/assets/6ff89e05-50ca-476d-b263-56e27544eede" width="384"><img src="https://github.com/user-attachments/assets/87f45f57-4aa2-4c7f-8cf6-c946d1bbaa75" width="384">
</p>
d4 and mm10 have been omitted due to the factor in execution time.

## References

1. References for Sleptsov Net Computing (SNC) to read, watch, run, cite, and join. https://dimazaitsev.github.io/snc.html
