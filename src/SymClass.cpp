/*
* LXS
* Copyright (C) 2010 nex
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <lxs.h>

void 
sym_read(sym_code_t * code)
{
	switch(code->mode)
	{
		default:
			std::cin >> code->mem[code->op_code];
			break;
	}
}

void
sym_write(sym_code_t * code)
{
	switch(code->mode)
	{
		case 0:
			std::cout << code->mem[code->op_code];
			break;
		case 1:
			std::cout << std::hex << code->mem[code->op_code];
			break;
		case 2:
			toBin(code->mem[code->op_code]);
			break;
		case 3:
			std::cout << (char)(code->mem[code->op_code]+'A');
			break;
		default:
			std::cout << code->mem[code->op_code];
			break;
	}

	std::cout << std::endl;
}

void 
sym_pop(sym_code_t * code)
{
	code->mem[code->op_code] = code->eax;
}

void
sym_push(sym_code_t * code)
{
	code->eax = code->mem[code->op_code];
}

void 
sym_add(sym_code_t * code)
{
	code->eax += code->mem[code->op_code];
}

void
sym_sub(sym_code_t * code)
{
	code->eax -= code->mem[code->op_code];
}

void
sym_mul(sym_code_t * code)
{
	code->eax *= code->mem[code->op_code];
}

void
sym_div(sym_code_t * code)
{
	if(code->mem[code->op_code] != 0)
		code->eax /= code->mem[code->op_code];
}

void
sym_mod(sym_code_t * code)
{
	if(code->mem[code->op_code] != 0)
		code->eax %= code->mem[code->op_code];
}

void 
sym_and(sym_code_t * code)
{
	code->eax &= code->mem[code->op_code];
}

void 
sym_or(sym_code_t * code)
{
	code->eax |= code->mem[code->op_code];
}

void
sym_xor(sym_code_t * code)
{
	code->eax ^= code->mem[code->op_code];
}

void 
sym_not(sym_code_t * code)
{
	code->eax = !code->mem[code->op_code];
}

void
sym_shl(sym_code_t * code)
{
	code->eax <<= code->mem[code->op_code];
}

void
sym_shr(sym_code_t * code)
{
	code->eax >>= code->mem[code->op_code];
}

void
sym_del(sym_code_t * code)
{
	code->mem[code->op_code] = 0;
}

void
sym_nop(sym_code_t * code)
{
	/* NULL :D */ 
}

void
sym_jmp(sym_code_t * code)
{
	code->ip = code->op_code-1;
}

void
sym_cmp(sym_code_t * code)
{
	if (code->eax == code->mem[code->op_code])
		code->flag = 0;
	else if (code->eax < code->mem[code->op_code])
		code->flag = 1;
	else if (code->eax > code->mem[code->op_code])
		code->flag = 2;
}

void 
sym_jn(sym_code_t * code)
{
	if ( code->flag != 0 )
		code->ip = code->op_code-1;
}

void
sym_jz(sym_code_t * code)
{
	if (!code->flag)
		code->ip = code->op_code-1;
}

void
sym_jm(sym_code_t * code)
{
	if (code->flag == 1)
		code->ip = code->op_code-1;
}

void 
sym_jg(sym_code_t * code)
{
	if (code->flag == 2)
		code->ip = code->op_code-1;
}

void 
sym_exit(sym_code_t * code)
{
	std::cout << "[?]Exit code: " << code->op_code << std::endl;
	
	code->ip = code->count;
}

void
sym_chmod(sym_code_t * code)
{
	code->mode = code->op_code;
}

void
sym_inc(sym_code_t * code)
{
	code->mem[code->op_code]++;
}

void
sym_dec(sym_code_t * code)
{
	code->mem[code->op_code]--;
}

void 
sym_call(sym_code_t * code)
{
	code->old_ip = code->ip + 1;

	code->ip = code->op_code-1;
}

void 
sym_ret(sym_code_t * code)
{
	code->ip = code->old_ip - 1;
}

void 
sym_stpush(sym_code_t * code)
{
	code->stack.mem = (int*)realloc(code->stack.mem,++code->stack.mm_len * sizeof(int));

	code->stack.mem[code->stack.mm_len-1] = code->mem[code->op_code];
}

void 
sym_stpop(sym_code_t * code)
{
	code->mem[code->op_code] = code->stack.mem[code->stack.mm_len-1];
	code->stack.mem = (int*)realloc(code->stack.mem,--code->stack.mm_len * sizeof(int));
}

