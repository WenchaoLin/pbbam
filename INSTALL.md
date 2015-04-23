# PacBio::BAM - building & integration

- [Dependencies](#dependencies)
- [Build](#build)
- [Test](#test)
- [Integration](#integration)
    - [CMake](#cmake)
	- [Other](#other)
- [SWIG](#swig)
    - [Python](#python)
	- [R](#r)
	- [CSharp](#csharp)

## Dependencies
  - CMake v2.8+
  - Boost 1.54+
  - zlib
  - samtools exe (*)

(*) NOTE: ppbam uses samtools for some of its tests, for now at least. The current
build system points uses a relative path to one of the "prebuilt" samtools binaries.
If you have checked out pbbam to any path that is NOT:

    ///depot/software/smrtanalysis/bioinformatics/staging/PostPrimary/pbbam

then please edit the Samtools_Dir variable in pbbam/tests/CMakeLists.txt to a place
that works for your setup. That could just be as simple as "" if you already have
samtools somewhere in your PATH.

## Build

To perform a simple build of the library (and its tests):

    $ cd <pbbam_root>
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

## Test
    
There are 2 options for testing the library. 

1) Run the test executable directly:

    $ <pbbam_root>/tests/bin/test_pbbam 

which displays the GoogleTest-formatted results for the 250+ individual tests. This
provides fine-grained info on any failed test.

2) The other option is to use CMake/CTest-generated 'make' command:

    $ cd <pbbam_root>/build
    $ make test

which collapses all of the test output into a single, CTest-formatted pass/fail display.

## Integration

### CMake

If you are using CMake for your library or application, you can use the following steps
to automate both the building of pbbam and its dependencies (if necessary) and importing
the proper include paths, library paths, etc. If the pbbam library already exists, then
the header/lib variables are simply imported.

    # just for convenience
    set(PacBioBAM_RootDir </anywhere/on/disk/path/to/pbbam>)  

    # add_subdirectory() sounds a bit misleading, the path can be *anywhere* on disk. 
    # the 2nd arg tells CMake where it should build pbbam if necessary
    add_subdirectory(${PacBioBAM_RootDir} ${PacBioBAM_RootDir}/build) 
  
    # setup your client 
    add_executable(foo ....)

    # PacBioBAM_INCLUDE_DIRS provides all pbbam headers, as well as dependencies
    include_directories( .... ${PacBioBAM_INCLUDE_DIRS} )

    # PacBioBAM_LIBRARIES provides libpbbam.a, as well as dependencies
    target_link_libraries( foo ..... ${PacBioBAM_LIBRARIES} )

### Other

The following instructions apply to all non-CMake-based builds. In addition to Boost headers & zlib, the relevant include paths for pbbam are:

    <pbbam_root>/include
    <pbbam_root>/third-party/htslib

which allows these statements:

    #include <pbbam/BamRecord.h>
    #include <htslib/sam.h>       

and so on in your code. And the relevant libraries to link to are:

    <pbbam_root>/lib/libpbbam.a
    <pbbam_root>/third-party/htslib/libhts.a

## SWIG

TODO: fill this in

### Python

TODO: fill this in

### R

TODO: fill this in

### CSharp

TODO: fill this in