# 编译大作业：AQL Subset

组长:朱术煌
组员：王嘉威 周吉昊 朱希冉

分工：
朱术煌：Toknizer类的编写，数据集的收集
王嘉威: Parser的类中regex 和 pattern 部分(PatternMatcher类)的编写
周吉昊: Lexer类,View类的编写以及Parser类中output部分的编写
朱希冉: Parser类中select部分的编写（？？？？）


运行环境:linux操作系统

运行方法：
1.在linux命令行终端，进入src文件夹，通过make指令生成可执行文件main
2.通过命令 ./main AQL文件名 文章数据文件名/文件夹名 运行程序

注意：
1) AQL中的保留字都为小写。
2) 若捕获的结果为 ""，输出会显示empty。
3) AQL文件中token以空格换行或制表符隔开，ID只包含数字和字母，以字母开头, 文章的token也以空格换行制表符作为分词依据。
4) select语句中select P.Per后可接as 列名，也可不接，不接时默认新创的view中的列名为来源列的列名
5) 运行时终端命令输入的文件夹名对应的文件夹内仅可含要检索的文章，不含其它文件。