#include <string.h>
#include <stdio.h>
#include <cairo.h>

#include "common.h"
#include "compiler.h"
#include "lasca.h"

static cairo_t *cr=0;

void draw();

void resize(struct tag *c) {
	cairo_text_extents_t te;
	cairo_text_extents(cr,c->s,&te);
	c->w=te.x_advance+(c->nospace?0:10); c->h=button_height+5;
}

void drawinit(cairo_t *cr1) {
	cr=cr1;
	cairo_select_font_face (cr, "times", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(cr, 12.0);
	cairo_set_line_width (cr,1);

	cairo_text_extents_t te;
	cairo_text_extents(cr,"abcdefghijklmnopqrstuvwxyz0123456789;",&te);
	button_height=te.height;
}

static void normalcolor()  {cairo_set_source_rgb(cr,0.5,0.9,0.5);}
static void builtincolor() {cairo_set_source_rgb(cr,0.9,0.5,0.9);}
static void macrocolor()   {cairo_set_source_rgb(cr,0.5,0.5,0.9);}
static void datacolor()    {cairo_set_source_rgb(cr,0.9,0.9,0.5);}
static void commandcolor() {cairo_set_source_rgb(cr,0.9,0.5,0.5);}

static inline void dullcolor()   { cairo_set_source_rgb(cr,0.8,0.8,0.8); }

static inline void textcolor() { cairo_set_source_rgb(cr,0,0,0); }

static int x,y;

static inline void pad(struct tag *o) {
	cairo_rectangle(cr,x,y,o->w,o->h);
	cairo_fill(cr);
}

static inline void text(struct tag *o) {
	cairo_move_to(cr, x+(o->nospace?0:5), y+button_height);
	cairo_show_text(cr, o->s);
	cairo_stroke(cr);
}

static void drawstack() {
	char s[10];
	cairo_move_to(cr, 5, 5+button_height);
	uint32_t *p=stack;

	printf("%08x %08x\n", (uint32_t)stack, (uint32_t)stackh);

	textcolor();
	while(p<stackh+32) {
		sprintf(s,"%x ",*p);
		cairo_show_text(cr, s);
		p++;
	}
	cairo_stroke(cr);
}

static void typecolor(enum nmflag t) {
	switch(t) {
		case builtin: builtincolor(); break;
		case command: commandcolor(); break;
		case compiled: normalcolor(); break;
		case macro: macrocolor(); break;
		case data: datacolor(); break;
	}
}

void shift(int *y) { if(*y>100-(button_height+5)) *y+=2*(button_height+5); }
void unshift(int *y) { if(*y>100+2*(button_height+5)) *y-=2*(button_height+5); }

static void drawtag(struct tag *t) {
	x=t->x; y=t->y;
	shift(&y);
	typecolor(t->t); pad(t);
	textcolor(); text(t);
}


void do_hexed() {
}

static inline void drawbyte(uint8_t c) {
	char *hex="0123456789abcdef";
	char b[3]={hex[c>>4],hex[c&0xf],0};
	cairo_move_to(cr,x,y+button_height);
	cairo_show_text(cr,b);
	cairo_stroke(cr);
}

static void draweditor() {
	int i;
	x=100; y=100;
	for(i=0;i<10;i++) {
		if(editor.w[i]!=0) {
			printf("draw %s\n", editor.w[i]->s);
			struct tag *t=editor.w[i];
			typecolor(t->t); pad(t);
			textcolor(); text(t);
			y+=button_height+5;
		}
	}
}

void draw() {
        cairo_set_source_rgb(cr,1,1,1);
        cairo_paint(cr);

	struct tag *e;
	for(e=builtins.heads;e<builtins.end;e++) { drawtag(e); }
	for(e=words.heads;e<words.end;e++) { drawtag(e); }
	for(e=commands.heads;e<commands.end;e++) { drawtag(e); }

	draweditor();
	drawstack();
}

