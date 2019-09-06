#ifndef DOOM_NUKEM_H
#define DOOM_NUKEM_H

# include "libft.h"
# include <math.h>
# include <pthread.h>
# include "stdio.h"
# include <time.h> // LIBC ?!
# include <SDL.h>
# define PRINT_DEBUG 0

# define	SCREEN_W 1280
# define	SCREEN_H 720
# define	WIN_TITLE "DOOM-NUKEM"

# define	COLOR_KEY_R 128
# define	COLOR_KEY_G 128
# define	COLOR_KEY_B 128

typedef struct	s_color
{
	int			r;
	int			g;
	int			b;
}				t_color;

typedef struct	s_image_chunk
{
	unsigned char z[1024];
}				t_image_chunk;

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
	int 		w;
}				t_vertex;

typedef struct	s_triangle
{
	t_vertex	*v[3];
	t_color		color;
}				t_triangle;

typedef struct	s_mesh
{
	t_vertex	*v;
	t_triangle	*t;
	int 		v_idx;
	int 		t_idx;
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

typedef struct	s_timer
{
	clock_t current_ticks;
	clock_t delta_ticks;
	float 	delta;
	clock_t fps;
}				t_timer;

typedef struct		s_inputs
{
	const Uint8		*keyboard;
	int				left_pressed;
	int				right_pressed;
	float			sensitivity;
	int				x;
	int				y;
	int				zoom;
}					t_inputs;

typedef struct		s_sdl
{
	SDL_Event		event;
	SDL_Window		*window;
	SDL_Surface		*surface;
	t_timer			*timer;
	int				half_height;
	int				half_width;
	int				height;
	int				width;
}					t_sdl;

typedef struct	s_app
{
	t_window		window;
	t_image			screen;
	t_timer			timer;
	t_kb_keys_state	keyboard;
	t_mouse_state	mouse;
	t_camera		camera;
	t_mat4x4		projection_mat;
	t_mat4x4		rotation_mat_z;
	t_mat4x4		rotation_mat_x;
	t_vertex		rot;
	t_mesh			cube;
	float 			speed;
	t_sdl			*sdl;
	t_inputs		*inputs;
	t_mesh			*mesh;
}
		t_app;

void	debug_mouse(t_app *app, char *event, int key_code);
void	debug_keyboard(char *event, int key_code);
void	do_input(t_app *app);

void	*image_clear(void *b, int c, size_t len);

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
void	translate_triangle(t_triangle *in, t_triangle *out, t_app *app);
void	scale_triangle(t_app *app, t_triangle *triangle);

void	set_vertex(t_vertex *vertex, float x, float y, float z);
float 	cross_product(t_vertex *v_1, t_vertex *v_2);

void	update_rotation_mat_z(t_app *app, float angle);
void	update_rotation_mat_x(t_app *app, float angle);

void	set_triangle(t_triangle *t, t_vertex *v0, t_vertex *v1, t_vertex *v2);

void	set_color(t_color *color, int r, int g, int b);

void	set_pixel(SDL_Surface *s, int x, int y, t_color *c);
void	draw_line(t_app *app, t_vertex start, t_vertex end, t_color *c);
void	draw_line2(t_app *app, t_vertex p0, t_vertex p1, t_color *c);
void	draw_triangle(t_app *app, t_triangle triangle);
void	fill_triangle(t_app *app, t_triangle t);
void	fill_triangle2(t_app *app, t_triangle t);

void	show_fps_sdl(t_timer *timer);

void	make_cube(t_mesh *m, float size);

void	get_ticks(t_timer *timer);
void	get_delta_time(t_timer *timer);
void	show_fps(t_app *app);

void	get_color(SDL_Surface *surface, int x, int y, t_color *c);
int 	out_of_borders(int x, int y);
int 	color_key(t_color *c);
void	quit_properly(t_app *app);

int		event_handling(t_app *app);

t_mat4x4	matrix_subtraction(t_mat4x4 matrix1, t_mat4x4 matrix2);
t_mat4x4	matrix_summary(t_mat4x4 matrix1, t_mat4x4 matrix2);
t_mat4x4	matrix_multiply_matrix(t_mat4x4 matrix1, t_mat4x4 matrix2);
t_vertex	matrix_multiply_vector(t_mat4x4 matrix, t_vertex vector);

t_vertex	vector_summary(t_vertex vector1, t_vertex vector2);
t_vertex	vector_subtract(t_vertex vector1, t_vertex vector2);
t_vertex	vector_multiply(t_vertex vector1, float k);
t_vertex	vector_divide(t_vertex vector1, float k);
float		dot_product(t_vertex vector1, t_vertex vector2);
float		vector_length(t_vertex vector);
t_vertex	normalise_vector(t_vertex vector);
t_vertex	vector_cross_product(t_vertex vector1, t_vertex vector2);

void		read_obj(char *path, t_mesh *mesh);
#endif
