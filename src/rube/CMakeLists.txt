
set( rube_source_files
	barn/ru_include.cpp
	barn/ru_include.h
	blocks/ru_gates.h
	engine/ru_rubereactor.cpp
	engine/ru_rubereactor.h
	engine/ru_rubesite.h
	engine/ru_tuple.h
)

add_library(rube ${rube_source_files})
