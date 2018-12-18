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
 A reference copy of those files are in the [data](../../o2/mch/mapping/data) directory. Note that there are also a number of generated test files (`test_*.json`) that can be used to validate a  new implementation (this is for instance what has been done with the Go implementation).
  
```
> mch-mapping-convert-to-json --all && ls -alh

total 414400
drwxr-xr-x  35 laurent  staff   1.1K Dec 17 22:51 .
drwxr-xr-x+ 73 laurent  staff   2.3K Dec 17 22:52 ..
-rw-r--r--   1 laurent  staff    18K Dec 17 22:51 bergs.json
-rw-r--r--   1 laurent  staff   695K Dec 17 22:51 buspatches.json
-rw-r--r--   1 laurent  staff   748K Dec 17 22:51 catsegs.json
-rw-r--r--   1 laurent  staff   4.0K Dec 17 22:51 chambers.json
-rw-r--r--   1 laurent  staff   4.8K Dec 17 22:51 ddls.json
-rw-r--r--   1 laurent  staff    63K Dec 17 22:51 detection_elements.json
-rw-r--r--   1 laurent  staff   231K Dec 17 22:51 legacyseg.json
-rw-r--r--   1 laurent  staff   1.6M Dec 17 22:51 motiftypes.json
-rw-r--r--   1 laurent  staff   2.0K Dec 17 22:51 padsizes.json
-rw-r--r--   1 laurent  staff    37K Dec 17 22:51 pcbs.json
-rw-r--r--   1 laurent  staff   3.3M Dec 17 22:51 test_channel_list.json
-rw-r--r--   1 laurent  staff    40M Dec 17 22:51 test_neighbours_list_100.json
-rw-r--r--   1 laurent  staff    39M Dec 17 22:51 test_neighbours_list_300.json
-rw-r--r--   1 laurent  staff   6.7M Dec 17 22:51 test_neighbours_list_500.json
-rw-r--r--   1 laurent  staff   8.2M Dec 17 22:51 test_neighbours_list_501.json
-rw-r--r--   1 laurent  staff   7.1M Dec 17 22:51 test_neighbours_list_502.json
-rw-r--r--   1 laurent  staff   4.3M Dec 17 22:51 test_neighbours_list_503.json
-rw-r--r--   1 laurent  staff   2.9M Dec 17 22:51 test_neighbours_list_504.json
-rw-r--r--   1 laurent  staff   6.9M Dec 17 22:51 test_neighbours_list_600.json
-rw-r--r--   1 laurent  staff   8.4M Dec 17 22:51 test_neighbours_list_601.json
-rw-r--r--   1 laurent  staff   7.3M Dec 17 22:51 test_neighbours_list_602.json
-rw-r--r--   1 laurent  staff   7.3M Dec 17 22:51 test_neighbours_list_700.json
-rw-r--r--   1 laurent  staff   9.4M Dec 17 22:51 test_neighbours_list_701.json
-rw-r--r--   1 laurent  staff   9.6M Dec 17 22:51 test_neighbours_list_702.json
-rw-r--r--   1 laurent  staff   5.8M Dec 17 22:51 test_neighbours_list_703.json
-rw-r--r--   1 laurent  staff   4.3M Dec 17 22:51 test_neighbours_list_704.json
-rw-r--r--   1 laurent  staff   2.1M Dec 17 22:51 test_neighbours_list_705.json
-rw-r--r--   1 laurent  staff   1.3M Dec 17 22:51 test_neighbours_list_706.json
-rw-r--r--   1 laurent  staff    10M Dec 17 22:51 test_neighbours_list_902.json
-rw-r--r--   1 laurent  staff   6.5M Dec 17 22:51 test_neighbours_list_903.json
-rw-r--r--   1 laurent  staff   5.0M Dec 17 22:51 test_neighbours_list_904.json
-rw-r--r--   1 laurent  staff   2.8M Dec 17 22:51 test_neighbours_list_905.json
-rw-r--r--   1 laurent  staff   316K Dec 17 22:51 test_random_pos.json
```
See [o2/mch/mapping dir](../../o2/mch/mapping/codegen) for the second step.

## Note on test files (dev notes)

Some test files are quite big `json` files. The writer (template) class used is `OF` by default, given a pretty (but large) `json` output. A compact form can be obtained instead using the `OFC` writer (not connected to an option yet, so you have to change the source code for that change). 

Alternatively, let `mch-mapping-convert-to-json` output pretty `json` and use a `json` manipulator, e.g. [jq](https://stedolan.github.io/jq/) to convert it to a compact form (or reverse).

For instance for the neighbours (of only one detection element, DE=100) :

```
> jq -Mc . test_neighbours_list.json > toto.json
> ls -alh *.json
-rw-r--r--  1 laurent  staff    40M Nov 30 15:44 /Users/laurent/tmp2/test_neighbours_list.json
-rw-r--r--  1 laurent  staff   5.5M Nov 30 15:46 /Users/laurent/tmp2/toto.json
```

And then they can of course be (g)zipped for even further reduction : 

```
-rw-r--r--  1 laurent  staff   811K Nov 30 15:44 /Users/laurent/tmp2/test_neighbours_list.json.gz
-rw-r--r--  1 laurent  staff   479K Nov 30 15:46 /Users/laurent/tmp2/toto.json.gz
```

So the smaller compressed files can be put e.g. into `github` and then decompressed locally when needed.

