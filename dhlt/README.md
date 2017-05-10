# Dev notes

Compared to what used to be in the `feature-muonhlt` AliRoot branch (or in HLT
in general), I've tried to minimize the number of source files. So there's only
one file per data structure, instead of 3 originally (Struct itself, Block of
struct, Block writer). Some reorganisation was anyway needed as some constants
had to be "extracted" from some common classes (`AliHLTMUONConstants` and
`AliHLTMUONDataTypes`)

# Testing the (HLT) preclustering device

## Preparatory steps (once) 

For the next steps you will need a default OCDB. Using [alibuild](http://alisw.github.io/git-advanced/) you can get one using : 

```
aliBuild build AliRoot-OCDB
```

and the OCDB path will be :

```
$ALIROOT_OCDB_ROOT/OCDB
```

as long as you've done : 

```
alienv enter AliRoot/latest,AliRoot-OCDB/latest,O2/latest,alo/latest
```

Without `alibuild` it's a matter of cloning the right repo and define yourself the environment variable:

```
cd ~/alice
git clone https://gitlab.cern.ch/alisw/AliRootOCDB.git
export ALIROOT_OCDB_ROOT=$PWD/AliRootOCDB
```

Next, in order to avoid having the preclustering component compute its internal mapping all over again each time it is
started, the mapping must be created once for all, using the `dhlt-generate-binmapfile` executable :

```
dhlt-generate-binmapfile -cdbpath local://$ALIROOT_OCDB_ROOT/OCDB -run 0 -binmapfile binmapfile.dat
```

Finally, you'll need to get a digit file, e.g. :

```
curl -o merged.digits.MB.196099.root https://cernbox.cern.ch/index.php/s/mgdDBOmu03XzBGk/download
```

You also _must_ set the `ALIHLT_HCDBDIR` to point to a valid OCDB, e.g. :

```
export ALIHLT_HCDBDIR=local://$ALIROOT_OCDB_ROOT/OCDB
```

## Execution

### Start digit reader

Launch, on one terminal, the Digit Reader

```bash
```

Hit 'r' to run it. Assuming this is the first device you launch, nothing should happen as no other device is asking for the data this one is producing.

### Start precluster finder

Get one (or several) cluster finder(s) running :

```bash
AliceHLTWrapperDevice 'Cluster Finder' --id 1 --channel-config 'name=data-in,type=pull,method=connect,size=10,address=tcp://localhost:45000' --library libdhlt --component MUONPreclusterFinder --parameter '-binmapfile binmapfile.dat' --run 16099
```

Hit 'r' to run it. That should trigger the start of the Digit Reader in the other terminal.

<!-- ### Misc -->
<!--  -->
<!-- Alternatively you can also try to launch the Digit Inspector (in another terminal) : -->
<!--  -->
<!-- ```bash -->
<!-- mch-digit-inspector --source tcp://localhost:45000 -->
<!-- ``` -->
<!--  -->
<!-- Hit 'r' to run it. That should trigger the start of the Digit Reader and display a raw dump of the digits in the current terminal. -->
<!--  -->
*/
