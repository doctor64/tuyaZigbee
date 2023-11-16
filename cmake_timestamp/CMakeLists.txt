###
# Compile-time timestamping library
#

#CMake 3.12 made using OBJECT libraries much nicer, so we use that.
cmake_minimum_required(VERSION 3.12)

# Compile the library that contains the global variables
add_library( timestamp OBJECT timestamp.c)


##########################################################
# Generate the compile-time data.
# Each data item is saved as a CMake variable, that is passed
# as a preprocessor define to the compiler.
#
# If you add new data items, they must be added to timestamp.c,
# timestamp.h and the target_compile_definitions below.


# Set CMake variable BUILD_TIME to 'now'.
# This variable drives the CMake auto-update scripts, so
# this must not be commented out!
# (TIMESTAMP is a CMake built-in command)
#string(TIMESTAMP BUILD_TIME "%Y-%m-%d %H:%M:%S" UTC)
string(TIMESTAMP BUILD_TIME "%Y%m%d %H:%M")


##########################################################
# Git versioning info.
# First a helper function, then use it for select git data.
find_package(Git)
function(git_cmd optstring output)
	# convert space separated string to CMake list, so it gets
	# split into argvs for the git command (and not passed, spaces and all,
	# in argv[1])
	string(REPLACE " " ";" cmd_list ${optstring})
	execute_process(
	COMMAND
		${GIT_EXECUTABLE} ${cmd_list}
	WORKING_DIRECTORY
		${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE
		res
	OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	set( ${output} ${res} PARENT_SCOPE )
endfunction()

git_cmd( "show -s --format=%H" GIT_HASH)
git_cmd( "show -s --format=%h" GIT_SHORT_HASH)
git_cmd( "diff -w --shortstat" GIT_DIRTY)
if( ${GIT_VERSION_STRING} VERSION_GREATER_EQUAL 2.22 )
	git_cmd( "branch --show-current" GIT_BRANCH)
endif()



##########################################################
# Add here more lines to do the CMake-variable to
# C preprocessor define conversion.
#
target_compile_definitions(
	timestamp
	PRIVATE -DBUILD_TIME="${BUILD_TIME}"
	PRIVATE -DGIT_HASH="${GIT_HASH}"
	PRIVATE -DGIT_SHORT_HASH="${GIT_SHORT_HASH}"
	PRIVATE -DGIT_BRANCH="${GIT_BRANCH}"
	PRIVATE -DGIT_DIRTY="${GIT_DIRTY}"
	PRIVATE -DCPACK_VERSION="${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}"
)

##########################################################
# Only building of the timestamp library below this point.
# No configuration should be necessary.
#
target_include_directories( timestamp
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

#Add a dummy target that removes the CMake variable BUILD_TIME from CMake's cache.
#this forces CMake to be-rerun when we hit this target.
add_custom_target(
	clear_cache
	COMMAND ${CMAKE_COMMAND} -U BUILD_TIME ${CMAKE_BINARY_DIR}
)

#Have the cache clearing be run before trying to build the timestamp library.
#This (I think) is the same as a PRE_BUILD custom_command. But PRE_BUILD is
#available for VS generators only, on others it is synonymous to PRE_LINK,
#i.e. "post compile"
add_dependencies(timestamp clear_cache)

