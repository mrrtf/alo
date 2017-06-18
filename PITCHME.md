# alo 

---

## what is it ? 

<i style="font-size: 2em" class="fa fa-long-arrow-down" aria-hidden="true"></i>

+++

a **small** package

+++

fated to **disappear**

+++

replacing `feature-muon-hlt` <i class="fa fa-leaf" aria-hidden="true"></i>

<!-- ![](http://res.cloudinary.com/apeinesec/image/upload/v1494091795/1494109701_git-branch_k9p3ui.svg) -->

+++

- avoids including AliRoot parts into O2
- keeps dependencies clean

---

## Contains

In principle all the past works from Philippe, Diego and Hugo wrt dHLT components.

Please check.

+++

## Mapping conversion tool 

### From custom TXT format to JSON

---

## Usage

```bash
aliBuild -w ../sw -z --defaults alo build alo
```


Will install `alo` but also `AliRoot` and `O2` as dependencies.


