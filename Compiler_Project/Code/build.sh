#!/bin/bash

cd -- "$(dirname -- "${BASH_SOURCE[0]}")" || exit 1
mkdir -p ../build || exit 1
gcc -Wall -ggdb ../Code/main.c -o ../build/jive 
ret_val=$?
if [ $ret_val -ne 0 ]; then
	echo ERROR: Failed to build
	exit $ret_val
fi

echo "Build successful!"

echo === TEST ON EXAMPLE.JIVE ===

../build/jive ../Jive_Programs/example.jive
ret_val=$?
if [ $ret_val -ne 0 ]; then
	echo ERROR: Compiler returned an error compiling example.jive
	exit $ret_val
fi

