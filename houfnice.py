#!/bin/usr/env python3

# This script canonizes the tokenized noarr structures

import sys
import os
import re

def tokenize(line):
    tokens = line.split(" ")
    
    for i in range(len(tokens)):
        yield tokens[i]

def parse_scalar(tokenizer):
    type = next(tokenizer, None)
    
    return {
        "type": "scalar",
        "scalar_type": type
    }
    
def parse_vector(tokenizer):
    dim = next(tokenizer, None)
    
    return {
        "type": "vector",
        "dim": dim,
        "length": None
    }
    
def parse_set_length(tokenizer):
    dim = next(tokenizer, None)
    length = next(tokenizer, None)
    
    return {
        "type": "set_length",
        "dim": dim,
        "length": length
    }

def parse_structure(tokenizer):
    for token in tokenizer:
        if token == "scalar":
            yield parse_scalar(tokenizer)
        elif token == "vector":
            # dim_databaze[("length", token["dim"])] = token
            yield parse_vector(tokenizer)
        elif token == "set_length":
            yield parse_set_length(tokenizer)
            # key = ("length", token["dim"])
            # if key in dim_databaze:
            #     dim_databaze[key]["length"] = token["length"]
            # else:
        else:
            raise Exception("Unknown token: " + token)

def fill_lengths(structure):
    dim_databaze = {}

    for token in structure:
        if token["type"] == "vector":
            dim_databaze[("length", token["dim"])] = token
        elif token["type"] == "set_length":
            key = ("length", token["dim"])
            if key in dim_databaze:
                dim_databaze[key]["length"] = token["length"]
                continue
            else:
                raise Exception("Unknown dimension: " + token["dim"])
        yield token

def canonize_dims(dim_databaze, structure):
    for token in structure:
        if "dim" in token:
            if token["dim"] not in dim_databaze:
                dim_databaze[token["dim"]] = len(dim_databaze)

            token["dim"] = dim_databaze[token["dim"]]
            
        yield token

def sequence(*fns):
    def sequence_helper(arg):
        for fn in fns:
            arg = fn(arg)
        return arg
    return sequence_helper

def structures(dim_databaze):

    for line in sys.stdin:
        yield [token for token in sequence(str.strip, tokenize, parse_structure, fill_lengths, lambda x: canonize_dims(dim_databaze, x))(line)]

def strip_scalars(structures, scalar_type):
    for structure in structures:
        if len(structure) == 0:
            raise Exception("Empty structure")
        
        if structure[0]["type"] != "scalar":
            raise Exception("First token is not scalar")

        if scalar_type[0] is None:
            scalar_type[0] = structure[0]["scalar_type"]
        elif scalar_type[0] != structure[0]["scalar_type"]:
            raise Exception("Scalar type mismatch")
        
        yield structure[1:]

if __name__ == "__main__":
    dim_databaze = {}
    for structure in strip_scalars(structures(dim_databaze), [None]):
        print(structure)
