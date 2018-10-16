
[![GitPitch](https://gitpitch.com/assets/badge.svg)](https://gitpitch.com/mrrtf/alo/master?grs=github&t=white)
A small package for Alice Muon Reconstruction development.

In fact, it is two packages in one. One depends _only_ AliceO2 and the other _only_ on AliRoot.

> alo used to depend on both O2 and AliRoot, but since the adoption of C++17 for O2, it was becoming almost impossible to compile AliRoot and O2 in compatible libs. Hence the sharp split.

The general idea of alo is to extract
some information from old AliRoot/Run2 data, massages it and in the end write it into a form
digestible by O2/Run3 (while waiting for the O2 part to be able to perform
the same kind of operation itself)

This is really a work-in-progress and will never be finalized, by
construction, as it is meant to disappear (the sooner the better)

More documentation is to be found under the different sub-directories :

- [aliroot](aliroot) : parts that depend on AliRoot
  - [dhlt](aliroot/dhlt/README.md) for HLT-related devices
  - [jsonmap](aliroot/jsonmap/README.md) for the work on MCH mapping format evolution
  - [r23](aliroot/r23/README.md) for the run2 to run3 data converters

- [o2](o2): parts that depend on O2
  - [mapping](o2/mapping) code generator for mch mapping
  - [dumper](o2/mch/dumper) code to dump digit and cluster files 

To build the AliRoot-dependent part, use :

```
aliBuild --defaults user-roo6 build alo-aliroot
```

To build the O2-dependent part, use :

```
$HOME/github.com/alisw/alibuild/aliBuild --defaults o2 build alo-o2
```

if you have a checkout of aliBuild, or just : 

```
aliBuild --defaults o2 build alo-o2
```

if you don't want to use the aliBuild git version.

The aliBuild master has the advantage of using the binary cache on CentOS7, so what gets rebuild by the command above should in principle be limited to alo itself. 

