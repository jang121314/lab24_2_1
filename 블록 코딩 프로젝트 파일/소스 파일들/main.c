

#include "head.h"

#define mainWindow_w 1080
#define mainWindow_h 600

#define SDL_MAIN_HANDLED


SDL_Window* MainWindow;
SDL_Renderer* MainRenderer;
SDL_Window* SubWindow;
SDL_Renderer* SubRenderer;
TTF_Font* Mainfont;

type_li main_block_id;
size_t mainID;
size_t subID;
bool quit = false;

SDL_Rect MainWindows = { 0, 0, 1080, 600 };
SDL_Rect SubWindows = { 0, 0, 400, 600 };
SDL_Rect Block_size = { 0, 0, 80, 40 };
SDL_Rect BlockMargin = { 0, 0, 10, 5 };
SDL_Color BlockColor = { 0, 150, 200, 255 };
SDL_Color BlockValColor = { 200, 150, 0, 255 };
SDL_Color BlockTextColor = { 0 , 200, 125, 125 };
SDL_Color UIColor = { 255, 255, 255, 255 };



Container mainContainer = { {0, }, {0, }, 1, 1, NULL };

void new_Container(Container* renext)
{
	Container* container = malloc(sizeof(Container));
	if (container == NULL) _error(" �����̳� ���� �Ҵ� ���� ");
	else SDL_memset(container, 0, sizeof(Container));
	if (renext == NULL) _error(" �����̳� ���� ���� ");
	else renext->nextContainer = container;
}

void del_Container(Container* renext)
{
	if (renext != NULL)
	{
		del_Container(renext->nextContainer);
		free(renext);
	}
}

/*

typedef struct TextContainer
{
	char* data[1024];
	uint16_t top;
} TextContainer;

TextContainer mainTextContainer = { 0, };

char* newStr(char* format)
{
	char* data = SDL_strdup(format);
	if (data == NULL) _error("�ؽ�Ʈ �Ҵ� ����"); else
	{
		for (int i = 0; i < mainTextContainer.top; i++)
		{
			if (mainTextContainer.data[i] == NULL)
			{
				mainTextContainer.data[i] = data;
				return data;
			}
		}

		if (1023 < mainTextContainer.top) _error("�����̳� Ǯ�� ���� ��"); else
		{
			mainTextContainer.data[mainTextContainer.top++] = data;
		}
		return data;
	}
	_error("�ƴ� ��");
}

void delStr(char** format)
{
	int i = -1;
	while (i < mainTextContainer.top)
	{
		if (mainTextContainer.data[++i] == *format) break;
	}
	if (i < mainTextContainer.top)
	{
		free(*format);
		*format = NULL;
		mainTextContainer.data[i] = NULL;
	}
}

void reStr(char** pointer, char* format)
{
	if (pointer == NULL || format == NULL) _error("����");
	else
	{
		delStr(pointer);
		*pointer = newStr(format);
		if (*pointer == NULL) _error("�Ҵ� ����");
	}
}

void reStrAdd(char** pointer, char* format)
{
	size_t size = snprintf(NULL, 0,  "%s%s", *pointer, format);
	char* data = malloc(size);
	if (data == NULL) _error("�̷�");
	else
	{
		snprintf(data, size, "%s%s", *pointer, format);
		delStr(pointer);
		char* get = newStr(data);
		free(data);
		if (get == NULL) _error("�Ҵ� ����");
		{
			*pointer = get;
		}
		
	}
}

*/

typedef struct
{
	bool down;
	int x, y;
	type_li id;
} Click;

typedef struct
{
	bool down;
	char text[1024];
	unsigned short int top;
	type_li id;
} Block_name_input;

typedef struct
{
	bool down;
	char put[8];
	type_li id;
} Block_Sum_Click;

typedef struct
{
	bool down;
} SubWindowEvent;

funBox functions = { {0, }, };

void functionsEvent()
{
	size_t top = 0;
	void (**s)(int) = functions.fun;

	s[top++] = NULL;

	while (top < 64)
	{
		s[top++] = NULL;
	}
}

SubWindowEvent subWindowEvent = { 0, };

Click MouseClick = { 0, 0, 0, 0 };
Click BlockClick = { 0, 0, 0, 0 };
Block_name_input MainBlock_input = { 0, "", 0, };
Block_Sum_Click MainSumClick = { 0, "+", 0 };
type_li Block_lastClinkBlockId = 0;

