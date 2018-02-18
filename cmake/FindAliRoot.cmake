# **************************************************************************
# * Copyright(c) 1998-2015, ALICE Experiment at CERN, All rights reserved. *
# *                                                                        *
# * Author: The ALICE Off-line Project.                                    *
# * Contributors are mentioned in the code where appropriate.              *
# *                                                                        *
# * Permission to use, copy, modify and distribute this software and its   *
# * documentation strictly for non-commercial purposes is hereby granted   *
# * without fee, provided that the above copyright notice appears in all   *
# * copies and that both the copyright notice and this permission notice   *
# * appear in the supporting documentation. The authors make no claims     *
# * about the suitability of this software for any purpose. It is          *
# * provided "as is" without express or implied warranty.                  *
# **************************************************************************

set(AliRoot_FOUND FALSE)

function(depends_on_aliroot target)
    # target depends on all argsn
    get_target_property(a AliRoot::${target} INTERFACE_LINK_LIBRARIES)
    if (a)
        set(tmp ${a})
    endif ()
    foreach (t ${ARGN})
        list(APPEND tmp AliRoot::${t})
    endforeach ()
    set_target_properties(AliRoot::${target} PROPERTIES INTERFACE_LINK_LIBRARIES "${tmp}")
endfunction()

function(depends_on_root target)
    # target depends on all argsn
    get_target_property(a AliRoot::${target} INTERFACE_LINK_LIBRARIES)
    if (a)
        set(tmp ${a})
    endif ()
    foreach (t ${ARGN})
        list(APPEND tmp ROOT::${t})
    endforeach ()
    set_target_properties(AliRoot::${target} PROPERTIES INTERFACE_LINK_LIBRARIES "${tmp}")
endfunction()

if (ALIROOT)

    # Check if AliRoot is really installed there
    if (EXISTS ${ALIROOT}/bin/aliroot AND EXISTS ${ALIROOT}/lib AND EXISTS ${ALIROOT}/include)

        foreach (t MUONmapping MUONgraphics MUONrec MUONbase MUONgeometry MUONtrigger MUONcalib MUONraw MUONcore
                STEER ANALYSIS HLTbase AliHLTHOMER CDB STAT RAWDatarec RAWDatabase ESD STEERBase RAWDatasim)
            if (NOT TARGET AliRoot::${t})
                add_library(AliRoot::${t} SHARED IMPORTED)
                set_target_properties(AliRoot::${t} PROPERTIES
                        INTERFACE_INCLUDE_DIRECTORIES ${ALIROOT}/include
                        IMPORTED_LOCATION ${ALIROOT}/lib/lib${t}.so)
            endif ()
        endforeach ()

        # the following is of course not a very clever way / labor intensive of doing this, but it kind of works...
        # the correct alternative would be the change AliRoot to properly export its targets, but
        # that alone would require some more work...


        depends_on_root(STEERBase Core EG Geom Gpad Graf3d Graf Hist MathCore Matrix Minuit Net Physics RIO Tree TreePlayer VMC)

        depends_on_aliroot(STEER CDB ESD HLTbase RAWDatabase RAWDatarec STEERBase)
        depends_on_root(STEER Core EG GenVector GeomPainter Geom Gpad Graf Hist MathCore Matrix Minuit Net Physics Proof RIO Tree VMC GeomBuilder RHTTP Gui TreePlayer Graf3d XMLParser Cling)

        depends_on_aliroot(CDB STAT STEERBase)
        depends_on_root(CDB Core GenVector Gpad Graf Gui Hist MathCore Matrix Minuit Net RIO Tree XMLParser EG Geom Graf3d Physics VMC TreePlayer GeomBuilder RHTTP)

        depends_on_aliroot(ESD STEERBase)
        depends_on_root(ESD Core EG Geom Gpad Graf3d MathCore Matrix Physics RIO Tree Graf Hist Minuit Net TreePlayer VMC)

        depends_on_aliroot(HLTbase RAWDatabase AliHLTHOMER ESD STEERBase)
        depends_on_root(HLTbase Core Geom Graf Hist MathCore Net RIO Tree XMLParser Cling Minuit TreePlayer VMC EG Gpad Graf3d Matrix Physics)

        depends_on_aliroot(RAWDatabase ESD STEERBase)
        depends_on_root(RAWDatabase Core Hist RIO Graf Minuit Net TreePlayer VMC EG Geom Gpad Graf3d MathCore Matrix Physics Tree)

        depends_on_aliroot(RAWDatarec RAWDatabase ESD STEERBase)
        depends_on_root(RAWDatarec Core Net RIO TreePlayer Tree EG Geom Gpad Graf3d Graf MathCore Matrix Minuit Physics VMC Hist)

        depends_on_aliroot(RAWDatasim RAWDatasim STEER RAWDatarec CDB STAT ANALYSIS HLTbase RAWDatabase ESD STEERBase AliHLTHOMER)
        depends_on_root(RAWDatasim GeomBuilder RHTTP Gui EG Graf3d TreePlayer XMLParser Core Cling GenVector GeomPainter Hist Geom Gpad Graf MathCore Matrix Minuit Net Physics Proof RIO Tree VMC)

        depends_on_aliroot(MUONmapping CDB MUONcore STEERBase)
        depends_on_root(MUONmapping Core Physics RIO GeomBuilder RHTTP GenVector Gui XMLParser EG Geom Gpad Graf3d Graf Hist MathCore Matrix Minuit Net TreePlayer VMC Tree)

        depends_on_aliroot(MUONcore STEERBase)
        depends_on_root(MUONcore Core Tree EG Geom Gpad Graf3d Graf Hist MathCore Matrix Minuit Net Physics RIO TreePlayer VMC)

        depends_on_aliroot(MUONraw RAWDatarec MUONcore RAWDatabase ESD STEERBase)
        depends_on_root(MUONraw Core EG Geom Gpad Graf3d Graf Hist MathCore Matrix Minuit Net Physics RIO TreePlayer VMC Tree)

        depends_on_aliroot(MUONgraphics MUONmapping MUONrec MUONbase RAWDatasim MUONgeometry STEER ANALYSIS HLTbase AliHLTHOMER MUONtrigger MUONcalib MUONmapping CDB STAT MUONraw RAWDatarec RAWDatabase ESD MUONcore STEERBase)
        depends_on_root(MUONgraphics Rint Cling GeomPainter Proof GenVector GeomBuilder RHTTP Gui XMLParser EG Geom Gpad Graf3d Graf Hist MathCore Matrix Minuit Net Physics RIO TreePlayer VMC Tree Core)

        set(AliRoot_FOUND TRUE)
        message(STATUS "AliRoot ... - found ${ALIROOT} - and target defined")

    else ()

        message(STATUS "AliRoot ... - not found")

    endif ()
endif (ALIROOT)

if (NOT AliRoot_FOUND)
    if (AliRoot_FIND_REQUIRED)
        message(FATAL_ERROR "Please point to the AliRoot Core installation using -DALIROOT=<ALIROOT_CORE_INSTALL_DIR> ")
    endif (AliRoot_FIND_REQUIRED)
endif (NOT AliRoot_FOUND)
