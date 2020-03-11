#ifndef DOOM_NUKEM_H
#define DOOM_NUKEM_H

# include "libft.h"
# include <math.h>
# include <pthread.h>
# include "stdio.h"
# include <SDL.h>
# include <SDL_ttf.h>
# include <SDL_mixer.h>

//# define	SCREEN_W 1920
//# define	SCREEN_H 1080

//# define	SCREEN_W 1280
//# define	SCREEN_H 720

# define	SCREEN_W 1024
# define	SCREEN_H 576

//# define	SCREEN_W 640
//# define	SCREEN_H 360

//# define	SCREEN_W 480
//# define	SCREEN_H 270

//# define	SCREEN_W 320
//# define	SCREEN_H 200

//# define	SCREEN_W 320
//# define	SCREEN_H 180

# define	WIN_TITLE "DOOM-NUKEM"
# define	TRANSPARENCY_COLOR 0xffff00ff

# define MIN(a,b) (((a)<(b))?(a):(b))
# define MAX(a,b) (((a)>(b))?(a):(b))
# define ABS(N) ((N<0)?(-N):(N))
# define SIGN(x) ((x < 0) ? -1 : 1)
# define SIGNF(x) ((x < 0.0) ? -1.0 : 1.0)
# define SWAP(x, y, T) do { T SWAP = x; x = y; y = SWAP; } while (0)
# define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
# define TO_RAD(a) (a * M_PI / 180.0)
# define TO_DEG(a) (a * 180.0 / M_PI)

# define RESOURCES_MD5 "308e4f5eda0502b663ce403aa8b71726"

# define STATUS_OK 0
# define STATUS_FILE_NOT_FOUND 1
# define STATUS_BAD_RESOURCES 2

# define MSG_OK "OK."
# define MSG_FILE_NOT_FOUND "FILE NOT FOUND: "
# define MSG_BAD_RESOURCES "BAD RESOURCES!"
# define DECOR_LEN 0.35355339059327379
# define DECOR_LEN_HALF 0.17677669529663689

# define SDL_MOUSE_LEFT 1
# define SDL_MOUSE_RIGHT 3
# define SDL_MOUSE_MIDDLE 2
# define SDL_MOUSE_SCROLL_UP 4
# define SDL_MOUSE_SCROLL_DOWN 5
# define THREADS_N 8

# define PLAYER_HEIGHT 1.1
# define PLAYER_SPEED 5.0
# define MOUSE_SPEED 1.2123
# define USE_DIST 1.25
# define LAVA_TIMER 0.5
# define HEAL_TIMER 0.025

# define WALL_SPRITE	162
# define OBJ_SPRITE		358
# define DECOR_SPRITE	372
# define FLOOR_SPRITE	151
# define CEIL_SPRITE	171
# define LAVA_SPRITE	60
# define DOOR_SIDE		227
# define DOOR_TOP_BOT	229
# define LAST_TEX		476
# define HEALER_SPRITE	105

# define MAX_SECTOR		100
# define MAX_WALL		50
# define MAX_DECOR		25
# define MAX_NPC		25
# define MAX_OBJ		25
# define MAX_SPRITE		487
# define MAX_SFX		107
# define MAX_BG			11

# define HUD_CARD_SPRITE 234
# define HUD_CARD_HOLDER 232
# define HUD_CARD_POS_X	(SCREEN_W - 100 - 64)
# define HUD_CARD_POS_Y	(SCREEN_H - 100 - 64)

# define HUD_HP_HOLDER	105
# define HUD_HP_POS_X	100
# define HUD_HP_POS_Y	(SCREEN_H - 100 - 64)

# define HUD_TIME_HOLDER 42
# define HUD_TIME_POS_X	(SCREEN_W / 2 - 32)
# define HUD_TIME_POS_Y	(SCREEN_H - 100 - 64)

# ifndef MULTITHREAD
#  ifdef __APPLE__
#    define MULTITHREAD
#  endif
# endif

enum e_hit_type
{
	wall,
	decor,
	npc,
	obj,
	floor_ceil
};

