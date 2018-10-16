This directory is supposed to contain the various [data converters](https://alice.its.cern.ch/jira/browse/MRRTF-17)
(from Run2 to Run3 formats) that are due for MRRTF.

The starting point was what was used in the past to
 produce [digit files for MB Pb-Pb](http://aphecetche.github.io/2016/03/13/o2-create-digit-files/) : 
  a bunch of functions quickly stuffed in a single class in the [legacy](legacy) directory.

Then some tools to [convert Run2 ESDS](esdconverter) into Run3 (tentative, so far) data formats. 

