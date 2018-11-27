#!/usr/bin/python
#coding:UTF-8

cache = {}

def add_dict(line):
    list_str = line.split(" ")
    word = list_str[0].strip("\n")
    meaning = list_str[1].strip("\n")
    #print "%s : %s" % (word, meaning)
    #dict_tmp = {word : meaning}
    #cache.update(dict_tmp)
    cache[word] = meaning

def read_cache():
    fp = open("../cache", "r")
    while True:
        line = fp.readline()
        if line:
            add_dict(line)
        else:
            break
    cache['external'] = "adj.外部的"
    cache['inline'] = "n.内联"
    cache['beauty'] = "n.美女"
    cache['define'] = "v.定义"
    fp.close()

def write_cache():
    fp = open("../cache", "w")
    num = len(cache) - 4
# 如果缓存中的元素个数超过指定的缓存大小,则按照时间顺序删除掉缓存中多余的元素
    if num > 0:
        for word in cache.keys():
            del cache[word]
            num = num - 1
            if num == 0:
                break
    for word, meaning in cache.items():
        fp.write(word + " " + meaning + "\n")
    fp.close()

def test_cache_content():
    #print cache
    for word, meaning in cache.items():
        if word == 'world':
            print meaning

def test_update_cache():
    num = len(cache) - 4
# 如果缓存中的元素个数超过指定的缓存大小,则按照时间顺序删除掉缓存中多余的元素
    if num > 0:
        for word in cache.keys():
            del cache[word]
            num = num - 1
            if num == 0:
                break
#    for word, meaning in cache.items():
#        print word + " : " + meaning


if __name__ == "__main__":
    read_cache()
    write_cache()
    # test_update_cache()