typedef struct		s_mat4x4
{
	double			m[16];
}					t_mat4x4;

typedef struct		s_v3d
{
	double			x;
	double			y;
	double			z;
	double 			w;
	double			tex_x;
	double			tex_y;
	int				i;
}					t_v3d;

typedef struct		s_vm_data
{
	double			cosp;
	double			sinp;
	double			cosy;
	double			siny;
	t_v3d			xa;
	t_v3d			ya;
	t_v3d			za;
}					t_vm_data;

typedef struct		s_wall
{
	t_v3d 			v[4];
	int				sprite;
	double 			sx;
	double 			sy;
	double			ox;
	double			oy;
	t_v3d			pos;
	t_v3d			quad;
	double 			size;
	double			shade;
	uint32_t 		inside;
	int 			decor;
	int 			is_exit;
	int				active;
	int				flip;
	enum e_hit_type	type;
	int 			collect;
	int 			is_card;
	int 			use;
	int				healer;
	int 			healer_cap;
	int 			rotate;
	int 			ori;
}					t_wall;

typedef struct		s_bmp_header
{
	uint16_t		type;
	uint32_t		size;
	uint16_t		reserved1;
	uint16_t		reserved2;
	uint32_t		offset;
	uint32_t		dib_header_size;
	int32_t			width_px;
	int32_t			height_px;
	uint16_t		num_planes;
	uint16_t		bits_per_pixel;
	uint32_t		compression;
	uint32_t		image_size_bytes;
	int32_t			x_resolution_ppm;
	int32_t			y_resolution_ppm;
	uint32_t		num_colors;
	uint32_t		important_colors;
}					t_bmp_header;

typedef struct		s_sprite
{
	t_bmp_header	header;
	uint32_t		pixels[65536];
}					t_sprite;

typedef struct		s_edge
{
	double			x;
	double			x_step;
	int 			y_start;
	int 			y_end;
	double			tex_x;
	double			tex_x_step;
	double			tex_y;
	double			tex_y_step;
	double			tex_z;
	double			tex_z_step;
	double			depth;
	double			depth_step;
}					t_edge;

typedef struct		s_gradient
{
	double			x[3];
	double			y[3];
	double			z[3];
	double			depth[3];
	double			x_x_step;
	double			x_y_step;
	double			y_x_step;
	double			y_y_step;
	double			z_x_step;
	double			z_y_step;
	double			one_over_dx;
	double			one_over_dy;
	double 			d_x_step;
	double 			d_y_step;
}					t_gradient;

typedef struct		s_triangle
{
	t_v3d			v[3];
}					t_triangle;


typedef struct		s_line
{
	t_v3d			cur;
	t_v3d			dir;
	t_v3d			inc;
	uint32_t 		color;
}					t_line;

typedef struct		s_mouse_state
{
	int 			x;
	int 			y;
}					t_mouse_state;

typedef struct		s_camera
{
	double 			z_near;
	double 			z_far;
	double 			fov;
	double 			for_rad;
	double 			asp_ratio;
	t_v3d			pos;
	t_v3d			rot;
	t_v3d			dir;
	t_mat4x4		view;
	t_mat4x4		projection;
	t_mat4x4		view_projection;
	t_mat4x4		transform;
	t_mat4x4		screen_space;
	int 			quad;
	t_v3d			forward;
	t_v3d			up;
	t_v3d			right;
	int				fly;
	double 			prev_y;
}					t_camera;

typedef struct		s_timer
{
	Uint64			prev;
	Uint64			fps;
	Uint64			fps_count;
	Uint64			fps_sum;
	double			delta;
	double			time;
	Uint64			frame;
}					t_timer;

typedef struct		s_inputs
{
	const Uint8		*keyboard;
	t_mouse_state	mouse;
	int				x;
	int				y;
}					t_inputs;

typedef struct		s_sdl
{
	SDL_Window		*window;
	SDL_Surface		*surface;
	int				height;
	int				width;
}					t_sdl;

