Data formats for MCH.

## Run2

Some custom made exchange format using [FlatBuffers](https://google.github.io/flatbuffers/) : [Run2.fbs](Run2.fbs).

Of some interest here is the `mch-dump-run2` program which takes a binary file as input (FlatBuffers format) and outputs (Run3-like) YAML for closer inspection / display / etc...

```
mch-dump-run2 -m 10 $HOME/cernbox/o2muon/MCH-EVENTS-100.dat --silent 1
```

will output one YAML file for each event in the FlatBuffer file `MCH-EVENTS-100.dat` which contains cluster(s). The YAML file contain clusters.

```
> cat cluster-10.yaml
- pre:
    digitgroup:
      reftime: 0
      digits:
        - deid: 100
          dsid: 235
          dsch: 16
          adc: 294
          charge: 4.661163
        - deid: 100
          dsid: 235
          dsch: 61
          adc: 538
          charge: 36.61433
        - deid: 100
          dsid: 235
          dsch: 17
          adc: 252
          charge: 0.8196875
        - deid: 100
          dsid: 235
          dsch: 60
          adc: 341
          charge: 4.358299
        - deid: 100
          dsid: 235
          dsch: 62
          adc: 347
          charge: 2.905123
        - deid: 100
          dsid: 1260
          dsch: 60
          adc: 433
          charge: 18.03755
        - deid: 100
          dsid: 1260
          dsch: 62
          adc: 637
          charge: 44.42493
  pos:
    x: 25.45887
    y: 74.50821
    z: -530.4827
  charge: 57.69108
```

## Run3

Completely preliminary. Do not use/test/watch ;-) 
