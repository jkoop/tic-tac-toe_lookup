#!/bin/sh

set -x # enable command echoing

make &&
./compute_tree > 00-tree.tsv &&
sort 00-tree.tsv | uniq > 01-graph.tsv
