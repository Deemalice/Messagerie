#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <sys/socket.h>

#include <sys/types.h>

#include <stdio.h>

#include <arpa/inet.h>

#include <netinet/in.h>

#include <unistd.h>

#include <signal.h>

#define SER_PORT 1201



#include <gtk/gtk.h>

#include <glib.h>





typedef struct {
    
    GtkWidget * entry, * textview;
    
} InputOutput;



typedef struct {
    
    GtkTextMark   * mark;
    
    GtkTextIter    start, end;
    
    GtkTextBuffer * buffer;
    
    GtkWidget     * chat,
    
    * button,
    
    * button2,
    
    * button3,
    
    * chat_space,
    
    * chat_read_space,
    
    * scrolled_window,
    
    * chat_write_space,
    
    * chat_right_column,
    
    * socket,
    
    * container_option;
    
    gchar   * nom_server,
    
    * nom_client,
    
    * ip;
    
    char string_read[512],
    
    string_send[512],
    
    nom_utilisateur[50],
   
    message[562],
    
    ipC[16];
    
    const gchar * test42;
    
    gboolean * connected,* is_server;
    
    gint note;
    
} ChatUI;



void open_file ();

void enter_text();

void save_file();

void start_session();

void client();

void server();

void enter_socket_text(char * data);

void timeout_read_2();

void save_username();

void enter_username();

void send_entered_text();

void end_session();

void save_mark();


ChatUI app;

InputOutput * windowIO;

int connection_handle;



int main(int argc,char *argv[])

{
    
    app.connected = FALSE;
    
    app.is_server = FALSE;
    
    windowIO = g_slice_new (InputOutput);
    
    
    
    gtk_init(&argc, &argv);
    
    
    
    
    
    //  Main window
    
    app.chat = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    
    gtk_container_set_border_width(GTK_CONTAINER(app.chat), 10);
    
    g_signal_connect(G_OBJECT(app.chat), "delete-event", G_CALLBACK(gtk_main_quit), NULL);
    
    gtk_window_set_default_size(GTK_WINDOW(app.chat), 600, 300);
    
    gtk_window_set_title(GTK_WINDOW(app.chat), "Conversation électronique");
    
    
    
    // GUI
    
    
    
    app.button  = gtk_button_new_with_label ("Terminer Session");
    
    app.button2 = gtk_button_new_with_label ("Demarrer Session");
    
    app.button3 = gtk_button_new_with_label ("Sauvegarde Fichier");
    
    
    
    app.scrolled_window   = gtk_scrolled_window_new(NULL, NULL);
    
    
    
    windowIO->textview    = gtk_text_view_new();
    
    windowIO->entry       = gtk_entry_new();
    
    app.chat_right_column = gtk_vbox_new(FALSE, 5);
    
    app.chat_space        = gtk_hpaned_new ();
    
    app.container_option  = gtk_vpaned_new ();
    
    
    
    gtk_text_view_set_editable(GTK_TEXT_VIEW(windowIO->textview), FALSE);
    
    
    
    gtk_paned_set_position (GTK_PANED(app.chat_space), 450);
    
    gtk_paned_set_position (GTK_PANED(app.container_option), 250);
    
    
    
    gtk_paned_add1 (GTK_PANED(app.chat_space), app.container_option);
    
    gtk_paned_add2 (GTK_PANED(app.chat_space), app.chat_right_column);
    
    gtk_paned_add1 (GTK_PANED(app.container_option), app.scrolled_window);
    
    gtk_paned_add2 (GTK_PANED(app.container_option), windowIO->entry);
    
    
    
    gtk_box_pack_start(GTK_BOX(app.chat_right_column), app.button2, FALSE, FALSE,  0);
    
    gtk_box_pack_start(GTK_BOX(app.chat_right_column), app.button3, FALSE, FALSE, 10);
    
    gtk_box_pack_end  (GTK_BOX(app.chat_right_column), app.button,  FALSE, FALSE, 25);
    
    
    
    
    
    gtk_container_add (GTK_CONTAINER(app.scrolled_window), windowIO->textview);
    
    gtk_container_add (GTK_CONTAINER(app.chat), app.chat_space);
    
    
    
    g_signal_connect (G_OBJECT (app.button3), "clicked",
                      
                      G_CALLBACK (save_file),
                      
                      NULL);
    
    
    
    g_signal_connect (G_OBJECT (app.button2), "clicked",
                      
                      G_CALLBACK (start_session),
                      
                      NULL);
    
    g_signal_connect (G_OBJECT (app.button), "clicked",
                      
                      G_CALLBACK (end_session),
                      
                      NULL);
    
    
    
    g_signal_connect (windowIO->entry, "activate",
                      
                      G_CALLBACK (enter_text),
                      
                      NULL);
    
    
    
    app.buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (windowIO->textview));
    
    
    
    gtk_widget_show_all(app.chat);
    
    
    
    gtk_main();
    
    return 0;
    
    
    
}



