#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>




static void
click_cb (GtkButton *btn, GtkWindow *win){
    g_print("Clicked.\n");
    gtk_window_destroy(win);

}

static int fill_widgets(GtkWidget **widget_list , int *list_cnt){

    char *line = NULL;
    size_t len = 0;
    int line_cnt = 0;
    GtkWidget *lab = NULL;

    while (getline(&line, &len, stdin) != -1) {
        printf("%s", line);
        g_strstrip(line);
        lab = gtk_label_new(line);
        gtk_label_set_justify(GTK_LABEL(lab), GTK_JUSTIFY_LEFT);
        gtk_label_set_xalign(GTK_LABEL(lab), 0.1);
        widget_list[line_cnt] = lab;
        line_cnt++;
    }

    *list_cnt = line_cnt;

    free(line);

    return 0;
}

static void app_activate(GApplication *app, gpointer *user_data) {
    g_print("GtkApplication is activated.\n");

    GtkWidget *win;
    GtkWidget *btn;
    GtkWidget *v_box;
    GtkWidget *scr;
    GtkWidget *widget_list[100];
    GtkWidget *search_bar;
    int list_cnt = 0;
    int i ;

    win = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_window_set_title(GTK_WINDOW(win), "Application Window Title");
    gtk_window_set_default_size(GTK_WINDOW(win), 800, 500);
    

    scr = gtk_scrolled_window_new();
    v_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 2);
    gtk_box_set_homogeneous(GTK_BOX(v_box), FALSE);

    search_bar = gtk_search_bar_new();
    gtk_box_append(GTK_BOX(v_box), search_bar);

    fill_widgets(widget_list,&list_cnt);

    for(i=0;i<list_cnt;i++){
        gtk_box_append(GTK_BOX(v_box),widget_list[i]);
    }
    btn = gtk_button_new_with_label("Close.");
    gtk_box_append(GTK_BOX(v_box), btn);

    gtk_scrolled_window_set_child(GTK_SCROLLED_WINDOW(scr),v_box);
    gtk_window_set_child(GTK_WINDOW(win), scr);
    // passing win to the callback as last argument
    g_signal_connect(btn, "clicked", G_CALLBACK(click_cb), win);
    gtk_window_present(GTK_WINDOW(win));
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;



    app = gtk_application_new("com.github.To", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}

