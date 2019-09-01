#ifndef DOOM_NUKEM_H
#define DOOM_NUKEM_H

# include "mlx.h"
# include "libft.h"
# include <math.h>
# include <pthread.h>
# include "stdio.h"

# define PRINT_DEBUG 1

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

# define	SCREEN_W 640
# define	SCREEN_H 480
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

typedef struct	s_app
{
	void			*mlx;
	t_window		window;
	t_image			screen;
	t_kb_keys_state	keyboard;
	t_mouse_state	mouse;
} 				t_app;

void	debug_mouse(t_app *app, char *event, int key_code);
void	debug_keyboard(char *event, int key_code);

void	init_window(t_window *window, void *mlx);
void	init_image(t_image *image, t_window *window, void *mlx);
void	init_app(t_app *app);

int 	keyboard_event_down(int key_code, t_app *app);
int 	keyboard_event_up(int key_code, t_app *app);

int 	mouse_event_down(int key_code, int x, int y, t_app *app);
int 	mouse_event_up(int key_code, int x, int y, t_app *app);
int 	mouse_event_move(int x, int y, t_app *app);

int		window_event_close(t_app *app);
int		window_event_expose(t_app *app);

void	update_inputs(t_app *app);
void	reset_inputs_states(t_app *app);

void 	app_close(t_app *app);
void	set_pixel(t_image *image, int x, int y, t_color c);

#endif