typedef struct			s_vr_list
{
	t_v3d				v;
	struct s_vr_list	*next;
}						t_vr_list;

typedef struct			s_polygon
{
	t_v3d				v;
	struct s_polygon	*next;
	struct s_polygon	*prev;
	int 				is_ear;
	double 				angle;
}						t_polygon;

typedef struct 		s_intersect
{
	t_v3d			v0v1;
	t_v3d			v0v2;
	t_v3d			qvec;
	t_v3d			tvec;
	t_v3d			pvec;
	double			u;
	double			v;
	double			t;
	double			det;
}					t_intersect;

typedef struct		s_clip_data
{
	t_v3d			v;
	Uint8			is_inside;
	double 			value;
}					t_clip_data;

typedef struct		s_sector
{
	t_wall			walls[MAX_WALL];
	t_wall			npc[MAX_NPC];
	t_wall			obj[MAX_OBJ];
	t_wall 			decor[MAX_DECOR];
	t_wall			floor;
	t_wall			ceil;
	uint8_t 		shade;
	int 			inside;
	int 			walls_count;
	int 			npcs_count;
	int 			objs_count;
	int 			decor_count;
	int 			decor_next;
	double 			floor_y;
	double 			ceil_y;
	double 			delta_y;
	double 			x_min;
	double 			z_min;
	double 			x_max;
	double 			z_max;
	t_v3d			fpts[MAX_WALL];
	t_v3d			cpts[MAX_WALL];
	t_triangle		ftrs[MAX_WALL - 2];
	t_triangle		ctrs[MAX_WALL - 2];
	int 			trs_count;
	int 			pts_count;
	int 			door;
	int 			need_card;
	int				lava;
	int 			door_anim;
	double 			door_dir;
	int				id;
}					t_sector;

typedef struct		s_hud_image
{
	int 			x;
	int 			y;
	uint8_t 		w;
	uint8_t 		h;
	double 			x_rat;
	double 			y_rat;
}					t_hud_data;

typedef struct		s_skybox
{
	t_v3d			v[8];
}					t_skybox;

typedef struct		s_depth_chunk
{
	double			z[SCREEN_W];
}					t_depth_chunk;

typedef struct		s_screen_chunk
{
	Uint32			z[SCREEN_W];
}					t_screen_chunk;

typedef struct		s_sl_data
{
	double			x;
	double			xs;
	double			y;
	double			ys;
	double			z;
	double			zs;
	double			d;
	double			ds;
	int				start;
	int				end;
	int				offset;
	uint8_t 		shade;
}					t_sl_data;

typedef struct		s_render
{
	int 			handedness;
	uint32_t		*t;
	double 			*depth;
	uint32_t		*screen;
	t_sl_data		sl[SCREEN_H];
	int				sl_counter;
	int 			y;
	double 			scale_x;
	double 			scale_y;
	uint8_t 		shade;
}					t_render;

typedef struct		s_thr_data
{
	t_render		*r;
	int				start;
	int 			end;
}					t_thr_data;

typedef struct		s_raw_sfx
{
	char			mem[150000];
	size_t			size;
}					t_raw_sfx;

typedef struct		s_raw_font
{
	char			mem[1650000];
	size_t			size;
}					t_raw_font;

typedef struct		s_raw_bg
{
	char			mem[1000000];
	size_t			size;
}					t_raw_bg;

typedef struct		s_pos_temp
{
	t_v3d			pos1;
	t_v3d			pos2;
}					t_pos_temp;

typedef struct		s_wus
{
	double			dx;
	double			dz;
	double			dy;
	double			v0;
	double			v1;
	double			v;
	double			h0;
	double			h1;
	double			h;
}					t_wus;

typedef struct		s_map_data
{
	t_v3d			start_pos;
	int 			start_set;
	int 			music_id;
	int 			card_set;
	int 			card_sector;
	t_v3d 			card_pos;
	int 			card_picked;
}					t_map_data;

