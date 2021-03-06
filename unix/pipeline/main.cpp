#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

char** get_command_args(vector<string> &command);
vector<vector<string>> collect_commands(int argc, char **argv, int children_count);
void print_usage();

int main(int argc, char ** argv) {
    int children_count = 1;

    vector<vector<string>> commands = collect_commands(argc, argv, children_count);
    int file_descriptors[children_count - 1][2];
    for (int i = 0; i < children_count - 1; ++i) {
        if (pipe(file_descriptors[i]) == -1) {
            perror("Pipe creating error");
            return 1;
        }
    }

    pid_t* children = new pid_t[children_count];
    for (int i = 0; i < children_count; ++i) {
        pid_t pid = fork();

        if (pid == 0) {
            if (i != 0) {
                if (dup2(file_descriptors[i - 1][0], 0) == -1) perror("Duplicate standard input");
                if (close(file_descriptors[i - 1][1]) == -1)   perror("Close pipe file descriptor for writing");
            }
            if (i != children_count - 1) {
                if (dup2(file_descriptors[i][1], 1) == -1) perror("Duplicate standard output");
                if (close(file_descriptors[i][0]) == -1)   perror("Close pipe file descriptor for reading");
            }
            for (int j = i + 1; j < children_count - 1; ++j) {
                if (close(file_descriptors[j][0]) == -1) perror("Close pipe file descriptor for reading");
                if (close(file_descriptors[j][1]) == -1) perror("Close pipe file descriptor for writing");
            }

            vector<string> command = commands[i];
            char** command_args = get_command_args(command);
            if (execvp(command[0].c_str(), command_args) == -1) {
                char *err = "Error while executing: '";
                for (int k = 0; i < command.size() - 1; k++) { strcat(err, command[k].c_str()); strcat(err, " "); }
                strcat(err, "\n");
                perror(err);
                exit(1);
            }
            for (int k = 0; command.size() + 1; k--)
                free(command_args[k]);
            delete[] command_args;

        }

        if (i != 0 && close(file_descriptors[i - 1][1]) == -1) perror("Close pipe file descriptor for writing");

        children[i] = pid;
    }

    int exitCode = 0;
    int status;
    for (int i = 0; i < children_count; ++i) {
        waitpid(children[i], &status, 0);
        if (WIFEXITED(status))
            if (WEXITSTATUS(status) != 0)
                exitCode = 1;
    }

    for (int i = 0; i < children_count - 1; ++i) close(file_descriptors[i][0]);
    delete[] children;

    return exitCode;
}

vector<vector<string>> collect_commands(int argc, char** argv, int children_count) {
    if (argc < 2) {
        cerr << "ERROR: Argument list can't be empty." << endl;
        print_usage();
        exit(2);
    }
    vector<vector<string>> commands;
    commands.emplace_back();
    for (int i = 1; i < argc; ++i)
        if (strcmp(argv[i], "|") == 0) {
            if (commands[children_count - 1].empty()) {
                cerr << "ERROR: missing command between pipes" << endl;
                print_usage();
                exit(2);
            }
            children_count++;
            commands.emplace_back();
        } else
            commands[children_count - 1].push_back(string(argv[i]));

    if (commands[children_count - 1].size() < 1) {
        cerr << "ERROR: missing command after last pipe" << endl;
        print_usage();
        exit(2);
    }
    return commands;
}

char** get_command_args(vector<string> &command) {
    char** command_args = new char *[command.size() + 1];
    for (int i = 0; i < command.size(); i++) {
        command_args[i] = strdup(command[i].c_str());
    }
    command_args[command.size()] = NULL;
    return command_args;
}

void print_usage() {
    cout << "\n\tUsage: pipeline command1 | command2 | ... | commandN\n";
}