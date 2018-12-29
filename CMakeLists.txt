# %Tag(FULLTEXT)%
cmake_minimum_required(VERSION 2.8.3)
project(interaccion)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg)

## Declare ROS messages and services
add_message_files(FILES inf_personal_usuario.msg)
add_message_files(FILES pos_usuario.msg)
add_message_files(FILES usuario.msg)

add_service_files(FILES multiplicador.srv)

## Generate added messages and services
generate_messages(DEPENDENCIES std_msgs)

## Declare a catkin package
catkin_package()

## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(inf_personal_usuario src/informacion_personal_nodo.cpp)
target_link_libraries(inf_personal_usuario ${catkin_LIBRARIES})
add_dependencies(inf_personal_usuario interaccion_gencpp)

add_executable(posicion_usuario_nodo src/posicion_usuario_nodo.cpp)
target_link_libraries(posicion_usuario_nodo ${catkin_LIBRARIES})
add_dependencies(posicion_usuario_nodo interaccion_gencpp)

add_executable(emocion_usuario_nodo src/emocion_usuario_nodo.cpp)
target_link_libraries(emocion_usuario_nodo ${catkin_LIBRARIES})
add_dependencies(emocion_usuario_nodo interaccion_gencpp)

add_executable(empaquetador_nodo src/empaquetador_nodo.cpp)
target_link_libraries(empaquetador_nodo ${catkin_LIBRARIES})
add_dependencies(empaquetador_nodo interaccion_gencpp)

add_executable(dialogo_nodo src/dialogo_nodo.cpp)
target_link_libraries(dialogo_nodo ${catkin_LIBRARIES})
add_dependencies(dialogo_nodo interaccion_gencpp)

add_executable(matematico_nodo src/matematico_nodo.cpp)
target_link_libraries(matematico_nodo ${catkin_LIBRARIES})
add_dependencies(matematico_nodo interaccion_gencpp)

add_executable(reloj_nodo src/reloj_nodo.cpp)
target_link_libraries(reloj_nodo ${catkin_LIBRARIES})
add_dependencies(reloj_nodo interaccion_gencpp)

# %EndTag(SRVCLIENT)%

# %EndTag(FULLTEXT)%
