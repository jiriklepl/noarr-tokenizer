#!/bin/usr/env python3

# This script transforms the houfnice output back into a definition of a noarr structure

import sys
import os

import houfnice

def reverse_dict(dict):
    return {v: k for k, v in dict.items()}

def uncanonize_dims(dim_databaze, structure):
    undim_databaze = reverse_dict(dim_databaze)
    
    for token in structure:
        if "dim" in token:
            token["dim"] = undim_databaze[token["dim"]]
            
        yield token

def transcribe_structure(structure, prefix = None):
    if prefix is not None:
        yield prefix

    for token in structure:
        if token["type"] == "vector":
            yield "^"
            yield ''.join(["noarr::sized_vector<'", str(token["dim"]), "'>(", str(token["length"]), ")"])
        else:
            raise Exception("Unknown token: " + token["type"])

if __name__ == "__main__":
    dim_database = {}
    structures = [structure for structure in houfnice.strip_scalars(houfnice.structures(dim_database), [None])]

    for structure in structures:
        print(' '.join(transcribe_structure(uncanonize_dims(dim_database, structure), "noarr::scalar<int>()")))
