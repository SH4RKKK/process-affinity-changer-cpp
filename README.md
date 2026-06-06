# process-affinity-changer-cpp

A console tool that sets the CPU affinity mask of a running process.
Supports up to 16 cores and saves the configuration to a text file for reuse on next run.

**Written:** October 2020  
**Language:** C++ (MSVC)

## Notes

- Only targets the first running instance when multiple processes share the same name
- Configuration is stored in `AffinityNumber.txt`; set it to `0` to trigger setup again
