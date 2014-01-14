#include <stdio.h>

int main(void)
{
    int a = 1 << 0;
    int b = 1 << 2;
    int c = ~(1u << 0);
    int d = 0x0;
    int e = 0x1;
    int f = 0x2;
    int g = 0x3;
    int h = 0x4;
    int i = 0x5;
    int j = 0x6;
    int k = 0x7;
    int l = 0x8;
    int m = 0x9;
    int n = 0xa;
    int o = 0xb;
    int p = 0xc;
    int z = 0x10;
	int chan = 0;
	printf("#############%x\n", ~1);
	printf("################%d\n", ~1);
    printf("a = %d\n", a);
    printf("b = %d\n", b);
    printf("c = %d\n", c);
 
	d |= (1<<chan);
	printf("d = %x\n", d);

	d &= ~(1u << chan);
	printf("d = %x\n", d);

	e |= (1<<chan); 
	printf("e = %x\n", e);

	e &= ~(1u << chan);
	printf("e = %x\n", e);

	f |= (1<<chan);
	printf("f = %x\n", f);

	f &= ~(1u << chan);
	printf("f = %x\n", f);

	g |= (1<<chan);
	printf("g = %x\n", g);

	g &= ~(1u << chan);
	printf("g = %x\n", g);

	h |= (1<<chan);
	printf("h = %x\n", h);

	h &= ~(1u << chan);
	printf("h = %x\n", h);

	i |= (1<<chan);
	printf("i = %x\n", i);

	i &= ~(1u << chan);
	printf("i = %x\n", i);

	j |= (1<<chan);
	printf("j = %x\n", j);

	j &= ~(1u << chan);
	printf("j = %x\n", j);

	k |= (1<<chan);
	printf("k = %x\n", k);

	k &= ~(1u << chan);
	printf("k = %x\n", k);

	l |= (1<<chan);
	printf("l = %x\n", l);

	l &= ~(1u << chan);
	printf("l = %x\n", l);

	m |= (1<<chan);
	printf("m = %x\n", m);

	m &= ~(1u << chan);
	printf("m = %x\n", m);

	n |= (1<<chan);
	printf("n = %x\n", n);

	n &= ~(1u << chan);
	printf("d = %x\n", n);

	o |= (1<<chan);
	printf("n = %x\n", o);

	o &= ~(1u << chan);
	printf("o = %x\n", o);

	p |= (1<<chan);
	printf("p = %x\n", p);

	p &= ~(1u << chan);
	printf("p = %x\n", p);
	z |= (1<<chan);
	printf("z = %x\n", z);

	z &= ~(1u << chan);
	printf("z = %x\n", z);
	printf("%x\n", 1 | 1);
    return 0;
}

