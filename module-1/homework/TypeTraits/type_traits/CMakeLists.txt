cmake_minimum_required(VERSION 3.14)

project(runner)

add_library(type_traits OBJECT is_copy_constructible.h is_nothrow_move_constructible.h move_if_noexcept.h utility.h)
set_target_properties(type_traits PROPERTIES LINKER_LANGUAGE CXX)