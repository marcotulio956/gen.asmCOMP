#!/bin/bash
make new >> comp.log
touch run.log
echo "" > run.log
files=./examples/*.comp
echo 'files:'
printf '\t%s\n' ${files[@]}
for file in $files; do
  echo "TESTING $file ---" >> run.log
  ./comp $file 1 >> run.log
done