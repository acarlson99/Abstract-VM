#! /bin/sh

cat $@ | sed 's/push [intflodubea83216]*(\(.*\))/\1/' | sed 's/add/\+/' | sed 's/mul/\*/' | sed 's/dump/f/' | sed 's/exit//' | sed 's/assert .*//' | sed 's/pop/P/'
