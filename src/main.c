#include <8052.h>
#include <stdlib.h>

#define DATA P2_0
#define LATCH P2_1
#define SHIFT P2_2

#define abs(x) (((x) < 0) ?  -(x) : (x))

#define set_data(x) DATA=x
#define set_latch(x) LATCH=x
#define set_shift(x) SHIFT=x

#define SCANNING_INTERVAL 5000
#define reset_t0() { TL0 = (65536 - SCANNING_INTERVAL) % 256; TH0 = (65536 - SCANNING_INTERVAL) / 256; }

void delay(unsigned int n){
	while(n--);
}

unsigned char animation_type = 0;
unsigned char button_pressed = 0;

unsigned char rowscan_counter = 0;
unsigned char cube[8][8] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
};

/**
 * Send row and column data through 74HC595.
 * Row order: top layer => bottom layer
 * Column order: clockwise
 */
void send_data() {

	unsigned char i, j;
	unsigned char col;

	unsigned char row_selector = 0x01 << rowscan_counter;
	
	set_latch(0);
	
	// row first, then column

	for (i = 0; i < 8; i++) {
		set_shift(0);
		set_data(row_selector & 0x0001);
		row_selector = row_selector >> 1;
		set_shift(1);
	}

	for (i = 0; i < 8; i++) {
		col = cube[rowscan_counter][i];
		for (j = 0; j < 8; j++) {
			set_shift(0);
			set_data(col & 0x0001);
			col = col >> 1;
			set_shift(1);
		}
	}

	set_latch(1);
	set_shift(0);
	set_latch(0);
}

/**
 * Shrinking frame
 */
void animation_0() {
	unsigned char i, j;
	unsigned char frame = 0;

	unsigned char __code frames[4][8][8] = {
		{
			{ 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff },
			{ 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81 },
			{ 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81 },
			{ 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81 },
			{ 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81 },
			{ 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81 },
			{ 0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81 },
			{ 0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff },
		},

		{
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x8e, 0x42, 0x42, 0x42, 0x42, 0x8e, 0x00 },
			{ 0x00, 0x42, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00 },
			{ 0x00, 0x42, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00 },
			{ 0x00, 0x42, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00 },
			{ 0x00, 0x42, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00 },
			{ 0x00, 0x8e, 0x42, 0x42, 0x42, 0x42, 0x8e, 0x00 },
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		},

		{
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x3c, 0x24, 0x24, 0x3c, 0x00, 0x00 },
			{ 0x00, 0x00, 0x24, 0x00, 0x00, 0x24, 0x00, 0x00 },
			{ 0x00, 0x00, 0x24, 0x00, 0x00, 0x24, 0x00, 0x00 },
			{ 0x00, 0x00, 0x3c, 0x24, 0x24, 0x3c, 0x00, 0x00 },
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		},

		{
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
			{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		}
	};

	while(1) {
		if (animation_type != 0) {
			return;
		}

		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				cube[i][j] = frames[frame][i][j];
			}
		}

		frame++;
		if (frame == 4) {
			frame = 0;
		}

		delay(50000);
		delay(50000);
		delay(50000);
		delay(50000);
	}
}

/**
 * Static frame
 */
void animation_1() {
	unsigned char i, j;

	while(1) {

		if (animation_type != 1) {
			return;
		}

		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if ((i == 0 || i == 7) && (j == 0 || j == 7)) {
					cube[i][j] = 0xff;
				} else if (i == 0 || j == 0 || i == 7 || j == 7) {
					cube[i][j] = 0x81;
				} else {
					cube[i][j] = 0x00;
				}
			}
		}
	}
}

/**
 * Moving wall
 */
void animation_2() {
	unsigned char i, j;

	unsigned char y = 0;

	while(1) {

		if (animation_type != 2) {
			return;
		}

		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				cube[i][j] = (j == y) ? 0xff : 0x00;
			}
		}

		y++;
		if (y == 8) {
			y = 0;
		}

		delay(50000);
		delay(50000);
		delay(50000);
		delay(50000);
	}
}


void clear_all() {
	unsigned char i, j;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
				cube[i][j] = 0x00;
		}
	}
}

void draw_point(unsigned char x, unsigned char y, unsigned char z) {
	cube[z][y] |= 1 << x;
}

void draw_ball(unsigned char x, unsigned char y, unsigned char z) {
	draw_point(x, y, z);
	draw_point(x+1, y, z);
	draw_point(x, y+1, z);
	draw_point(x+1, y+1, z);
	draw_point(x, y, z+1);
	draw_point(x+1, y, z+1);
	draw_point(x, y+1, z+1);
	draw_point(x+1, y+1, z+1);
}

/**
 * Single random ball
 */
void animation_3() {
	unsigned char z = 1, y = 2, x = 3;
	signed char dz = 1, dy = 1, dx = 1;

	while(1) {

		if (animation_type != 3) {
			return;
		}

		clear_all();
		draw_ball(x, y, z);

		z += dz;
		y += dy;
		x += dx;

		if (z == 0 || z == 6) {
			dz = -dz;
		}

		if (y == 0 || y == 6) {
			dy = -dy;
			if (rand() % 4) dx = -dx;
		}

		if (x == 0 || x == 6) {
			dx = -dx;
			if (rand() % 4) dy = -dy;
		}

		delay(50000);
		delay(50000);
		delay(50000);
		delay(50000);
	}
}

