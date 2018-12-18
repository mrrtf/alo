
The json files are initially generated using [aliroot-dependent code](../../../aliroot/jsonmap)

```
> mch-mapping-generate-code detection_elements.json catsegs.json bergs.json padsizes.json motiftypes.json --detection_elements --catsegs --bergs --padsizes --motiftypes --impl 3 && ls -alh *.h *.cxx

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
The generated files must be passed through `clang-format` for good measure before they can be included the `Mapping/Impl#` package.

```
find . -name "*.h" -exec clang-format -i {} \;   
```

The Go implementation is generated using :

```
> mch-mapping-generate-code detection_elements.json catsegs.json bergs.json padsizes.json motiftypes.json --detection_elements --catsegs --bergs --padsizes --motiftypes --go && ls -al *.go

-rw-r--r--  1 laurent  staff  30463 Dec  8 16:59 createSegType0.go
-rw-r--r--  1 laurent  staff  39066 Dec  8 16:59 createSegType1.go
-rw-r--r--  1 laurent  staff  13509 Dec  8 16:59 createSegType10.go
-rw-r--r--  1 laurent  staff  23461 Dec  8 16:59 createSegType11.go
-rw-r--r--  1 laurent  staff  12578 Dec  8 16:59 createSegType12.go
-rw-r--r--  1 laurent  staff  12299 Dec  8 16:59 createSegType13.go
-rw-r--r--  1 laurent  staff  11673 Dec  8 16:59 createSegType14.go
-rw-r--r--  1 laurent  staff   3688 Dec  8 16:59 createSegType15.go
-rw-r--r--  1 laurent  staff   3353 Dec  8 16:59 createSegType16.go
-rw-r--r--  1 laurent  staff  14812 Dec  8 16:59 createSegType17.go
-rw-r--r--  1 laurent  staff  10725 Dec  8 16:59 createSegType18.go
-rw-r--r--  1 laurent  staff  10107 Dec  8 16:59 createSegType19.go
-rw-r--r--  1 laurent  staff  14009 Dec  8 16:59 createSegType2.go
-rw-r--r--  1 laurent  staff   4638 Dec  8 16:59 createSegType20.go
-rw-r--r--  1 laurent  staff  18572 Dec  8 16:59 createSegType3.go
-rw-r--r--  1 laurent  staff  12485 Dec  8 16:59 createSegType4.go
-rw-r--r--  1 laurent  staff   7669 Dec  8 16:59 createSegType5.go
-rw-r--r--  1 laurent  staff   7068 Dec  8 16:59 createSegType6.go
-rw-r--r--  1 laurent  staff  12877 Dec  8 16:59 createSegType7.go
-rw-r--r--  1 laurent  staff  17447 Dec  8 16:59 createSegType8.go
-rw-r--r--  1 laurent  staff   9496 Dec  8 16:59 createSegType9.go
-rw-r--r--  1 laurent  staff   2930 Dec  8 16:59 detelemid2segtype.go
 
```

The generated Go files must be passed through `gofmt` for good measure before they can be included the `mapping/impl#` package.

```
find . -name '*.go' -exec gofmt -w {} \;

```
