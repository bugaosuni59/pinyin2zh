from pyhanlp import *

fword=open('一二级汉字表.txt','r')
zh=set()
for line in fword:
    for c in line:
        zh.add(c)
    # print(line)

def isok(s):
    for c in s:
        if(c not in zh):
            return False
    return True

files = ["2.txt","4.txt","5.txt","6.txt","7.txt","8.txt","9.txt","10.txt","11.txt"]
for fname in files:
    outfile = open('fenci'+fname,'w')
    fname = "o_"+fname
    f=open(fname,'r')
    for line in f:
        res=HanLP.segment(line)
        for term in res:
            if(isok(term.word)):
                outfile.write(term.word+" ")
        # print('{}\t{}'.format(term.word, term.nature)) # 获取单词与词性
