#ifndef DOOM_NUKEM_H
#define DOOM_NUKEM_H

# include "libft.h"
# include <math.h>
# include <pthread.h>
# include "stdio.h"
# include <time.h>
# include <SDL.h>
# define PRINT_DEBUG 0

# define	SCREEN_W 1280
# define	SCREEN_H 720
# define	WIN_TITLE "DOOM-NUKEM"

# define	COLOR_KEY_R 255
# define	COLOR_KEY_G 0
# define	COLOR_KEY_B 255

# ifdef		_WIN64
#  define	CPS 1000.0f
# elif		__APPLE__
#  define	CPS 1000000.0f
# endif

# define MIN(a,b) (((a)<(b))?(a):(b))
# define MAX(a,b) (((a)>(b))?(a):(b))
# define ABS(N) ((N<0)?(-N):(N))
# define SIGN(x) ((x < 0) ? -1 : 1);
# define SWAP(x, y, T) do { T SWAP = x; x = y; y = SWAP; } while (0)
# define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

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

typedef struct	s_vector
{
	float		x;
	float		y;
	float		z;
	float 		w;
}				t_vector;

typedef struct	s_triangle
{
	int			i[3];
	t_vector	v[3];
	t_color		color;
	int 		visible;
}				t_triangle;

typedef struct	s_mesh
{
	t_vector	*v_orig;
	t_vector	*v_buff;
	t_triangle	*t;
	int 		v_count;
	int 		t_count;
	t_vector	pos;
	t_vector	rot;
	t_mat4x4	rot_mat_x;
	t_mat4x4	rot_mat_y;
	t_mat4x4	rot_mat_z;
	t_mat4x4	trans_mat;
	t_mat4x4	transform;
}				t_mesh;

typedef struct	s_line
{
	t_vector	cur;
	t_vector	dir;
	t_vector	inc;
	t_color		color;
}				t_line;

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
	int 			x;
	int 			y;
	int 			left;
	int 			right;
	int 			middle;
	int 			scroll_u;
	int 			scroll_d;
	float 			sens;
}				t_mouse_state;

typedef struct	s_camera
{
	float 		z_near;
	float 		z_far;
	float 		fov;
	float 		for_rad;
	float 		asp_ratio;
	t_vector	pos;
	t_vector	rot;
	t_mat4x4	rot_mat_x;
	t_mat4x4	rot_mat_y;
	t_mat4x4	rot_mat_z;
	t_vector	target;
	t_vector	dir;
	t_mat4x4	rot_mat;
	t_mat4x4	view_mat;
}				t_camera;

typedef struct	s_world
{
	t_vector	rot;
	t_vector	trans;
	t_mat4x4	rot_mat_x;
	t_mat4x4	rot_mat_y;
	t_mat4x4	rot_mat_z;
	t_mat4x4	trans_mat;
	t_mat4x4	mat;
}				t_world;

typedef struct	s_timer
{
	clock_t current_ticks;
	clock_t delta_ticks;
	clock_t fps;
	float	delta;
	float 	time;
}				t_timer;

typedef struct		s_inputs
{
	const Uint8		*keyboard;
	t_mouse_state	mouse;
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
	float			half_height;
	float			half_width;
	int				height;
	int				width;
}					t_sdl;

typedef struct		s_tr_list
{
	t_triangle			tr;
	int 				last;
	struct s_tr_list	*next;
}					t_tr_list;

typedef struct	s_plane
{
	t_vector		p;
	t_vector		n;
}				t_plane;

typedef struct	s_app
{
	t_timer			*timer;
	t_camera		*camera;
	t_world			*world;
	t_mat4x4		projection_mat;
	t_sdl			*sdl;
	t_inputs		*inputs;
	t_mesh			*meshes;
	t_tr_list		*triangles;
	t_tr_list		*triangles_head;
}				t_app;

void		clip_triangle(t_tr_list **tr_lst);

