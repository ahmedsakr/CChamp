/* This file is part of CChamp.
 *
 * CChamp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * CChamp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with CChamp.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <sys/mman.h>
#include <cchamp/cchamp.h>
#include <cchamp_api.h>
#include <cchamp_channel.h>
#include <cchamp_utils.h>

/*
 * API path arguments for different combinations of regions and APIs.
 */
char *regions[] = {
    "na1", "eun1", "euw1", "ru", "tr1",
    "kr", "br1", "oc1", "jp1", "la1", "la2"
};
char *api_path[] = {
    "champion-mastery", "platform", "league", "static-data",
    "status", "match", "spectator", "summoner", "platform"
};


static __CBUFF buffer;
static char url[512];
struct curl_slist *http_headers;

extern void webstr(char *dest, char *str);
int __channel_blocks_allocate();
void __channel_blocks_free();
void * __channel_blocks_claim();
void __channel_blocks_relinquish(Request* request);
void __channel_arguments_free(Argument* request, int *size);


/**
 * Creates a path argument on the heap.
 *
 * @param value The value of the path argument being created.
 * @param next  The next Argument to link up with.
 *
 * @return A pointer to the newly created argument.
 */
Argument* path_arg(Request *request, char* value, Argument* next)
{
    Argument* arg = calloc(1, sizeof(Argument));
    arg->next = next;
    webstr(arg->value, value);

    request->arguments.path.size++;
    return arg;
}


/**
 * Creates a query argument on the heap.
 *
 * @param key   The key of the argument.
 * @param value The value of the argument.
 * @param next  The next Argument to link up with.
 */
Argument* query_arg(Request *request, char* key, char* value, Argument* next)
{
    Argument* arg = calloc(1, sizeof(Argument));
    arg->next = next;
    sprintf(arg->value, "%s=%s", key, value);
    webstr(arg->value, value);

    request->arguments.query.size++;
    return arg;
}


/**
 * Updates the API token into the HTTP header struct.
 * The official API requires this header so that it may authorize the query.
 *
 * @param key The API key used to authenticate the query.
 */
void channel_update_token(char *key)
{
    if (http_headers != NULL) {
        curl_slist_free_all(http_headers);
    }

    /*
     * Token is fixed size at 100 characters as this should always suffice.
     * The API key is always 42 characters long.
     */
    char token[100] = {0};
    sprintf(token, "X-Riot-Token: %s", key);
    http_headers = curl_slist_append(http_headers, token);
}


/**
 * Builds the query url by extracting the necessary information from a request struct.
 *
 * @param request The struct used for information in building the query.
 *
 * @return The fully qualified query url that services the request.
 */
char* channel_url(Request* request)
{
    memset(url, 0x00, 512);
    sprintf(url, "https://%s.api.riotgames.com/lol/%s/v%d", regions[get_bit_index(request->region)],
            api_path[get_bit_index(request->api)], API_VERSION);

    for (Argument* arg = request->arguments.path.head; arg != NULL; arg = arg->next) {
        strcat(url, arg->value);
    }

    if (request->arguments.query.head != NULL) {
        strcat(url, "?");

        for (Argument* arg = request->arguments.query.head; arg != NULL; arg = arg->next) {
            strcat(url, arg->value);

            // If there is another query argumenteter then it must be prefixed with an "&".
            if (arg->next != NULL) {
                strcat(url, "&");
            }
        }
    }

    return url;
}


/**
 * Transfers the response received from the server to the request's response buffer.
 * Curl is instructed to pass the relevant Request struct into this function using curl_easy_setopt() in
 * cchamp_init().
 *
 * @param ptr           A pointer to the beginning of the received response.
 * @param size          The number of blocks of the response.
 * @param nmemb         The number of bytes in this block.
 * @param request       The request struct corresponding to the query.
 *
 * @return the total number of bytes received.
 */
