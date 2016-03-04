#!/bin/sh

# create a local Build directory
mkdir -p build

# generate the build system in the Build directory:
cd build
cmake ..
returncode=$?
if [ $returncode != 0 ] ; then
    echo "There was a failure when configuring the project with CMake."
    echo "Please check the output to resolve the problem."
    return $rc
fi

echo "You may now run 'make' in the build/ directory"
echo "  make                        - build the full software suite"