/**
 * Double random balls
 */
void animation_4() {
	unsigned char z1 = 1, y1 = 2, x1 = 3;
	unsigned char z2 = 4, y2 = 3, x2 = 5;
	signed char dz1 = 1, dy1 = 1, dx1 = 1;
	signed char dz2 = -1, dy2 = 1, dx2 = 1;

	while(1) {

		if (animation_type != 4) {
			return;
		}

		clear_all();
		draw_ball(x1, y1, z1);
		draw_ball(x2, y2, z2);

		z1 += dz1;
		y1 += dy1;
		x1 += dx1;

		if (z1 == 0 || z1 == 6) {
			dz1 = -dz1;
		}

		if (y1 == 0 || y1 == 6) {
			dy1 = -dy1;
			if (rand() % 4) dx1 = -dx1;
		}

		if (x1 == 0 || x1 == 6) {
			dx1 = -dx1;
			if (rand() % 4) dy1 = -dy1;
		}


		z2 += dz2;
		y2 += dy2;
		x2 += dx2;

		if (z2 == 0 || z2 == 6) {
			dz2 = -dz2;
		}

		if (y2 == 0 || y2 == 6) {
			dy2 = -dy2;
			if (rand() % 4) dx2 = -dx2;
		}

		if (x2 == 0 || x2 == 6) {
			dx2 = -dx2;
			if (rand() % 4) dy2 = -dy2;
		}

		delay(50000);
		delay(50000);
		delay(50000);
		delay(50000);
	}
}

/**
 * Floating cube
 */
void animation_5() {

	unsigned char i, j, k, r;
	unsigned char cx = 3, cy = 3, cz = 3;
	unsigned int rnd;

	r = 2;

	while(1) {

		if (animation_type != 5) {
			return;
		}

		clear_all();

		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				for (k = 0; k < 8; k++) {
					if (abs(i - cx) <= r && abs(j - cy) <= r && abs(k - cz) <= r) {
						cube[i][j] |= 0x01 << k;
					}
				}
			}
		}

		// change the size
		rnd = rand() / 10;
		if (rnd < 5) {
			r++;
		} else {
			r--;
		}

		if (r < 1) {
			r = 1;
		}

		if (r > 3) {
			r = 3;
		}

		// offset the center
		rnd = rand() / 10;

		if (rnd < 6) {
			cx = cx == 3 ? 4 : 3;
		}

		rnd = rand() / 10;
		if (rnd < 6) {
			cy = cy == 3 ? 4 : 3;
		}

		rnd = rand() / 10;
		if (rnd < 6) {
			cz = cz == 3 ? 4 : 3;
		}

		delay(50000);
		delay(50000);
		delay(50000);
		delay(50000);
	}
}

/**
 * Spinning panel
 */
void animation_6() {
	unsigned char i, j, frame;

	unsigned char __code frames[8][8] = {
		{ 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00 },  // -
		{ 0x00, 0x00, 0x03, 0x0c, 0x30, 0xc0, 0x00, 0x00 },
		{ 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 },  // /
		{ 0x04, 0x04, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20 },
		{ 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08 },  // |
		{ 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04 },
		{ 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 },
		{ 0x00, 0x00, 0xc0, 0x30, 0x0c, 0x03, 0x00, 0x00 }
	};

	frame = 0;

	while(1) {

		if (animation_type != 6) {
			return;
		}

		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				cube[i][j] = frames[frame][j];
			}
		}

		frame++;
		if (frame == 8) {
			frame = 0;
		}

		delay(50000);
		delay(50000);
		delay(50000);
		delay(50000);
	}
}


void main(){
	// enable EX0 for the switch button

	EA = 1;
	EX0 = 1;
	TCON = 0x01;

	// enable T0 for scanning
	reset_t0();
	TMOD = 0x01;
	ET0 = 1;
	TR0 = 1;
	
	while(1){
		delay(50000);
		delay(50000);
		delay(50000);
		delay(50000);
		button_pressed = 0;
		if (animation_type == 0) {
			animation_0();
		} else if (animation_type == 1) {
			animation_1();
		} else if (animation_type == 2) {
			animation_2();
		} else if (animation_type == 3) {
			animation_3();
		} else if (animation_type == 4) {
			animation_4();
		} else if (animation_type == 5) {
			animation_5();
		} else if (animation_type == 6) {
			animation_6();
		}
	}
}

// Switch Animation button
void switch_button_handler() __interrupt 0 {
	if (!button_pressed) {
		animation_type++;
		if (animation_type == 7) {
			animation_type = 0;
		}
		button_pressed = 1;
	}
}

void scanning_timer() __interrupt 1 {

	send_data();
	rowscan_counter++;
	if (rowscan_counter == 8) {
		rowscan_counter = 0;
	}

	reset_t0();
}