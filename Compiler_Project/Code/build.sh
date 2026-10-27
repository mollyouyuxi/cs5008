#!/bin/bash

cd -- "$(dirname -- "${BASH_SOURCE[0]}")" || exit 1
gcc -Wall -ggdb ../Code/test_lexer.c -o test_lexer.out
ret_val=$?
if [ $ret_val -ne 0 ]; then
	echo ERROR: Failed to build
	exit $ret_val
fi

echo "Build successful!"

echo === TEST ON EXAMPLE.JIVE ===

./test_lexer.out ../Jive_Programs/example.jive
ret_val=$?
if [ $ret_val -ne 0 ]; then
	echo ERROR: Lexer returned an error compiling simple.jive
	exit $ret_val
fi

