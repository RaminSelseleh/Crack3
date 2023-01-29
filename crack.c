#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "md5.h"

const int PASS_LEN=50;        // Maximum any password will be


// Stucture to hold both a plaintext password and a hash.
struct entry 
{
    char plain[PASS_LEN]; 
    char hash[34];
    
};


int do_ent_cmp(const void *a, const void *b) {
    struct entry *aa = (struct entry *)a;
    struct entry *bb = (struct entry *)b;
    return strcmp(aa->hash, bb->hash);
}

int do_ent_search(const void *a, const void *b) {
    char *aa = (char *)a;
    struct entry *bb = (struct entry *)b;
    return strcmp(aa, bb->hash);
}

// TODO
// Read in the dictionary file and return an array of entry structs.
// Each entry should contain both the hash and the dictionary
// word.
struct entry *read_dictionary(char *filename, int *size)
{
	FILE *in = fopen(filename, "r");
	if (!in)
	{
	    perror("Can't open dictionary");
	    exit(1);
	}

    *size = 0;
    int cap = 10;

    struct entry * arr = malloc(cap * sizeof(struct entry));

	// Read line by line.
    char line[PASS_LEN];
    while (fgets(line, PASS_LEN, in) != NULL) {

        char *nl = strchr(line, '\n');
        if (nl) *nl = '\0';


	// Expand array if necessary (realloc).
        if (*size == cap) {
            cap += 10;
            arr = realloc(arr, cap * sizeof(struct entry));
        }

        // Hash the line 
        char *hash = md5(line, strlen(line) );

        // copy the plaintext
        strcpy(arr[*size].plain, line);

        // copy the hashed
        strcpy(arr[*size].hash, hash);

        // increment 
        (*size)++;

        // Free up space
        free(hash);
    }

    // close in
    fclose(in);

    // return array
    return arr;
}


int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        printf("Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }

    // TODO: Read the dictionary file into an array of entry structures 
    int size;
    struct entry *dict = read_dictionary(argv[2], &size);
    
    // TODO: Sort the hashed dictionary using qsort.
    // You will need to provide a comparison function that
    // sorts the array by hash value.
    qsort(dict, size, sizeof(struct entry), do_ent_cmp);

    // Open the hash file for reading. 
    FILE *ht = fopen(argv[1], "r");
    if (!ht)
    {
        perror("cant open file for reading");
        exit(1);
    }

    char hashLine[34];
    int amountFound = 0;
    while (fgets(hashLine, 34, ht) != NULL) {

        char *nl = strchr(hashLine, '\n');
        if (nl) *nl = '\0';

        struct entry *found = bsearch(hashLine, dict, size, sizeof(struct entry), do_ent_search); 

        if (found) {
            printf("Found: %s", hashLine);
            printf(" -----> %s\n", found->plain);
            amountFound++;
        }

        // else print not found message
        else {
            printf("No match found.\n");
        }

    }

    // how many we found
    printf("Found %d lines to hash\n", amountFound);
    
    // free up space then close
    free(dict);
    fclose(ht);
    
} // end main

