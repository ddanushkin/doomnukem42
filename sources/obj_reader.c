#include "doom_nukem.h"

void	get_coords(t_mesh *mesh, char *line)
{
	char **data;

	data = ft_strsplit(line, ' ');
	mesh->v[mesh->v_count].x = strtof(data[1], NULL);
	mesh->v[mesh->v_count].y = strtof(data[2], NULL);
	mesh->v[mesh->v_count].z = strtof(data[3], NULL);
	mesh->v[mesh->v_count].w = 1.0f;
	mesh->v_count++;
	ft_delarr(data);
}

void	get_triangle(t_mesh *mesh, char *line)
{
	char **data;

	data = ft_strsplit(line, ' ');
	mesh->t[mesh->t_count].i[0] = ft_atoi(data[1]) - 1;
	mesh->t[mesh->t_count].i[1] = ft_atoi(data[2]) - 1;
	mesh->t[mesh->t_count].i[2] = ft_atoi(data[3]) - 1;
	mesh->t_count++;
	ft_delarr(data);
}

void	read_obj(char *path, t_mesh *mesh, int vrs, int trs)
{
	int		fd;
	char	*line;

	fd = open(path, O_RDONLY);

	if (fd < 3)
	{
		ft_putstr("error: obj file not found\n");
		exit(0);
	}
	mesh->v = (t_vector *)malloc(sizeof(t_vector) * vrs);
	mesh->t = (t_triangle *)malloc(sizeof(t_triangle) * trs);
	mesh->v_count = 0;
	mesh->t_count = 0;
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

