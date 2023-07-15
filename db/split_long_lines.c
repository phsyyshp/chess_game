#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

// Helper function to split strings longer than len_trsh
char **split_string(char *str, int len_trsh, int *num_substrings) {
    int length = strlen(str);
    *num_substrings = (int)ceil((double)length / len_trsh);
    char **split_str = malloc((*num_substrings) * sizeof(char *));
    for(int i = 0; i < *num_substrings; i++) {
        split_str[i] = malloc((len_trsh + 1) * sizeof(char));
        strncpy(split_str[i], &str[i * len_trsh], len_trsh);
        split_str[i][len_trsh] = '\0';
    }
    return split_str;
}

// Main function to split array of strings based on len_trsh
char **split_array(char **array, int array_size, int len_trsh, int *new_size) {
    char **new_array = NULL;
    *new_size = 0;
    for(int i = 0; i < array_size; i++) {
        int num_substrings;
        char **split_str = split_string(array[i], len_trsh, &num_substrings);
        new_array = realloc(new_array, (*new_size + num_substrings) * sizeof(char *));
        for(int j = 0; j < num_substrings; j++) {
            new_array[*new_size + j] = split_str[j];
        }
        *new_size += num_substrings;
        free(split_str); // We've copied the pointers, so we can free the container
    }
    return new_array;
}

int main() {
    // Test array
    char *array[] = {"abcd", "abcdefghijk"};
    int array_size = sizeof(array) / sizeof(char *);
    int len_trsh = 3;
    
    // Split array
    int new_size;
    char **new_array = split_array(array, array_size, len_trsh, &new_size);
    
    // Print new array
    for(int i = 0; i < new_size; i++) {
        printf("%s\n", new_array[i]);
    }
    
    // Clean up
    for(int i = 0; i < new_size; i++) {
        free(new_array[i]);
    }
    free(new_array);
    
    return 0;
}
