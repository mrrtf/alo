This directory contains :

- O2Muon functions from early works (circa 2014-2015) that can convert Run2 data into compact forms (but still Root-based, and based on AliRoot objects). Was used for instance to produce [digit files for MB Pb-Pb](http://aphecetche.xyz/2016/03/13/o2-create-digit-files/).

- More recent tools to inspect how the Run2 clustering is working. Mainly WIP.

## Clustering inspection

The idea here is twofold :

- first, to be able to _inspect_, for a small number of clusters (possible a single one), what exactly the Run2 clustering is doing. 
- second, to _scan_ a large number of (known) clusters to extract figures of merit of the current clustering.

For the first point, the `mch-clusterizer-inspector` takes as input a YAML file describing a few clusters and outputs another YAML file with the input clusters plus some information added during the clustering. That output YAML can be visualized with [galo](https://github.com/aphecetche/galo).

```
mch-clusterizer-inspector -i cluster.yaml -i pixel.yaml
```

The clustering under inspection has been copied from AliRoot into `MLEMdebug` versions in this directory, so some methods can be made public to give access to the "guts" of the classes.
