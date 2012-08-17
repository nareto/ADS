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

#lxml.etree.fromstring('<?xml version="1.1"?><foo>fdsa<br>fdas</br>fdas</foo>').xpath('//text()')

import sys
import re

WORK_INS={'articlein': re.compile(r'<article.*?>'),
          'bookin': re.compile(r'<book.*?>'),
          'inproceedingsin': re.compile(r'<inproceedings.*?>'),
          'wwwin': re.compile(r'<www.*?>')}

WORK_OUTS={'articleout': re.compile(r'</article.*>'),
          'bookout': re.compile(r'</book.*>'),
          'inproceedingsout': re.compile(r'</inproceedings.*>'),
          'wwwout': re.compile(r'</www.*>')}

PATTERNS={'authorin': re.compile(r'<author.*?>'),
          'authorout': re.compile(r'</author.*>'),
          'titlein': re.compile(r'<title.*?>'),
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
                    outf.write('\n')
                    while current_authors != []:
                        outf.write(current_authors.pop() + '\n')
                    break
            if is_work: #if the previous for loop hasn't matched any closing tag, search for authors
                start_closing_tag = len(line) - 1
                for i in range(len(line)-1,0,-1):
                    if line[i] == "<":
                        break
                    else:
                        start_closing_tag = i -1
        
                authorin=PATTERNS['authorin'].match(line)
                if authorin != None:
                    line = line[len(authorin.group(0)):]
                    start_closing_tag -= len(authorin.group(0)) 
                    authorout=PATTERNS['authorout'].match(line,start_closing_tag)
                    if authorout != None:
                        line = line[:-len(authorout.group(0)) - 1]
                    	#authors are inserted at the start of current_authors
                        current_authors = [line] + current_authors 

                titlein=PATTERNS['titlein'].match(line)
                if titlein != None:
                    line = line[len(titlein.group(0)):]
                    start_closing_tag -= len(titlein.group(0))
                    titleout=PATTERNS['titleout'].match(line,start_closing_tag)
                    if titleout != None:
                        line = line[:-len(titleout.group(0)) - 1]
                        #the title is inserted at the end of current_authors
                        current_authors = current_authors + [line]

        elif line == '\n':
            pass
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
                                    
