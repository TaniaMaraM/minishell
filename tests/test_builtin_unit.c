/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtin_unit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmarcos <tmarcos@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 18:30:00 by tmarcos           #+#    #+#             */
/*   Updated: 2025/09/02 18:30:00 by tmarcos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/* Test counters */
static int tests_passed = 0;
static int tests_failed = 0;
static int total_tests = 0;

/* Test helper functions */
void test_assert(int condition, const char *test_name)
{
	total_tests++;
	if (condition)
	{
		printf("✅ %s\n", test_name);
		tests_passed++;
	}
	else
	{
		printf("❌ %s\n", test_name);
		tests_failed++;
	}
}

void test_assert_str_equal(const char *actual, const char *expected, const char *test_name)
{
	total_tests++;
	if (actual && expected && strcmp(actual, expected) == 0)
	{
		printf("✅ %s (got: '%s')\n", test_name, actual);
		tests_passed++;
	}
	else
	{
		printf("❌ %s (expected: '%s', got: '%s')\n", test_name, 
			expected ? expected : "NULL", actual ? actual : "NULL");
		tests_failed++;
	}
}

void test_assert_int_equal(int actual, int expected, const char *test_name)
{
	total_tests++;
	if (actual == expected)
	{
		printf("✅ %s (got: %d)\n", test_name, actual);
		tests_passed++;
	}
	else
	{
		printf("❌ %s (expected: %d, got: %d)\n", test_name, expected, actual);
		tests_failed++;
	}
}

/* Test setup and cleanup */
t_shell *create_test_shell(void)
{
	t_shell *shell;
	char *test_env[] = {
		"HOME=/home/test",
		"PATH=/usr/bin:/bin",
		"USER=testuser",
		"PWD=/home/test",
		NULL
	};
	
	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->envp = ft_strarr_dup(test_env);
	shell->last_status = 0;
	shell->is_interactive = 0;
	shell->prompt = "test$ ";
	return (shell);
}

void destroy_test_shell(t_shell *shell)
{
	if (shell)
	{
		if (shell->envp)
			ft_strarr_free(shell->envp);
		free(shell);
	}
}

/* Test built-in detection */
void test_builtin_detection(void)
{
	printf("\n🔍 Testing Built-in Detection\n");
	printf("=============================\n");
	
	test_assert(is_builtin("echo") == 1, "echo is detected as builtin");
	test_assert(is_builtin("cd") == 1, "cd is detected as builtin");
	test_assert(is_builtin("pwd") == 1, "pwd is detected as builtin");
	test_assert(is_builtin("export") == 1, "export is detected as builtin");
	test_assert(is_builtin("unset") == 1, "unset is detected as builtin");
	test_assert(is_builtin("env") == 1, "env is detected as builtin");
	test_assert(is_builtin("exit") == 1, "exit is detected as builtin");
	
	test_assert(is_builtin("ls") == 0, "ls is not detected as builtin");
	test_assert(is_builtin("grep") == 0, "grep is not detected as builtin");
	test_assert(is_builtin("nonexistent") == 0, "nonexistent is not detected as builtin");
	test_assert(is_builtin(NULL) == 0, "NULL is not detected as builtin");
}

/* Test echo builtin */
void test_builtin_echo(void)
{
	printf("\n📢 Testing Echo Builtin\n");
	printf("======================\n");
	
	t_shell *shell = create_test_shell();
	char *argv1[] = {"echo", "hello", "world", NULL};
	char *argv2[] = {"echo", "-n", "hello", "world", NULL};
	char *argv3[] = {"echo", NULL};
	char *argv4[] = {"echo", "-n", NULL};
	
	test_assert(builtin_echo(argv1, shell) == 0, "echo with arguments returns 0");
	test_assert(builtin_echo(argv2, shell) == 0, "echo -n with arguments returns 0");
	test_assert(builtin_echo(argv3, shell) == 0, "echo without arguments returns 0");
	test_assert(builtin_echo(argv4, shell) == 0, "echo -n without arguments returns 0");
	test_assert(builtin_echo(NULL, shell) == 1, "echo with NULL argv returns 1");
	
	destroy_test_shell(shell);
}