void enter_text(){
    
    app.mark = gtk_text_buffer_get_insert (app.buffer);
    
    
    
    //enter_username();
    
    
    
    
    
    /* Insert newline (only if there's already text in the buffer). */
    
    //    if (gtk_text_buffer_get_char_count(app.buffer)){
    
    //        gtk_text_buffer_insert (app.buffer, &app.end, "\n", 1);
    
    //    }
    
    
    
    if (app.connected) {
    
    
    
    app.test42 = gtk_entry_get_text (GTK_ENTRY (windowIO->entry));
    
    
    
    // enter_socket_text("\n\0");
    
    enter_socket_text(app.nom_utilisateur);
    
    enter_socket_text("\n\0");
    
    memset(app.string_send,'\0',512);
    
    strcpy(app.string_send, app.test42);
    
    //        gtk_text_buffer_get_end_iter (app.buffer,&app.end);
    
    //gtk_text_buffer_create_mark(app.buffer, app.mark, &app.end, FALSE);
    
    //        gtk_text_buffer_get_iter_at_offset(app.buffer,&app.end,-1);
    
    gtk_text_buffer_get_iter_at_mark (app.buffer, &app.end, app.mark); // Initialise l'itérateur 'end' a 'mark' ( Position du curseur -> Fin du buffer )
    
    //        gtk_text_buffer_get_iter_at_line(app.buffer,&app.end,0);
    
    //        gtk_text_buffer_get_end_iter (app.buffer,&app.end);
    
    //        gtk_text_buffer_get_bounds (app.buffer, &app.start, &app.end);
    
    gtk_text_buffer_insert (app.buffer, &app.end, app.test42, -1); // Ajoute le 'test42' dans le 'buffer' a l'emplacement de 'end' ( Fin du buffer ) 
    
    //gtk_text_buffer_get_end_iter (app.buffer,app.end);
    
    gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(windowIO->textview), app.mark, 0.0, FALSE, 1.0,1.0);
    
    enter_socket_text("\n\0");
    
    enter_socket_text("\n\0");
    
    //        gtk_text_buffer_insert (app.buffer, &app.end, app.test42, sizeof(app.buffer));
    
    //enter_username();
    
    send_entered_text();
    
    
    
    //gtk_text_view_scroll_to_iter(GTK_TEXT_VIEW(windowIO->textview), &app.end, 0.0, FALSE, 1.0,1.0);
    
    //                gtk_text_view_scroll_mark_onscreen (GTK_TEXT_VIEW(windowIO->textview), app.mark);
    
    }
    
    
    
    gtk_entry_set_text(GTK_ENTRY(windowIO->entry), ""); // Vide le champs
    
    
    
}



