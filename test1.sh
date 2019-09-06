#!/bin/sh

cd projects/assignment-3/code
# change all .cpp files to .cc files
if ls *.cc >/dev/null 2>&1
    then
        for file in *.cc   
            do
                mv $file ${file%.cc}.cpp
            done

# add copyright to all .cc files
        for file in *.cpp
            do 
                if grep -q "(C)" $file
                    then
                        exit 1
                fi
                cat copyright.copyright $file > temp.temp
                cat temp.temp > $file
            done
else
    for file in *.cpp   
        do
            mv $file ${file%.cpp}.cc
        done

# add copyright to all .cc files

        for file in *.cc
            do 
                if grep -q "(C)" $file
                    then
                        exit 1
                fi
                cat copyright.copyright $file > temp.temp
                cat temp.temp > $file
            done
fi