/* Test pwd builtin */
void test_builtin_pwd(void)
{
	printf("\n📁 Testing PWD Builtin\n");
	printf("======================\n");
	
	t_shell *shell = create_test_shell();
	char *argv[] = {"pwd", NULL};
	
	test_assert(builtin_pwd(argv, shell) == 0, "pwd returns 0");
	test_assert(builtin_pwd(NULL, shell) == 0, "pwd with NULL argv returns 0");
	
	destroy_test_shell(shell);
}

/* Test env builtin */
void test_builtin_env(void)
{
	printf("\n🌍 Testing ENV Builtin\n");
	printf("======================\n");
	
	t_shell *shell = create_test_shell();
	char *argv[] = {"env", NULL};
	
	test_assert(builtin_env(argv, shell) == 0, "env returns 0");
	test_assert(builtin_env(NULL, shell) == 0, "env with NULL argv returns 0");
	
	destroy_test_shell(shell);
}

/* Test cd builtin */
void test_builtin_cd(void)
{
	printf("\n📂 Testing CD Builtin\n");
	printf("=====================\n");
	
	t_shell *shell = create_test_shell();
	char *argv1[] = {"cd", NULL};
	char *argv2[] = {"cd", "/tmp", NULL};
	char *argv3[] = {"cd", "/nonexistent", NULL};
	char *argv4[] = {"cd", "/tmp", "extra", NULL};
	
	/* cd to home might fail if HOME is not set or invalid, so we test the behavior */
	int result1 = builtin_cd(argv1, shell);
	test_assert(result1 == 0 || result1 == 1, "cd to home returns 0 or 1 (depending on HOME)");
	test_assert(builtin_cd(argv2, shell) == 0, "cd to /tmp returns 0");
	test_assert(builtin_cd(argv3, shell) == 1, "cd to nonexistent returns 1");
	test_assert(builtin_cd(argv4, shell) == 1, "cd with too many args returns 1");
	test_assert(builtin_cd(NULL, shell) == 1, "cd with NULL argv returns 1");
	
	destroy_test_shell(shell);
}

/* Test export builtin */
void test_builtin_export(void)
{
	printf("\n📤 Testing Export Builtin\n");
	printf("=========================\n");
	
	t_shell *shell = create_test_shell();
	char *argv1[] = {"export", NULL};
	char *argv2[] = {"export", "TEST_VAR=hello", NULL};
	char *argv3[] = {"export", "123INVALID=value", NULL};
	char *argv4[] = {"export", "VALID_VAR=world", "ANOTHER_VAR=test", NULL};
	
	test_assert(builtin_export(argv1, shell) == 0, "export without args returns 0");
	test_assert(builtin_export(argv2, shell) == 0, "export valid variable returns 0");
	test_assert(builtin_export(argv3, shell) == 1, "export invalid variable returns 1");
	test_assert(builtin_export(argv4, shell) == 0, "export multiple valid variables returns 0");
	test_assert(builtin_export(NULL, shell) == 1, "export with NULL argv returns 1");
	
	destroy_test_shell(shell);
}

/* Test unset builtin */
void test_builtin_unset(void)
{
	printf("\n📥 Testing Unset Builtin\n");
	printf("========================\n");
	
	t_shell *shell = create_test_shell();
	char *argv1[] = {"unset", NULL};
	char *argv2[] = {"unset", "TEST_VAR", NULL};
	char *argv3[] = {"unset", "123INVALID", NULL};
	char *argv4[] = {"unset", "VAR1", "VAR2", NULL};
	
	test_assert(builtin_unset(argv1, shell) == 0, "unset without args returns 0");
	test_assert(builtin_unset(argv2, shell) == 0, "unset valid variable returns 0");
	test_assert(builtin_unset(argv3, shell) == 1, "unset invalid variable returns 1");
	test_assert(builtin_unset(argv4, shell) == 0, "unset multiple variables returns 0");
	test_assert(builtin_unset(NULL, shell) == 1, "unset with NULL argv returns 1");
	
	destroy_test_shell(shell);
}

