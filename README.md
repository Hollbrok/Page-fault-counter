# Page fault counter + table painter

this one was written one day before the final test.
This program counts the number of PF(page faults) and also prints tables for different algorithms (FIFS, LRU, OPT)

# Code

Don't pay too much attention to this code, because there is a lot of __shitcode__, xD.

# Hot to use

```console
make
./a __PROC_ALLOC_FRAMES__ [queue of frames]
```

# Example

```console
./a 3 1 2 3 5 2 1 4 5 2 1 3 4 1 2 5 1 3 2 4
```

There is __3__ -- number of frames allocated to the process.

So there you can see example of output:

![output](https://github.com/Hollbrok/System-Programming/blob/master/Page%20Fault%20counter/out_example.png?raw=true)
