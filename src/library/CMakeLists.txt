cmake_minimum_required(VERSION 3.5.0)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#Add any source files that will create a single library
set(jmatrix_LIB
    debugpar.cpp
    jmatrix.cpp
    jmatrixaux.cpp
    fullmatrix.cpp
    sparsematrix.cpp
    symmetricmatrix.cpp
    matgetcols.cpp
    matgetrows.cpp
    matgetdiag.cpp
    matinfo.cpp
    matmetadata.cpp
    matreadwritecsv.cpp
    memhelper.cpp
)

if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
	execute_process(
		COMMAND git rev-parse HEAD
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		OUTPUT_VARIABLE GIT_BRANCH
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
else()
	set(GIT_BRANCH "Unknown")
endif()
add_definitions(-DGIT_BRANCH=${GIT_BRANCH})
#set build dir for your library
set(LIBRARY_OUTPUT_PATH ${CMAKE_BUILD_DIR}/lib)
message( STATUS "LIBRARY_OUTPUT_PATH: " ${LIBRARY_OUTPUT_PATH} )

set(LIBRARY_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/lib)
message( STATUS "LIBRARY_INSTALL_PATH: " ${LIBRARY_INSTALL_PATH} )

file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH})

# Tell CMake to create the jmatrixlib shared lib and set it's properties
add_library(jmatrix SHARED ${jmatrix_LIB})
set_target_properties(jmatrix
    PROPERTIES
    VERSION 1.0
    SOVERSION 0
    )

#If your app, links to an external lib -ie Boost
#target_link_libraries( jmatrixlib ${Boost_LIBRARIES} )

# Install
install(TARGETS jmatrix
        LIBRARY DESTINATION lib
        )


