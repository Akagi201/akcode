## http://coolshell.cn/articles/9070.html

# 输出第1列和第4例
* awk '{print $1, $4}' netstat.txt

# 格式化输出, C语言的printf没什么两样
* awk '{printf "%-8s %-8s %-8s %-18s %-22s %-15s\n",$1,$2,$3,$4,$5,$6}' netstat.txt

# 过滤记录, 下面过滤条件为：第三列的值为0 && 第6列的值为LISTEN
* awk '$3==0 && $6=="LISTEN" ' netstat.txt

* awk ' $3>0 {print $0}' netstat.txt

# 需要表头的话，我们可以引入内建变量NR
* awk '$3==0 && $6=="LISTEN" || NR==1 ' netstat.txt 

# 这里 NR==1 与 {print $0} 效果相同, 都是表示把表头打印出来.

* awk '$3==0 && $6=="LISTEN" || NR==1 {printf "%-20s %-20s %s\n",$4,$5,$6}' netstat.txt

## 内建变量
* $0: 当前记录（这个变量中存放着整个行的内容）
* $1~$n: 当前记录的第n个字段，字段间由FS分隔
* FS: 输入字段分隔符 默认是空格或Tab
* NF: 当前记录中的字段个数，就是有多少列
* NR: 已经读出的记录数，就是行号，从1开始，如果有多个文件话，这个值也是不断累加中。
* FNR: 当前记录数，与NR不同的是，这个值会是各个文件自己的行号
* RS: 输入的记录分隔符， 默认为换行符
* OFS: 输出字段分隔符， 默认也是空格
* ORS: 输出的记录分隔符，默认为换行符
* FILENAME: 当前输入文件的名字

* awk '$3==0 && $6=="ESTABLISHED" || NR==1 {printf "%02s %s %-20s %-20s %s\n",NR, FNR, $4,$5,$6}' netstat.txt

# 指定分隔符
* awk  'BEGIN{FS=":"} {print $1,$3,$6}' /etc/passwd
* awk  -F: '{print $1,$3,$6}' /etc/passwd
# 如果你要指定多个分隔符，你可以这样来
* awk -F '[;:]'

# 以\t作为分隔符输出
* awk  -F: '{print $1,$3,$6}' OFS="\t" /etc/passwd

# 字符串匹配, ~ 表示模式开始。/ /中是模式。这就是一个正则表达式的匹配。
* awk '$6 ~ /FIN/ || NR==1 {print NR,$4,$5,$6}' OFS="\t" netstat.txt

# 其实awk可以像grep一样的去匹配第一行
* awk '/LISTEN/' netstat.txt

# 我们可以使用 “/FIN|TIME/” 来匹配 FIN 或者 TIME
* awk '$6 ~ /FIN|TIME/ || NR==1 {print NR,$4,$5,$6}' OFS="\t" netstat.txt

# 模式取反
* awk '$6 !~ /WAIT/ || NR==1 {print NR,$4,$5,$6}' OFS="\t" netstat.txt

# 或者
* awk '!/WAIT/' netstat.txt

# 折分文件, awk拆分文件很简单，使用重定向就好了。下面这个例子，是按第6例分隔文件，相当的简单（其中的NR!=1表示不处理表头）。
* awk 'NR!=1{print > $6}' netstat.txt

# 把指定的列输出到文件
* awk 'NR!=1{print $4,$5 > $6}' netstat.txt

# if-else-if语句，可见awk其实是个脚本解释器
* awk 'NR!=1{if($6 ~ /TIME|ESTABLISHED/) print > "1.txt";
else if($6 ~ /LISTEN/) print > "2.txt";
else print > "3.txt" }' netstat.txt

# 统计
# 计算所有的C文件，CPP文件和H文件的文件大小总和
* ls -l  *.cpp *.c *.h | awk '{sum+=$5} END {print sum}'

# 统计各个connection状态
* awk 'NR!=1{a[$6]++;} END {for (i in a) print i ", " a[i];}' netstat.txt

# 统计每个用户的进程的占了多少内存
* ps aux | awk 'NR!=1{a[$1]+=$6;} END { for(i in a) print i ", " a[i]"KB";}'

# END的意思是“处理完所有的行的标识”，即然说到了END就有必要介绍一下BEGIN，这两个关键字意味着执行前和执行后的意思
* BEGIN{ 这里面放的是执行前的语句 }
* END {这里面放的是处理完所有的行后要执行的语句 }
* {这里面放的是处理每一行时要执行的语句}

# 环境变量, 我们来看看怎么和环境变量交互：（使用-v参数和ENVIRON，使用ENVIRON的环境变量需要export）

#从file文件中找出长度大于80的行
awk 'length>80' file
 
#按连接数查看客户端IP
netstat -ntu | awk '{print $5}' | cut -d: -f1 | sort | uniq -c | sort -nr
 
#打印99乘法表
seq 9 | sed 'H;g' | awk -v RS='' '{for(i=1;i<=NF;i++)printf("%dx%d=%d%s", i, NR, i*NR, i==NR?"\n":"\t")}'