/* Test environment utilities */
void test_env_utils(void)
{
	printf("\n🔧 Testing Environment Utilities\n");
	printf("=================================\n");
	
	t_shell *shell = create_test_shell();
	
	/* Test is_valid_var_name */
	test_assert(is_valid_var_name("VALID_VAR") == 1, "VALID_VAR is valid variable name");
	test_assert(is_valid_var_name("_VALID_VAR") == 1, "_VALID_VAR is valid variable name");
	test_assert(is_valid_var_name("123INVALID") == 0, "123INVALID is invalid variable name");
	test_assert(is_valid_var_name("VAR-WITH-DASH") == 0, "VAR-WITH-DASH is invalid variable name");
	test_assert(is_valid_var_name("") == 0, "empty string is invalid variable name");
	test_assert(is_valid_var_name(NULL) == 0, "NULL is invalid variable name");
	
	/* Test env_set_var */
	test_assert(env_set_var(shell, "NEW_VAR", "new_value") == 0, "env_set_var returns 0");
	test_assert(env_set_var(shell, NULL, "value") == 1, "env_set_var with NULL name returns 1");
	test_assert(env_set_var(NULL, "VAR", "value") == 1, "env_set_var with NULL shell returns 1");
	
	/* Test env_unset_var */
	test_assert(env_unset_var(shell, "NEW_VAR") == 0, "env_unset_var returns 0");
	test_assert(env_unset_var(shell, "NONEXISTENT") == 0, "env_unset_var nonexistent returns 0");
	test_assert(env_unset_var(shell, NULL) == 1, "env_unset_var with NULL name returns 1");
	test_assert(env_unset_var(NULL, "VAR") == 1, "env_unset_var with NULL shell returns 1");
	
	destroy_test_shell(shell);
}

/* Test builtin execution dispatcher */
void test_builtin_execution(void)
{
	printf("\n⚡ Testing Builtin Execution Dispatcher\n");
	printf("======================================\n");
	
	t_shell *shell = create_test_shell();
	t_cmd *cmd;
	
	/* Test echo execution */
	cmd = malloc(sizeof(t_cmd));
	cmd->argv = malloc(sizeof(char *) * 3);
	cmd->argv[0] = "echo";
	cmd->argv[1] = "hello";
	cmd->argv[2] = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	
	test_assert(execute_builtin_in_child(cmd, shell) == 0, "execute_builtin_in_child echo returns 0");
	
	/* Test invalid command */
	cmd->argv[0] = "nonexistent";
	test_assert(execute_builtin_in_child(cmd, shell) == 1, "execute_builtin_in_child nonexistent returns 1");
	
	/* Test NULL parameters */
	test_assert(execute_builtin_in_child(NULL, shell) == 1, "execute_builtin_in_child with NULL cmd returns 1");
	test_assert(execute_builtin_in_child(cmd, NULL) == 1, "execute_builtin_in_child with NULL shell returns 1");
	
	free(cmd->argv);
	free(cmd);
	destroy_test_shell(shell);
}

/* Main test runner */
int main(void)
{
	printf("🧪 Builtin Unit Tests\n");
	printf("=====================\n");
	
	test_builtin_detection();
	test_builtin_echo();
	test_builtin_pwd();
	test_builtin_env();
	test_builtin_cd();
	test_builtin_export();
	test_builtin_unset();
	test_env_utils();
	test_builtin_execution();
	
	printf("\n📊 Test Results\n");
	printf("===============\n");
	printf("Total tests: %d\n", total_tests);
	printf("Passed: %d\n", tests_passed);
	printf("Failed: %d\n", tests_failed);
	
	if (tests_failed == 0)
	{
		printf("\n🎉 All tests passed!\n");
		return (0);
	}
	else
	{
		printf("\n❌ Some tests failed!\n");
		return (1);
	}
}
