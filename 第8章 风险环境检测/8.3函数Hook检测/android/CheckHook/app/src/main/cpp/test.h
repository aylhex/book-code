//
// Created by Administrator on 2021/9/7.
//
#pragma once
#ifndef CHECKNATIVEHOOK_TEST_H
#define CHECKNATIVEHOOK_TEST_H

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "common.h"
#include <elf.h>

#ifdef __cplusplus
extern "C" {
#endif

void *my_malloc(size_t size);
void say_hello();

#ifdef __cplusplus
}
#endif
#endif //CHECKNATIVEHOOK_TEST_H
