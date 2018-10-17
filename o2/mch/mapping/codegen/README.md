
The json files are initially generated using [aliroot-dependent code](../../../aliroot/jsonmap)

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
