
#include "head.h"

typedef long long int type_lls;
#define ll_maxmin (1LL << 63)
#define stack_max 256

extern Container* mainContainer;
extern SDL_Window* SubWindow;
extern SDL_Renderer* SubRenderer;
extern funBox functions;
extern SDL_Rect SubWindows;
extern TTF_Font* Mainfont;

extern type_li main_block_id;

SDL_Color SubColor = { 255, 255, 255, 255 };

struct compiler_struct
{
	Block* block;
	type_li id;
	type_data flag;
} bf_st;

struct addCopiler_struct
{
	Block* block;
	Block* arg[8];
} bf_ast;

struct shortStack
{
	type_li d[64];
	size_t top;
} sStack = 
{
	.d = {0}, .top = 0 
};

void sStack_Push(type_li d)
{
	sStack.d[sStack.top++] = d;
}

type_li sStack_pop()
{
	if (sStack.top == 0) return 0;
	else return sStack.d[--sStack.top];
}

typedef struct Stack
{
	type_lls data[stack_max];
	unsigned char top[stack_max / 16 + 1];
	struct Stack* next;

} Stack;

Stack mainStack = { 0, };

Stack* new_stack(Stack* head)
{
	if (head->next == NULL)
	{
		Stack* self = SDL_malloc(sizeof(Stack));
		if (self == NULL) _error("메모리 할당 오류"); else
		{
			SDL_memset(self, 0, sizeof(Stack));
			head->next = self;
			return self;
		}
	}
	else return head->next;
}

void del_stack(Stack* stack)
{
	if (stack == NULL); else
	{
		del_stack(stack->next);
		SDL_free(stack);
	}
}

size_t Stack_Push(Stack* stack, type_lls data)
{
	size_t index = Stack_get_index(stack->top);
	
	if (index == stack_max+1) return Stack_Push(stack->next ? stack->next : new_stack(stack), data);
	stack->data[index] = data;
	stack->top[index / 8] |= (1 << (index % 8));

	return index;
}

type_lls Stack_peek(Stack* stack, size_t index)
{
	if (stack->top[index / 8] & 1 << (index % 8))
	{
		return stack->data[index];
	}
	else return ll_maxmin;
}

type_lls Stack_pop(Stack* stack, size_t index)
{
	if (stack->top[index / 8] & 1 << (index % 8))
	{
		stack->top[index / 8] &= ~(1 << (index % 8));
		return stack->data[index];
	}
	else return ll_maxmin;
}

Stack VarStack = { 0, }; // 변수 블록의 id를 저장하는 스텍임


Stack textStack = { 0, }; // 텍스트 스텍

// 이거라도 쓰자

type_li text_Push(Stack* stack, char* input)
{
	type_li id = new_str(input);
	Stack_Push(stack, id);
	return id;
}

char* text_peek(Stack* stack, size_t index)
{
	type_li id = Stack_peek(stack, index);
	return get_str(id);
}

#define _case(a, b, c, d) case a : if(c) block0->data = block1->Return b block2->Return; else block0->data = d; break
void sum0(type_li id) 
{
	Block* block0 = get_Block(id);
	type_data flag = block0->flag; // flag 1~5 + - * / % |||| 11~16 == != < <= > >= && || & |
	Block* block1;
	Block* block2;
	if (block0 == NULL) _error("계산하기에는 복잡한가 봅니다.");
	else if (block0->type != 2)
	{
		return;
	}
	else if (block0->arg == 0 || ((block1 = get_Block(block0->arg)) == NULL))
	{
		if(flag == 3 || flag == 4 || flag == 5) block0->data = 1;
		else block0->data = 0;
	}
	else if (block1->next == 0 || ((block2 = get_Block(block1->next)) == NULL))
	{
		Block_reader(block1->id);
		block0->data = block1->Return;
	}
	else
	{
		Block_reader(block1->id);
		Block_reader(block2->id);
		sum0(block1->id);
		sum0(block2->id);

		switch (flag)
		{
			_case(1, +, 1, 0);
			_case(2, -, 1, 0);
			_case(3, *, 1, 1);
			_case(4, /, block2->Return != 0, 1);
			_case(5, %, block2->Return != 0, 0);
			_case(11, ==, 1, 0);
			_case(12, !=, 1, 0);
			_case(13, <, 1, 0);
			_case(14, <=, 1, 0);
			_case(15, >, 1, 0);
			_case(16, >=, 1, 0);
			_case(17, && , 1, 0);
			_case(18, || , 1, 0);
			_case(19, & , 1, 0);
			_case(20, | , 1, 0);

		}
	}
	if (block0 != NULL) block0->Return = block0->data;
}
#undef _case

