
[![GitPitch](https://gitpitch.com/assets/badge.svg)](https://gitpitch.com/mrrtf/alo/master?grs=github&t=white)

A small package for Alice Muon Reconstruction development.

It (might) depends both on AliceO2 and on AliRoot and is meant to hold
interim solutions that bridge the gap between the two.  It e.g extracts
some information from old AliRoot, massages it and writes it into a form
digestible by O2 (while waiting for the O2 part to be able to perform
the same kind of operation itself)

This is really a work-in-progress and will never be finalized, by
construction, as it is meant to disappear (the sooner the better)

More documentation is to be found under the different sub-directories :

- [dhlt](dhlt/README.md) for HLT-related devices
- [jsonmap](jsonmap/README.md) for the work on MCH mapping format evolution
- [r23](r23/README.md) for the run2 to run3 data converters

