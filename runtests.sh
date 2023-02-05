#!/usr/bin/env bash

for mimg_example in cmake-build-debug/mimg_*;
do
  echo Executing "${mimg_example}"...;
  ${mimg_example} >> /dev/null;
done