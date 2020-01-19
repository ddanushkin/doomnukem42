#include "doom_nukem.h"

void	exit_with_status(int status, char *fnf_path)
{
	if (status == STATUS_FILE_NOT_FOUND)
	{
		ft_putstr(MSG_FILE_NOT_FOUND);
		ft_putendl(fnf_path);
	}
	if (status == STATUS_BAD_RESOURCES)
		ft_putendl(MSG_BAD_RESOURCES);
	exit(0);
}

void	quit_properly()
{
	//getchar();
	exit(0);
}