void MainSumKeyEvent(char put)
{
	if (put != '\0')
	{
		switch (put)
		{
		case '+':
			snprintf(MainSumClick.put, 2, "%c", put);
			break;
		case '-':
			snprintf(MainSumClick.put, 2, "%c", put);
			break;
		case '*':
			snprintf(MainSumClick.put, 2, "%c", put);
			break;
		case '/':
			snprintf(MainSumClick.put, 2, "%c", put);
			break;
		case '%':
			snprintf(MainSumClick.put, 2, "%c", put);
			break;
		case '=':
			switch (MainSumClick.put[0])
			{
			case '=':
				snprintf(MainSumClick.put, 3, "==");
				break;
			case '!':
				snprintf(MainSumClick.put, 3, "!=");
				break;
			case '<':
				snprintf(MainSumClick.put, 3, "<=");
				break;
			case '>':
				snprintf(MainSumClick.put, 3, ">=");
				break;
			default:
				snprintf(MainSumClick.put, 2, "=");
				break;
			}
			break;
		case '<':
			snprintf(MainSumClick.put, 2, "%c", put);
			break;
		case '>':
			snprintf(MainSumClick.put, 2, "%c", put);
			break;
		case '!':
			snprintf(MainSumClick.put, 2, "%c", put);
			break;
		case '&':
			if(MainSumClick.put[0] == '&') snprintf(MainSumClick.put, 3, "&&");
			else snprintf(MainSumClick.put, 2, "&");
			break;
		case '|':
			if(MainSumClick.put[0] == '|') snprintf(MainSumClick.put, 3, "||");
			else snprintf(MainSumClick.put, 2, "|");
			break;
		}
	}
}



type_li get_MainSumFlag()
{
	type_li flag = 0;
	int (*cmp)(const char* str1, const char* str2) = &SDL_strcmp;
	if (cmp(MainSumClick.put, "+") == 0)
	{
		flag = 1;
	}
	else if (cmp(MainSumClick.put, "-") == 0)
	{
		flag = 2;
	}
	else if (cmp(MainSumClick.put, "*") == 0)
	{
		flag = 3;
	}
	else if (cmp(MainSumClick.put, "/") == 0)
	{
		flag = 4;
	}
	else if (cmp(MainSumClick.put, "%") == 0)
	{
		flag = 5;
	}
	else if (cmp(MainSumClick.put, "==") == 0)
	{
		flag = 11;
	}
	else if (cmp(MainSumClick.put, "!=") == 0)
	{
		flag = 12;
	}
	else if (cmp(MainSumClick.put, "<") == 0)
	{
		flag = 13;
	}
	else if (cmp(MainSumClick.put, "<=") == 0)
	{
		flag = 14;
	}
	else if (cmp(MainSumClick.put, ">") == 0)
	{
		flag = 15;
	}
	else if (cmp(MainSumClick.put, ">=") == 0)
	{
		flag = 16;
	}
	else if (cmp(MainSumClick.put, "&&") == 0)
	{
		flag = 17;
	}
	else if (cmp(MainSumClick.put, "||") == 0)
	{
		flag = 18;
	}
	else if (cmp(MainSumClick.put, "&") == 0)
	{
		flag = 19;
	}
	else if (cmp(MainSumClick.put, "|") == 0)
	{
		flag = 20;
	}
	
	if (1)
	{
		Block* block = get_Block(MainSumClick.id);
		if (block == NULL) _error("�� �÷��� ���� ����"); else
		{
			block->flag = flag;
		}
	}

	return flag;
}

/*
	block UI ��ȹ��
		1��: �̺�Ʈ, ����, �б�, �����, ����
		
			�̺�Ʈ: �Լ� -> main, Ŀ����

			����: ��Ģ����, ��ⷯ ����, ���� ���� �� + - * / % == != < <= > >=

			�б�: �Լ� ȣ��, while, if

			�����: print�� ����

			����: int or char�� ����

		������?

		�̺�Ʈ: B88CC2 event

		����: 33D8E6 calc

		�б�: 9EC935 flow

		�����: C9A436 show

		����: C9622A define


		
			
*/

type_li extraId(type_li input)
{
	static type_li data = 0;
	if (input == 0); else
	{
		data = input;
	}
	return data;
}

typedef struct UIRect
{
	int x;
	int y;
	int w;
	int h;
	char* text;
	int flag; // 512�� ä���, 1024�� �����
	SDL_Color color;
	void (*clickEvent)(int);
} UIRect;

UIRect UIRects[128] = { {0,}, };

SDL_Color _getColor( const unsigned long long int format)
{
	SDL_Color color = { 0, };

	color.r = ((format >> 16) & 255);
	color.g = ((format >> 8) & 255);
	color.b = format & 255;
	color.a = 255;

	return color;
}

void _new_UIRect(size_t index, int x, int y, int w, int h, char* text, int flag, const unsigned long long _color, void (*fun)(int) )
{
	if (x < 0)
	{
		x += MainWindows.w;
	}
	if (y < 0)
	{
		y += MainWindows.h;
	}

	SDL_Color color = _getColor(_color);
	UIRect self = { x, y, w, h, text, flag, color,  fun };
	UIRects[index] = self;
}

void _dragRect(type_li id)
{
	Block* block = get_Block(id);
	if (block == NULL) _error("�巡�� ���� �����ϴ�"); else
	{
		SDL_GetMouseState(&block->drag.x, &block->drag.y);
		block->drag.x += MainWindows.x - (Block_size.w / 2);
		block->drag.y += MainWindows.y - (Block_size.h / 2);
		BlockClick.down = 1;
		BlockClick.id = id;
	}
}