void bf_st_id_Event(type_li id)
{
	Block* block = get_Block(id);
	if (block == NULL) _error("서브 버퍼에 입력 실패");
	{
		bf_st.block = block;
		bf_st.id = id;
		bf_st.flag = block->flag;
	}
}

void bf_st_Event(Block* block)
{
	if (block == NULL) _error("누가 NULL을 가져온거야"); else
	{
		bf_st.block = block;
		bf_st.id = block->id;
		bf_st.flag = block->flag;
	}
}

void block_function_print(int flag)
{
	Block* block = bf_st.block;
	if (Block_arg_read(block, 1) == -1) return;
	Block* arg = bf_ast.arg[0];
	Block_reader(arg->id);
	switch (arg->type)
	{
	case 2:
		if (1)
		{
			size_t size = snprintf(NULL, 0, "%lld", arg->data) + 1;
			char* s = malloc(size);
			snprintf(s, size, "%lld", arg->data);
			text_Push(&textStack, s);
			free(s);
		}
		break;
	case 5:
		switch (arg->tag)
		{
		case 1:
			if (1)
			{
				size_t size = snprintf(NULL, 0, "%lld", arg->data)+1;
				char* s = malloc(size);
				snprintf(s, size, "%lld", arg->data);
				text_Push(&textStack, s);
				free(s);
			}
			break;
		case 2:
			if (1)
			{
				char* str = get_str(arg->data);
				text_Push(&textStack, str);
			}
			break;
		case 3:
			break;
		case 4:
			if (1)
			{
				size_t size = snprintf(NULL, 0, "%lld", arg->Return) + 1;
				char* s = malloc(size);
				snprintf(s, size, "%lld", arg->Return);
				text_Push(&textStack, s);
				free(s);
			}
			break;
		}
		break;
	}
	
}

void block_function_Num(int flag)
{
	Block* block = bf_st.block;
	char* s = get_str(block->name);
	if (SDL_sscanf(s, "%lld", &block->data)); else _error("읽기 에러");
	block->Return = block->data;
}

void block_function_Str(int flag)
{
	Block* block = bf_st.block;
	block->Return = block->data = block->name;
}

void block_function_Var(int flag)
{
	Block* block = bf_st.block;
	if (block->arg == 0) return;
	if (Block_arg_read(block, 1) == -1) return;
	Block* arg = bf_ast.arg[0];
	Block_reader(arg->id);
	block->Return = block->data = arg->data;
}

void block_function_Var_pointer(int flag)
{
	Block* block = bf_st.block;
	if (block->data == 0) return; else
	{
		Block* point = get_Block(block->data);
		if (point == NULL) _error("뭐함"); else
		{
			if (block->arg == 0); else
			{
				Block_reader(block->arg);
				if (Block_arg_read(block, 1) == -1) return;
				Block* arg = bf_ast.arg[0];
				point->data = arg->Return;
				point->Return = point->data;
				printf("%lld", arg->Return);
			}
			block->Return = point->data;

		}
	}
}

