All what's needed to build and run the `mch-esd-converter` on SAF (which is a SLC6 system).

# Build

Must clone `alo` repository in some directory `basedir`

Then : 

```
source env.sh # <- specify here the base AliRoot used for ESD reading
source build.sh basedir
```

# Run

The run part is a couple of scripts to launch condor jobs from text file(s) (ending by .list) containing the list of esds to process. 
The ESD filenames should be specified without any host prepended (e.g. just starting at `/alice/data/...`) : the `list-input.sh` script will
 prepend the SAF storage prefix.