char UI_Flag = 0;
#define if_define_block(_block, _id) \
Block* _block = NULL;  \
_block = get_Block(_id); \
if (_block == NULL) _error("��� ��ũ�� ����") \


void UIRectf_flag()
{
	UIRects[6].flag |= 1024;
	UIRects[7].flag |= 1024;
	UIRects[8].flag |= 1024;
	UIRects[9].flag |= 1024;
	UIRects[11].flag |= 1024;
	UIRects[12].flag |= 1024;
	UIRects[13].flag |= 1024;
	switch (UI_Flag)
	{
	case 0:
		
		break;
	case 1:
		UIRects[6].flag &= ~(1024);
		UIRects[9].flag &= ~(1024);
		break;
	case 3:
		UIRects[7].flag &= ~(1024);
		break;
	case 5:
		UIRects[8].flag &= ~(1024);
		UIRects[11].flag &= ~(1024);
		UIRects[12].flag &= ~(1024);
		UIRects[13].flag &= ~(1024);
		break;
	}

}

void UIRectf_Event(int flag)
{
	UI_Flag = 1;
	UIRectf_flag();
}
void UIRectf_Sum(int flag)
{
	type_li id = new_Block();
	if_define_block(block, id); else
	{
		block->type = 2;
		block->tag = 0;
		block->flag = 1;
		char* putname = "Sum";
		size_t size = snprintf(NULL, 0, "%s", putname) + 1;
		char* name = get_str(block->name);
		snprintf(name, size, "%s", putname);
		_dragRect(id);
	}
}
void UIRectf_Flow(int flag)
{
	UI_Flag = 3;
	UIRectf_flag();
}
void UIRectf_Def(int flag)
{
	UI_Flag = 5;
	UIRectf_flag();
}

void UIRectf_Print(int flag)
{
	type_li id = 0;
	Block* block = NULL;
	if(1)
	{
		id = new_Block();
		if (id == 0) _error("�ƹ��� �ϵ� ������");
		block = get_Block(id);
		if (block == NULL) _error("����� �������� �ִ�.");
	}
	if (block != NULL)
	{
		block->type = 4;
		block->tag = 1;
		char* putname = "print";
		size_t size = snprintf(NULL, 0, "%s", putname)+1;
		char* name = get_str(block->name);
		snprintf(name, size, "%s", putname);
		
	}
	_dragRect(id);
}

void UIRectf_Begin(int flag)
{
	static bool frist = true;
	type_li id = 0;
	id = new_Block();
	if (id == NULL) _error("�̷����� ������?"); else
	{
		if_define_block(block, id); else
		{
			block->type = 1;
			block->tag = frist ? 1 : 2; 
			char* putname = frist ? "main" : "Event";
			if (frist) main_block_id = id;
			if(frist) frist = false;
			size_t size = snprintf(NULL, 0, "%s", putname) + 1;
			char* name = get_str(block->name);
			snprintf(name, size, "%s", putname);
			_dragRect(id);
		}
	}
}

void UIRectf_Num(int flag)
{
	type_li id = 0;
	id = new_Block();
	if (id == NULL) _error("�̷����� ����?"); else
	{
		if_define_block(block, id); else
		{
			block->type = 5;
			block->tag = 1;
			block->data = 1;
			char* putname = "1";
			size_t size = snprintf(NULL, 0, "%s", putname) + 1;
			char* name = get_str(block->name);
			snprintf(name, size, "%s", putname);
			_dragRect(id);
		}
	}
}

void UIRectf_Str(int flag)
{
	type_li id = 0;
	id = new_Block();
	if (id == NULL) _error("�̷����� ����?"); else
	{
		if_define_block(block, id); else
		{
			block->type = 5;
			block->tag = 2;
			char* putname = "\" \"";
			size_t size = snprintf(NULL, 0, "%s", putname) + 1;
			char* name = get_str(block->name);
			snprintf(name, size, "%s", putname);
			_dragRect(id);
		}
	}
}

void UIRectf_Var(int flag)
{
	type_li id = 0;
	id = new_Block();
	if (id == NULL) _error("�̷����� ����?"); else
	{
		if_define_block(block, id); else
		{
			block->type = 5;
			block->tag = 3;
			char* putname = "Var";
			size_t size = snprintf(NULL, 0, "%s", putname) + 1;
			char* name = get_str(block->name);
			snprintf(name, size, "%s", putname);
			_dragRect(id);
		}
	}
}

void UIRectf_upload(int flag)
{
	subWindowEvent.down = 1;
	_subWindows();
}

