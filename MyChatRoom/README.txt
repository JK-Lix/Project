因为图方便我就没有再写makefile文件
此处只需要对Client和Server的源文件进行编译即可使用
或者直接应用run文件夹内的文件即可
但对于代码修改和重新编译
可执行以下语句：
gcc Client.c -lpthread ../common/common.c -I ../common -o 文件名
