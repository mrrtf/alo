# Mapping Code Generator

The MCH mapping for O2 is a mix of "regular" code (written by hand) and computer generated code.
 
This package handles the code generation part.
 The input for this part is a set of JSON mapping files (see `jsonmap/creator`).
 
 All generated files start with the `gen` prefix. 

```
> cd dir_with_json_mapping_files
> mchMappingCodeGenerator segmentations.json motiftypes.json padsizes.json bergs.json detection_elements.json --segmentations --motiftypes --padsizes --bergs --detection_elements
```

Before moving them to their final destination, please 
 remember to apply the correct style, e.g. :
  
```
> clang-format -style=file genSegmentationCreatorForSegType*.cxx -i
> cp genSegmentationCreatorForSegType*.cxx $HOME/alice/o2-dev/alo/mapping/impl2
```