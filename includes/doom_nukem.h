#ifndef DOOM_NUKEM_H
#define DOOM_NUKEM_H

# include "libft.h"
# include <math.h>
# include <pthread.h>
# include "stdio.h"
# include <SDL.h>
# include <SDL_ttf.h>

# define	PRINT_DEBUG 0

//# define	SCREEN_W 1920
//# define	SCREEN_H 1080

//# define	SCREEN_W 1280
//# define	SCREEN_H 720

# define	SCREEN_W 640
# define	SCREEN_H 360

//# define	SCREEN_W 480
//# define	SCREEN_H 270

//# define	SCREEN_W 320
//# define	SCREEN_H 180

# define	WIN_TITLE "DOOM-NUKEM"

# define	TRANSPARENCY_COLOR 0xffff00ff
# define	COLOR_KEY_G 0
# define	COLOR_KEY_B 254

# define	OUTLINE_COLOR new_color(0, 0, 0)

# define MIN(a,b) (((a)<(b))?(a):(b))
# define MAX(a,b) (((a)>(b))?(a):(b))
# define ABS(N) ((N<0)?(-N):(N))
# define SIGN(x) ((x < 0) ? -1 : 1);
# define SWAP(x, y, T) do { T SWAP = x; x = y; y = SWAP; } while (0)
# define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
# define TO_RAD(a) (a * M_PI / 180.0)
# define TO_DEG(a) (a * 180.0 / M_PI)

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

typedef struct	s_mat4x4
{
	double m[16];
}				t_mat4x4;

typedef struct	s_v3d
{
	double		x;
	double		y;
	double		z;
	double 		w;
	double		tex_x;
	double		tex_y;
}				t_v3d;

typedef struct	s_wall
{
	t_v3d 		v[4];
	int			sprite_index;
	double 		scale_x;
	double 		scale_y;
	t_v3d		pos;
	t_v3d		quad;
	double 		size;
}				t_wall;

typedef struct	s_v2d
{
	double		u;
	double		v;
	double		w;
}				t_v2d;

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

typedef struct	s_edge
{
	double		x;
	double		x_step;
	int 		y_start;
	int 		y_end;
	double		tex_x;
	double		tex_x_step;
	double		tex_y;
	double		tex_y_step;
	double		tex_z;
	double		tex_z_step;
	double		depth;
	double		depth_step;
}				t_edge;

typedef struct	s_gradient
{
	double		x[3];
	double		y[3];
	double		z[3];
	double		depth[3];
	double		x_x_step;
	double		x_y_step;
	double		y_x_step;
	double		y_y_step;
	double		z_x_step;
	double		z_y_step;
	double		one_over_dx;
	double		one_over_dy;
	double 		d_x_step;
	double 		d_y_step;
}				t_gradient;

