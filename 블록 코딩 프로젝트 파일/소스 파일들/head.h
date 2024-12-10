#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>

void __error(const char* text, const unsigned int line);
#define _error(text) __error(text, __LINE__ )

#define ContainerSize 64
#define ContainerStrSize 64

typedef uint64_t type_li;
typedef int64_t type_data;

typedef struct
{
	int x, y;
	bool is_;
} DragState;

typedef struct struct_Block
{
	type_li id;
	type_data data;
	type_data flag;
	type_data type; // 전체적인 종류
	type_data tag; // 어떤 블록인가 정의
	DragState drag;
	type_li name;

	type_li arg; // 인자 포인터
	type_li next; // 다음 블록
	type_li renext; // 이전 블록

	type_data Return;

} Block;

typedef struct struct_Container // 컨테이너
{
	Block block[ContainerSize];
	char str[ContainerSize][ContainerStrSize];
	uint64_t block_top;
	uint64_t str_top;
	struct struct_Container* nextContainer;

} Container;

typedef struct struct_funBox
{
	void (*fun[64])(int);
} funBox;

Block* get_Block(type_li id);
type_li new_Block();
type_li new_str(char*);
char* get_str(type_li id);

int Block_arg_read(Block* block, int flag);
void _subWindows();
void _subWindows_loop(SDL_Event* event);

void Block_reader(type_li id);
void sub_str_draws();

size_t Stack_get_index(char* top);

void SQL_save();
void SQL_load();