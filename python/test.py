#shebang
#!usr/bin/python
a = {'ip':'127.0.0.1','potr' :80}       
for key in a:
    print (key,a[key])
#for num in range(1,100):
#    if num%3 == 0:
#        continue
#    print num

if a == 10:
    pass
else :
    print a

a = [1,2,3,4]
b = []
#for num in a:
#    b.append(num ** 2)
#print(b)
#list jiexi 
b = [x ** 2 for x in a if x % 2 == 1]
print b
#Func

#def Func start 
#same Func_name cover

def add(x,y,z):
    return x+y+z
def add(x = 0,y = 0):
    ret = x+y
    return ret
print add(2)

#Func retrun more than one value 
def get_point():
    x = 10;
    y = 10;
    return x,y
#unpack value 
x,y = get_point()
#tuple 
tmp = get_point()
print (type(tmp))
_,y = get_point()

#Func is object 
#Func type is function 
print(type(get_point))

#filename mode   
#return about txt object
handle = open('./test.txt','r')
word_dict = {}
for word in handle:
    #strip cut block singal 
    word = word.strip()
    if word in word_dict:
        word_dict[word] += 1
    else:
        word_dict[word] = 1
print(word_dict)
#return list
#open after close 
handle.close()

#calc.py must puts ./  or xitong mulu  
import calc
print(calc.add(10,20))

server = {
    'ip' : '127.0.0.1',
    'port': 80
}

#x = y = 1
x,y = 1,2
x,y = y,x
print (x,y)

#True,False = False,True
#python2 is 
print 2**3

def Test():
    x = 1
    print("Func ",locals())
    return x

#Func is object
Test()
print('=' * 40)
print("global",locals())

for i in range(1,10):
    print i*2

#import time
#time.sleep(3)
#print("hello ")

import calc

#print(calc.add.__doc__)
#help(calc)
x = type(10)
print id(calc)
print id(calc.add)
print id(x)

a = 2
b = 3

print id(a)
print id(b)


