typedef struct		s_app
{
	t_timer			*timer;
	t_camera		*camera;
	t_mat4x4		proj_mat;
	t_sdl			*sdl;
	t_inputs		*inputs;
	double			*depth_buffer;
	TTF_Font		*font;
	enum e_hit_type	hit_type;
	enum e_hit_type	render_type;
	t_wall			*hit_wall;
	t_v3d			hit_point;
	t_sector		*hit_sector;
	double 			hit_dist;
	t_sector		*cs;
	double 			grid_size;
	t_wall			*rw;
	int 			triangles_counter;
	t_skybox		skybox;
	t_depth_chunk	depth_chunk;
	t_depth_chunk	*depth_chunk_array;
	t_screen_chunk	screen_chunk;
	t_screen_chunk	*screen_chunk_array;
	int 			is_skybox;
	int 			hp;
	int 			font_size;
	uint32_t 		font_color;
	double			height;
	double 			y_vel;
	double 			y_acc;
	double 			speed;
	double			acc;
	int 			ground;
	t_v3d 			floor_point;
	double			floor_dist;
	double			prev_dy;
	t_sector		*floor_sector;
	t_v3d 			ceil_point;
	double			ceil_dist;
	t_sector		*ceil_sector;
	int				point_mode;
	t_v3d			points[MAX_WALL];
	int				points_count;
	double 			cursor_x;
	double 			cursor_y;
	t_map_data		md;
	uint8_t 		keys[512];
	uint8_t 		mouse[6];
	uint32_t 		*screen;
	t_sprite		*sprites;
	int 			sprites_count;
	t_raw_sfx		*audio;
	t_raw_bg		*music;
	t_raw_font		fonts;
	Mix_Chunk		**sfx;
	Mix_Music		**bg;
	t_sector		*sectors;
	int 			sectors_count;
	uint32_t 		t[65536];
	double 			bad_close;
	int				bflag;
	uint32_t 		bclr[2];
	int 			game_data_init;
	int				map_init;
	double 			falling;
	int 			jumped;
	t_v3d			ld;
	double 			lava_timer;
	int				hth;
	double 			temp;
	t_hud_data 		card_hud;
	t_hud_data 		hp_hud;
	t_hud_data 		time_hud;
	t_wall			card_w;
	double 			heal_tick;
}					t_app;

