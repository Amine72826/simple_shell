#include "shell.h"

/**
 * main - Entry point of the shell program
 * @argc: Number of command-line arguments
 * @argv: Array of command-line arguments
 * @env: Array of environment variables
 * Return: 0 on success
 */
int main(int argc, char *argv[], char *env[])
{
	data_of_program data_struct = {NULL};
	data_of_program *data = &data_struct;
	char *prompt = "";

	inicialize_data(data, argc, argv, env);

	signal(SIGINT, handle_ctrl_c);

	if (is_interactive_mode(argc))
	{
		errno = 2;
		prompt = PROMPT_MSG;
	}

	errno = 0;
	sisifo(prompt, data);

	return (0);
}

/**
 * handle_ctrl_c - Handles the SIGINT signal (Ctrl+C)
 * @sig: Signal number
 */
void handle_ctrl_c(int sig UNUSED)
{
	_print("\n");
	_print(PROMPT_MSG);
}

/**
 * inicialize_data - Initializes the program's data structure
 * @data: Pointer to the program's data structure
 * @argc: Number of command-line arguments
 * @argv: Array of command-line arguments
 * @env: Array of environment variables
 */
void inicialize_data(data_of_program *data, int argc, char *argv[], char **env)
{
	int i = 0;

	data->program_name = argv[0];
	data->input_line = NULL;
	data->command_name = NULL;
	data->exec_counter = 0;

	if (argc == 1)
		data->file_descriptor = STDIN_FILENO;
	else
	{
		data->file_descriptor = open_file(argv[1]);
	}

	data->tokens = NULL;

	copy_env_variables(data, env);
	initialize_alias_list(data);
}

/**
 * sisifo - Main loop of the shell program
 * @prompt: Prompt to be printed
 * @data: Pointer to the program's data structure
 */
void sisifo(char *prompt, data_of_program *data)
{
	int error_code = 0, string_len = 0;

	while (++(data->exec_counter))
	{
		_print(prompt);
		error_code = string_len = _getline(data);

		if (error_code == EOF)
		{
			free_all_data(data);
			exit(errno);
		}

		if (string_len >= 1)
		{
			expand_alias(data);
			expand_variables(data);
			tokenize(data);

			if (data->tokens[0])
			{
				error_code = execute(data);
				if (error_code != 0)
					_print_error(error_code, data);
			}

			free_recurrent_data(data);
		}
	}
}
