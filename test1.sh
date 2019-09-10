#!/bin/sh

cd projects/assignment-3/code

# change all .cc files to .cpp files
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
        # change float to doule
        for file in *.cpp
            do
                sed -i 's/float/double/g' file
        done
# change all .cpp files to .cc files
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
        # change double to float
        for file in *.cpp
            do
                sed -i 's/double/float/g' file
        done

fi
