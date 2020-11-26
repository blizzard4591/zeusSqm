cmake_minimum_required (VERSION 3.13)

# Set project name
project (ZeusSqm CXX C)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake/")
include(CheckIncludeFiles)
include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(CheckLibraryExists)
include(CheckCSourceCompiles)
include(CheckCSourceRuns)
include(CMakeDependentOption)
include(CheckCXXSymbolExists)
include(CheckTypeSize)
include(FindThreads)
include(GNUInstallDirs)
include(CheckTypeSize)
include(CMakePackageConfigHelpers)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

option(ZEUSSQM_USE_NSIS "Use NSIS generator to produce a Windows installer." OFF)

SET(ZEUSSQM_CMAKE_SEARCH_PATH "C:/Qt/5.15.1/msvc2019_64" CACHE PATH "Additional Qt5 search path" )

SET(CMAKE_PREFIX_PATH "${ZEUSSQM_CMAKE_SEARCH_PATH}/lib/cmake/")
SET(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake/")

set(ZEUSSQM_CUSTOM_VERSION_STRING "" CACHE STRING "Disables Git version number checking and uses the custom string instead. Should be of the form 0.1.2-34-567890ab, i.e. major.minor.patch-commitsSinceTag-shortHash")
SET(ZEUSSQM_ADDITIONAL_INSTALL_FILES "" CACHE STRING "A semicolon separated list of files that will be copied to the bin directory on install.")

if(MINGW OR WIN32)
	set(CLI_INSTALL_DIR    ".")
	set(BIN_INSTALL_DIR    ".")
	set(PLUGIN_INSTALL_DIR ".")
	set(DATA_INSTALL_DIR   "share")
endif()

# Find the Qt library
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

if(CMAKE_COMPILER_IS_GNUCC)
	# Set standard flags for GCC
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -funroll-loops")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pedantic")
	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
