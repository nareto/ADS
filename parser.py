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

PATTERNS={'authorin': re.compile(r'<author.*?>'),
          'authorout': re.compile(r'</author.*>'),
          'titlein': re.compile(r'<title.*?>'),
          'titleout': re.compile(r'</title.*>'),
          'articlein': re.compile(r'<article.*?>'),
          'articleout': re.compile(r'</article.*>')}

def usage():
    print "USAGE: {0} dblp.xml output-file".format(sys.argv[0])

def parse(inputpath, outputpath):
    try:
        inf = open(inputpath,'r')
        outf = open(outputpath,'w')
    except IOError as e:
        print "I/O error({0}): {1}".format(e.errno, e.strerror)
        return

    current_article = [] 
    is_article = 0
    line = inf.readline()
    while line != '':
        if is_article:
            #search for  </article>
            articleout=PATTERNS['articleout'].match(line)
            if articleout != None:
                is_article = 0
                outf.write('\n')
                while current_article != []:
                    outf.write(current_article.pop() + '\n')
            else:
            	#search for authors:
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
                    	#authors are inserted at the start of current_article
                        current_article = [line] + current_article 

                titlein=PATTERNS['titlein'].match(line)
                if titlein != None:
                    line = line[len(titlein.group(0)):]
                    start_closing_tag -= len(titlein.group(0))
                    titleout=PATTERNS['titleout'].match(line,start_closing_tag)
                    if titleout != None:
                        line = line[:-len(titleout.group(0)) - 1]
                        #the title is inserted at the end of current_article
                        current_article = current_article + [line]

        elif line == '\n':
            pass
        else:
            #is the new line <article...> ? if yes set is_article=1
            articlein=PATTERNS['articlein'].match(line)
            if articlein != None:
                is_article = 1
        line = inf.readline()        

    inf.close()
    outf.close()

if __name__ =="__main__":
    if len(sys.argv) != 3:
        usage()
        exit
    else:
        parse(sys.argv[1], sys.argv[2])
                                    
