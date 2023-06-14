#!/bin/bash

THIS_DIR="$(dirname "$BASH_SOURCE[0]")"
lounch_dir=$(realpath $THIS_DIR)

BLOB_NAME="assembly_syscall"

set -e
set -x

pushd $lounch_dir
# Compile the loader program and the binary blob
make BLOB_NAME=$BLOB_NAME all

# Print the content of the binary blob source file
cat $BLOB_NAME.c

# Dump the binary blob content and intetpret it as x86_64 instructions
make BLOB_NAME=$BLOB_NAME dumpBlob

# Load and execute the binary blob
./load_and_execute $BLOB_NAME.bin

# Hex dump of the "Hello World!" string
echo "Hello World!" | hd

# Hex dump of the binary blob file
hd $BLOB_NAME.bin

make BLOB_NAME=$BLOB_NAME clean
popd

