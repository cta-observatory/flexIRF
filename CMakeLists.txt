cmake_minimum_required(VERSION 2.8)
project(IRM)

## Setup CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMakeModules/")

## Include FITS libraries                                                                                                                                                        
find_package(CFITSIO REQUIRED)
include_directories(${CFITSIO_INCLUDE_DIRS})
set( EXTRA_LIBS ${EXTRA_LIBS} ${CFITSIO_LIBRARIES} )

## ROOT 

## Include ROOT libraries   
find_package(ROOT)
IF(ROOT_FOUND)
	include_directories(${ROOT_INCLUDE_DIR})
	set( ROOT_LIBS ${ROOT_LIBS} ${ROOT_LIBRARIES} )
	set( rootinc ${PROJECT_SOURCE_DIR}/include )
	include_directories(${rootinc})
ENDIF()                                                                                                                                          


## Add flexIRF code
set( srcForfIRF ${PROJECT_SOURCE_DIR}/flexirf/src )
include_directories(${srcForfIRF})

set( incForfIRF ${PROJECT_SOURCE_DIR}/flexirf/include )
include_directories(${incForfIRF})

## Specify source code
set( srcsIRFm ${PROJECT_SOURCE_DIR}/src/IRFmerger_fitsio.cc )

## Building library                                                                                                                  
add_subdirectory (flexirf) 
add_executable( IRFmerger_fitsio ${srcsIRFm} )

## Link everything
target_link_libraries( IRFmerger_fitsio ${EXTRA_LIBS} flexirf ${ROOT_LIBS} )

##Other tools
set( srcsIRFconv ${PROJECT_SOURCE_DIR}/src/IRFconverter_root2FITS.cc )
add_executable( IRFconverter_root2FITS ${srcsIRFconv} )
target_link_libraries( IRFconverter_root2FITS flexirf ${EXTRA_LIBS} ${ROOT_LIBS} )

set( srcsIRFg ${PROJECT_SOURCE_DIR}/src/IRFgenerator_rootio.cc )
add_executable( IRFgenerator_rootio ${srcsIRFg} )
target_link_libraries( IRFgenerator_rootio flexirf ${EXTRA_LIBS} ${ROOT_LIBS} )

set( srcsIRFsimpleWriteIrf ${PROJECT_SOURCE_DIR}/src/IRFsimpleWriteIrf.cc )
add_executable( IRFsimpleWriteIrf ${srcsIRFsimpleWriteIrf} )
target_link_libraries( IRFsimpleWriteIrf flexirf ${EXTRA_LIBS} ${ROOT_LIBS} )

set( srcsIRFsimpleWriteParamIrf ${PROJECT_SOURCE_DIR}/src/IRFsimpleWriteParamIrf.cc )
add_executable( IRFsimpleWriteParamIrf ${srcsIRFsimpleWriteParamIrf} )
target_link_libraries( IRFsimpleWriteParamIrf flexirf ${EXTRA_LIBS} ${ROOT_LIBS} )

set( srcsIRFsimpleReadIrf ${PROJECT_SOURCE_DIR}/src/IRFsimpleReadIrf.cc )
add_executable( IRFsimpleReadIrf ${srcsIRFsimpleReadIrf} )
target_link_libraries( IRFsimpleReadIrf flexirf ${EXTRA_LIBS} ${ROOT_LIBS} )

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
install(TARGETS IRFconverter_root2FITS IRFgenerator_rootio IRFsimpleWriteIrf IRFsimpleWriteParamIrf IRFsimpleReadIrf DESTINATION bin)
