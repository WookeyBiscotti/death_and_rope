#!python3
from curses.ascii import isspace
import os
import argparse


STRING_DEF = "ALTR("

parser = argparse.ArgumentParser(description='Generate localization file')
parser.add_argument('-e', metavar='ext', type=str, nargs='+',
                    help='list of file extensions')
parser.add_argument('-d', metavar='dir', type=str, nargs='+',
                    help='list of dirs')

args = parser.parse_args()


def skipSpaces(text, pos):
    while text[pos].isspace():
        pos += 1

    return pos


def parseSimpleString(text, pos):
    while text[pos] != "\"":
        if text[pos] == "\\" and text[pos + 1] == "\"":
            pos += 1
        pos += 1
    
    return pos


def parseWord(text, pos):
    while text[pos].isalpha():
        pos += 1

    return pos


def extractStrings(file):
    result = {}

    with open(file, "r") as f:
        text = f.read()

    pos = text.find(STRING_DEF)

    while pos != -1:
        pos += len(STRING_DEF)

        pos = skipSpaces(text, pos)

        start_pos = pos
        pos = parseWord(text, pos)
        end_pos = pos

        lang = text[start_pos:end_pos]
        pos = skipSpaces(text, pos) + 1
        str = ""
        while text[pos] != ")":
            pos = skipSpaces(text, pos)

            if text[pos] == "\"":
                pos += 1
                start_pos = pos
                pos = parseSimpleString(text, pos)
                end_pos = pos

            pos = skipSpaces(text, pos) + 1

            str += text[start_pos:end_pos]

        result.setdefault(lang, [])
        result[lang].append(str)

        pos = text.find(STRING_DEF, pos)

    print(result)
    return result


def parse(file):
    if not os.path.isdir(file):
        filename, extension = os.path.splitext(file)
        extension = extension.replace('.', '')
        if extension in args.e:
            extractStrings(file)
    else:
        for f in os.listdir(file):
            parse(file + f)


for f in args.d:
    parse(f)
