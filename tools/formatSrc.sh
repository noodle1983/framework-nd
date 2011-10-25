#!/bin/bash
find . -name *.h -o -name *.cpp -o -name *.pkg |xargs perl -pi -e 's/\t/    /g'
find . -name *.h -o -name *.cpp -o -name *.pkg |xargs perl -pi -e 's/[ \t]*$//g'