size_t channel_response_received(char *ptr, size_t size, size_t nmemb, void *argument)
{
    // This is guaranteed by the curl_easy_setopt call in cchamp_init().
    Request *request = (Request *)argument;

    /*
     * If the request does not have a block in the buffer reserved to it yet, then
     * an attempt to claim one must be done before writing any data.
     */
    if (request->response.addr == NULL) {
        if (buffer.status != 0xFF) {
            request->response.addr = __channel_blocks_claim();
        } else {

            /*
             * All possible buffers are currently exhausted and this new response cannot be serviced.
             * Set cc_error to indicate this encountered error.
             */
            cc_error = E2MANY;
            return 0;
        }
    }

    // Load the received data into the correct buffer address and refresh the size with the latest one.
    memcpy(request->response.addr + request->response.size, ptr, size * nmemb);
    request->response.size += size * nmemb;
    return size * nmemb;
}


/**
 * Give up channel memory resources held by this request.
 *
 * @param request The request whose argumenteters logic is being freed.
 */
void channel_clean(Request* request)
{
    // Mark the channel block as free.
    __channel_blocks_relinquish(request);

    // Free up the heap arguments.
    __channel_arguments_free(request->arguments.path.head, &request->arguments.path.size);
    __channel_arguments_free(request->arguments.query.head, &request->arguments.query.size);
}


/**
 * Anonymously maps necessary pages for having sufficient memory backing for query responses
 * for up to CHANNEL_BLOCK_NUM (8) concurrent queries.
 *
 * @return      On success, a non-zero, postive number that represents the number of bytes allocated.
 *              0   If the mmap has failed.
 */
int __channel_blocks_allocate()
{
    buffer.status = 0;
    buffer.addr = mmap(NULL, CHANNEL_BLOCK_NUM * CHANNEL_BLOCK_SIZE, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if (buffer.addr == MAP_FAILED) {
        return 0;
    }

    return CHANNEL_BLOCK_NUM * CHANNEL_BLOCK_SIZE;
}


/**
 * Frees all anonymously mapped pages held by the channel.
 */
void __channel_blocks_free()
{
    if (buffer.addr != MAP_FAILED || buffer.addr != NULL) {
        munmap(buffer.addr, CHANNEL_BLOCK_NUM * CHANNEL_BLOCK_SIZE);
    }
}


/**
 * Atmemts to claim a block in the buffer.
 *
 * @return      NULL    If it was unsucessful in acquiring a block (i.e. all blocks are in-use).
 *              Pointer If it was successful. The starting address for the block is returned.
 */
void * __channel_blocks_claim()
{
    uint8_t free_buffer = 0x01;

    // Keep looping until the first cleared bit is detected.
    while ((buffer.status & free_buffer) != 0)
    {
        free_buffer <<= 1;
    }

    // if free_buffer is 0, then it must have overflowed which only happens when no buffers are available.
    if (free_buffer == 0) {
        return NULL;
    }

    // Claim the block in the buffer by setting its corresponding bit in the buffer status.
    buffer.status |= free_buffer;
    return buffer.addr + (CHANNEL_BLOCK_SIZE * get_bit_index(free_buffer));
}


/*
 * Clears the specified bit from the buffer status, effectively setting it to free.
 * Data in the block pages do not have to be flushed, only invalidated.
 *
 * @param block The identifier for the block in the buffer.
 */
void __channel_blocks_relinquish(Request* request)
{
    /*
     * Using the request's response address, the block index in the buffer must be reverse engineered.
     * It is quite simple to do so because all blocks have fixed sizes.
     */
    size_t offset = (uintptr_t)request->response.addr - (uintptr_t)buffer.addr;
    size_t block_index = offset / CHANNEL_BLOCK_SIZE;

    // Flush the response struct in the request in preparation for a relinquish of the block.
    request->response.size = 0;
    request->response.addr = NULL;

    // Clear the corresponding block index.
    buffer.status &= ~(1 << block_index);
}



/**
 * Free up a linked-list structured argument list.
 *
 * @param head The head node of the argument linked list.
 * @param size The tracked size of the arguments.
 */
void __channel_arguments_free(Argument *head, int *size)
{
    Argument* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free((void *)temp);
    }

    // All arguments have been freed; reset the arguments size counter to 0.
    *size = 0;
}
