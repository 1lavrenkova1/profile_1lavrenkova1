#!/bin/bash

#вводимая директория
directory=$1


new_file="$directory/$i.txt"
touch $new_file

echo "Test file" >> "$new_file"

