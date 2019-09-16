#include "doom_nukem.h"

void	get_coords(t_mesh *mesh, char *line)
{
	char **data;

	data = ft_strsplit(line, ' ');
	mesh->v[mesh->v_idx].x = strtof(data[1], NULL);
	mesh->v[mesh->v_idx].y = strtof(data[2], NULL);
	mesh->v[mesh->v_idx].z = strtof(data[3], NULL);
	mesh->v_idx++;
	ft_delarr(data);
}

void	get_triangle(t_mesh *mesh, char *line)
{
	char **data;

	data = ft_strsplit(line, ' ');
	mesh->t[mesh->t_idx].v[0] = mesh->v[ft_atoi(data[1]) - 1];
	mesh->t[mesh->t_idx].v[1] = mesh->v[ft_atoi(data[2]) - 1];
	mesh->t[mesh->t_idx].v[2] = mesh->v[ft_atoi(data[3]) - 1];
	set_color(&mesh->t[mesh->t_idx].color, 128, 128, 128);
	mesh->t_idx++;
	ft_delarr(data);
}

void	read_obj(char *path, t_mesh *mesh)
{
	int		fd;
	char	*line;

	fd = open(path, O_RDONLY);

	if (fd < 3)
	{
		ft_putstr("error: obj file not found\n");
		exit(0);
	}
	mesh->v = (t_vector *)malloc(sizeof(t_vector) * 3644);
	mesh->t = (t_triangle *)malloc(sizeof(t_triangle) * 6320);
	mesh->v_idx = 0;
	mesh->t_idx = 0;
	while(ft_gnl(fd, &line))
	{
		if (line[0] == 'v' && line[1] == ' ')
			get_coords(mesh, line);
		if (line[0] == 'f' && line[1] == ' ')
		{
			get_triangle(mesh, line);
		}
		ft_strdel(&line);
	}
	ft_strdel(&line);
}