void UIRects_spawn()
{
	size_t index = 0;
	void (*_new)(size_t, int, int, int, int, char*, int, const unsigned long long, void (*)(int)) = &_new_UIRect;
	_new(index++, 80, 150, 110, 250, NULL, 512, 0xF7D19B, NULL); // 0
	_new(index++, 80, 50, 100, 40, "Event", 512, 0xB88CC2, UIRectf_Event);
	_new(index++, 80, 100, 100, 40, "Sum", 512, 0x33D8E6, UIRectf_Sum);
	_new(index++, 80, 150, 100, 40, "Flow", 512, 0x9EC935, UIRectf_Flow);
	_new(index++, 80, 200, 100, 40, "Print", 512, 0xC9A436, UIRectf_Print);
	_new(index++, 80, 250, 100, 40, "Def", 512, 0xC9622A, UIRectf_Def);
	_new(index++, 190, 150, 110, 250, NULL, 1536, 0xB88CC2, NULL); // 6
	_new(index++, 190, 150, 110, 250, NULL, 1536, 0x9EC935, NULL);
	_new(index++, 190, 150, 110, 250, NULL, 1536, 0xC9622A, NULL);
	_new(index++, 190, 50, 100, 40, "Begin", 1024, 0xFFFFFF, UIRectf_Begin); // 9

	_new(index++, -100, 60, 120, 40, "upload", 512, 0xF7D19B, UIRectf_upload);// 10
	_new(index++, 190, 50, 100, 40, "Num", 1024, 0xFFFFFF, UIRectf_Num); // 11
	_new(index++, 190, 100, 100, 40, "Str", 1024, 0xFFFFFF, UIRectf_Str);
	_new(index++, 190, 150, 100, 40, "Var", 1024, 0xFFFFFF, UIRectf_Var);
}

void UIRects_draw()
{
	size_t size = sizeof(UIRects) / sizeof(UIRects[0]);
	for (int i = 0; i < size; i++)
	{
		if (UIRects[i].h || UIRects[i].w); else break;
		if (UIRects[i].flag & 1024) continue;
		UIRect s = UIRects[i];
		
		SDL_Rect rect = { s.x, s.y, s.w, s.h };

		if (1)
		{
			SDL_Rect _rect = rect;
			_rect.x -= _rect.w / 2;
			_rect.y -= _rect.h / 2;
			SDL_SetRenderDrawColor(MainRenderer, s.color.r, s.color.g, s.color.b, s.color.a);
			if (s.flag & 512)
			{
				SDL_RenderFillRect(MainRenderer, &_rect);
			} else
			{
				SDL_RenderDrawRect(MainRenderer, &_rect);
			}
		}

		if ( s.text != NULL )
		{
			SDL_Surface* sur = TTF_RenderText_Blended(Mainfont, s.text, UIColor);
			if (sur == NULL) _error("�̹��� ���� ���� �̽ʴϱ�?"); else
			{
				SDL_Texture* ure = SDL_CreateTextureFromSurface(MainRenderer, sur);
				SDL_FreeSurface(sur); sur = NULL;
				if (ure == NULL) _error("�� �׷��ʴϱ�?");
				{
					SDL_Rect _rect = { 0, };
					SDL_QueryTexture(ure, NULL, NULL, &_rect.w, &_rect.h);

					_rect.x = rect.x - ( _rect.w)/2;
					_rect.y = rect.y - ( _rect.h)/2;
					SDL_RenderCopy(MainRenderer, ure, NULL, &_rect);
					SDL_DestroyTexture(ure); ure = NULL;
				}

			}
		}





		
	}
}

char UIRect_Click()
{
	size_t size = sizeof(UIRects) / sizeof(UIRects[0]);
	for (int i = 0; i < size; i++)
	{
		UIRect s = UIRects[i];
		if ((s.h < 1) || (s.w < 1) || (s.flag & 1024) || (s.clickEvent == NULL)) continue;
		
		SDL_Rect rect = {
			.x = s.x - s.w / 2, .y = s.y - s.h / 2, .w = s.w, .h = s.h
		};


		char b = 0;
		if (1)
		{
			int x = 0;
			int y = 0;
			SDL_GetMouseState(&x, &y);
			x -= rect.x;
			y -= rect.y;
			b = (0 < x && x < rect.w) && (0 < y && y < rect.h);
		}

		if (b)
		{
			if (s.clickEvent == NULL);
			else s.clickEvent(s.flag & 255);
			return 1;
		}
	}

	return 0;
}




/*
Block* get_Block(type_li id)
{
	Container* container = &mainContainer;
	while (ContainerSize <= id) if ((container = container->nextContainer) == NULL) return NULL; else id -= ContainerSize;
	Block* block = &(container->block[id]);
	if (block->id == id) return block;
	else _error("��� ��ġ�� ������ �����ϴ�.");
	return NULL;
}

type_li new_Block()
{
	Container* container = &mainContainer;
	while (ContainerSize < container->block_top + 2) if (container->nextContainer == NULL) new_Container(container); else container = container->nextContainer;

	type_li id = container->block_top++;
	Block block = (Block){ id, 0, 0, NULL, 0, (DragState) { 0, 0, 0 }, "null", NULL, NULL, NULL };
	container->block[id] = block;
	return id;
}
*/

Block* get_Block(type_li id)
{
	Container* container = &mainContainer;

	while (63 < id)
	{
		if (container->nextContainer == NULL) new_Container(container);
		else
		{
			container = container->nextContainer;
			id -= 64;
		}
	}

	return &container->block[id];
}

