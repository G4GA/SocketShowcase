#include <socket_handler.h>

int main (int argc, char *argv[])
{
    char *example_url = "https://www.google.com:443/search?q=c"; //https:// 
    url_info test_info;
    int rc = 0;

    rc = url_parser(example_url, &test_info);

    return rc;
}
