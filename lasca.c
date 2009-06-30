#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "common.h"
#include "compiler.h"
#include "draw.h"

extern uint8_t gen;

static struct e editcode[1024];
static struct e final={.n=0,.t=macro};

static void do_exit() { exit(0); }

int nospace=0;
struct tag1 *add(int x, int y, char *s, void *f, int len, enum tagtype tt, enum wordtype wt) {
	struct word *w=words.end++;
	struct tag1 *t=tags.end++;
	
	t->t=tt;
	t->nospace=nospace;
	t->w=w;
	t->x=x;
	t->y=y;

	w->gen=gen;
	w->data=f;
	w->len=len;
	strncpy(w->s,s,7);
	resize(w);

	w->def=wt==compiled?&final:0;
	return t;
}

static void do_create() { add(100,100,"",0,0,normal,compiled); }

static void do_execute() {
	draw();
}

static inline void change_type(enum tagtype p) {
	draw();
}
static void do_macro() { change_type(macro); }
static void do_normal() { change_type(compiled); }
static void do_data() { change_type(data); }


void init(cairo_t *cr) {
	editcode_e=editcode;
	drawinit(cr);
	
	add(30,90,"execute", do_execute,0,command,builtin);
	add(30,70,"compile", do_compile,0,command,builtin);
	add(30,50,"exit", do_exit,0,command,builtin);
	add(30,30,"create", do_create,0,command,builtin);
	final.w=add(30,130,";",do_ret,0,macro,builtin)->w;


	add(30,150,"macro",do_macro,0,command,builtin);
	add(30,170,"normal",do_normal,0,command,builtin);
	add(30,190,"data",do_data,0,command,builtin);

	add_builtins();
}

void go() {
}

