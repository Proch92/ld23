#include <cmath>
#include <string>
#include <time.h>
#include "../include.h"

Water::Water() {
}

void Water::init(int xleft, int ytop) {
	int i, j;
	
	LENGHT = WATER_WIDTH;
	
	particles = (struct particle*) malloc(sizeof(struct particle) * LENGHT);
	
	x = xleft;
	y = ytop;
	
	for (i=0; i!=LENGHT; i++) {
		particles[i].height = 0.9f;
		particles[i].vel = 0.f;
	}
	
	//creating water layer
	wl = (GLubyte*) malloc(sizeof(GLubyte) * SCREEN_HEIGHT * SCREEN_WIDTH * 4);
	
	GLubyte *ptr = wl;
	waterlayer = (GLubyte***) malloc(sizeof(GLubyte**) * SCREEN_HEIGHT);
	for(i=0; i!=SCREEN_HEIGHT; i++) {
		waterlayer[i] = (GLubyte**) malloc(sizeof(GLubyte*) * SCREEN_WIDTH);
		for(j=0; j!=SCREEN_WIDTH; j++) {
			waterlayer[i][j] = ptr;
			ptr += 4;
		}
	}
	
	for (i=0; i!=SCREEN_WIDTH; i++)
		for (j=0; j!=SCREEN_HEIGHT; j++) {
			waterlayer[j][i][0] = 25;
			waterlayer[j][i][1] = 115;
			waterlayer[j][i][2] = 205;
			waterlayer[j][i][3] = 0;
		}
	
	glGenTextures(1, &watertex.id);
    
	glBindTexture( GL_TEXTURE_2D, watertex.id);
	
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_BGRA, GL_UNSIGNED_BYTE, waterlayer);
	
	//randomInit();
	delta.start();
}

void Water::randomInit() {
	int i;
	
	srand((unsigned int) time(0));
	
	for (i=1 ; i!=LENGHT; i++) {
		if (i%300 == 0)
			giveInput(i, rand()%W_AMPLITUDE - (W_AMPLITUDE / 2));
	}
	
	compute();
	
	move();
}

void Water::compute() {
	int i;
	float diffprev, diffnext, diffvelprev, diffvelnext, diffHcorr[LENGHT], diffVcorr[LENGHT], heightCorr[LENGHT];
	
	const float dHc = 0.98, dVc = 0.08, hC = 0.017;
	
	for (i=1; i!=LENGHT; i++) {
		//height difference correction
		if (i > 0) 
			diffprev = (particles[i].height - particles[i-1].height);
		if (i < LENGHT) 
			diffnext = (particles[i].height - particles[i+1].height);
		diffHcorr[i] = (diffnext + diffprev);
		
		//vel difference correction
		if (i > 0) 
			diffvelprev = (particles[i].vel - particles[i-1].vel);
		if (i < LENGHT) 
			diffvelnext = (particles[i].vel - particles[i+1].vel);
		diffVcorr[i] = (diffvelprev + diffvelnext);
		
		//height correction
		heightCorr[i] = particles[i].height; 
	}
	
	//applying corrections
	for (i=1; i!=LENGHT; i++)
		particles[i].vel -= ((diffHcorr[i] * dHc) + (diffVcorr[i] * dVc) + (heightCorr[i] * hC)) * VISCOSITY;
}

void Water::move() {
	int i;	
	
	for (i=1; i!=LENGHT; i++)
		particles[i].height += particles[i].vel * (delta.get_ticks() / 1000.f) * DENSITY;
	
	delta.start();
}

void Water::show() {
	int i, j, h, pixx;
	float approximation;
	bool first;
	Uint8 A;
	
	int end = x + LENGHT;
	int end2 = (SCREEN_HEIGHT / 2) + WATER_DEEP;
	for (i=x; i!=end; i++)
		for (j=(SCREEN_HEIGHT / 2) - WATER_DEEP; j!=end2; j++)
			waterlayer[j][i][3] = 0;
			
	
	pixx = x;
	for(i=0; i!=LENGHT; i++) {
		first = true;
		h = (SCREEN_HEIGHT / 2) - particles[i].height;
		for (; h < end2; h++) {
			if (first) {
				first = false;
				approximation = fabs(particles[i].height - ((int) particles[i].height));
				if (particles[i].height < 0)
					approximation = 1 - approximation;
				A = 255 * approximation;
			}
			else
				A = 255;
			waterlayer[h][pixx][3] = A;
		}
		pixx++;
	}
	
	//change texture
	glBindTexture(GL_TEXTURE_2D, watertex.id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, wl);
	apply_surface (0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, watertex);
}

void Water::giveInput(int x, float value) {
	particles[x].vel -= value;
}

int Water::operator [] (int n) {
	return (int)(particles[n].height);
}