type_li new_Block()
{
	Container* container = &mainContainer;
	size_t block_id = mainContainer.block_top;
	type_li name = new_str("NULL");

	
	Block block = (Block){ block_id, 0, 0, 0, 0, (DragState){ 0, 0, 0 }, name, NULL, NULL, NULL, 0 };
	while (63 < block_id)
	{
		if (container->nextContainer == NULL) new_Container(container);
		else 
		{
			container = container->nextContainer;
			block_id -= 64;
		}
	}

	container->block[block_id] = block;
	mainContainer.block_top++;
	return block.id;
}

char* get_str(type_li id)
{
	Container* container = &mainContainer;
	while (63 < id)
	{
		if (container->nextContainer == NULL) new_Container(container);
		else
		{
			container = container->nextContainer;
			id -= 64;
		}
	}
	return container->str[id];
}

type_li new_str(char* format)
{
	Container* container = &mainContainer;
	size_t id = mainContainer.str_top;
	while (63 < id)
	{
		if (container->nextContainer == NULL) new_Container(container);
		else
		{
			container = container->nextContainer;
			id -= 64;
		}
	}

	size_t size = snprintf(NULL, 0, "%s", format);
	size = size < 64 ? size : 64;
	SDL_memcpy(container->str[id], format, 64);
	mainContainer.str_top++;
	return mainContainer.str_top-1;
}

SDL_Texture* drawText( char* text, TTF_Font* font, SDL_Color color, SDL_Renderer* renderer )
{
	SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
	if (surface == NULL) _error("�ؽ�Ʈ ���ǽ� ������ �����Ͽ����ϴ�.");
	else 
	{
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		if (texture == NULL) _error("�ؽ�Ʈ �ؽ��� ������ �����Ͽ����ϴ�.");
		else return texture;
	}
	return NULL;
}


void BlockDrawText(type_li id)
{
	Block* block = get_Block(id);
	if (block == NULL) _error("��Ͽ� �ؽ�Ʈ�� �׸��⸦ ������ �� �����ϴ�."); else
	{
		if (get_str(block->name) == NULL || get_str(block->name)[0] == '\0') return;
		size_t textSize = 0;
		//char* text = malloc(textSize = snprintf(NULL, 0, "%s: %llu", get_str(block->name), block->data)+1);
		//if (text && snprintf(text, textSize, "%s: %llu", get_str(block->name), block->data)); else _error("�ؽ�Ʈ ���� ����");

		SDL_Texture* texture = drawText(get_str(block->name), Mainfont, BlockTextColor, MainRenderer);
		if (texture==NULL) _error("�Ұ����� ����� ���Դϴ�");
		else
		{
			SDL_Rect rect = { 0 };
			SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
			rect.y = block->drag.y + (Block_size.h - rect.h) / 2 - MainWindows.y;
			rect.x = block->drag.x + (Block_size.w - rect.w) / 2 - MainWindows.x;
			SDL_RenderCopy(MainRenderer, texture, NULL, &rect);
			SDL_DestroyTexture(texture);
		}
		//free(text);
	}
}


int _Draw_blocks_get_arg_next(type_li id)
{
	Block* block = NULL;
	if (NULL == (block = get_Block(id))) _error("����� �ϴ� ���̸� ���ϴ� �κп��� ������ ����ٴ� ���"); else
	{
		if (block->arg == 0 || ((block = get_Block(block->arg)) == NULL)) return 0;
		int i = 0;
		while (!(block->next == 0 || ((block = get_Block(block->next)) == NULL))) i++;
		return i;
	}
	return 0;
}

void block_Draw(type_li id)
{
	Block* block = get_Block(id);
	if (block == NULL) _error("�׸� ����� �����ϴ�."); else
	{
		DragState drag = block->drag;
		SDL_Rect rect = { drag.x - MainWindows.x, drag.y - MainWindows.y, Block_size.w, Block_size.h };
		if(block->type==5 && block->tag==4) SDL_SetRenderDrawColor(MainRenderer, BlockValColor.r, BlockValColor.g, BlockValColor.b, BlockValColor.a);
		else if (block->type == 3 && block->tag == 1) SDL_SetRenderDrawColor(MainRenderer, BlockValColor.r, BlockValColor.g, BlockValColor.b, BlockValColor.a);
		else SDL_SetRenderDrawColor(MainRenderer, BlockColor.r, BlockColor.g, BlockColor.b, BlockColor.a);
		SDL_RenderDrawRect(MainRenderer, &rect);
		BlockDrawText(id);
	}
	return;
}

