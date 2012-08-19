#!/usr/bin/env python2
#coding=UTF8

#scopo di questo script è leggere il file dblp.xml e scrivere un file col seguente formato:
#Titolo Articolo
#Autore 1
#
#Titolo Articolo
#Autore 1
#Autore 2
#

import sys
import re

WORK_INS={'articlein': re.compile(r'<article.*?>'),
          'bookin': re.compile(r'<book(?!title).*?>'),
          'inproceedingsin': re.compile(r'<inproceedings.*?>'),
          'wwwin': re.compile(r'<www.*?>')}

WORK_OUTS={'articleout': re.compile(r'</article.*>'),
          'bookout': re.compile(r'</book(?!title).*>'),
          'inproceedingsout': re.compile(r'</inproceedings.*>'),
          'wwwout': re.compile(r'</www.*>')}

#WORK_INS={'articlein': re.compile(r'<article.*?>')}
#WORK_OUTS={'articleout': re.compile(r'</article.*>')}

PATTERNS={'authorin': re.compile(r'<author.*?>'),
          'authorout': re.compile(r'</author.*>'),
          'titlein': re.compile(r'<title.*?>(?!Home Page)'),
          'titleout': re.compile(r'</title.*>')}

def usage():
    print "USAGE: {0} dblp.xml output-file".format(sys.argv[0])

def parse(inputpath, outputpath):
    try:
        inf = open(inputpath,'r')
        outf = open(outputpath,'w')
    except IOError as e:
        print "I/O error({0}): {1}".format(e.errno, e.strerror)
        return

    current_authors = [] 
    is_work = 0
    line = inf.readline()
    while line != '':
        if is_work:
            #search for ending of WORK block
            match,workout=0,1
            for work_out in WORK_OUTS.iteritems():
                if work_out[1].match(line) != None:
                    is_work = 0
                    while current_authors != []:
                        outf.write(current_authors.pop() + '\n')
                    outf.write('\n')
                    break
            if is_work: #if the previous for loop hasn't matched any closing tag, search for authors
                start_closing_tag = len(line) - 1
                for i in range(len(line)-1,0,-1):
                    if line[i-2:i] == "</":
                        start_closing_tag = i-2
                        break
                    else:
                        start_closing_tag = i -1
        
                authorin=PATTERNS['authorin'].match(line)
                if authorin != None:
                    current_authors = [line[len(authorin.group(0)):start_closing_tag]] + current_authors

                titlein=PATTERNS['titlein'].match(line)
                if titlein != None:
                    current_authors = current_authors + [line[len(titlein.group(0)):start_closing_tag]]

        elif line == '\n':
            current_authors = []
        else:
            #search for starting of WORK block
            for work_in in WORK_INS.iteritems():
                if work_in[1].match(line) != None:
                    is_work = 1
                    break
        line = inf.readline()        

    inf.close()
    outf.close()

if __name__ =="__main__":
    if len(sys.argv) != 3:
        usage()
        exit
    else:
        parse(sys.argv[1], sys.argv[2])
                                    
