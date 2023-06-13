#!/bin/bash

THIS_DIR="$(dirname "$BASH_SOURCE[0]")"
lounch_dir=$(realpath $THIS_DIR)

# BLOB_NAME="assembly_syscall"
BLOB_NAME="memory_region_examination"

shopt -s expand_aliases
alias goto="cat >/dev/null <<"

# Use this GOTO to jump to specific parts of this script when needed
goto GOTO_1
GOTO_1

set -e
set -x

pushd $lounch_dir
make BLOB_NAME=$BLOB_NAME all

cat $BLOB_NAME.c

make BLOB_NAME=$BLOB_NAME dumpBlob

./load_and_execute $BLOB_NAME.bin

echo "Hello World!" | hd

hd $BLOB_NAME.bin

make BLOB_NAME=$BLOB_NAME clean
popd