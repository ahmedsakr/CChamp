struct api_node {
    char *api_key;
    int max_requests;
};

struct api_request {
    struct api_node api;
    char *region;
    char *qualified_request;
}

typedef struct api_node RiotAPI;
typedef struct api_request Request;