void _Draw_blocks(type_li id)
{

	Block* block = get_Block(id);
	if (block == NULL) _error("����"); else
	{

		block_Draw(id);

		type_li __id = 0;
		if (block == NULL) _error("����");
		else if (block->next != 0)
		{
			int x = block->drag.x;
			int y = block->drag.y;
			y += (Block_size.h + BlockMargin.h) * (1 + _Draw_blocks_get_arg_next(id));

			__id = block->next;
			Block* block = get_Block(__id);
			if (block == NULL) _error("�μ��� ���ٴ� ���"); else
			{
				block->drag.x = x;
				block->drag.y = y;
				_Draw_blocks(__id);
			}

		}

		__id = 0;
		if (block->arg != 0)
		{
			int x = block->drag.x;
			int y = block->drag.y;
			x += Block_size.w + BlockMargin.w;

			__id = block->arg;
			Block* block = get_Block(__id);
			if (block == NULL) _error("�μ��� ���ٴ� ���"); else
			{
				block->drag.x = x;
				block->drag.y = y;
				_Draw_blocks(__id);
			}

		}


	}


}



void Draw_blocks()
{
	if (mainContainer.block_top < 1) return;
	for (type_li i = 1; i < mainContainer.block_top; i++)
	{
		Block* block = get_Block(i);
		if (block == NULL) _error("����"); else if (block->renext == 0)
		{
			_Draw_blocks(i);
		}

	}
}

type_li blockCilck(int x, int y, type_li id)
{
	Block* block = get_Block(id);
	if (block == NULL) _error("�ƴ� ��"); else
	{
		DragState drag = block->drag;
		int dx = x - drag.x + MainWindows.x;
		int dy = y - drag.y + MainWindows.y;
		if (0 < dx && 0 < dy && dx < Block_size.w && dy < Block_size.h) return block->id;
		else return 0;
	}
}

type_li blocksCilck(int x, int y)
{
	for (type_li i = 1; i < mainContainer.block_top; i++)
	{
		if (blockCilck(x, y, i) != 0) return i;
	}
	return 0;
}

void recatBlock(type_li a)
{
	Block* ablock = get_Block(a);
	if (ablock == NULL) _error("���� ����"); else
	{
		Block* vblock = get_Block(ablock->renext);
		if (vblock == NULL) _error("���� ����"); else
		{
			ablock->renext = 0;
			if (vblock->next == a) vblock->next = 0;
			else if (vblock->arg == a) vblock->arg = 0;
		}

		ablock->drag.x += (Block_size.w >> 3);
		ablock->drag.y += (Block_size.h >> 3);
	}

}



void __catBlock(type_li a, type_li v, type_li flag)
{
	Block* ablock = get_Block(a);
	Block* vblock = get_Block(v);
	if (ablock == NULL || vblock == NULL) _error("���� ����"); else
	{
		type_li nid = 0;
		if (flag == 1)
		{
			nid = ablock->next;
			ablock->next = v;
		}
		else if (flag == 2)
		{
			nid = ablock->arg;
			ablock->arg = v;
		}

		vblock->renext = a;
		recatBlock(nid);

	}
	return;
}
void catNextBlock(type_li a, type_li v)
{
	__catBlock(a, v, 1);
}
void catArgBlock(type_li a, type_li v)
{
	Block* block = get_Block(a);
	if (block == NULL) _error("���ڷ� ���� ����"); else
	{
		if(block ->type != 1)
		{
			__catBlock(a, v, 2);
		}
	}
}

/*
void spawnBlock()
{
	Block* block = get_Block(new_Block());
	if (block == NULL) _error("��� ������ �� �Ǿ����ϴ�."); else
	{
		SDL_GetMouseState(&block->drag.x, &block->drag.y);
		block->drag.x += windows.x - (Block_size.w / 2);
		block->drag.y += windows.y - (Block_size.h / 2);
	}
}
*/

void __error(const char* text, const unsigned int line)
{
	printf("line %d, error: ", line);
	puts(text);
	exit(-1);
}
#define _error(text) __error(text, __LINE__ )

void SDL_ini()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) _error("SDL �ʱ�ȭ ����");
	if (TTF_Init() < 0) _error("SDL �ؽ�Ʈ �ʱ�ȭ ����");
	if (!(Mainfont = TTF_OpenFont("C:\\Windows\\Fonts\\Arial.ttf", 16))) _error("��Ʈ ���� ����");
	functionsEvent();
}




