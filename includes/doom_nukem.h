#ifndef DOOM_NUKEM_H
#define DOOM_NUKEM_H

# include "mlx.h"
# include "libft.h"
# include <math.h>
# include <pthread.h>
# include "stdio.h"
# include <time.h>

# define PRINT_DEBUG 0

# define KB_ESQ 53
# define KB_TAB 48
# define KB_SHIFT_L 257
# define KB_CTRL_L 260
# define KB_ALT_L 261
# define KB_SPACE 49
# define KB_ALT_R 262
# define KB_CTRL_R 259
# define KB_SHIFT_R 258
# define KB_ENTER 36
# define KB_BACKSPACE 51

# define KB_1 18
# define KB_2 19
# define KB_3 20
# define KB_4 21
# define KB_5 23
# define KB_6 22
# define KB_7 26
# define KB_8 28
# define KB_9 25
# define KB_0 29
# define KB_MINUS 27
# define KB_EQUAL 24

# define KB_UP 126
# define KB_DOWN 125
# define KB_LEFT 123
# define KB_RIGHT 124

# define KB_Q 12
# define KB_W 13
# define KB_E 14
# define KB_R 15
# define KB_T 17
# define KB_Y 16
# define KB_U 32
# define KB_I 34
# define KB_O 31
# define KB_P 35
# define KB_A 0
# define KB_S 1
# define KB_D 2
# define KB_F 3
# define KB_G 5
# define KB_H 4
# define KB_J 38
# define KB_K 40
# define KB_L 37
# define KB_Z 6
# define KB_X 7
# define KB_C 8
# define KB_V 9
# define KB_B 11
# define KB_N 45
# define KB_M 46

# define MS_LEFT 1
# define MS_RIGHT 2
# define MS_MIDDLE 3

# define MS_SCROLL_UP 4
# define MS_SCROLL_DOWN 5

# define	SCREEN_W 1280
# define	SCREEN_H 720
# define	WIN_TITLE "DOOM-NUKEM"

# define	COLOR_KEY_R 128
# define	COLOR_KEY_G 128
# define	COLOR_KEY_B 128

# define	ZERO_SIZE 1024 * 8

typedef struct	s_128byte
{
	unsigned long int	uli1;
	unsigned long int	uli2;
	unsigned long int	uli3;
	unsigned long int	uli4;
	unsigned long int	uli5;
	unsigned long int	uli6;
	unsigned long int	uli7;
	unsigned long int	uli8;
	unsigned long int	uli9;
	unsigned long int	uli10;
	unsigned long int	uli11;
	unsigned long int	uli12;
	unsigned long int	uli13;
	unsigned long int	uli14;
	unsigned long int	uli15;
	unsigned long int	uli16;
}				t_128byte;

typedef struct	s_zero_char
{
	unsigned char z[1024];
}				t_zero_char;

typedef struct	s_color
{
	int			r;
	int			g;
	int			b;
}				t_color;

typedef struct	s_mat4x4
{
	float m[4][4];
}				t_mat4x4;

typedef struct	s_vector_i
{
	int			x;
	int			y;
	int			z;
}				t_vector_i;

typedef struct	s_vertex
{
	float		x;
	float		y;
	float		z;
}				t_vertex;

typedef struct	s_triangle
{
	t_vertex	*v[3];
	t_color		color;
}				t_triangle;

typedef struct	s_mesh
{
	t_vertex	*v;
	t_triangle	t[100];
	int 		last_vt;
	int 		last_tr;
}				t_mesh;

typedef struct	s_kb_keys_state
{
	int			up[300];
	int 		down[300];
}				t_kb_keys_state;

typedef struct	s_ms_keys_state
{
	int			up[10];
	int 		down[10];
}				t_ms_keys_state;

typedef struct	s_mouse_state
{
	int				move;
	t_ms_keys_state	keys;
	int 			x;
	int 			y;
	int 			x_prev;
	int 			y_prev;
	int 			hdir;
	int 			vdir;
}				t_mouse_state;

typedef struct	s_image
{
	void		*ptr;
	char		*pixels;
	int			bpp;
	int			s_l;
	int			endian;
}				t_image;

typedef struct	s_window
{
	void		*ptr;
	char		*title;
	int			w;
	int			h;
	float		ratio;
	int			half_w;
	int			half_h;
}				t_window;

typedef struct	s_camera
{
	float 		z_near;
	float 		z_far;
	float 		fov;
	float 		for_rad;
	float 		asp_ratio;
	t_vertex	pos;
}				t_camera;

typedef struct	s_app
{
	void			*mlx;
	t_window		window;
	t_image			screen;
	t_kb_keys_state	keyboard;
	t_mouse_state	mouse;
	t_camera		camera;
	t_mat4x4		projection_mat;
	t_mat4x4		rotation_mat_z;
	t_mat4x4		rotation_mat_x;
	t_vertex		rot;
	t_mesh			cube;
	float 			speed;
} 				t_app;

void	debug_mouse(t_app *app, char *event, int key_code);
void	debug_keyboard(char *event, int key_code);
void	do_input(t_app *app);


void 	app_close(t_app *app);

void	init_window(t_window *window, void *mlx);
void	init_image(t_image *image, t_window *window, void *mlx);
void	init_app(t_app *app);

void	update_inputs(t_app *app);

int 	keyboard_event_down(int key_code, t_app *app);
int 	keyboard_event_up(int key_code, t_app *app);

int 	mouse_event_down(int key_code, int x, int y, t_app *app);
int 	mouse_event_up(int key_code, int x, int y, t_app *app);
int 	mouse_event_move(int x, int y, t_app *app);

int		window_event_close(t_app *app);
int		window_event_expose(t_app *app);

void	reset_inputs_states(t_app *app);

void	project_triangle(t_triangle *in, t_triangle *out, t_mat4x4 *proj_mat);
void	translate_triangle(t_triangle *in, t_triangle *out);
void	scale_triangle(t_app *app, t_triangle *triangle);

void	set_vertex(t_vertex *vertex, float x, float y, float z);
float 	cross_product(t_vertex *v_1, t_vertex *v_2);

void	update_rotation_mat_z(t_app *app, float angle);
void	update_rotation_mat_x(t_app *app, float angle);

void	set_color(t_color *color, int r, int g, int b);

void	set_pixel(t_image *image, int x, int y, t_color *c);
void	draw_line(t_app *app, t_vertex start, t_vertex end, t_color *c);
void	draw_line2(t_app *app, t_vertex p0, t_vertex p1, t_color *c);
void	draw_triangle(t_app *app, t_triangle triangle);
void	fill_triangle(t_app *app, t_triangle *t);

void	make_cube(t_mesh *m, float size);
#endif
