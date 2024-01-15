#!/bin/usr/env python3

# This script transforms the houfnice output back into a definition of a noarr structure

import sys
import os

import houfnice

def reverse_dict(dict):
    return {v: k for k, v in dict.items()}

def uncanonize_dims(dim_database, structure):
    undim_database = reverse_dict(dim_database)
    for token in structure["tokens"]:
        token["dim"] = undim_database[token["dim"]]

    return structure

def transcribe_structure(structure, prefix = None):
    def transcribe_tokens(tokens):
        if prefix is not None:
            yield prefix

        for token in tokens:
            if token["type"] == "vector":
                yield "^"
                yield ''.join(["noarr::sized_vector<'", str(token["dim"]), "'>(", str(token["length"]), ")"])
            else:
                raise Exception("Unknown token: " + token["type"])
    
    return "auto " \
        + str(structure["name"]) \
        + " = " \
        + ' '.join(transcribe_tokens(structure["tokens"])) \
        + ";"
    

if __name__ == "__main__":
    dim_database = {}
    for structure in houfnice.strip_scalars(houfnice.structures(dim_database), [None]):
        print(transcribe_structure(uncanonize_dims(dim_database, structure), "noarr::scalar<int>()"))
