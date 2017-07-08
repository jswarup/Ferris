
set( cove_source_files
	barn/cv_cexpr.h
	barn/cv_include.cpp
	barn/cv_include.h
	barn/cv_minion.h
	silo/cv_crate.h
	silo/cv_cstr.h
	silo/cv_dlinklist.h
	silo/cv_arena.cpp
	silo/cv_iota.cpp
	silo/cv_typebouqet.cpp
	silo/cv_typecask.cpp
	silo/cv_typeusher.cpp
	flux/cv_flume.cpp
	flux/cv_flume.h
	flux/cv_spritz.cpp
	flux/cv_spritz.h
	flux/cv_xdoclistener.h
	flux/cv_xmlencoder.cpp
	flux/cv_xmlencoder.h
)

add_library(cove ${cove_source_files})