void _Event(SDL_Event* event)
{

	while (SDL_PollEvent(event) != 0)
	{
		SDL_Window* window = SDL_GetWindowFromID(event->window.windowID);
		if (window == NULL) continue; //_error("�����츦 ã�� ���߽��ϴ�.");
		SDL_Renderer* renderer = SDL_GetRenderer(window);
		if (renderer == NULL) _error("�������� ã�� ���߽��ϴ�.");

		const short int windowTag = (event->window.windowID == mainID ? 1 : (event->window.windowID == subID ? 2 : 0)); // 1�� ����, 2�� ����


		switch (event->type)
		{

		case SDL_QUIT:
			quit = true;
			break;
		case SDL_WINDOWEVENT:
			if (event->window.event == SDL_WINDOWEVENT_CLOSE)
				if (windowTag == 2)
				{
					SDL_HideWindow(window);
					subWindowEvent.down == 0;
				}
				else if (windowTag == 1) quit = true; else;
			break;

		case SDL_KEYDOWN:
			if (event->key.keysym.sym == SDLK_BACKSPACE)
			{
				if (MainBlock_input.down == 1)
				{
					if (1 < MainBlock_input.top) MainBlock_input.text[MainBlock_input.top-= 1] = '\0';
					else if (MainBlock_input.top == 1) MainBlock_input.text[MainBlock_input.top-= 1] = ' ';
					Block* block = get_Block(MainBlock_input.id);
					if (block == NULL) _error("�ؽ�Ʈ �Է� �߿� ��� �� �ҷ���"); else
					{
						char* name = get_str(block->name);
						size_t size = snprintf(NULL, 0, "%s", MainBlock_input.text)+1;
						snprintf(name, size, "%s", MainBlock_input.text);

					}
				}
			}
			break;

		case SDL_TEXTINPUT:
			if(MainBlock_input.down == 1)
			{
				bool this_type = false;
				Block* block = get_Block(MainBlock_input.id); if (block == NULL) _error("�ؽ�Ʈ �Է� �߿� ��� �� �ҷ���"); else
				if(MainBlock_input.top < 64)
				{
					if (block->type == 5 ? block->tag == 1 ? this_type = true : 0 : 0)
					{
						char str[8] = { 0, };
						snprintf(str, 8, "%s", event->text.text);
						if (str[0] == '-' && MainBlock_input.text[0] == ' ')
						{

						}
						else if (str[0] < '0' || '9' < str[0]) break;
					}

					if (MainBlock_input.top == 0 )
					{
						size_t size = snprintf(NULL, 0, "%s", event->text.text)+1;
						snprintf(MainBlock_input.text, size, "%s", event->text.text);
						MainBlock_input.top = size-1;
					}
					else
					{
						size_t size = snprintf(NULL, 0, "%s%s", MainBlock_input.text, event->text.text) + 1;
						snprintf(MainBlock_input.text, size, "%s%s", MainBlock_input.text, event->text.text);
						MainBlock_input.top = size - 1;
					}

					if (1) 
					{
						char* name = get_str(block->name);
						size_t size = snprintf(NULL, 0, "%s", MainBlock_input.text) + 1;
						snprintf(name, size, "%s", MainBlock_input.text);
					}
				}
			}
			else if (MainSumClick.down == 1)
			{
				char put = event->text.text[0];
				MainSumKeyEvent(put);
				get_MainSumFlag();
			}

			break;

		case SDL_MOUSEBUTTONUP:
			
			if (MainBlock_input.down == 1)
			{
				MainBlock_input.down = 0;
				SDL_StopTextInput();
			}
			else if (BlockClick.down == 1)
			{
				type_li __id = 0;
				Block* block = get_Block(BlockClick.id);
				if (block == NULL) _error("����");
				else if (block->type != 1 && (__id = blocksCilck(event->motion.x - Block_size.w, event->motion.y)))
				{
					catArgBlock(__id, BlockClick.id);
				}
				else if (__id = blocksCilck(event->motion.x, event->motion.y - Block_size.h))
				{
					catNextBlock(__id, BlockClick.id);
				}
			}
			
			if (event->button.button == SDL_BUTTON_RIGHT)
			{
				if (MainSumClick.down == 1)
				{
					SDL_StopTextInput();
					MainSumClick.down = 0;
					Block* block = get_Block(MainSumClick.id);
					if (block == NULL) _error("�ι� Ŭ���� ���� �߳�"); else
					{
						type_li flag = get_MainSumFlag();
						block->flag = flag;
						char* name = get_str(block->name);
						size_t size = snprintf(NULL, 0, "Sum: %s", MainSumClick.put) + 1;
						snprintf(name, size, "Sum: %s", MainSumClick.put);
					}
				}
				else
				{
					type_li __id = 0;
					if ((__id = blocksCilck(event->motion.x, event->motion.y)) != 0)
					{
						Block* block = get_Block(__id);
						if (block == NULL) _error("ã�� ����� ���ٳ�");
						else
						{
							if (block->type != 2)
							{
								MainBlock_input.down = 1;
								MainBlock_input.id = __id;
								MainBlock_input.top = 0;
								MainBlock_input.text[0] = '\0';
								SDL_StartTextInput();
							}
							else if (block->type == 2)
							{
								
								MainSumClick.down = 1;
								SDL_StartTextInput();
								MainSumClick.id = __id;
								char* name = get_str(block->name);
								size_t size = snprintf(NULL, 0, "Sum: %s", MainSumClick.put) + 1;
								snprintf(name, size, "Sum: %s", MainSumClick.put);
							}
						}
					}
					// else spawnBlock();
				}
			}

			MouseClick.down = 0;
			BlockClick.down = 0;
			break;


		case SDL_MOUSEBUTTONDOWN:

			if( event->button.button == SDL_BUTTON_LEFT )
			{
				type_li __id = 0;
				if (windowTag == 2)
				{
					MouseClick = (Click){ 1, event->motion.x + SubWindows.x, event->motion.y + SubWindows.y };
				}
				else if ( UIRect_Click() )
				{

				}
				//else if ((__id = blocksCilck(event->motion.x, event->motion.y)) == 0) MouseClick = (Click){ 1, event->motion.x + MainWindows.x, event->motion.y + MainWindows.y }; else
				else if( __id = blocksCilck(event->motion.x, event->motion.y) )
				{
					BlockClick.down = 1;
					BlockClick.id = __id;

					Block* block = get_Block(__id);
					if (block == NULL) _error("��ٰ� ������"); else
					{
						recatBlock(__id);

						if (block->type == 1 && block->tag == 1)
						{
							main_block_id = block->id;
						}
					}
					Block_lastClinkBlockId = __id;
				}
				else if (1)
				{
					if (windowTag == 1)
					{
						MouseClick = (Click){ 1, event->motion.x + MainWindows.x, event->motion.y + MainWindows.y };
					}
				}
			}
			
			break;


		case SDL_MOUSEMOTION:

			if (MouseClick.down == 1)
			{
				if(windowTag == 1)
				{
					MainWindows.x = MouseClick.x - event->motion.x;
					MainWindows.y = MouseClick.y - event->motion.y;
				}
				else if (windowTag == 2)
				{
					//SubWindows.x = MouseClick.x - event->motion.x;
					SubWindows.y = MouseClick.y - event->motion.y;
				}
			}
			else if (BlockClick.down == 1)
			{
				Block* block = get_Block(BlockClick.id);
				block->drag.x = event->motion.x - (Block_size.w / 2) + MainWindows.x;
				block->drag.y = event->motion.y - (Block_size.h / 2) + MainWindows.y;
			}
			break;

		}



		if ( !MainBlock_input.down ) if (event->type == SDL_KEYUP) switch (event->key.keysym.sym)
		{
		case SDLK_SPACE:
			//spawnBlock();
			break;
		case SDLK_c:
			if (1)
			{
				if (Block_lastClinkBlockId == 0) continue;
				type_li id = new_Block();
				Block* block = get_Block(id);
				Block* blocked = get_Block(Block_lastClinkBlockId);
				if (NULL == blocked || block == NULL) _error("��� ���� ����");
				{
					Block backup = *block;
					*block = *blocked;
					block->id = id;
					block->renext = 0;
					block->next = 0;
					block->arg = 0;
					size_t size = snprintf(NULL, 0, "%s", get_str(block->name))+1;
					block->name = backup.name;
					snprintf(get_str(block->name), size, "%s", get_str(blocked->name));
					if (block->type == 5 && (block->tag == 3))
					{
						block->name = blocked->name;
						block->data = Block_lastClinkBlockId;
						block->tag = 4;
					}
					else  if(block->type == 1 && block->tag == 2)
					{
						block->name = blocked->name;
						block->data = Block_lastClinkBlockId;
						block->type = 3;
						block->tag = 1;
					}
					SDL_GetMouseState(&block->drag.x, &block->drag.y);
					block->drag.x += MainWindows.x - (Block_size.w / 2);
					block->drag.y += MainWindows.y - (Block_size.h / 2);
				}
			}
			break;
		}






	}
}