void enter_socket_text(char * data){
    
    
    
    app.mark = gtk_text_buffer_get_insert (app.buffer);
    
    
    
    const gchar   * text = (gchar * )data;
    
    
    
    //app.buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (windowIO->textview));
    
    //    text = (gchar * )data;
    
    //    app.mark = gtk_text_buffer_get_insert (app.buffer);
    
    //    gtk_text_buffer_create_mark(app.buffer, app.mark, &app.end, TRUE);
    
    gtk_text_buffer_get_iter_at_mark (app.buffer, &app.end, app.mark);
    
    
    
    //    gtk_text_buffer_get_iter_at_offset(app.buffer,&app.end,-1);
    
    //
    
    //    if (gtk_text_buffer_get_char_count(app.buffer)){
    
    //        gtk_text_buffer_insert (app.buffer, &app.end, "\n \n", 1);
    
    //    }
    
    //    qgtk_text_buffer_get_end_iter (app.buffer,&app.end);
    
    //    gtk_text_buffer_get_iter_at_line(app.buffer,&app.end,0);
    
    //    gtk_text_buffer_get_end_iter (app.buffer,&app.end);
    
    //    gtk_text_buffer_get_bounds (app.buffer, &app.start, &app.end);
    
    //    gtk_text_buffer_get_end_iter (app.buffer,app.end);
    
    gtk_text_buffer_insert (app.buffer, &app.end, text, -1);
    
    gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(windowIO->textview), app.mark, 0.0, FALSE, 1.0,1.0);
    
    //    gtk_text_buffer_get_end_iter (app.buffer,app.end);
    
    
    
    //    gtk_text_view_scroll_to_mark(GTK_TEXT_VIEW(windowIO->textview), app.mark, 0.0, FALSE, 1.0,1.0);
    
    //        gtk_text_view_scroll_mark_onscreen (GTK_TEXT_VIEW(windowIO->textview), app.mark);
    
}





void open_file(){
    
    GtkTextBuffer * buffer = 0;
    
    GtkWidget * dialog;
    
    int result;
    
    gchar * filename ;
    
    gchar * contents;
    
    
    
    dialog = gtk_file_chooser_dialog_new("Open A File", NULL,
                                         
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         
                                         GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                         
                                         GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                         
                                         NULL);
    
    
    
    result = gtk_dialog_run (GTK_DIALOG (dialog));
    
    
    
    switch ( result ) {
            
        case GTK_RESPONSE_ACCEPT:
            
            filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER ( dialog )); // Sélectionne le dossier
            
            g_file_get_contents(filename, &contents, NULL , NULL); // Alloue l'espace mémoire nécessaire a 'contents' pour contenir 'filename'
            
            buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (windowIO->textview)); // Nom du fichier initialisé par l'utilisateur
            
            gtk_text_buffer_set_text(buffer, contents, -1);
            
            g_free(filename);
            
            g_free(contents);
            
            gtk_widget_destroy ( dialog );
            
        case GTK_RESPONSE_CANCEL:
            
            gtk_widget_destroy ( dialog );
            
            return ;
            
    }
    
}





void save_file(){
    
    if (app.is_server) {
        save_mark(); // 
    }
        
    GtkTextBuffer * buffer = 0;
    
    GtkWidget *dialog;
    
    int res;
    
    gchar * filename ;
    
    gchar * contents;
    
    GtkTextIter start, end;
    
    
    
    dialog = gtk_file_chooser_dialog_new("Save File", NULL,
                                         
                                         GTK_FILE_CHOOSER_ACTION_SAVE,
                                         
                                         GTK_STOCK_SAVE, GTK_RESPONSE_APPLY,
                                         
                                         GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                         
                                         NULL);
    
    
    
    res = gtk_dialog_run (GTK_DIALOG (dialog));
    
    
    
    
    
    if (res == GTK_RESPONSE_APPLY && app.is_server)
        
    {
        
        
        
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        
        buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(windowIO->textview));
        
        gtk_text_buffer_get_bounds(buffer, &start, &end);
        
        contents = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
        
        g_file_set_contents(filename, contents, -1, NULL);
        
        g_free(filename);
        
        g_free(contents);
        
        
        
    }
    
    app.connected = FALSE;
    
    gtk_widget_destroy (dialog);
    
    
    
}