void		debug_mouse(t_app *app, char *event, int key_code);
void		debug_keyboard(char *event, int key_code);
void		do_input(t_app *app);

void		*image_clear(void *b, int c, size_t len);

void 		app_close(t_app *app);

void		init_app(t_app *app);

void		update_inputs(t_app *app);

int 		keyboard_event_down(int key_code, t_app *app);
int 		keyboard_event_up(int key_code, t_app *app);

int 		mouse_event_down(int key_code, int x, int y, t_app *app);
int 		mouse_event_up(int key_code, int x, int y, t_app *app);
int 		mouse_event_move(int x, int y, t_app *app);

int			window_event_close(t_app *app);
int			window_event_expose(t_app *app);

void		reset_inputs_states(t_app *app);

void		project_triangle(t_triangle *tr, t_mat4x4 *proj_mat);
void		translate_triangle(t_triangle *tr, t_app *app);
void		offset_triangle(t_triangle *tr, t_app *app);

void		set_vector(t_vector *v, float x, float y, float z);

t_mat4x4	rotation_mat_z(float angle);
t_mat4x4	rotation_mat_x(float angle);
t_mat4x4	rotation_mat_y(float angle);

void		set_triangle(t_triangle *t, t_vector *v0, t_vector *v1, t_vector *v2);

void		set_color(t_color *color, int r, int g, int b);

void		set_pixel(SDL_Surface *s, int x, int y, t_color *c);
void		draw_line(t_app *app, t_vector *start, t_vector *end, t_color color);

void		check_triangle(t_app *app, t_triangle *tr);
void		render_triangle(t_app *app, t_triangle tr);
void		fill_triangle(t_app *app, t_triangle t);

void		show_fps_sdl(t_timer *timer);

void		make_cube(t_mesh *m, float size);

void		get_ticks(t_timer *timer);
void		get_delta_time(t_timer *timer);
void		show_fps(t_app *app);

void		draw_cross(t_app *app, float size, int r, int g, int b);

void		get_color(SDL_Surface *surface, int x, int y, t_color *c);
t_color		new_color(int r, int g, int b);

int 		out_of_borders(int x, int y);
int 		color_key(t_color *c);
void		quit_properly(t_app *app);

int			event_handling(t_app *app);

t_tr_list	*new_triangle_list(int len);
t_triangle	new_triangle(t_vector v0, t_vector v1, t_vector v2);

void		mouse_update(t_app *app);

t_mat4x4	matrix_subtraction(t_mat4x4 m1, t_mat4x4 m2);
t_mat4x4	matrix_summary(t_mat4x4 m1, t_mat4x4 m2);
t_mat4x4	matrix_multiply_matrix(t_mat4x4 m1, t_mat4x4 m2);
t_vector	matrix_multiply_vector(t_mat4x4 m, t_vector v);
t_mat4x4	matrix_look_at(t_vector from, t_vector to);
t_mat4x4	matrix_inverse(t_mat4x4 m);
t_mat4x4	matrix_identity();
t_mat4x4	init_translation_mat(t_vector trans_v);

void		transform_vertices(t_app *app, int mesh_id);
void		assemble_triangles(t_app *app, int mesh_id);
void		check_triangles(t_app *app, int mesh_id);
void		draw_triangles(t_app *app);

t_vector	new_vector(float x, float y, float z);
t_vector	vector_sum(t_vector vector1, t_vector vector2);
t_vector	vector_sub(t_vector vector1, t_vector vector2);
t_vector	vector_mul_by(t_vector v, float k);
t_vector	vector_div_by(t_vector v, float k);
t_vector	vector_normalise(t_vector v);
float		vector_length(t_vector v);
t_vector	vector_cross_product(t_vector v1, t_vector v2);
float		vector_dot_product(t_vector v1, t_vector v2);

void		read_obj(char *path, t_mesh *mesh, int vrs, int trs);
#endif
