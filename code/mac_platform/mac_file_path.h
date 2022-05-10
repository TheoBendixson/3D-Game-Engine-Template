
#define MAC_MAX_FILENAME_SIZE 4096

struct mac_app_path
{
    char Filename[MAC_MAX_FILENAME_SIZE];
    char *OnePastLastAppFileNameSlash;
};
