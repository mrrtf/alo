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
                STEER ANALYSIS HLTbase AliHLTHOMER CDB STAT RAWDatarec RAWDatabase ESD STEERBase RAWDatasim
                AliHLTMUON MUONevaluation MUONsim)
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

        # we specify the first level dependencies of each library

        depends_on_root(STEERBase Core EG Geom Gpad Graf3d Graf Hist MathCore Matrix Minuit Net Physics RIO Tree TreePlayer VMC)

        depends_on_aliroot(STEER CDB ANALYSIS RAWDatarec HLTbase)
        depends_on_root(STEER GeomPainter Proof)

        depends_on_aliroot(CDB STAT)
        depends_on_root(CDB GenVector XMLParser)

        depends_on_aliroot(STAT STEERBase)
        depends_on_root(STAT Gui GeomBuilder RHTTP)

        depends_on_aliroot(ANALYSIS STEERBase)
        depends_on_root(ANALYSIS XMLParser)

        depends_on_aliroot(ESD STEERBase)

        depends_on_aliroot(RAWDatarec RAWDatabase)

        depends_on_aliroot(RAWDatabase ESD)

        depends_on_aliroot(HLTbase RAWDatabase AliHLTHOMER)
        depends_on_root(HLTbase Cling XMLParser)

        depends_on_root(AliHLTHOMER Core)

        depends_on_aliroot(RAWDatasim STEER)

        depends_on_aliroot(MUONmapping CDB MUONcore)

        depends_on_aliroot(MUONcore STEERBase)

        depends_on_aliroot(MUONraw RAWDatarec MUONcore)

        depends_on_aliroot(MUONgraphics MUONrec)
        depends_on_root(MUONgraphics Rint)

        depends_on_aliroot(MUONrec MUONbase)

        depends_on_aliroot(MUONbase RAWDatasim MUONgeometry MUONtrigger)

        depends_on_aliroot(AliHLTMUON MUONevaluation)

        depends_on_aliroot(MUONevaluation MUONrec MUONsim)

        depends_on_aliroot(MUONsim MUONbase)

        depends_on_aliroot(MUONgeometry STEER MUONmapping)

        depends_on_aliroot(MUONtrigger MUONraw MUONcalib)

        depends_on_aliroot(MUONcalib MUONmapping)

        set(AliRoot_FOUND TRUE)
        message(STATUS "AliRoot ... - found ${ALIROOT} - and target dependencies defined")

    else ()

        message(STATUS "AliRoot ... - not found")

    endif ()
endif (ALIROOT)

if (NOT AliRoot_FOUND)
    if (AliRoot_FIND_REQUIRED)
        message(FATAL_ERROR "Please point to the AliRoot Core installation using -DALIROOT=<ALIROOT_CORE_INSTALL_DIR> ")
    endif (AliRoot_FIND_REQUIRED)
endif (NOT AliRoot_FOUND)
