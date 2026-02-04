# process-affinity-changer-cpp
A simple console application to modify process affinity mask of a running executable.


The current limit for setting up the mask is (first) 16 cores

Only works on the first runnig .exe with identical names.
For example, when multiclienting only the first .exe will have its affinity set to the one in the configuration.

If you want to change the affinity configuration you can simply open AffinityNumber.txt and set both of the the values in the file to 0, then save.
Next time you run the executable you will get the configuration setup again in the console.
