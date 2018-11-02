#include <stdio.h>
#include <stdlib.h>

#include <json-c/json.h>


static int run(const char* path);
static int load_file(const char *path, char **body);
static int analysze_json(const char *data);


int main(void)
{
    if (run("sample.json") == -1) {
        return 1;
    }

    return 0;
}

int run(const char *path)
{
    char *data = NULL;
    int rc = -1;

    if (load_file(path, &data) == -1) {
        goto clean;
    }

    if (analysze_json(data) == -1) {
        goto clean;
    }

    rc = 0;

clean:
    free(data);
    return rc;
}

// load_file loads the entire contents of a file into a newly allocated buffer.
// Returns 0 on success and -1 on failure. The buffer is assigned to *data on
// success.
int load_file(const char *path, char **data)
{
    FILE *file = NULL;
    char *buf = NULL;
    int rc = -1;

    file = fopen(path, "r");
    if (file == NULL) {
        goto clean;
    }

    // Get file size
    if (fseek(file, 0, SEEK_END) == -1) {
        goto clean;
    }

    long endpos = ftell(file);
    if (endpos < 0 || (size_t) endpos > SIZE_MAX) {
        goto clean;
    }

    size_t size = (size_t) endpos;
    buf = malloc(size);
    if (buf == NULL) {
        goto clean;
    }

    if (fseek(file, 0, SEEK_SET) == -1) {
        goto clean;
    }

    if (fread(buf, 1, size, file) < size) {
        goto clean;
    }

    *data = buf;
    buf = NULL;
    rc = 0;

clean:
    free(buf);
    fclose(file);
    return rc;
}

// analyze_json parses data as json and prints some fields. Returns 0 on success
// or -1 on failure.
int analysze_json(const char *data)
{
    struct json_object *json = NULL;
    int rc = -1;

    json = json_tokener_parse(data);
    if (json == NULL) {
        goto clean;
    }

    struct json_object *token = json_object_object_get(json, "token");
    if (token == NULL) {
        goto clean;
    }
    printf("token = %s\n", json_object_get_string(token));

    struct json_object *id = json_object_object_get(json, "id");
    if (id == NULL) {
        goto clean;
    }
    printf("id = %d\n", json_object_get_int(id));

    rc = 0;

clean:
    json_object_put(json);
    return rc;
}