void
toBin(int num)
{
	int a;

	if ((a = num / 2))
		toBin(a);

	std::cout << (char)((num % 2)+'0');
}

SymClass::SymClass()
{
	this->simpletron.op_code = 0;
	this->simpletron.count = 0;
	this->simpletron.eax = 0;
	this->simpletron.flag = -1;
	this->simpletron.ip = 0;
	this->simpletron.mode = 0;
	this->simpletron.stack.mem = (int*)malloc(sizeof(int));
	this->simpletron.stack.mm_len = 0;
}

int
SymClass::atoi(char * line)
{
	int ret;
 
	for(ret = 0;*line;line++)
	{
		if (*line >= '0' && *line <= '9')
			ret = ret * 10 + ((int)*line - '0');
		else
			break;
	}

	return ret;
}

int
SymClass::execute()
{
	int op = 0;
	int len = this->simpletron.count;
	
	for(this->simpletron.ip = 0;this->simpletron.ip < len;this->simpletron.ip++)
		op = execute_op(this->simpletron.mem[this->simpletron.ip]);

	return 0;
}

int
SymClass::execute_op(int op)
{
	int call = (op / 100)-10;
	
	this->simpletron.op_code = op % 100;

	if (op == -9999)
		return 0;
	
	if(call >= 0 && call <= MAX_CALL)
		sym_code_table[call](&simpletron);
	
	return 0;
}

void
SymClass::dump()
{
	int i;

	std::cout << std::dec << "\n=== DUMP ===\n";
	std::cout << "AX :\t\t\t" << this->simpletron.eax << "\n";
	std::cout << "Instruction no. :\t" << this->simpletron.count << "\n";
	std::cout << "Operation Code :\t" << this->simpletron.op_code << "\n";
	std::cout << "\n";
	std::cout << "Memory:\n";

	for (i = 0; i < 10; i++)
		printf(" %5d", i);

	for (i = 0; i < 100; i++)
	{
		if (i % 10 == 0) 
			std::cout << "\n" << i << ((!i) ? " " : "");
		printf(" %+.4d", this->simpletron.mem[i]);
	}

	std::cout << "\n";
}

void
SymClass::stdin_read()
{
	int op;
		
	while((op != -9999) && (this->simpletron.count < MAX_MEM))
	{
		printf("%02d ? ",this->simpletron.count);
		std::cin >> op;
		
		this->simpletron.mem[this->simpletron.count++] = op;
	}
}

int
SymClass::readfile(char * name)
{
	std::ifstream file(name,std::ifstream::in);
	char * line = new char[256];
	int op;
	
	if (!file.is_open())
		return 1;
		
	while(!file.eof())
	{
		
		file.getline(line,256);

		if(line[0] == '#' || line[0] == '\0')
			continue;

		op = this->atoi(line);

		this->simpletron.mem[this->simpletron.count++] = op;
	}
	
	file.close();
	
	delete [] line;
	
	return 0;
}

int
SymClass::read_bin(char * name)
{
	std::ifstream file(name,std::ifstream::binary);
	char * line = new char[4];
	int i;
	
	if (!file.is_open())
		return 1;
		
	file.read(line,4);
	
	if(strcmp(line,BIN_MAGIC))
		return 2;
		
	while(!file.eof())
	{
		file.read(line,4);

		for(i=0;i<4;i++)
			if (line[i] >= 0 && line[i] <= 9)
				line[i] += '0';

		this->simpletron.mem[this->simpletron.count++] = this->atoi(line);
	}
	
	file.close();

	delete [] line;
	
	return 0;
}

int
SymClass::pseudo_compile(char * name,char * out)
{
	std::ifstream file(name,std::ifstream::in);
	std::ofstream  outf(out,std::ofstream::binary);
	char * line = new char[256];
	int i;
	
	if (!file.is_open() || !outf.is_open())
		return 1;
		
	strcpy(line,BIN_MAGIC);
	
	outf.write(line,4);
		
	while(!file.eof())
	{
		memset(line,0,256);

		file.getline(line,256);
		
		if(!*line)
			break;

		if(*line == '#')
			continue;

		for(i = 0;i < 4;i++)
			line[i] -= '0';

		outf.write(line,4);
	}
	
	file.close();
	outf.close();
	
	delete [] line;
	
	return 0;
}