void start_session(){
    
    /* Secure glib */
    
    if( ! g_thread_supported() )
        
        g_thread_init( NULL );
    
    
    
    
    
    GError    *error = NULL;
    
    GThread * thread;
    
    GtkWidget *dialog;
    
    GtkWidget *entry, * entry1, * entry2;
    
    GtkWidget *content_area;
    
    GtkWidget *label,*label1,*label2;
    
    gint result;
    
    
    
    memset(app.nom_utilisateur,'\0',50);
    
    memset(app.string_read,'\0',512);
    
    memset(app.string_send,'\0',512);
    
    
    
    dialog = gtk_dialog_new();
    
    gtk_dialog_add_button(GTK_DIALOG(dialog), "OK", TRUE);
    
    gtk_dialog_add_button(GTK_DIALOG(dialog), "CANCEL", FALSE);
    
    
    
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    
    
    
    entry  = gtk_entry_new();
    
    entry1 = gtk_entry_new();
    
    entry2 = gtk_entry_new();
    
    
    
    label  = gtk_label_new("Etes vous serveur ? O/N");
    
    label1 = gtk_label_new("Nom d'utilisateur: ");
    
    label2 = gtk_label_new("Ip adresse (si client): ");
    
    
    
    gtk_container_add(GTK_CONTAINER(content_area), label);
    
    gtk_container_add(GTK_CONTAINER(content_area), entry );
    
    gtk_container_add(GTK_CONTAINER(content_area), label1);
    
    gtk_container_add(GTK_CONTAINER(content_area), entry1);
    
    gtk_container_add(GTK_CONTAINER(content_area), label2 );
    
    gtk_container_add(GTK_CONTAINER(content_area), entry2);
    
    
    
    gtk_widget_show_all(dialog);
    
    
    
    result = gtk_dialog_run(GTK_DIALOG(dialog));
    
    
    
    if (result == TRUE){
        
        app.nom_server = gtk_entry_get_text(GTK_ENTRY(entry));
        
        app.nom_client = gtk_entry_get_text(GTK_ENTRY(entry1));
        
        app.ip = gtk_entry_get_text(GTK_ENTRY(entry2));
        
        strcpy(app.ipC, app.ip);
        
        strcpy(app.nom_utilisateur, app.nom_client);
        
        
        
        if (strcmp(app.nom_server, "O") == 0 || strcmp(app.nom_server, "o") == 0) {
            
            app.is_server = TRUE;
            
            gtk_widget_destroy(GTK_WIDGET(dialog));
            
            save_username();
            
            printf("app.nom_utilisateur = %s",app.nom_utilisateur);
            
            thread = g_thread_create( server, windowIO,
                                     
                                     TRUE, &error );
            
        }else if (strcmp(app.nom_server, "N") == 0 || strcmp(app.nom_server, "n") == 0){
            
            gtk_widget_destroy(GTK_WIDGET(dialog));
            
            save_username();
            
            printf("app.nom_utilisateur = %s",app.nom_utilisateur);
            
            thread = g_thread_create(client, windowIO,
                                     
                                     TRUE, &error );
            
        }else{
            
            printf("Error cant find if server or client");
            
        }
        
        
        
    }else if (result == FALSE) {
        
        printf("Cancelled out");
        
    }    gtk_widget_destroy(dialog);
    
}



void client(){
    
    app.connected = FALSE;
    
    /* Release gtk's global lock */
    
    gdk_threads_leave();
    
    /* Secure glib */
    
    /*if( ! g_thread_supported() )
        
        g_thread_init( NULL );
    
    
    
    GError    *error = NULL;
    
    GThread * thread;*/
    
    struct sockaddr_in cliaddr;
    
    cliaddr.sin_port = htons(SER_PORT);
    
    cliaddr.sin_family = AF_INET;
    
    cliaddr.sin_addr.s_addr = inet_addr(app.ipC);
    
    connection_handle = socket(AF_INET,SOCK_STREAM,0);
    
    
    
    
    
    if(connection_handle < 0)
        
    {
        
        perror("\n SOCKET");
        
        exit(0);
        
    }
    
    if(connect(connection_handle, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) < 0)
        
    {
        
        perror("\n CONNECT");
        
        exit(0);
        
    }
    
    app.connected = TRUE;
    
    printf("\nCLIENT\n");
    
    
    
    listen(connection_handle, 1);
    
    
    
    //thread = g_thread_create( timeout_read_2, windowIO,
    
    //                         TRUE, &error );
    
    
    
    timeout_read_2();
    
}

