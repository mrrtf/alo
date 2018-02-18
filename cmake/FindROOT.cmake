# search ROOTConfig.cmake
find_package(ROOT CONFIG)

if (ROOT_FOUND)

    # Add missing include dirs TODO this should be done by upstream
    foreach (t ROOT::vectorDict ROOT::listDict ROOT::forward_listDict ROOT::dequeDict ROOT::mapDict ROOT::map2Dict ROOT::unordered_mapDict ROOT::multimapDict ROOT::multimap2Dict ROOT::unordered_multimapDict ROOT::setDict ROOT::unordered_setDict ROOT::multisetDict ROOT::unordered_multisetDict ROOT::complexDict ROOT::valarrayDict ROOT::Cling ROOT::MultiProc ROOT::Rint ROOT::Thread ROOT::Imt ROOT::New ROOT::Core ROOT::rmkdepend ROOT::MathCore ROOT::MathMore ROOT::Matrix ROOT::Minuit ROOT::Minuit2 ROOT::Fumili ROOT::Physics ROOT::MLP ROOT::Quadp ROOT::Foam ROOT::Smatrix ROOT::SPlot ROOT::GenVector ROOT::Genetic ROOT::Hist ROOT::HistPainter ROOT::Spectrum ROOT::SpectrumPainter ROOT::Unfold ROOT::Hbook ROOT::Tree ROOT::TreePlayer ROOT::TreeViewer ROOT::RIO ROOT::SQLIO ROOT::XMLIO ROOT::XMLParser ROOT::Net ROOT::RootAuth ROOT::Krb5Auth ROOT::SrvAuth ROOT::rootd ROOT::Netx ROOT::NetxNG ROOT::RHTTP ROOT::Gpad ROOT::Graf ROOT::Postscript ROOT::mathtext ROOT::GX11 ROOT::GX11TTF ROOT::ASImage ROOT::ASImageGui ROOT::Graf3d ROOT::X3d ROOT::Eve ROOT::RGL ROOT::GLEW ROOT::FTGL ROOT::Gviz3d ROOT::Gui ROOT::Ged ROOT::FitPanel ROOT::GuiBld ROOT::GuiHtml ROOT::Recorder ROOT::SessionViewer ROOT::Proof ROOT::ProofPlayer ROOT::ProofDraw ROOT::ProofBench ROOT::proofd ROOT::XrdProofd ROOT::proofexecv ROOT::Proofx ROOT::pq2 ROOT::Html ROOT::EG ROOT::VMC ROOT::EGPythia6 ROOT::EGPythia8 ROOT::Geom ROOT::GeomBuilder ROOT::GeomPainter ROOT::Gdml ROOT::root ROOT::minicern ROOT::MemStat ROOT::rootn.exe ROOT::roots.exe ROOT::ssh2rpd ROOT::xpdtest ROOT::root.exe ROOT::proofserv.exe ROOT::hadd ROOT::rootnb.exe ROOT::g2root ROOT::h2root ROOT::rootcling ROOT::PyROOT ROOT::JupyROOT ROOT::PgSQL ROOT::RSQLite ROOT::TMVA ROOT::TMVAGui ROOT::RooFitCore ROOT::RooFit ROOT::RooStats ROOT::HistFactory ROOT::hist2workspace)
        if (TARGET ${t})
            get_target_property(incdirs ${t} INTERFACE_INCLUDE_DIRECTORIES)
            if (NOT incdirs)
                set_target_properties(${t} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${ROOT_INCLUDE_DIRS})
            endif ()
        endif ()
    endforeach ()
else ()
    message(WARNING "ROOT was not found...")
endif ()