void		init_sdl(t_sdl *sdl);
void 		init_font(t_app *app);
void 		init_sfx(t_app *app);
void 		init_bg(t_app *app);
int			switch_mode(t_app *app);
void		point_mode_inputs(t_app *app);
void		live_mode_change_shade(t_app *app);
void		live_mode_use_wall(t_app *app);
void		live_mode_door_open(t_app *app);
void		live_mode_sector_io(t_app *app);
void		live_mode_rotate_npc(t_app *app);
void		live_mode_change_floor_h(t_app *app);
void		live_mode_change_ceil_h(t_app *app);
void		live_mode_wall_offset(t_app *app);
void		live_mode_wall_bot(t_app *app);
void		live_mode_wall_top(t_app *app);
void		live_mode_set_exit(t_app *app);
void		live_mode_set_start(t_app *app);
void		live_mode_set_bg(t_app *app);
void		live_mode_toggle_healer(t_app *app);
void		live_mode_toggle_door(t_app *app);
void		live_mode_toggle_lava(t_app *app);
void		live_mode_add_decor(t_app *app, int healer);
void		live_mode_add_obj(t_app *app);
void		live_mode_add_card(t_app *app);
void		live_mode_add_npc(t_app *app);
void 		render_sector(t_app *app, t_sector *s);
void 		sector_new(t_app *app, t_sector *s);
double		get_angle(t_v3d v0, t_v3d v1, t_v3d v2);
int 		is_inside(t_v3d p, t_v3d p1, t_v3d p2, t_v3d p3);
int			is_convex(t_v3d p1, t_v3d p2, t_v3d p3);
void		polygon_delete(t_polygon **p, t_polygon *del);
int 		polygon_size(t_polygon *p);
void 		sector_copy_v_1(t_sector *s, t_v3d *p, int len);
void 		sector_copy_v_2(t_sector *s, t_v3d *p, int len);
t_mat4x4	matrix_rotation_z(double angle);
t_mat4x4	matrix_rotation_y(double angle);
t_mat4x4	matrix_rotation_x(double angle);
void		scanline_threads(register t_render *r, int size);
void		ray_floor_ceil(t_app *app, t_v3d p, t_triangle tr);
void		get_quad(t_camera *c);
void		floor_is_lava(t_app *app, t_camera *c, double dt);
void		get_speed(t_app *app);
void 		check_collision(t_app *app, t_v3d *pos, t_v3d f);
void		sector_update_shade(t_sector *s);
t_wall		*decor_add(t_v3d lp, t_sector *cs, t_wall *hit_w, t_camera *cam);
void 		camera_live_mode(t_v3d *rot);
void 		camera_point_mode(t_v3d *pos, t_v3d *rot);
void 		point_draw(t_app *app, t_v3d p, Uint32 c);
t_v3d		point_2d_to_3d(t_app *app, double x, double z, int grid);
t_v3d		point_3d_to_2d(t_app *app, t_v3d p);
int 		point_in_screen(t_v3d p);
double		tr_area(t_v3d a, t_v3d b, t_v3d c);
int 		line_intersection(t_v3d v0, t_v3d v1, t_v3d v2, t_v3d v3);
void		points_add_check(t_v3d *points, int *size);
int			point_on_line(t_v3d v0, t_v3d v1, t_v3d p);
void 		sector_pts_h(t_v3d *pts, int size, double amount);
void		update_floor_dist(t_app *app, t_v3d new_pos);
void 		app_reset_floor_ceil_hit(t_app *app);
void 		font_reset(t_app *app);
void 		font_set(t_app *app, int size, uint32_t color);
void		state_reset(t_app *app);
int			hit_fill_data(t_app *app, double distance);
int			hit_fill_floor(t_app *app, t_v3d pos, t_v3d dir, double distance);
int			hit_fill_ceil(t_app *app, t_v3d pos, t_v3d dir, double distance);
void		player_crouch(t_app *app, t_camera *c, const uint8_t *key, double dt);
void		floor_collision(t_app *app, t_camera *c, double dt);
void		player_fly(t_app *app, t_camera *c);
void		player_jump(t_app *app);
void		player_movement(t_app *app, t_camera *c, const uint8_t *key, double dt);
void		view_rotation(t_mouse_state	*mouse, t_camera *c, double ms);
void		draw_hud(t_app *app);
t_hud_data	hud_image_new(int x, int y, int w, int h);
void 		move(t_v3d *v, t_v3d dir, double amount);
void 		draw_line_3d(t_app *app, t_v3d start, t_v3d end, uint32_t c);
void 		draw_point_mode(t_app *app);
void 		draw_grid_point(t_app *app, t_v3d *gp, Uint32 c);
void		triangulate(t_triangle *trs, int *trs_size, t_polygon *polygon);
void		process_inputs(t_app *app, double delta_time);
void		process_points_inputs(t_app *app, double delta_time);
void 		update_camera(t_app *app, t_camera *camera);
t_mat4x4	view_matrix(t_v3d eye, double pitch, double yaw);
void 		update_points_camera(t_camera *c);
void		live_mode_inputs(t_app *app);
void 		draw_points(t_app *app, t_v3d *p, int size);
void		draw_sectors(t_app *app);
void		wall_reset_tex(t_wall *w);
t_wall		wall_new();
void 		wall_update_scale(t_wall *w);
double 		calc_tex(double min, double cur, double max);
void 		wall_update_tex(t_wall *w);
t_edge		edge_new(t_gradient	g, t_v3d min, t_v3d max, int index);
void		edge_step(t_edge *edge);
double 		gradient_calc_x_step(double coords[3], t_triangle tr,double one_over_dx);
double 		gradient_calc_y_step(double coords[3], t_triangle tr, double one_over_dy);
t_gradient	gradient_new(t_v3d min, t_v3d mid, t_v3d max);
void 		draw_exit(t_app *app);
void 		draw_start(t_app *app);
void 		print_to_screen(t_app *app, int x, int y, char *text);
Uint8		vertex_inside(t_v3d *v);
void		vertex_perspective_divide(t_v3d *v);
void		polygon_add(t_polygon **poly, t_v3d v);
void		scan_triangle(t_v3d min, t_v3d mid, t_v3d max, t_render *r);
int			ray_intersect(t_app *app, t_v3d v0, t_v3d v1, t_v3d v2);
int			ray_floor(t_app *app, t_v3d pos, t_triangle tr);
int			ray_ceil(t_app *app, t_v3d pos, t_triangle tr);
void		vr_list_add(t_vr_list **list, t_v3d v);
void 		vr_list_free(t_vr_list **list);
t_vr_list	*vr_list_last(t_vr_list *head);
void 		clip_fill_triangle(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3);
void		render_triangle_0(t_app *app, t_v3d v0, t_v3d v1, t_v3d v2);
void		render_triangle_1(t_app *app, t_v3d v0, t_v3d v3, t_v3d v1);
void 		sector_min_max(t_sector *s);
void 		render_wall(t_app *app, t_wall *w);
void 		render_billboard(t_app *app, t_wall *w);
void		render_map(t_app *app);
void 		fill_triangle(t_app *app, t_v3d v1, t_v3d v2, t_v3d v3);
void 		draw_cross(t_app *app, int x, int y, double size);
void		update_fps_text(t_app *app);
double		get_orientation(t_v3d *polygon, int size);
t_polygon	*points_to_list(t_sector *s);
void 		sector_close(t_app *app, t_sector *s);
void		render_skybox(t_app *app, t_skybox s);
uint8_t		wall_outside(t_v3d *v0, t_v3d *v1, t_v3d *v2, t_v3d *v3);
uint32_t	wall_inside(t_v3d *v0, t_v3d *v1, t_v3d *v2, t_v3d *v3);
void		texture_change(t_app *app);
void		texture_scale_y_change(t_app *app);
void		texture_scale_x_change(t_app *app);
void 		sector_update_height(t_sector *cs, t_v3d *fpts, t_v3d *cpts);
t_mat4x4 	get_transform_matrix(t_mat4x4 view_projection);
void		exit_with_status(int status, char *fnf_path);
void		init_app(t_app *app);
t_mat4x4	matrix_rotation(double x, double y, double z);
void		pixel_set(SDL_Surface *surface, int offset, Uint32 c);
void		draw_line(t_app *app, t_v3d *start, t_v3d *end, uint32_t color);
t_v3d		get_triangle_normal(t_v3d v0, t_v3d v1, t_v3d v2);
double		signed_tetra_volume(t_v3d ba, t_v3d ca, t_v3d da);
void		get_delta_time(t_timer *timer);
void		quit_properly();
int			event_handling(t_app *app);
t_mat4x4	matrix_screen_space();
t_mat4x4	matrix_persp(double fov, double ar, double z_near, double z_far);
t_mat4x4	matrix_translation(double x, double y, double z);
t_mat4x4	matrix_multiply(t_mat4x4 m1, t_mat4x4 m2);
t_v3d		matrix_transform(t_mat4x4 mat, t_v3d v);
t_mat4x4	matrix_identity();
t_v3d		new_vector(double x, double y, double z);
t_v3d		v3d_sum(t_v3d vector1, t_v3d vector2);
t_v3d		v3d_sub(t_v3d vector1, t_v3d vector2);
t_v3d		v3d_mul_by(t_v3d v, double k);
t_v3d		v3d_div_by(t_v3d v, double k);
t_v3d		v3d_normalise(t_v3d v);
double		v3d_length(t_v3d v);
t_v3d		v3d_cross(t_v3d v1, t_v3d v2);
double		v3d_dot(t_v3d v1, t_v3d v2);
void		bmp_load(t_app *app, char *path);
#endif