elseif(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /MP")

	add_definitions(/DNOMINMAX)
	add_definitions(/MP)
	add_definitions(/std:c++latest)
	add_definitions(/D_WINSOCK_DEPRECATED_NO_WARNINGS)
	
	add_definitions(-DUNICODE)
	add_definitions(-D_UNICODE)
endif()

include_directories("${PROJECT_SOURCE_DIR}/srcShared")

# For version.h
include_directories("${PROJECT_SOURCE_DIR}")

#
# Make a version file containing the current version from git.
#

# First check if this is a Git checkout or if a custom version tag should be used.
if ((NOT "${ZEUSSQM_CUSTOM_VERSION_STRING}" STREQUAL "") AND (NOT "${ZEUSSQM_CUSTOM_VERSION_STRING}" MATCHES "^[0-9]+\\.[0-9]+\\.[0-9]+\\-[0-9]+\\-[a-z0-9]+(\\-.*)?$"))
	message(FATAL_ERROR "Builtin version information: A custom version string is set, but it is malformed. Should be: 0.1.2-34-567890ab, i.e. major.minor.patch-commitsSinceTag-shortHash")
endif()

set(ZEUSSQM_GIT_VERSION_STRING "")
if (NOT "${ZEUSSQM_CUSTOM_VERSION_STRING}" STREQUAL "")
	message(STATUS "Builtin version information: custom version string set, using ${ZEUSSQM_CUSTOM_VERSION_STRING}")
	set(ZEUSSQM_GIT_VERSION_STRING "${ZEUSSQM_CUSTOM_VERSION_STRING}")
elseif (NOT EXISTS "${PROJECT_SOURCE_DIR}/.git")
	message(STATUS "Builtin version information: .git folder non-existant, falling back to 0.0.0-0-00000000")
	set(ZEUSSQM_GIT_VERSION_STRING "0.0.0-0-00000000")
else()
	message(STATUS "Builtin version information: .git folder exists, using git_describe_checkout")
	include(GetGitRevisionDescription)
	git_describe_checkout(ZEUSSQM_GIT_VERSION_STRING)
endif()

if ("${ZEUSSQM_GIT_VERSION_STRING}" MATCHES "^[0-9]+\\.[0-9]+\\.[0-9]+\\-[0-9]+\\-[a-z0-9]+(\\-.*)?$")
	# Parse the git Tag into variables
	string(REGEX REPLACE "^([0-9]+)\\..*" "\\1" ZEUSSQM_CPP_VERSION_MAJOR "${ZEUSSQM_GIT_VERSION_STRING}")
	string(REGEX REPLACE "^[0-9]+\\.([0-9]+).*" "\\1" ZEUSSQM_CPP_VERSION_MINOR "${ZEUSSQM_GIT_VERSION_STRING}")
	string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" ZEUSSQM_CPP_VERSION_PATCH "${ZEUSSQM_GIT_VERSION_STRING}")
	string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.[0-9]+\\-([0-9]+)\\-.*" "\\1" ZEUSSQM_CPP_VERSION_COMMITS_AHEAD "${ZEUSSQM_GIT_VERSION_STRING}")
	string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.[0-9]+\\-[0-9]+\\-([a-z0-9]+).*" "\\1" ZEUSSQM_CPP_VERSION_HASH "${ZEUSSQM_GIT_VERSION_STRING}")
	string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.[0-9]+\\-[0-9]+\\-[a-z0-9]+\\-(.*)" "\\1" ZEUSSQM_CPP_VERSION_APPENDIX "${ZEUSSQM_GIT_VERSION_STRING}")
	if ("${ZEUSSQM_CPP_VERSION_APPENDIX}" MATCHES "^.*dirty.*$")
		set(ZEUSSQM_CPP_VERSION_DIRTY 1)
	else()
		set(ZEUSSQM_CPP_VERSION_DIRTY 0)
	endif()
message(STATUS "ZeusSqm - Version information: ${ZEUSSQM_CPP_VERSION_MAJOR}.${ZEUSSQM_CPP_VERSION_MINOR}.${ZEUSSQM_CPP_VERSION_PATCH} (${ZEUSSQM_CPP_VERSION_COMMITS_AHEAD} commits ahead of Tag) build from ${ZEUSSQM_CPP_VERSION_HASH} (Dirty: ${ZEUSSQM_CPP_VERSION_DIRTY})")
else()
	message(STATUS "No GIT support, no tags fetched or other problems. Defaulting to version 0.0.0-0-00000000 (result was ${ZEUSSQM_GIT_VERSION_STRING})")
	set(ZEUSSQM_CPP_VERSION_MAJOR 0)
	set(ZEUSSQM_CPP_VERSION_MINOR 0)
	set(ZEUSSQM_CPP_VERSION_PATCH 0)
	set(ZEUSSQM_CPP_VERSION_COMMITS_AHEAD 0)
	set(ZEUSSQM_CPP_VERSION_HASH "00000000")
	set(ZEUSSQM_CPP_VERSION_DIRTY 0)
endif()

# Generate a version string
set(ZEUSSQM_VERSION_NUM "${ZEUSSQM_CPP_VERSION_MAJOR}.${ZEUSSQM_CPP_VERSION_MINOR}.${ZEUSSQM_CPP_VERSION_PATCH}")
set(ZEUSSQM_VERSION "${ZEUSSQM_CPP_VERSION_MAJOR}.${ZEUSSQM_CPP_VERSION_MINOR}.${ZEUSSQM_CPP_VERSION_PATCH}")
if (ZEUSSQM_CPP_VERSION_COMMITS_AHEAD GREATER 0)
	set(ZEUSSQM_VERSION "${ZEUSSQM_VERSION}+${ZEUSSQM_CPP_VERSION_COMMITS_AHEAD}")
endif()
set(ZEUSSQM_VERSION "${ZEUSSQM_VERSION}-${ZEUSSQM_CPP_VERSION_HASH}")

# Check for %zu support
try_run(ZEUSSQM_POINTER_SIZE_TEST_RUN_RESULT ZEUSSQM_POINTER_SIZE_TEST_COMPILE_RESULT "${PROJECT_BINARY_DIR}/testPointerSize" "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_ptr_size.cpp" COMPILE_OUTPUT_VARIABLE ZEUSSQM_POINTER_SIZE_TEST_COMPILE_OUTPUT RUN_OUTPUT_VARIABLE ZEUSSQM_POINTER_SIZE_TEST_RUN_OUTPUT)
if (NOT ${ZEUSSQM_POINTER_SIZE_TEST_COMPILE_RESULT})
	message(FATAL_ERROR "Failed to compile pointer size testing program, please contact a developer! Extended Information: ${ZEUSSQM_POINTER_SIZE_TEST_COMPILE_OUTPUT}") 
elseif(${ZEUSSQM_POINTER_SIZE_TEST_RUN_RESULT} LESS 0 OR ${ZEUSSQM_POINTER_SIZE_TEST_RUN_RESULT} GREATER 0)
	message(FATAL_ERROR "Failed to run pointer size testing program, please contact a developer! Extended Information: ${ZEUSSQM_POINTER_SIZE_TEST_RUN_OUTPUT}") 
else()
	set(ZEUSSQM_CPP_POINTER_SIZE "${ZEUSSQM_POINTER_SIZE_TEST_RUN_OUTPUT}")
	message(STATUS "Collected pointer size information (pointer has ${ZEUSSQM_POINTER_SIZE_TEST_RUN_OUTPUT} Bits)")
endif()

# Check for std::make_unique support
try_run(ZEUSSQM_MAKE_UNIQUE_TEST_RUN_RESULT ZEUSSQM_MAKE_UNIQUE_TEST_COMPILE_RESULT "${PROJECT_BINARY_DIR}/testMakeUnique" "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_make_unique.cpp" COMPILE_OUTPUT_VARIABLE ZEUSSQM_MAKE_UNIQUE_TEST_COMPILE_OUTPUT RUN_OUTPUT_VARIABLE ZEUSSQM_MAKE_UNIQUE_TEST_RUN_OUTPUT)
if (NOT ${ZEUSSQM_MAKE_UNIQUE_TEST_COMPILE_RESULT})
	set(ZEUSSQM_CONFIG_HAVE_MAKE_UNIQUE OFF)
	message(STATUS "Compiler does NOT support std::make_unique, using replacement. ${ZEUSSQM_MAKE_UNIQUE_TEST_COMPILE_OUTPUT}") 
elseif(${ZEUSSQM_MAKE_UNIQUE_TEST_RUN_RESULT} EQUAL 0)
	set(ZEUSSQM_CONFIG_HAVE_MAKE_UNIQUE ON)
	message(STATUS "Compiler does support std::make_unique.") 
else()
	set(ZEUSSQM_CONFIG_HAVE_MAKE_UNIQUE OFF)
	message(FATAL_ERROR "Failed to run std::make_unique testing program, please contact a developer! Extended Information: ${ZEUSSQM_MAKE_UNIQUE_TEST_RUN_RESULT} - ${ZEUSSQM_MAKE_UNIQUE_TEST_RUN_OUTPUT}") 
endif()

# Configure a file to pass the ZeusSqm version to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/version.cpp.in"
	"${PROJECT_BINARY_DIR}/include/version.cpp"
)


