set ff=unix
#!/usr/bash  -x

#从终端读取一个数
read str 

#字符串比较不能用于 eq比较
[ "X$str" == "Xhelloworld" ]
#左边括号是一个命令 
#[ $myint -eq 100 ] # -ne(不相等) -lt(小于)  -eq (相等) -gt (大于) -le (小于等于) -ge(大于等于)
#test $myint -eq 100 # -ne(不相等) -lt(小于)  -eq (相等) -gt (大于) -le (小于等于) -ge(大于等于)
echo $?

test
