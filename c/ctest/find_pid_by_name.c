
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define READ_BUF_SIZE (1024)

/** @brief      通过 /proc/pid/status 中的第一行Name字段, 由process的pid名称获取一个pid序列
                移植于 busybox_0.60_stable/libbb/find_pid_by_name.c 
                (SHA-1: b24d65659f193cd7497dfdae4d8aa1bc91dbf343)
  * @param[in]  pidName: pid名称
  * @param[out] 无
  * @return     a list of all matching PIDs / NULL
  */
long *find_pid_by_name(char *pidName)
{
    DIR *dir = NULL;
    struct dirent *next = NULL;
    long *pidList = NULL;
	FILE *status = NULL;
    char filename[READ_BUF_SIZE] = {0};
    char buffer[READ_BUF_SIZE] = {0};
    char name[READ_BUF_SIZE] = {0};
    int i = 0;

	if (NULL == pidName)
	{
		return NULL;
	}

    dir = opendir("/proc");

    if (!dir)
    {
        //perror_msg_and_die("Cannot open /proc");
        printf("Open /proc failed. errno = %d.\n", errno);
		return NULL;
    }

    while ((next = readdir(dir)) != NULL)
    {
        /* Must skip ".." since that is outside /proc */
        if (strcmp(next->d_name, "..") == 0)
        {
            continue;
        }

        /* If it isn't a number, we don't want it */
        if (!isdigit(*next->d_name))
        {
            continue;
        }

		memset(filename, 0, sizeof(filename));
        snprintf(filename, sizeof(filename), "/proc/%s/status", next->d_name);

        if (!(status = fopen(filename, "r")))
        {
            continue;
        }

		memset(buffer, 0, sizeof(buffer));
        if (fgets(buffer, READ_BUF_SIZE - 1, status) == NULL)
        {
            fclose(status);
            continue;
        }

        fclose(status);

        /* Buffer should contain a string like "Name:   binary_name" */
		memset(name, 0, sizeof(name));
        sscanf(buffer, "%*s %s", name);

        if (strcmp(name, pidName) == 0)
        {
            pidList = realloc( pidList, sizeof(long) * (i + 2));
            pidList[i++] = strtol(next->d_name, NULL, 0);
        }
    }

    if (pidList)
    {
        pidList[i] = 0;
    }
    else
    {
        pidList = realloc( pidList, sizeof(long));
        pidList[0] = -1;
    }

    return pidList;
}

int main(int argc, char *argv[])
{
	long *pid = NULL;

	if (argc != 2)
	{
		printf("Usage %s <filename>\n", argv[0]);
		return -1;
	}
	pid = find_pid_by_name(argv[1]);

	while(pid != 0 && *pid != 0)
	{
		printf("%s is [%d]\n", argv[1], *pid);
		++pid;
	}

	return 0;
}