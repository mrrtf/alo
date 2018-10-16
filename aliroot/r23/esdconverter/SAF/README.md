All what's needed to build and run the `mch-esd-convertor` on SAF (which is a SLC6 system).

The build part is just a custom `CMakeLists.txt` which gets required sources from O2 (Detectors/MUON/MCH/Mapping parts) and alo (esd-convertor itself).

# Build

Must clone `O2` and `alo` repositories in some directory `basedir`

Then : 

```
source env.sh # <- specify here the base AliRoot used for ESD reading
source build.sh basedir
```

*WARNING* 

A few things are hard-coded in the `build.sh` : the path to find the [https://github.com/Microsoft/GSL](Guidelines Support Library) or the [https://github.com/google/flatbuffers](flatbuffer compiler).

# Run

The run part is a couple of scripts to launch condor jobs from text file(s) (ending by .list) containing the list of esds to process. 
The ESD filenames should be specified without any host prepended (e.g. just starting at `/alice/data/...`) : the `list-input.sh` script will
 prepend the SAF storage prefix.