# Sources of Build Tools
file(GLOB_RECURSE ZEUSSQM_BUILDTOOLS_VERSIONINFO_HEADERS ${PROJECT_SOURCE_DIR}/buildTools/versionInfo/*.h)
file(GLOB_RECURSE ZEUSSQM_BUILDTOOLS_VERSIONINFO_SOURCES_CPP ${PROJECT_SOURCE_DIR}/buildTools/versionInfo/*.cpp)

# Main Sources
file(GLOB_RECURSE ZEUSSQM_HEADERS ${PROJECT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE ZEUSSQM_SOURCES_CPP ${PROJECT_SOURCE_DIR}/src/*.cpp)

# 'Collect Mods' Sources
file(GLOB_RECURSE ZEUSSQM_COLLECTMODS_HEADERS ${PROJECT_SOURCE_DIR}/srcCollectMods/*.h)
file(GLOB_RECURSE ZEUSSQM_COLLECTMODS_SOURCES_CPP ${PROJECT_SOURCE_DIR}/srcCollectMods/*.cpp)

# Shared Sources
file(GLOB_RECURSE ZEUSSQM_SHARED_HEADERS ${PROJECT_SOURCE_DIR}/srcShared/*.h)
file(GLOB_RECURSE ZEUSSQM_SHARED_SOURCES_CPP ${PROJECT_SOURCE_DIR}/srcShared/*.cpp)

###
# LibPBO
#
set(LIBPBO_BASEDIR "${PROJECT_SOURCE_DIR}/thirdParty/libpbo")
include_directories("${LIBPBO_BASEDIR}/include/")
set(LIBPBO_SRCS
	"${LIBPBO_BASEDIR}/src/compress.cpp"
	"${LIBPBO_BASEDIR}/src/cryptokey.cpp"
	"${LIBPBO_BASEDIR}/src/entry.cpp"
	"${LIBPBO_BASEDIR}/src/pbo.cpp"
	"${LIBPBO_BASEDIR}/src/properties.cpp"
	"${LIBPBO_BASEDIR}/src/signature.cpp"
	"${LIBPBO_BASEDIR}/src/signature_generator.cpp"
)

# Add the executables
add_library(zeusSqmShared STATIC ${ZEUSSQM_SHARED_HEADERS} ${ZEUSSQM_SHARED_SOURCES_CPP})

add_executable(zeusSqm ${ZEUSSQM_HEADERS} ${ZEUSSQM_SOURCES_CPP} "${PROJECT_BINARY_DIR}/include/version.cpp")

add_executable(zeusSqmCollectMods ${ZEUSSQM_COLLECTMODS_HEADERS} ${ZEUSSQM_COLLECTMODS_SOURCES_CPP} "${PROJECT_BINARY_DIR}/include/version.cpp")

add_executable(zeusSqmVersionInfo ${ZEUSSQM_BUILDTOOLS_VERSIONINFO_HEADERS} ${ZEUSSQM_BUILDTOOLS_VERSIONINFO_SOURCES_CPP} "${PROJECT_BINARY_DIR}/include/version.cpp")

target_include_directories(zeusSqm PRIVATE "${PROJECT_SOURCE_DIR}/src")

if (MSVC)
#	set_target_properties(zeusSqm PROPERTIES LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\"")
#	set_target_properties(zeusSqm PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
	
#	set_target_properties(zeusSqmPlot PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
endif(MSVC)

# Use the required modules from Qt 5.
target_link_libraries(zeusSqmShared Qt5::Core)
target_link_libraries(zeusSqmShared Qt5::Gui)

target_link_libraries(zeusSqm zeusSqmShared)

target_link_libraries(zeusSqmCollectMods zeusSqmShared)

target_link_libraries(zeusSqmVersionInfo Qt5::Core)

# Targets, CPACK...
include(InstallRequiredSystemLibraries)

install(TARGETS zeusSqm RUNTIME DESTINATION bin COMPONENT ZeusUtilities)
install(TARGETS zeusSqmVersionInfo RUNTIME DESTINATION bin COMPONENT ZeusUtilities)

if (MSVC)
	# Custom target for packaging.
	if(ZEUSSQM_USE_NSIS)
		set(CPACK_GENERATOR "NSIS")
	else()
		set(CPACK_GENERATOR "ZIP")
	endif()

	set(ZEUSSQM_WINDEPLOYQT_EXE "${Qt5Core_DIR}/../../../bin/windeployqt.exe")
	
	if ("$ENV{VCINSTALLDIR}" STREQUAL "")
		get_filename_component(ZEUSSQM_VCINSTALLDIR "${CMAKE_LINKER}" DIRECTORY)
		file(TO_NATIVE_PATH "${ZEUSSQM_VCINSTALLDIR}/../../" VCINSTALLDIR_NATIVEPATH)
		
		STRING(REGEX REPLACE "^(.+)\\\\VC\\\\.+" "\\1\\\\VC" VCINSTALLDIR_NATIVEPATH ${VCINSTALLDIR_NATIVEPATH})
		
		set(ENV{VCINSTALLDIR} "${VCINSTALLDIR_NATIVEPATH}")
	endif()
	
	add_custom_command(TARGET zeusSqm POST_BUILD COMMAND "${CMAKE_SOURCE_DIR}/cmake/callwindeployqt.bat" "${ZEUSSQM_WINDEPLOYQT_EXE}" --dir "${PROJECT_BINARY_DIR}/windeployqt" "$<TARGET_FILE:zeusSqm>" "$ENV{VCINSTALLDIR}" COMMENT "Preparing Qt runtime dependencies")
	install(DIRECTORY "${PROJECT_BINARY_DIR}/windeployqt/" DESTINATION bin COMPONENT ZeusUtilities)
endif(MSVC)
   
if (NOT "${ZEUSSQM_ADDITIONAL_INSTALL_FILES}" STREQUAL "")
	foreach(ADD_INST_FILE IN LISTS ZEUSSQM_ADDITIONAL_INSTALL_FILES)
		if(IS_DIRECTORY "${ADD_INST_FILE}")
			message(STATUS "Also installing folder ${ADD_INST_FILE}")
			install(DIRECTORY "${ADD_INST_FILE}/" DESTINATION bin COMPONENT ZeusUtilities)
		else()
			message(STATUS "Also installing file ${ADD_INST_FILE}")
			install(FILES "${ADD_INST_FILE}" DESTINATION bin COMPONENT ZeusUtilities)
		endif()
	endforeach()
endif()

# CPack
set(CPACK_PACKAGE_NAME "zeusutilities")
set(CPACK_PACKAGE_VENDOR "ZeusUtilities project team")

set(CPACK_PACKAGE_VERSION_MAJOR "${ZEUSSQM_CPP_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${ZEUSSQM_CPP_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${ZEUSSQM_CPP_VERSION_PATCH}")

if(WIN32)
	set(CPACK_PACKAGE_INSTALL_DIRECTORY "ZeusUtilities")
endif()

#set(CPACK_NSIS_INSTALLED_ICON_NAME "${PROJECT_SOURCE_DIR}/resources/icon-small-for-ico.ico")
set(CPACK_NSIS_HELP_LINK "https://github.com/blizzard4591/zeusUtilities/")
set(CPACK_NSIS_URL_INFO_ABOUT "https://github.com/blizzard4591/zeusUtilities/")
set(CPACK_NSIS_CONTACT "packaging@openmittsu.de")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	set(CPACK_NSIS_DISPLAY_NAME "ZeusUtilities x64")
else()
	set(CPACK_NSIS_DISPLAY_NAME "ZeusUtilities x86")
endif()

set(CPACK_PACKAGE_VERSION "${ZEUSSQM_CPP_VERSION_MAJOR}.${ZEUSSQM_CPP_VERSION_MINOR}.${ZEUSSQM_CPP_VERSION_PATCH}plus${ZEUSSQM_CPP_VERSION_COMMITS_AHEAD}")

SET(CPACK_PACKAGE_EXECUTABLES "zeusSqm;ZeusSqm")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
if (WIN32 AND NOT UNIX)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExecWait '\\\"$INSTDIR\\\\bin\\\\vcredist_x64.exe\\\" /install /passive /norestart'")
	else()
		SET(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExecWait '\\\"$INSTDIR\\\\bin\\\\vcredist_x86.exe\\\" /install /passive /norestart'")
	endif()
else(WIN32 AND NOT UNIX)
	SET(CPACK_STRIP_FILES ON)
	SET(CPACK_SOURCE_STRIP_FILES "")
endif(WIN32 AND NOT UNIX)
set(CPACK_SOURCE_IGNORE_FILES ".git")
set(CPACK_BUNDLE_NAME "zeusSqm")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")

set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
set(CPACK_COMPONENTS_ALL ZeusUtilities)

INCLUDE(CPack)