typedef struct	s_triangle
{
	int			iv[3];
	int			it[3];
	int			in[3];
	t_v3d		v[3];
	t_v2d		t[3];
	t_v3d		n[3];
	t_color		color;
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

typedef struct	s_mesh
{
	t_v3d		*vo;
	t_v3d		*vb;
	t_v2d		*tx;
	t_triangle	*tr;
	int 		v_count;
	int 		tx_count;
	int 		tr_count;
}				t_mesh;

typedef struct	s_line
{
	t_v3d		cur;
	t_v3d		dir;
	t_v3d		inc;
	uint32_t 	color;
}				t_line;

typedef struct	s_mouse_state
{
	int 		x;
	int 		y;
	int 		left;
	int 		right;
	int 		middle;
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
	t_v3d		pos_prev;
	t_v3d		rot;
	t_v3d		dir;
	t_mat4x4	view;
	t_mat4x4	rotation;
	t_mat4x4	projection;
	t_mat4x4	translation;
	t_mat4x4	view_projection;
	t_mat4x4	transform;
	t_mat4x4	screen_space;
	int 		quad;
	t_v3d		up;
	t_v3d		right;
}				t_camera;

typedef struct	s_timer
{
	Uint64		prev;
	Uint64		fps;
	double		delta;
	double		time;
	Uint64		frame;
}				t_timer;

typedef struct	s_inputs
{
	const Uint8		*keyboard;
	t_mouse_state	mouse;
	int				x;
	int				y;
}				t_inputs;

typedef struct	s_sdl
{
	SDL_Event		event;
	SDL_Window		*window;
	SDL_Surface		*surface;
	SDL_Surface		*screen;
	int				height;
	int				width;
}				t_sdl;

typedef struct	s_vr_list
{
	t_v3d				v;
	struct s_vr_list	*next;
}				t_vr_list;

typedef struct	s_polygon
{
	t_v3d				v;
	struct s_polygon	*next;
	struct s_polygon	*prev;
	int 				is_ear;
	double 				angle;
}				t_polygon;

typedef struct	s_scanline
{
	int			start;
	int			end;
	double		pre_step;
	double		dist;
	double		x_step;
	double		y_step;
	double		z_step;
	double 		d_step;
	double		tex_x;
	double		tex_y;
	double		tex_z;
	double		depth;
	double 		scale_x;
	double 		scale_y;
}				t_scanline;

typedef struct 	s_intersect
{
	t_v3d		v0v1;
	t_v3d		v0v2;
	t_v3d		qvec;
	t_v3d		tvec;
	t_v3d		pvec;
	double		u;
	double		v;
	double		t;
	double		det;
}				t_intersect;

typedef struct	s_clip_data
{
	t_v3d		v;
	Uint8		is_inside;
	double 		value;
}				t_clip_data;

typedef struct	s_sector
{
	t_wall		*walls;
	t_wall		*objs;
	t_wall		floor;
	t_wall		ceil;
	int 		walls_count;
	int 		objs_count;
	double 		floor_height;
	double 		ceil_height;
	int			ready;
	t_polygon 	*polygon;
	t_triangle	*triangles;
	int 		triangles_count;
	double 		x_min;
	double 		z_min;
	double 		x_max;
	double 		z_max;
}				t_sector;

typedef struct	s_skybox
{
	t_v3d		v[8];
	int			sprite_index;
	double 		value;
}				t_skybox;

typedef struct	s_depth_chunk
{
	double		z[SCREEN_W];
}				t_depth_chunk;

typedef struct	s_screen_chunk
{
	Uint32		z[SCREEN_W];
}				t_screen_chunk;

typedef struct	s_app
{
	t_timer		*timer;
	t_camera	*camera;
	t_mat4x4	projection_mat;
	t_sdl		*sdl;
	t_inputs	*inputs;
	t_mesh		*meshes;
	t_sprite	*sprites;
	int 		sprites_count;
	double		*depth_buffer;
	TTF_Font	*font;
	int 		hit;
	int 		hit_first;
	t_wall		*hit_wall;
	t_v3d		hit_point;
	double 		hit_dist;
	int 		edge_selected;
	t_wall		edit_wall;
	t_sector	*sectors;
	int 		current_sector;
	int 		sectors_count;
	int			input_g;
	double 		grid_size;
	double 		input_plus;
	double 		input_minus;
	int 		tex_switch;
	t_wall		*render_wall;
	int 		triangles_counter;
	int 		collide_x;
	int 		collide_z;
	t_v3d 		collide_point;
	t_v3d		prev_pos;
	t_skybox	skybox;
	t_depth_chunk	depth_chunk;
	t_depth_chunk	*depth_chunk_array;
	t_screen_chunk	screen_chunk;
	t_screen_chunk	*screen_chunk_array;
}				t_app;

t_v3d	get_forward(t_v3d qt);
t_v3d	get_right(t_v3d qt);

void 	draw_polygon_line(t_app *app, t_v3d start, t_v3d end);
void	triangulate(t_sector *current_sector);

void	process_inputs(t_app *app, double delta_time);
void 	update_camera(t_camera *camera);

void	wall_reset_tex(t_wall *w);
t_wall	wall_new();
void 	wall_update_scale(t_wall *w);
void	update_walls_data(t_app *app);
t_edge	edge_new(t_gradient	g, t_v3d min, t_v3d max, int index);
void	edge_step(t_edge *edge);
double 		gradient_calc_x_step(double coords[3], t_triangle tr, double one_over_dx);
double 		gradient_calc_y_step(double coords[3], t_triangle tr, double one_over_dy);
t_gradient	gradient_new(t_v3d min, t_v3d mid, t_v3d max);
void 	draw_grid(t_app *app);
void 	print_to_screen(t_app *app, int x, int y, char *text);
int 	vertex_inside(t_v3d *v);
void vertex_perspective_divide(t_v3d *v);
void	polygon_add(t_polygon **poly, t_v3d v);
void	scan_triangle(t_app *app, t_v3d min, t_v3d mid, t_v3d max, int handedness);
void	ray_intersect(t_app *app, t_v3d v0, t_v3d v1, t_v3d v2);
void	vr_list_add(t_vr_list **list, t_v3d v);
void 	vr_list_free(t_vr_list **list);
t_vr_list	*vr_list_last(t_vr_list *head);
void 	clip_fill_triangle(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3);
int		render_triangle(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3);
void 	get_sector_min_max(t_sector *cs);
void 	render_wall(t_app *app, t_wall *w);
void 	render_billboard(t_app *app, t_wall *w);
void	render_map(t_app *app);
void 	fill_triangle(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3);
void 	draw_cross(t_app *app, int x, int y, double size, Uint32 color);
void	update_fps_text(t_app *app);
int		find_linked_wall(t_sector *sector, t_v3d v, int skip);
double	get_orientation(t_v3d *polygon, int size);
int 	compare_vertex(t_v3d *v1, t_v3d *v2);
void 	get_floor_poly(t_sector *cs);
void 	sector_close(t_app *app);
void	draw_new_wall(t_app *app);
void	save_new_wall(t_app *app);
void 	draw_edge(t_app *app, t_v3d edge);
void 	render_skybox(t_app *app, t_skybox s);

void	texture_change(t_app *app);
void	texture_scale_y_change(t_app *app);
void	texture_scale_x_change(t_app *app);

t_mat4x4 	get_transform_matrix(t_mat4x4 view_projection);

void		exit_with_status(int status, char *fnf_path);

void		*image_clear(void *b, int c, size_t len);

void 		app_close(t_app *app);

void		init_app(t_app *app);

void		set_vector(t_v3d *v, double x, double y, double z);

t_mat4x4	matrix_rotation(double x, double y, double z);

void		set_color(t_color *color, int r, int g, int b);
uint32_t	sprite_get_color(t_sprite *s, int offset);
void		set_pixel_uint32(SDL_Surface *surface, int offset, Uint32 c);
void		set_pixel(SDL_Surface *surface, int x, int y, t_color c);
void		draw_line(t_app *app, t_v3d *start, t_v3d *end, uint32_t color);
void		sprite_draw(SDL_Surface *screen, t_sprite *sprite, int x, int y, int size_x, int size_y);

void		get_ticks(t_timer *timer);
void		get_delta_time(t_timer *timer);
void 	show_edge(t_app *app);

void		get_color(SDL_Surface *surface, int x, int y, t_color c);
t_color		color_new(int r, int g, int b);
t_color		color_sub(t_color color, int k);

int 		out_of_borders(int x, int y);
int 		color_key(t_color c);
void		quit_properly();

int			event_handling(t_app *app);

void		mouse_update(t_app *app);

t_mat4x4	matrix_screen_space();
t_mat4x4	matrix_perspective(double fov, double aps_ratio, double z_near, double z_far);
t_mat4x4	matrix_translation(double x, double y, double z);
t_mat4x4	matrix_multiply(t_mat4x4 m1, t_mat4x4 m2);
t_v3d		matrix_transform(t_mat4x4 mat, t_v3d v);

t_mat4x4	matrix_subtraction(t_mat4x4 m1, t_mat4x4 m2);
t_mat4x4	matrix_summary(t_mat4x4 m1, t_mat4x4 m2);
t_mat4x4	matrix_multiply_matrix(t_mat4x4 m1, t_mat4x4 m2);
t_v3d		matrix_multiply_vector(t_mat4x4 m, t_v3d v);
t_mat4x4	matrix_look_at(t_v3d from, t_v3d to);
t_mat4x4	matrix_inverse(t_mat4x4 m);
t_mat4x4	matrix_identity();
t_mat4x4	init_translation_mat(t_v3d trans_v);

t_v3d		new_vector(double x, double y, double z);
t_v3d		vector_sum(t_v3d vector1, t_v3d vector2);
t_v3d		vector_sub(t_v3d vector1, t_v3d vector2);
t_v3d		vector_mul_by(t_v3d v, double k);
t_v3d		vector_div_by(t_v3d v, double k);
t_v3d		vector_normalise(t_v3d v);
double		vector_length(t_v3d v);
t_v3d		vector_cross_product(t_v3d v1, t_v3d v2);
double		vector_dot_product(t_v3d v1, t_v3d v2);

void		bmp_load(t_app *app, char *path);
void		obj_load(char *path, t_mesh *mesh);

#endif
