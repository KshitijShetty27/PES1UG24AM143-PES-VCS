#include "pes.h"
#include "index.h"
#include "commit.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

void cmd_init(void) {
    mkdir(PES_DIR, 0755);
    mkdir(OBJECTS_DIR, 0755);
    mkdir(".pes/refs", 0755);
    mkdir(REFS_DIR, 0755);
    FILE *f = fopen(HEAD_FILE, "w");
    if (f) { fprintf(f, "ref: refs/heads/main\n"); fclose(f); }
    printf("Initialized empty PES repository in .pes/\n");
}

void cmd_add(int argc, char *argv[]) {
    if (argc < 3) { fprintf(stderr, "Usage: pes add <file>...\n"); return; }
    static Index index;
    index_load(&index);
    for (int i = 2; i < argc; i++) {
        if (index_add(&index, argv[i]) == 0)
            printf("Added: %s\n", argv[i]);
    }
}

void cmd_status(void) {
    static Index index;
    index_load(&index);
    index_status(&index);
}

static void print_commit(const ObjectID *id, const Commit *c, void *ctx) {
    (void)ctx;
    char hex[HASH_HEX_SIZE + 1];
    hash_to_hex(id, hex);
    printf("commit %s\n", hex);
    printf("Author: %s\n", c->author);
    printf("Date:   %llu\n", (unsigned long long)c->timestamp);
    printf("\n    %s\n\n", c->message);
}

void cmd_log(void) {
    if (commit_walk(print_commit, NULL) != 0)
        fprintf(stderr, "No commits yet.\n");
}

void cmd_commit(int argc, char *argv[]) {
    const char *message = NULL;
    for (int i = 2; i < argc - 1; i++) {
        if (strcmp(argv[i], "-m") == 0) { message = argv[i + 1]; break; }
    }
    if (!message) {
        fprintf(stderr, "error: commit requires a message (-m \"message\")\n");
        return;
    }
    ObjectID id;
    if (commit_create(message, &id) == 0) {
        char hex[HASH_HEX_SIZE + 1];
        hash_to_hex(&id, hex);
        printf("Committed: %.12s... %s\n", hex, message);
    } else {
        fprintf(stderr, "error: commit failed\n");
    }
}

void cmd_branch(int argc, char *argv[]) {
    (void)argc; (void)argv;
    printf("branch command not implemented\n");
}

void cmd_checkout(int argc, char *argv[]) {
    (void)argc; (void)argv;
    printf("checkout command not implemented\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: pes <command> [args]\n");
        fprintf(stderr, "\nCommands:\n");
        fprintf(stderr, "  init                Create a new PES repository\n");
        fprintf(stderr, "  add <file>...       Stage files for commit\n");
        fprintf(stderr, "  status              Show working directory status\n");
        fprintf(stderr, "  commit -m <msg>     Create a commit from staged files\n");
        fprintf(stderr, "  log                 Show commit history\n");
        return 1;
    }
    const char *cmd = argv[1];
    if      (strcmp(cmd, "init")     == 0) cmd_init();
    else if (strcmp(cmd, "add")      == 0) cmd_add(argc, argv);
    else if (strcmp(cmd, "status")   == 0) cmd_status();
    else if (strcmp(cmd, "commit")   == 0) cmd_commit(argc, argv);
    else if (strcmp(cmd, "log")      == 0) cmd_log();
    else if (strcmp(cmd, "branch")   == 0) cmd_branch(argc, argv);
    else if (strcmp(cmd, "checkout") == 0) cmd_checkout(argc, argv);
    else {
        fprintf(stderr, "Unknown command: %s\n", cmd);
        return 1;
    }
    return 0;
}