void server(){
    
    app.connected = FALSE;
    
    /* Release gtk's global lock */
    
    gdk_threads_leave();
    
    
    
    /* Secure glib */
    
    if( ! g_thread_supported() )
        
        g_thread_init( NULL );
    
    
    
    GError    *error = NULL;
    
    GThread * thread;
    
    
    
    int serverSock;
    
    struct sockaddr_in seraddr;
    
    struct sockaddr_in cliinfo;
    
    socklen_t csize = sizeof(cliinfo);
    
    seraddr.sin_family = AF_INET;
    
    seraddr.sin_port = htons(SER_PORT);
    
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    
    
    if((serverSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        
    {
        
        perror("error creation");
        
        exit(0);
        
    }
    
    if(bind(serverSock, (struct sockaddr *)&seraddr, sizeof(seraddr)) < 0)
        
    {
        
        perror("error bind");
        
        exit(0);
        
    }
    
    if(listen(serverSock,1) < 0)
        
    {
        
        perror("error listen");
        
        exit(0);
        
    }
    
    
    
    printf("Server initialized...");
    
    if((connection_handle = accept(serverSock, (struct sockaddr *)&cliinfo, &csize)) < 0)
        
    {
        
        perror("Accept ");
        
        exit(0);
        
    }
    
    else
        
        printf("\n now connected to %s\n", inet_ntoa(cliinfo.sin_addr));
    
    app.connected = TRUE;
    
    
    
    listen(connection_handle, 1);
    
    
    
    //thread = g_thread_create( timeout_read_2, windowIO, TRUE, &error );
    
    timeout_read_2();
    
    close(serverSock);
    
}



void send_entered_text(){
    
    strcat(app.message, app.nom_utilisateur);
    
    strcat(app.message, "\n");
    
    strcat(app.message, app.string_send);
    
    send(connection_handle, app.message, sizeof(app.message), 0);
    
    memset(app.message,'\0',562);
    
}



void timeout_read_2(){
    
    
    do{
        
        
        
        if(read(connection_handle,app.string_read,sizeof(app.string_read)) != -1){
        
            enter_socket_text(app.string_read);
        
            memset(app.string_read,'\0',512);
        
            enter_socket_text("\n\0");
        
        }
        else
            app.connected = FALSE;
        
    }while(app.connected != FALSE);
    
    printf("Sortie boucle do");
}

void save_username(){
    
    if (app.is_server) {
        
        (strcmp(app.nom_utilisateur, "")) == 0  ? strcpy(app.nom_utilisateur, "Server:\0") : NULL;
        
    }else
        
        (strcmp(app.nom_utilisateur, "")) == 0  ? strcpy(app.nom_utilisateur, "Client:\0") : NULL;
    
}

void enter_username(){
    
    char tmp_username[50];
    
    memset(tmp_username,'\0',50);
    
    strcat(tmp_username, app.nom_utilisateur);
    
    strcat(tmp_username, "\n");
    
    strcpy(app.nom_utilisateur, tmp_username);
    
}

void save_mark(){
    
    GtkWidget *dialog2;
    
    GtkWidget *entry;
    
    GtkWidget *content_area;
    
    GtkWidget *label;
    
    dialog2 = gtk_dialog_new();
    
    gtk_dialog_add_button(GTK_DIALOG(dialog2), "OK", TRUE);
    
    gtk_dialog_add_button(GTK_DIALOG(dialog2), "CANCEL", FALSE);
    
    
    
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog2));
    
    entry  = gtk_entry_new();
    
    label  = gtk_label_new("Quel note attribué ?");
    
    
    gtk_container_add(GTK_CONTAINER(content_area), label);
    
    gtk_container_add(GTK_CONTAINER(content_area), entry );
    
    
    gtk_widget_show_all(dialog2);
    
    
    gtk_dialog_run(GTK_DIALOG(dialog2));
        
    app.note = gtk_entry_get_text(GTK_ENTRY(entry));

    enter_socket_text(app.note);
    
    gtk_widget_destroy(dialog2);
}

/* ********************************************* end_session ****************************************** */

void end_session(){
    if( ! g_thread_supported() )
        g_thread_init( NULL );
    
    GError    *error = NULL;
    
    GThread * thread;
    thread = g_thread_create( close, connection_handle,
                             TRUE, &error );
    app.connected = FALSE;
    printf("connected to FALSE");
    //gtk_text_view_set_buffer(GTK_TEXT_VIEW(windowIO->textview), app.buffer);
}