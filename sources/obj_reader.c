#include "doom_nukem.h"

void	get_coords(t_mesh *mesh, char *line)
{
	char **data;

	data = ft_strsplit(line, ' ');
	mesh->vo[mesh->v_count].x = strtof(data[1], NULL);
	mesh->vo[mesh->v_count].y = strtof(data[2], NULL);
	mesh->vo[mesh->v_count].z = strtof(data[3], NULL);
	mesh->vo[mesh->v_count].w = 1.0;
	mesh->v_count++;
	ft_delarr(data);
}

void	get_triangle(t_mesh *mesh, char *line)
{
	char	**data;
	int 	i;
	char	**tmp;

	i = 1;
	data = ft_strsplit(line, ' ');
	while (data[i])
	{
		/* TODO Fix split skip '//' */
		tmp = ft_strsplit(data[i], '/');
		mesh->tr[mesh->tr_count].iv[i - 1] = ft_atoi(tmp[0]) - 1;
		//mesh->tr[mesh->tr_count].it[i] = ft_atoi(tmp[1]) - 1;
		//mesh->tr[mesh->tr_count].in[i] = ft_atoi(tmp[2]) - 1;
		ft_delarr(tmp);
		i++;
	}
	mesh->tr_count++;
	ft_delarr(data);
}

void	set_counts(int fd, t_mesh *mesh)
{
	char	*line;
	char 	**data;
	int 	vrs;
	int 	trs;

	ft_gnl(fd, &line);
	data = ft_strsplit(line, ' ');
	vrs = ft_atoi(data[0]);
	trs = ft_atoi(data[1]);
	mesh->vo = (t_v3d *)malloc(sizeof(t_v3d) * vrs);
	mesh->vb = (t_v3d *)malloc(sizeof(t_v3d) * vrs);
	mesh->tr = (t_triangle *)malloc(sizeof(t_triangle) * trs);
	ft_strdel(&line);
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
	set_counts(fd, mesh);
	mesh->rot = new_vector(0.0, 0.0, 0.0);
	mesh->pos = new_vector(0.0, 0.0, 0.0);
	mesh->v_count = 0;
	mesh->tr_count = 0;
	mesh->tx_count = 0;
	mesh->nr_count = 0;
	while(ft_gnl(fd, &line))
	{
		if (line[0] == 'v' && line[1] == ' ')
			get_coords(mesh, line);
		if (line[0] == 'f' && line[1] == ' ')
			get_triangle(mesh, line);
		ft_strdel(&line);
	}
	ft_strdel(&line);
}

