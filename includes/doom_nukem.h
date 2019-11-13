#ifndef DOOM_NUKEM_H
#define DOOM_NUKEM_H

# include "libft.h"
# include <math.h>
# include <pthread.h>
# include "stdio.h"
# include <SDL.h>
# define PRINT_DEBUG 0

# define	SCREEN_W 1280
# define	SCREEN_H 720
# define	WIN_TITLE "DOOM-NUKEM"

# define	COLOR_KEY_R 255
# define	COLOR_KEY_G 0
# define	COLOR_KEY_B 254

# define	OUTLINE_COLOR new_color(0, 0, 0)

# define MIN(a,b) (((a)<(b))?(a):(b))
# define MAX(a,b) (((a)>(b))?(a):(b))
# define ABS(N) ((N<0)?(-N):(N))
# define SIGN(x) ((x < 0) ? -1 : 1);
# define SWAP(x, y, T) do { T SWAP = x; x = y; y = SWAP; } while (0)
# define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

# define RESOURCES_MD5 "92e21b66507aebb010adb353fc83badb"

# define STATUS_OK 0
# define STATUS_FILE_NOT_FOUND 1
# define STATUS_BAD_RESOURCES 2

# define MSG_OK "OK."
# define MSG_FILE_NOT_FOUND "FILE NOT FOUND: "
# define MSG_BAD_RESOURCES "BAD RESOURCES!"

typedef struct	s_color
{
	uint8_t		r;
	uint8_t		g;
	uint8_t		b;
}				t_color;

typedef struct	s_image_chunk
{
	unsigned char z[1024];
}				t_image_chunk;

typedef struct	s_mat4x4
{
	double m[4][4];
}				t_mat4x4;

typedef struct	s_v3d
{
	double		x;
	double		y;
	double		z;
	double 		w;
}				t_v3d;

typedef struct	s_v2d
{
	double		u;
	double		v;
	double		w;
}				t_v2d;

typedef struct	s_io
{
	t_v3d		vin[3];
	t_v3d		vout[3];
	t_v2d		tin[3];
	t_v2d		tout[3];
	int 		vins;
	int 		vouts;
}				t_io;

typedef struct	s_bmp_header
{
	uint16_t	type;
	uint32_t	size;
	uint16_t	reserved1;
	uint16_t	reserved2;
	uint32_t	offset;
	uint32_t	dib_header_size;
	int32_t		width_px;
	int32_t		height_px;
	uint16_t	num_planes;
	uint16_t	bits_per_pixel;
	uint32_t	compression;
	uint32_t	image_size_bytes;
	int32_t		x_resolution_ppm;
	int32_t		y_resolution_ppm;
	uint32_t	num_colors;
	uint32_t	important_colors;
}				t_bmp_header;

typedef struct	s_sprite
{
	t_bmp_header	header;
	uint32_t		*pixels;
}				t_sprite;

typedef struct	s_triangle
{
	int			iv[3];
	int			it[3];
	int			in[3];
	t_v3d		v[3];
	t_v2d		t[3];
	t_v3d		n[3];
	t_color		color;
	double 		light_dp;
	int 		visible;
}				t_triangle;

typedef struct	s_tex_v
{
	int			x;
	int			y;
	int			z;
	double		u;
	double		v;
	double		w;
	t_sprite	*s;
}				t_tex_v;

typedef struct	s_tex_tr
{
	t_tex_v		v[3];
	t_sprite	*s;
}				t_tex_tr;

typedef struct	s_mesh
{
	t_v3d		*vo;
	t_v3d		*vb;
	t_v2d		*tx;
	t_triangle	*tr;
	int 		v_count;
	int 		tx_count;
	int 		tr_count;
	t_v3d		pos;
	t_v3d		rot;
	t_mat4x4	rot_mat_x;
	t_mat4x4	rot_mat_y;
	t_mat4x4	rot_mat_z;
	t_mat4x4	trans_mat;
	t_mat4x4	transform;
}				t_mesh;

typedef struct	s_line
{
	t_v3d		cur;
	t_v3d		dir;
	t_v3d		inc;
	t_color		color;
}				t_line;

typedef struct	s_mouse_state
{
	double 		x;
	double 		y;
	int 		left;
	int 		right;
	int 		middle;
	int 		scroll_u;
	int 		scroll_d;
	double 		sens;
}				t_mouse_state;

typedef struct	s_camera
{
	double 		z_near;
	double 		z_far;
	double 		fov;
	double 		for_rad;
	double 		asp_ratio;
	t_v3d		pos;
	t_v3d		rot;
	t_mat4x4	rot_mat_x;
	t_mat4x4	rot_mat_y;
	t_mat4x4	rot_mat_z;
	t_v3d		target;
	t_v3d		dir;
	t_mat4x4	rot_mat;
	t_mat4x4	view_mat;
}				t_camera;

