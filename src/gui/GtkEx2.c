#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>

/* 회원가입한 유저의 정보 */
struct user_inform{
	char id[30]; 			//id
	char pw[30]; 			//password
	char e_mail[30];	//email
}user_inform;

/* 로그인 핸들러페이지 */
static void
user_login (GtkWidget *button, gpointer user_data) { //로그인
  char str[30];
	strcpy(str, user_data);
	g_print("%s", str);
	gtk_widget_destroy((GtkWidget*)user_data);
}

static void
new_user (GtkWidget *button, gpointer user_data) { //회원가입
	
 
}


static void activate (GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *button; 
  GtkWidget *box;
  GtkWidget *label1, *label2;
	GtkWidget *entry1, *entry2;	

  window = gtk_application_window_new (app); 
  gtk_window_set_title (GTK_WINDOW (window), "Window");	
  gtk_window_set_default_size (GTK_WINDOW (window), 200, 400); 

  box = gtk_button_box_new (GTK_ORIENTATION_VERTICAL); 
  gtk_container_add (GTK_CONTAINER (window), box); 

	label1 = gtk_label_new("ID");
	gtk_container_add(GTK_CONTAINER (box), label1);
	entry1 = gtk_entry_new();
	gtk_container_add(GTK_CONTAINER (box), entry1);

	label2 = gtk_label_new("Password");
	gtk_container_add(GTK_CONTAINER (box), label2);
	entry2 = gtk_entry_new();
	gtk_container_add(GTK_CONTAINER (box), entry2);

  button = gtk_button_new_with_label ("로그인");
  g_signal_connect (button, "clicked", G_CALLBACK (user_login), entry1);
  gtk_container_add (GTK_CONTAINER (box), button);

  button = gtk_button_new_with_label("회원가입");
  g_signal_connect (button, "clicked", G_CALLBACK (new_user), NULL);
  gtk_container_add (GTK_CONTAINER (box), button);
  
  button = gtk_button_new_with_label("게임 종료");
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_widget_destroy), window);
  gtk_container_add (GTK_CONTAINER (box), button);

  gtk_widget_show_all (window);
}

int main (int argc, char **argv) {

	gtk_init(&argc, &argv);
  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}


