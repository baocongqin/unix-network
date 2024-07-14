#编译器
cc=g++
# 获取src目录下所.cpp文件列表 形如main.cpp test.cpp app.cpp hello.cpp....
all_src_files:=$(notdir $(wildcard src/*.cpp))
# 替换.o 为.cpp
all_obj_files:=$(patsubst %.cpp,%.o,$(all_src_files))
# 文件前加前缀 bin/
all_obj_files:=$(addprefix bin/,$(all_obj_files))
target=a.out
# $^所以的依赖 $@目标文件
$(target):$(all_obj_files)
	$(cc) $^ -o $@ -Wall

# 一定要用%号 $<为第一个依赖
# 最后分别为每个目标调用命令 bin/% 以all_obj_files为准
bin/%.o:src/%.cpp
	$(cc)  -c -o $@ $<

clean:
	rm bin/*.o a.out
