#include "shell.h"

/**
 * builtin_exit - Exit the program with the specified status
 * @data: Struct for the program's data
 *
 * Return: Always returns 0
 */
int builtin_exit(data_of_program *data)
{
	int status = 0;

	if (data->tokens[1] != NULL)
	{
		if (!is_numeric(data->tokens[1]))
		{
			errno = 2;
			return (2);
		}
		status = _atoi(data->tokens[1]);
	}

	free_all_data(data);
	exit(status);
}

/**
 * builtin_cd - Change the current directory
 * @data: Struct for the program's data
 *
 * Return: Returns 0 on success, or an error code on failure
 */
int builtin_cd(data_of_program *data)
{
	char *dir_home = env_get_key("HOME", data);
	char *dir_old = NULL;
	char old_dir[128] = {0};
	int error_code = 0;

	if (data->tokens[1])
	{
		if (str_compare(data->tokens[1], "-", 0))
		{
			dir_old = env_get_key("OLDPWD", data);
			if (dir_old)
				error_code = set_work_directory(data, dir_old);
			_print(env_get_key("PWD", data));
			_print("\n");
			return (error_code);
		}
		else
		{
			return (set_work_directory(data, data->tokens[1]));
		}
	}
	else
	{
		if (!dir_home)
			dir_home = getcwd(old_dir, 128);
		return (set_work_directory(data, dir_home));
	}

	return (0);
}

/**
 * set_work_directory - Set the working directory
 * @data: Struct for the program's data
 * @new_dir: Path to be set as the working directory
 *
 * Return: Returns 0 on success, or an error code on failure
 */
int set_work_directory(data_of_program *data, char *new_dir)
{
	char old_dir[128] = {0};
	int err_code = 0;

	getcwd(old_dir, 128);

	if (!str_compare(old_dir, new_dir, 0))
	{
		err_code = chdir(new_dir);
		if (err_code == -1)
		{
			errno = 2;
			return (3);
		}
		env_set_key("PWD", new_dir, data);
	}

	env_set_key("OLDPWD", old_dir, data);
	return (0);
}

/**
 * builtin_help - Display help information for built-in commands
 * @data: Struct for the program's data
 *
 * Return: Always returns 0
 */
int builtin_help(data_of_program *data)
{
	char *help_messages[] = {
		HELP_MSG,
		HELP_EXIT_MSG,
		HELP_ENV_MSG,
		HELP_SETENV_MSG,
		HELP_UNSETENV_MSG,
		HELP_CD_MSG,
		NULL
	};
	int i;

	if (data->tokens[1] == NULL)
	{
		_print(help_messages[0] + 6);
		return (1);
	}

	if (data->tokens[2] != NULL)
	{
		errno = E2BIG;
		perror(data->command_name);
		return (5);
	}

	for (i = 1; help_messages[i]; i++)
	{
		if (str_compare(data->tokens[1], help_messages[i], str_length(data->tokens[1]
		)))
		{
		_print(help_messages[i] + str_length(data->tokens[1]) + 1);
		return (1);
		}
	}

	errno = EINVAL;
	perror(data->command_name);
	return (0);
}

/**
 * builtin_alias - Add, remove, or show aliases
 * @data: Struct for the program's data
 *
 * Return: Always returns 0
 */
int builtin_alias(data_of_program *data)
{
	int i = 0;

	if (data->tokens[1] == NULL)
		return (print_alias(data, NULL));

	while (data->tokens[++i])
	{
		if (count_characters(data->tokens[i], "="))
			set_alias(data->tokens[i], data);
		else
			print_alias(data, data->tokens[i]);
	}

	return (0);
}

