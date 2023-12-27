#include <libssh/libssh.h>
#include <stdio.h>
#include <stdlib.h>


int verify_knownhost(ssh_session session){
    enum ssh_known_hosts_e state;
    unsigned char *hash  = NULL;
    size_t hlen;
    char *hexa;
    ssh_key srv_pubkey = NULL;
    int rc;

    rc = ssh_get_server_publickey(session, &srv_pubkey);
    if(rc < 0){
        return -1;
    }
    rc = ssh_get_publickey_hash(srv_pubkey , SSH_PUBLICKEY_HASH_SHA1, &hash, &hlen);
    ssh_key_free(srv_pubkey);

    if(rc<0){
        return -1;
    }

    state = ssh_session_is_known_server(session);

    switch (state) {
        case SSH_KNOWN_HOSTS_OK:
            printf("Host is Know");
            break;

        case SSH_KNOWN_HOSTS_CHANGED:
            fprintf( stderr , "Host Key for server changed\n");
            ssh_print_hexa("Public Key hash", hash, hlen);
            fprintf(stderr,"For security reason, your connection will be stopped");
            ssh_clean_pubkey_hash(&hash);
            return -1;
        case SSH_KNOWN_HOSTS_OTHER:
            fprintf(stderr, "Host key for the server was not found but an other type of key exists\n");
            fprintf(stderr, "An attacker might change the default server key to confuse your client into thinking the key does not exist\n");
            ssh_clean_pubkey_hash(&hash);
            return -1;
        case SSH_KNOWN_HOSTS_NOT_FOUND:
            fprintf(stderr, "Could not find known host file.\n");
            fprintf(stderr, "If you accept the host key here, the file will be automatically created\n");
            /* FALLTHROUGHT TO SSH_SERVER_NOT_KNOW behaviour */

        case SSH_KNOWN_HOSTS_UNKNOWN:
            hexa = ssh_get_hexa(hash, hlen);
            fprintf(stderr, "The server is unknown. Do you trust the host key?\n");
            fprintf(stderr, "Public key hash : %s\n",hexa);
            ssh_string_free_char(hexa);
            ssh_clean_pubkey_hash(&hash);
            break;
        case SSH_KNOWN_HOSTS_ERROR:
            fprintf(stderr, "Error : %s",ssh_get_error(session));
            ssh_clean_pubkey_hash(&hash);
            return -1;
        default:
            printf("Unexpected Error in  ssh_session_is_known_server\n");
            ssh_clean_pubkey_hash(&hash);
            return -1;
            break;

    }
    ssh_clean_pubkey_hash(&hash);
    return 0;

}

int main() {
    ssh_session my_ssh_session;
    int verbosity = SSH_LOG_PROTOCOL;
    int port = 22;
    int rc;

    my_ssh_session = ssh_new();
    if (my_ssh_session == NULL) {
        exit(-1);
    }

    ssh_options_set(my_ssh_session,SSH_OPTIONS_HOST,"localhost");
    ssh_options_set(my_ssh_session,SSH_OPTIONS_LOG_VERBOSITY,&verbosity);
    ssh_options_set(my_ssh_session,SSH_OPTIONS_PORT,&port);

    rc  = ssh_connect(my_ssh_session);
    if(rc != SSH_OK){
        fprintf(stderr, "Error: Connecting to Server: %s\n", ssh_get_error(my_ssh_session));
    }


    ssh_disconnect(my_ssh_session);
    ssh_free(my_ssh_session);
}
