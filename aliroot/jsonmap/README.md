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

This directory is the "creator" part, ie. the classes that actually convert the Aliroot mapping objects into JSON files.

The other part, the "codegen" one, is available in the [o2/mch/mapping dir](../../o2/mch/mapping/codegen).

There are currently 3 different implementations in the works. Only impl3 is feature complete. impl2 
 is still buggy and impl1 will be removed simply (was kind of a template experiment that went too far ;-) 
 
## Usage 

It's a two steps process : first generate `JSON` files from `AliRoot` mapping, then
 generate code from the `JSON` files. Note that not all `JSON` files are required for all implementations.
 A reference copy of those files are in the [data](data) directory. 
  
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
See [o2/mch/mapping dir](../../o2/mch/mapping/codegen) for the second step.

