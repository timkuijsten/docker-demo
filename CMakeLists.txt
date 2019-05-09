
cmake_minimum_required (VERSION 3.1 FATAL_ERROR)
project ("docker-demo" C)

macro(builddir DIR)
	file(STRINGS "${DIR}/Dockerfile" LINES)
	set(DEPS "")
	set(EXTDEPS "")
	foreach(LINE IN LISTS LINES)
		# string(REGEX REPLACE "^FROM ([^ ])+ .*$" "\\1" SRC "${LINE}")
		string(REGEX MATCH "^FROM ([^ ])+" SRC "${LINE}")
		if (NOT ("${SRC}" STREQUAL ""))
			string(REGEX REPLACE "^FROM ([^ ]+).*$" "\\1" SRC1 "${LINE}")
			#DEBUG# message("SRC1 = ${SRC1}")
			set(DOT ${DOT} "   \"${SRC1}\" -> \"arpa2/${DIR}\"\n")
			string(REGEX REPLACE "^([^/: ]+)[/:]([^/: ]+)$" "\\1-\\2" SRC2 "${SRC1}")
			#DEBUG# message("SRC2 = ${SRC2}")
			set(DEPS ${DEPS} ${SRC2})
			string (REGEX REPLACE "^([^/: ]+)[/:].*$" "\\1" SRC3 "${SRC1}")
			#DEBUG# message("SRC3 = ${SRC3}")
			if(NOT ("${SRC3}" STREQUAL "arpa2"))
				set(EXTDEPS ${EXTDEPS} ${SRC1})
			endif()
		endif()
	endforeach()
	#DEBUG# message("${DIR} depends on ${DEPS}")
	add_custom_target(${DIR}
		COMMAND docker build -t "${DIR}" "${CMAKE_SOURCE_DIR}/${DIR}"
		DEPENDS ${DEPS})
	#DEBUG# message("arpa2-${DIR} depends on ${DIR}")
	add_custom_target("arpa2-${DIR}"
		COMMAND docker tag "${DIR}" "arpa2/${DIR}"
		DEPENDS ${DIR})
	foreach(EXTDEP IN LISTS EXTDEPS)
		string (REGEX REPLACE "^([^/: ]+)[/:]([^/: ]+).*$" "\\1-\\2" SRC4 "${EXTDEP}")
		#DEBUG# message("External dependency: ${EXTDEP} added as ${SRC4}")
		add_custom_target(${SRC4}
			COMMAND docker pull ${EXTDEP})
	endforeach()
endmacro(builddir)


#FAILS ON SLASHES IN TARGETS# file(GLOB_RECURSE DOCKERFILES RELATIVE "${CMAKE_SOURCE_DIR}" "Dockerfile")
file(GLOB DOCKERFILES RELATIVE "${CMAKE_SOURCE_DIR}" "*/Dockerfile")
#DEBUG# message ("DOCKERFILES are ${DOCKERFILES}")

set(DOT "")

foreach(DOCKERFILE IN LISTS DOCKERFILES)
	#DEBUG# message("DOCKERFILE is ${DOCKERFILE}")
	string(REGEX REPLACE "^(.*)/Dockerfile$" "\\1" DEMODIR "${DOCKERFILE}")
	#DEBUG# message("DEMODIR is ${DEMODIR}")
	builddir(${DEMODIR})
endforeach()

set(DOT "digraph arpa2demo{\n" ${DOT} "}\n")

file(WRITE "${CMAKE_SOURCE_DIR}/deps.dot" ${DOT})

execute_process(COMMAND dot -Tpng ${CMAKE_SOURCE_DIR}/deps.dot OUTPUT_FILE ${CMAKE_SOURCE_DIR}/deps.tmp ERROR_QUIET)
file(READ "${CMAKE_SOURCE_DIR}/deps.tmp" DOTTED LIMIT 10)
if("${DOTTED}" STREQUAL "")
	message(WARNING "FAILED at redrawing dependencies: You do not have \"dot\" installed!")
else()
	file(RENAME ${CMAKE_SOURCE_DIR}/deps.tmp ${CMAKE_SOURCE_DIR}/deps.png)
endif()

