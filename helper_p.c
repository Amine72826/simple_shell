#include "shell.h"

/**
 * _print - writes an array of chars to standard output
 * @string: pointer to the array of chars
 * Return: the number of bytes written
 *         On error, -1 is returned, and errno is set appropriately.
 */
int _print(char *string)
{
	return (write(STDOUT_FILENO, string, str_length(string)));
}

/**
 * _printe - writes an array of chars to standard error
 * @string: pointer to the array of chars
 * Return: the number of bytes written
 *         On error, -1 is returned, and errno is set appropriately.
 */
int _printe(char *string)
{
	return (write(STDERR_FILENO, string, str_length(string)));
}

/**
 * _print_error - writes an array of chars to standard error
 * @errorcode: error code to print
 * @data: a pointer to the program's data
 * Return: the number of bytes written
 *         On error, -1 is returned, and errno is set appropriately.
 */
int _print_error(int errorcode, data_of_program *data)
{
	char n_as_string[10] = {'\0'};

	long_to_string((long)data->exec_counter, n_as_string, 10);

	switch (errorcode)
	{
		case 2:
		case 3:
			_printe(data->program_name);
			_printe(": ");
			_printe(n_as_string);
			_printe(": ");
			_printe(data->tokens[0]);
			_printe((errorcode == 2) ? ": Illegal number: " : ": can't cd to ");
			_printe(data->tokens[1]);
			_printe("\n");
			break;

		case 127:
			_printe(data->program_name);
			_printe(": ");
			_printe(n_as_string);
			_printe(": ");
			_printe(data->command_name);
			_printe(": not found\n");
			break;

		case 126:
			_printe(data->program_name);
			_printe(": ");
			_printe(n_as_string);
			_printe(": ");
			_printe(data->command_name);
			_printe(": Permission denied\n");
			break;
	}

	return (0);
}
