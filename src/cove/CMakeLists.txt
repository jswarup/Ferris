
set( cove_source_files
	barn/cv_cexpr.h
	barn/cv_include.cpp
	barn/cv_include.h
	barn/cv_lackey.h
	silo/cv_crate.h
	silo/cv_cstr.h
	silo/cv_dlinklist.h
)

add_library(cove ${cove_source_files})
 