void loop()
{
	SDL_Event event;
	while (!quit)
	{
		_Event(&event);

		if(quit)
		{
			break;
		}

		SDL_SetRenderDrawColor(MainRenderer, 255, 255, 255, 255);
		SDL_RenderClear(MainRenderer);

		SDL_SetRenderDrawColor(MainRenderer, 255, 0, 255, 255);
		UIRects_draw();
		SDL_SetRenderDrawColor(MainRenderer, 0, 0, 255, 255);
		Draw_blocks();

		SDL_RenderPresent(MainRenderer);

		if(subWindowEvent.down) _subWindows_loop(&event);

	}
}

void create_window()
{
	SDL_ini();
	MainWindow = SDL_CreateWindow("main", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, MainWindows.w, MainWindows.h, SDL_WINDOW_SHOWN);
	if (MainWindow == NULL) _error("���� �����찡 �������� �ʾҽ��ϴ�.");
	MainRenderer = SDL_CreateRenderer(MainWindow, -1, SDL_RENDERER_ACCELERATED);
	if (MainRenderer == NULL) _error("���� �������� �������� �ʾҽ��ϴ�.");

	SubWindow = SDL_CreateWindow("sub", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SubWindows.w, SubWindows.h, SDL_WINDOW_HIDDEN);
	if (SubWindow == NULL) _error("���� �����찡 �������� �ʾҽ��ϴ�.");
	SubRenderer = SDL_CreateRenderer(SubWindow, -1, SDL_RENDERER_ACCELERATED);
	if (SubRenderer == NULL) _error("���� �������� �������� �ʾҽ��ϴ�.");

	UIRects_spawn();
	SDL_SetRenderDrawColor(MainRenderer, 255, 255, 255, 255);
	SDL_RenderClear(MainRenderer);

	mainID = SDL_GetWindowID(MainWindow);
	subID = SDL_GetWindowID(SubWindow);


	loop();
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	SQL_load();
	create_window();
	SQL_save();
	//del_Container(mainContainer.nextContainer);
	return 0;
}