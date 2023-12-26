#include <libssh/libssh.h>
#include <stdio.h>
#include <stdlib.h>

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
