#include <iostream>
#include <fstream>
#include <cstring>
#include <lxs.h>

void sym_read(sym_code_t * code)
{
	std::cin >> code->mem[code->op_code];
}

void sym_write(sym_code_t * code)
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
		default:
			std::cout << code->mem[code->op_code];
			break;
	}

	std::cout << std::endl;
}

void sym_pop(sym_code_t * code)
{
	code->mem[code->op_code] = code->eax;
}

void sym_push(sym_code_t * code)
{
	code->eax = code->mem[code->op_code];
}

void sym_add(sym_code_t * code)
{
	code->eax += code->mem[code->op_code];
}

void sym_sub(sym_code_t * code)
{
	code->eax -= code->mem[code->op_code];
}

void sym_mul(sym_code_t * code)
{
	code->eax *= code->mem[code->op_code];
}

void sym_div(sym_code_t * code)
{
	if(code->mem[code->op_code] != 0)
		code->eax /= code->mem[code->op_code];
}

void sym_mod(sym_code_t * code)
{
	if(code->mem[code->op_code] != 0)
		code->eax %= code->mem[code->op_code];
}

void sym_and(sym_code_t * code)
{
	code->eax &= code->mem[code->op_code];
}

void sym_or(sym_code_t * code)
{
	code->eax |= code->mem[code->op_code];
}

void sym_xor(sym_code_t * code)
{
	code->eax ^= code->mem[code->op_code];
}

void sym_not(sym_code_t * code)
{
	code->eax = !code->mem[code->op_code];
}

void sym_shl(sym_code_t * code)
{
	code->eax <<= code->mem[code->op_code];
}

void sym_shr(sym_code_t * code)
{
	code->eax >>= code->mem[code->op_code];
}

void sym_del(sym_code_t * code)
{
	code->mem[code->op_code] = 0;
}

void sym_nop(sym_code_t * code)
{
	/* NULL :D */ 
}

void sym_jmp(sym_code_t * code)
{
	code->ip = code->op_code-1;
}

void sym_cmp(sym_code_t * code)
{
	if (code->eax == code->mem[code->op_code])
		code->flag = 0;
	else if (code->eax < code->mem[code->op_code])
		code->flag = 1;
	else if (code->eax == code->mem[code->op_code])
		code->flag = 2;
}

void sym_jn(sym_code_t * code)
{
	if ( code->flag == 1 || code->flag == 2 )
		code->ip = code->op_code-1;
}

void sym_jz(sym_code_t * code)
{
	if (!code->flag)
		code->ip = code->op_code-1;
}

void sym_jm(sym_code_t * code)
{
	if (code->flag == 1)
		code->ip = code->op_code-1;
}

void sym_jg(sym_code_t * code)
{
	if (code->flag == 2)
		code->ip = code->op_code-1;
}

void sym_exit(sym_code_t * code)
{
	std::cout << "[?]Exit code: " << code->op_code << std::endl;
	
	code->ip = code->count;
}

void sym_chmod(sym_code_t * code)
{
	code->mode = code->op_code;
}

void toBin(int num)
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
}

int SymClass::atoi(char * line)
{
	int ret;
 
	for(ret = 0;*line;line++)
		if (*line >= '0' && *line <= '9')
			ret = ret * 10 + ((int)*line - '0');
	return ret;
}

int SymClass::execute()
{
	int op = 0;
	int len = this->simpletron.count;
	
	for(this->simpletron.ip=0;this->simpletron.ip < len;this->simpletron.ip++)
	{
		op = execute_op(this->simpletron.mem[this->simpletron.ip]);
		if(op)
			return op;
	}

	return 0;
}

int SymClass::execute_op(int op)
{
	int call = (op / 100)-10;
	
	this->simpletron.op_code = op % 100;

	if (op == -9999)
		return 0;
	
	if(call >= 0 && call <= 24)
		sym_code_table[call](&simpletron);
	
	return 0;
}

void SymClass::dump()
{
	int i;

	std::cout << std::dec << "\n=== Registri: ===\n";
	std::cout << "Accumulatore:        " << this->simpletron.eax << "\n";
	std::cout << "Numero istruzione:   " << this->simpletron.count << "\n";
	std::cout << "Operation Code:      " << this->simpletron.op_code << "\n";
	std::cout << "\n";
	std::cout << "Memoria:\n";

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

void SymClass::stdin_read()
{
	char * line = new char[256];
	int op;
		
	while((op != -9999) && (this->simpletron.count < MAX_MEM))
	{
		std::cout << (long)this->simpletron.count << " ? ";
		std::cin >> op;
		
		this->simpletron.mem[this->simpletron.count++] = op;
	}

	delete [] line;
}

int SymClass::readfile(char * name)
{
	std::ifstream file(name,std::ifstream::in);
	char * line = new char[256];
	int op;
	
	if (!file.is_open())
		return 1;
		
	while(!file.eof())
	{
		//file >> line;
		
		file.getline(line,256);

		op = this->atoi(line);

		this->simpletron.mem[this->simpletron.count++] = op;
	}
	
	file.close();
	
	delete [] line;
	
	return 0;
}

int SymClass::read_bin(char * name)
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

int SymClass::pseudo_compile(char * name,char * out)
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

		for(i=0;i<4;i++)
			line[i] -= '0';

		outf.write(line,4);
	}
	
	file.close();
	outf.close();
	
	delete [] line;
	
	return 0;
}
