# Tools to move the AliRoot MCH mapping into O2 era.

## Rationale

The starting point is to convert the custom text files used in the MCH mapping 
(in `AliRoot/MUON/mapping/data`) into [JSON](http://www.json.org) format.

Moving to a widely established text format means : 

- we no longer need custom parsers (`AliMp***Reader` classes in AliRoot)
- the files can easily be read in other languages (think Javascript for the display(s))
- some libraries (e.g. [FlatBuffers](https://google.github.io/flatbuffers/)) can be used to easily 
get a binary representation of those files, if need be.

For the JSON encoding/decoding we rely on the [RapidJSON](http://rapidjson.org) library. 

Compared to a hand-cooked solution (e.g. using bare `cout`)
RapidJSON has the advantage of producing valid JSON or just crash, 
 so we are sure to produce legal JSON every time. Plus it can also write pretty or compact JSON with a simple 
  class selection. Plus it can read back and parse JSON as well, of course.  

This directory is split in :

- `creator` with the classes that actually convert the Aliroot mapping objects into JSON files
- `codegen` which reads the JSON files built by the creator and turns them into C++ code to be used in O2

There are currently 3 different implementations in the works. Only impl3 is feature complete. impl2 
 is still buggy and impl1 will be removed simply (was kind of a template experiment that went too far ;-) 
 
## Usage 

It's a two steps process : first generate `JSON` files from `AliRoot` mapping, then
 generate code from the `JSON` files. Note that not all `JSON` files are required for all implementations.
 
  
```
> mch-mapping-convert-to-json && ls -alh

total 7976
drwxr-xr-x   13 laurent  staff   416B Feb 20 16:52 .
drwxr-xr-x+ 225 laurent  staff   7.0K Feb 20 16:52 ..
-rw-r--r--    1 laurent  staff    18K Feb 20 16:52 bergs.json
-rw-r--r--    1 laurent  staff   695K Feb 20 16:52 buspatches.json
-rw-r--r--    1 laurent  staff   4.0K Feb 20 16:52 chambers.json
-rw-r--r--    1 laurent  staff   4.8K Feb 20 16:52 ddls.json
-rw-r--r--    1 laurent  staff    63K Feb 20 16:52 detection_elements.json
-rw-r--r--    1 laurent  staff   231K Feb 20 16:52 legacyseg.json
-rw-r--r--    1 laurent  staff   1.6M Feb 20 16:52 motiftypes.json
-rw-r--r--    1 laurent  staff   2.0K Feb 20 16:52 padsizes.json
-rw-r--r--    1 laurent  staff    37K Feb 20 16:52 pcbs.json
-rw-r--r--    1 laurent  staff   748K Feb 20 16:52 segmentations.json

```

```
> mch-mapping-generate-code detection_elements.json segmentations.json bergs.json padsizes.json motiftypes.json --detection_elements --segmentations --bergs --padsizes --motiftypes --impl 3 && ls -alh *.h *.cxx

-rw-r--r--    1 laurent  staff   1.9K Feb 20 16:56 GenDetElemId2SegType.cxx
-rw-r--r--    1 laurent  staff   555B Feb 20 16:56 GenDetElemId2SegType.h
-rw-r--r--    1 laurent  staff    22K Feb 20 16:56 GenSegmentationCreatorForSegType0.cxx
-rw-r--r--    1 laurent  staff    28K Feb 20 16:56 GenSegmentationCreatorForSegType1.cxx
-rw-r--r--    1 laurent  staff    11K Feb 20 16:56 GenSegmentationCreatorForSegType10.cxx
-rw-r--r--    1 laurent  staff    19K Feb 20 16:56 GenSegmentationCreatorForSegType11.cxx
-rw-r--r--    1 laurent  staff   9.5K Feb 20 16:56 GenSegmentationCreatorForSegType12.cxx
-rw-r--r--    1 laurent  staff   9.8K Feb 20 16:56 GenSegmentationCreatorForSegType13.cxx
-rw-r--r--    1 laurent  staff   9.4K Feb 20 16:56 GenSegmentationCreatorForSegType14.cxx
-rw-r--r--    1 laurent  staff   2.9K Feb 20 16:56 GenSegmentationCreatorForSegType15.cxx
-rw-r--r--    1 laurent  staff   2.7K Feb 20 16:56 GenSegmentationCreatorForSegType16.cxx
-rw-r--r--    1 laurent  staff    11K Feb 20 16:56 GenSegmentationCreatorForSegType17.cxx
-rw-r--r--    1 laurent  staff   8.4K Feb 20 16:56 GenSegmentationCreatorForSegType18.cxx
-rw-r--r--    1 laurent  staff   8.0K Feb 20 16:56 GenSegmentationCreatorForSegType19.cxx
-rw-r--r--    1 laurent  staff    11K Feb 20 16:56 GenSegmentationCreatorForSegType2.cxx
-rw-r--r--    1 laurent  staff   3.6K Feb 20 16:56 GenSegmentationCreatorForSegType20.cxx
-rw-r--r--    1 laurent  staff    15K Feb 20 16:56 GenSegmentationCreatorForSegType3.cxx
-rw-r--r--    1 laurent  staff   9.8K Feb 20 16:56 GenSegmentationCreatorForSegType4.cxx
-rw-r--r--    1 laurent  staff   6.1K Feb 20 16:56 GenSegmentationCreatorForSegType5.cxx
-rw-r--r--    1 laurent  staff   5.8K Feb 20 16:56 GenSegmentationCreatorForSegType6.cxx
-rw-r--r--    1 laurent  staff    10K Feb 20 16:56 GenSegmentationCreatorForSegType7.cxx
-rw-r--r--    1 laurent  staff    14K Feb 20 16:56 GenSegmentationCreatorForSegType8.cxx
-rw-r--r--    1 laurent  staff   7.3K Feb 20 16:56 GenSegmentationCreatorForSegType9.cxx

```