aux_source_directory(. SRC_LIST)
#生成动态库
add_library(MPRPC SHARED ${SRC_LIST}) 
#  muduo库是静态库
# /usr/local/lib/libmuduo_http.so
# /usr/local/lib/libmuduo_protobuf_codec.so
# /usr/local/lib/libmuduo_net.so
# /usr/local/lib/libmuduo_curl.so
# /usr/local/lib/libmuduo_net.so.2
# /usr/local/lib/libmuduo_net.so.2.0.0
# /usr/local/lib/libmuduo_protorpc_wire.so
# /usr/local/lib/libmuduo_inspect.so
# /usr/local/lib/libmuduo_protorpc.so
# /usr/local/lib/libmuduo_base.so
target_link_libraries(MPRPC muduo_net muduo_base pthread zookeeper_mt)