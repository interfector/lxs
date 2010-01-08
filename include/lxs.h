#ifndef _LXS_H_
#define _LXS_H_

#define MAX_MEM 1000

#define BIN_MAGIC "SYM\x7a"

#define SYM_READ	0
#define SYM_WRITE	1
#define SYM_POP	2
#define SYM_PUSH	3
#define SYM_ADD	4
#define SYM_SUB	5
#define SYM_MUL	6
#define SYM_DIV	7
#define SYM_MOD	8
#define SYM_AND	0
#define SYM_OR		10
#define SYM_XOR	11
#define SYM_NOT	12
#define SYM_SHL	13
#define SYM_SHR	14
#define SYM_DEL	15
#define SYM_NOP	16
#define SYM_JMP	17
#define SYM_CMP	18
#define SYM_JN		19
#define SYM_JZ		20
#define SYM_JM		21
#define SYM_JG		22
#define SYM_EXIT	23

extern "C" {
	typedef struct {
		int mem[MAX_MEM];

		int count;
		int eax;
		int flag;
	
		int ip;
		int op_code;
	
	} sym_code_t;

	void sym_read(sym_code_t * code);
	void sym_write(sym_code_t * code);
	void sym_pop(sym_code_t * code);
	void sym_push(sym_code_t * code);
	void sym_add(sym_code_t * code);
	void sym_sub(sym_code_t * code);
	void sym_mul(sym_code_t * code);
	void sym_div(sym_code_t * code);
	void sym_mod(sym_code_t * code);
	void sym_and(sym_code_t * code);
	void sym_or(sym_code_t * code);
	void sym_xor(sym_code_t * code);
	void sym_not(sym_code_t * code);
	void sym_shl(sym_code_t * code);
	void sym_shr(sym_code_t * code);
	void sym_del(sym_code_t * code);
	void sym_nop(sym_code_t * code);
	void sym_jmp(sym_code_t * code);
	void sym_cmp(sym_code_t * code);
	void sym_jn(sym_code_t * code);
	void sym_jz(sym_code_t * code);
	void sym_jm(sym_code_t * code);
	void sym_jg(sym_code_t * code);
	void sym_exit(sym_code_t * code);

	static void (*sym_code_table[])(sym_code_t *) = { 
		sym_read,
		sym_write,
		sym_pop,
		sym_push,
		sym_add,
		sym_sub,
		sym_mul,
		sym_div,
		sym_mod,
		sym_and,
		sym_or,
		sym_xor,
		sym_not,
		sym_shl,
		sym_shr,
		sym_del,
		sym_nop,
		sym_jmp,
		sym_cmp,
		sym_jn,
		sym_jz,
		sym_jm,
		sym_jg,
		sym_exit };
		
};

class SymClass {
	public:
		SymClass();
		
		sym_code_t simpletron;
		
		int readfile(char * name);
		void stdin_read();
		int pseudo_compile(char * name,char * out);
		int read_bin(char * name);
		int execute();
		void dump();
	private:
		int execute_op(int op);
		int atoi(char * str);
};

#endif
