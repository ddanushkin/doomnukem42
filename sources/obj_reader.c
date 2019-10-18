#include "doom_nukem.h"

void	obj_set_coords(t_mesh *mesh, char *line)
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

void	obj_set_triangle(t_mesh *mesh, char *line)
{
	char	**data;
	int 	i;
	char	**tmp;

	i = 1;
	data = ft_strsplit(line, ' ');
	while (data[i])
	{
		tmp = ft_strsplit(data[i], '/');
		mesh->tr[mesh->tr_count].iv[i - 1] = ft_atoi(tmp[0]) - 1;
		mesh->tr[mesh->tr_count].it[i - 1] = ft_atoi(tmp[1]) - 1;
		mesh->tr[mesh->tr_count].in[i - 1] = ft_atoi(tmp[2]) - 1;
		ft_delarr(tmp);
		i++;
	}
	mesh->tr_count++;
	ft_delarr(data);
}

void	obj_set_normal(t_mesh *mesh, char *line)
{
	char **data;

	data = ft_strsplit(line, ' ');
	mesh->nr[mesh->nr_count].x = strtof(data[1], NULL);
	mesh->nr[mesh->nr_count].y = strtof(data[2], NULL);
	mesh->nr[mesh->nr_count].z = strtof(data[3], NULL);
	mesh->nr[mesh->nr_count].w = 1.0;
	mesh->nr_count++;
	ft_delarr(data);
}

void	obj_set_texel(t_mesh *mesh, char *line)
{
	char **data;

	data = ft_strsplit(line, ' ');
	mesh->tx[mesh->tx_count].u = strtof(data[1], NULL);
	mesh->tx[mesh->tx_count].v = strtof(data[2], NULL);
	mesh->tx[mesh->tx_count].w = 1.0;
	mesh->tx_count++;
	ft_delarr(data);
}

void	reset_counts(t_mesh *mesh)
{
	mesh->v_count = 0;
	mesh->v_count = 0;
	mesh->tr_count= 0;
	mesh->tx_count = 0;
	mesh->nr_count = 0;
}

void	set_counts(t_mesh *mesh)
{
	mesh->vo = NULL;
	mesh->vb = NULL;
	mesh->tr = NULL;
	mesh->tx = NULL;
	mesh->nr = NULL;
	if (mesh->v_count > 0)
		mesh->vo = (t_v3d *)malloc(sizeof(t_v3d) * mesh->v_count);
	if (mesh->v_count > 0)
		mesh->vb = (t_v3d *)malloc(sizeof(t_v3d) * mesh->v_count);
	if (mesh->tr_count > 0)
		mesh->tr = (t_triangle *)malloc(sizeof(t_triangle) * mesh->tr_count);
	if (mesh->tx_count > 0)
		mesh->tx = (t_v2d *)malloc(sizeof(t_v2d) * mesh->tx_count);
	if (mesh->nr_count > 0)
		mesh->nr = (t_v3d *)malloc(sizeof(t_v3d) * mesh->nr_count);
}

void	obj_reader_count(char *path, t_mesh *mesh)
{
	int		fd;
	char	*line;

	fd = open(path, O_RDONLY);
	if (fd < 3)
	{
		ft_putstr("error: obj file not found\n");
		exit(0);
	}
	reset_counts(mesh);
	while(ft_gnl(fd, &line))
	{
		if (line[0] == 'v' && line[1] == ' ')
			mesh->v_count++;
		if (line[0] == 'f' && line[1] == ' ')
			mesh->tr_count++;
		if (line[0] == 't' && line[1] == ' ')
			mesh->tx_count++;
		if (line[0] == 'n' && line[1] == ' ')
			mesh->nr_count++;
		ft_strdel(&line);
	}
	ft_strdel(&line);
	set_counts(mesh);
	reset_counts(mesh);
}

void	obj_reader_load(char *path, t_mesh *mesh)
{
	int		fd;
	char	*line;

	fd = open(path, O_RDONLY);
	if (fd < 3)
	{
		ft_putstr("error: obj file not found\n");
		exit(0);
	}
	mesh->rot = new_vector(0.0, 0.0, 0.0);
	mesh->pos = new_vector(0.0, 0.0, 0.0);
	while(ft_gnl(fd, &line))
	{
		if (line[0] == 'v' && line[1] == ' ')
			obj_set_coords(mesh, line);
		if (line[0] == 'f' && line[1] == ' ')
			obj_set_triangle(mesh, line);
		if (line[0] == 't' && line[1] == ' ')
			obj_set_texel(mesh, line);
		if (line[0] == 'n' && line[1] == ' ')
			obj_set_normal(mesh, line);
		ft_strdel(&line);
	}
	ft_strdel(&line);
}

void	obj_reader(char *path, t_mesh *mesh)
{
	obj_reader_count(path, mesh);
	obj_reader_load(path, mesh);
}