typedef struct	s_world
{
	t_v3d		rot;
	t_v3d		trans;
	t_mat4x4	rot_mat_x;
	t_mat4x4	rot_mat_y;
	t_mat4x4	rot_mat_z;
	t_mat4x4	trans_mat;
	t_mat4x4	mat;
}				t_world;

typedef struct	s_timer
{
	Uint64		curr;
	Uint64		fps;
	double		delta;
	double		time;
}				t_timer;

typedef struct		s_inputs
{
	const Uint8		*keyboard;
	t_mouse_state	mouse;
	int				left_pressed;
	int				right_pressed;
	double			sensitivity;
	int				x;
	int				y;
	int				zoom;
}					t_inputs;

typedef struct		s_sdl
{
	SDL_Event		event;
	SDL_Window		*window;
	SDL_Surface		*surface;
	double			half_height;
	double			half_width;
	int				height;
	int				width;
}					t_sdl;

typedef struct		s_tr_list
{
	t_triangle			tr;
	struct s_tr_list	*next;
}					t_tr_list;

typedef struct	s_plane
{
	t_v3d		p;
	t_v3d		n;
}				t_plane;

typedef struct	s_app
{
	t_timer		*timer;
	t_camera	*camera;
	t_world		*world;
	t_mat4x4	projection_mat;
	t_sdl		*sdl;
	t_inputs	*inputs;
	t_mesh		*meshes;
	t_sprite	*sprites;
	double		*z_buf;
}				t_app;

void		clip_triangles(t_tr_list **tr_lst);

void		exit_with_status(int status, char *fnf_path);

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

void		set_vector(t_v3d *v, double x, double y, double z);

t_mat4x4	rotation_mat_z(double angle);
t_mat4x4	rotation_mat_x(double angle);
t_mat4x4	rotation_mat_y(double angle);

void		set_triangle(t_triangle *t, t_v3d *v0, t_v3d *v1, t_v3d *v2);

void		set_color(t_color *color, int r, int g, int b);
uint32_t	sprite_get_color_by_uv(t_sprite *s, double u, double v);
void		set_pixel_uint32(SDL_Surface *surface, int offset, Uint32 c);
void		set_pixel(SDL_Surface *surface, int x, int y, t_color c);
void		draw_line(t_app *app, t_v3d *start, t_v3d *end, t_color color);
void		sprite_draw(SDL_Surface *screen, t_sprite *sprite, int x, int y, int size_x, int size_y);

void		check_triangle(t_app *app, t_triangle *tr);
void		render_triangle(t_app *app, t_triangle tr);
void		fill_triangle(t_app *app, t_triangle t);

void		show_fps_sdl(t_timer *timer);

void		make_cube(t_mesh *m, double size);

void		get_ticks(t_timer *timer);
void		get_delta_time(t_timer *timer);
void		show_fps(t_app *app);

void		draw_cross(t_app *app, double size, int r, int g, int b);

void		get_color(SDL_Surface *surface, int x, int y, t_color c);
t_color		color_new(int r, int g, int b);
t_color		color_sub(t_color color, int k);

int 		out_of_borders(int x, int y);
int 		color_key(t_color c);
void		quit_properly(t_app *app);

int			event_handling(t_app *app);

t_tr_list	*new_triangle_list(int len);
t_triangle	new_triangle(t_v3d v0, t_v3d v1, t_v3d v2);

void		mouse_update(t_app *app);

t_mat4x4	matrix_subtraction(t_mat4x4 m1, t_mat4x4 m2);
t_mat4x4	matrix_summary(t_mat4x4 m1, t_mat4x4 m2);
t_mat4x4	matrix_multiply_matrix(t_mat4x4 m1, t_mat4x4 m2);
t_v3d		matrix_multiply_vector(t_mat4x4 m, t_v3d v);
t_mat4x4	matrix_look_at(t_v3d from, t_v3d to);
t_mat4x4	matrix_inverse(t_mat4x4 m);
t_mat4x4	matrix_identity();
t_mat4x4	init_translation_mat(t_v3d trans_v);

void		transform_vertices(t_app *app, int mesh_id);
void		assemble_triangles(t_app *app, int mesh_id);
void		check_triangles(t_app *app, int mesh_id);

t_v3d		new_vector(double x, double y, double z);
t_v3d		vector_sum(t_v3d vector1, t_v3d vector2);
t_v3d		vector_sub(t_v3d vector1, t_v3d vector2);
t_v3d		vector_mul_by(t_v3d v, double k);
t_v3d		vector_div_by(t_v3d v, double k);
t_v3d		vector_normalise(t_v3d v);
double		vector_length(t_v3d v);
t_v3d		vector_cross_product(t_v3d v1, t_v3d v2);
double		vector_dot_product(t_v3d v1, t_v3d v2);

void		bmp_load(char *path, t_sprite *sprite);
void		obj_load(char *path, t_mesh *mesh);

#endif
