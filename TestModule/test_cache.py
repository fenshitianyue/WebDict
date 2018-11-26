#!/usr/bin/python
#coding:UTF-8

cache = {}

def add_dict(line):
    list_str = line.split(" ")
    word = list_str[0]
    meaning = list_str[1]
    #print "%s : %s" % (word, meaning)
    dict_tmp = {word : meaning}
    cache.update(dict_tmp)

def read_cache():
    fp = open("../cache", "r")
    while True:
        line = fp.readline()
        if line:
            add_dict(line)
        else:
            break
    fp.close()

def write_cache():
    fp = open("../cache", "a")
    fp.close()

if __name__ == "__main__":
    read_cache()
    #print cache
