#!python
from curses.ascii import isspace
from hashlib import new
import os
import json
import argparse
import copy
from types import new_class

def lineFromFilePos(f, curr_pos):
    f.seek(0)
    line = 0
    pos = 0
    while True:
        str = f.readline()
        line += 1
        if len(str) != 0:
            if pos + len(str) > curr_pos:
                return line
            pos += len(str)
        else:
            break
    return line


class Phrase:
    def __init__(self) -> None:
        self.text = {}
        self.meta = {}
        self.origin = str()

    def fromJs(self, js):
        self.meta = js["meta"]
        self.origin = js["origin"]
        self.text = js["text"]

    def toJson(self, js):
        js["meta"] = self.meta
        js["origin"] = self.origin
        js["text"] = self.text

    def __str__(self):
        return f"{{ text: {self.text}, origin: {self.origin}, meta:{self.meta}}}"

    def __repr__(self):
        return self.__str__()


class I18n:
    def __init__(self) -> None:
        self.used = []
        self.unused = []

    def fromJs(self, js):
        js.setdefault("used", [])
        used = js["used"]
        for pj in used:
            p = Phrase()
            p.fromJs(pj)
            self.used.append(p)

        js.setdefault("unused", [])

        unused = js["unused"]
        for pj in unused:
            p = Phrase()
            p.fromJs(pj)
            self.unused.append(p)

    def toJson(self, js):
        for p in self.used:
            pj = {}
            p.toJson(pj)
            js.setdefault("used", [])
            js["used"].append(pj)

        for p in self.unused:
            pj = {}
            p.toJson(pj)
            js.setdefault("unused", [])
            js["unused"].append(pj)

    def updateFromOther(self, other):
        def isIn(p, all):
            for [l,t] in p.text.items():
                all.setdefault(l, {})
                if t in all[l]:
                    return all[l][t]
            return None

        all_other = {}
        all_self = {}

        for p in other.used:
            for [l, t] in p.text.items():
                all_other.setdefault(l, {})
                all_other[l][t] = p

        for p in self.unused:
            for [l, t] in p.text.items():
                all_self.setdefault(l, {})
                all_self[l][t] = p
        for p in self.used:
            for [l, t] in p.text.items():
                all_self.setdefault(l, {})
                all_self[l][t] = p

        newI18n = I18n()

        for p in self.used:
            if not isIn(p, all_other):
                newI18n.unused.append(p)
        for p in self.unused:
            if not isIn(p, all_other):
                newI18n.unused.append(p)

        for p in other.used:
            if isIn(p, all_self):
                np = copy.copy(isIn(p, all_self))
                np.meta = p.meta
                newI18n.used.append(np)
            else:
                newI18n.used.append(p)

        self.unused = newI18n.unused
        self.used = newI18n.used


STRING_DEF = "ALTR("

parser = argparse.ArgumentParser(description='Generate localization file')
parser.add_argument('-e', metavar='ext', type=str, nargs='+',
                    help='list of file extensions')
parser.add_argument('-d', metavar='dir', type=str, nargs='+',
                    help='list of dirs')
parser.add_argument('-i', metavar='i18n', type=str,
                    help='i18n file')

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


def extractStrings(file, i18n):
    str_found = 0
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
            tstr = ""
            line = 0
            while text[pos] != ")":
                pos = skipSpaces(text, pos)

                if text[pos] == "\"":
                    pos += 1
                    start_pos = pos
                    pos = parseSimpleString(text, pos)
                    end_pos = pos

                pos = skipSpaces(text, pos) + 1

                tstr += text[start_pos:end_pos]
                line = lineFromFilePos(f, end_pos)

            p = Phrase()
            p.origin = lang
            p.text[lang] = tstr
            p.meta["file"] = file + ":" + str(line)
            str_found += 1

            i18n.used.append(p)

            pos = text.find(STRING_DEF, pos)

    if str_found != 0:
        print(file, str_found)

def parse(file, i18n):
    if not os.path.isdir(file):
        filename, extension = os.path.splitext(file)
        extension = extension.replace('.', '')
        if extension in args.e:
            extractStrings(file, i18n)
    else:
        for f in os.listdir(file):
            parse(file + "/" + f, i18n)


newI18n = I18n()
for f in args.d:
    parse(f, newI18n)

oldI18n = I18n()

old_js = {}
try:
    f = open(args.i, 'r')
    old_js = json.load(f)
except:
    pass

oldI18n.fromJs(old_js)
oldI18n.updateFromOther(newI18n)

f = open(args.i + ".tmp", 'w')
js = {}
oldI18n.toJson(js)
json.dump(js, f, ensure_ascii=False, indent=2)

os.replace(args.i + ".tmp", args.i)