void _compiler()
{
	
	bool qc = true;
	Block* block = get_Block(main_block_id);
	if (block == NULL || main_block_id == 0) _error("0%의 확률 통과");
	else
	{
		type_li getch_id = main_block_id;
		while (qc)
		{
			block = get_Block(getch_id);
			if (block == NULL)
			{

			}
			else if (getch_id == 0)
			{
				type_li id = sStack_pop();
				if (id == 0) qc = false;
				else
				{
					block = get_Block(id);
					getch_id = id;
				}
			}
			else
			{
				Block_reader(block->id);
				getch_id = block->next;
				if (block->type == 3 && block->tag == 1)
				{
					sStack_Push(block->next);
					getch_id = block->data;
				}
			}
		}
	}
}

int Block_arg_read(Block* block, int flag)
{
	type_li id;
	Block* _block = NULL;
	if (id = block->arg) if ((_block = get_Block(block->arg)) == NULL) _error("주요 로직에서 못 읽음");
	{
		bf_ast.block = block;
		bf_ast.arg[0] = _block;
		if (_block == NULL) return -1; //_error("가능함?");
		else if (flag == 0);
		else if (flag == 1);
		else for (int i = 1; i < (flag < 8 ? flag : 7); i++)
		{
			id = _block->next;
			if (id == 0) return 0;
			_block = get_Block(id);
			if (_block == NULL) return 0;
			bf_ast.arg[i] = _block;
		}
	}
}

void Block_reader(type_li id)
{
	bf_st_id_Event(id);
	Block* block = bf_st.block;
	switch (block->type)
	{
	case 1: 
		break;
	case 2:
		sum0(id);
		break;
	case 3:
		break;
	case 4:
		switch (block->tag)
		{
		case 1:
			block_function_print(block->flag & 255);
			break;
		case 2:
			break;
		}
		break;
	case 5:
		switch (block->tag)
		{
		case 1:
			block_function_Num(block->flag & 255);
			break;
		case 2:
			block_function_Str(block->flag & 255);
			break;
		case 3:
			block_function_Var(block->flag & 255);
			break;
		case 4:
			block_function_Var_pointer(block->flag & 255);
			break;
		}
		break;
	}
}

void _subWindows()
{
	SDL_ShowWindow(SubWindow);
	SDL_RaiseWindow(SubWindow);
	_compiler();
}

void _subWindows_loop(SDL_Event* event)
{
	SDL_SetRenderDrawColor(SubRenderer, 0, 0, 0, 255);
	SDL_RenderClear(SubRenderer);

	sub_str_draws();

	SDL_RenderPresent(SubRenderer);
}

void sub_str_draw(char* str, SDL_Rect rect)
{
	SDL_Surface* surface = TTF_RenderText_Blended(Mainfont, str, SubColor);
	if (surface == NULL) _error("폰트 오류"); else
	{
		SDL_Texture* texture = SDL_CreateTextureFromSurface(SubRenderer, surface);
		SDL_FreeSurface(surface);
		if (texture == NULL) _error("그냥 오류"); else
		{
			SDL_Rect _rect = {0, };
			SDL_QueryTexture(texture, NULL, NULL, &_rect.w, &_rect.h);
			//_rect.x = rect.x - (_rect.w / 2);
			_rect.x = rect.x;
			_rect.y = rect.y - (_rect.h / 2);
			SDL_RenderCopy(SubRenderer, texture, NULL, &_rect);
			SDL_DestroyTexture(texture); texture = NULL;
		}
		
	}
}

void sub_str_draws()
{
	SDL_Rect rect = { 20, 40, 80, 40 };
	rect.x -= SubWindows.x;
	rect.y -= SubWindows.y;

	for (int i = 0; i < stack_max; i++) 
	{
		if (textStack.top[i / 8] & (1 << (i % 8)))
		{
			if( 20 < rect.y && rect.y < 580 )sub_str_draw(text_peek(&textStack, i), rect);
			rect.y += 40;
		}
	}
}


size_t Stack_get_index(char* top)
{
	for (int i = 1; i < stack_max; i++)
	{
		if (top[i / 8] & 1 << (i % 8)); else return i;
	}
	return stack_max+1;
}
