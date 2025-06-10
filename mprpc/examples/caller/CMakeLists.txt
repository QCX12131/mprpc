# set(SRC_LIST calluserservice.cpp ../user.pb.cc)

# # 框架使用的示例生成的可执行文件
# add_executable(Consumer ${SRC_LIST})

# # 示例代码需要链接框架生成的动态库，前面已经设置了MPRPC在哪一个文件夹里面
# # protobuf和muduo库一样会去默认的文件夹路径下面找
# target_link_libraries(Consumer MPRPC protobuf)

set(SRC_LIST callfriendservice.cpp ../friends.pb.cc)

# 框架使用的示例生成的可执行文件
add_executable(Consumer ${SRC_LIST})

# 示例代码需要链接框架生成的动态库，前面已经设置了MPRPC在哪一个文件夹里面
# protobuf和muduo库一样会去默认的文件夹路径下面找
target_link_libraries(Consumer MPRPC protobuf)