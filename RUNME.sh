#!/bin/sh

set -x # enable command echoing

make &&
./00-tree > 00-tree.tsv &&
sort 00-tree.tsv | uniq > 01-graph.tsv
