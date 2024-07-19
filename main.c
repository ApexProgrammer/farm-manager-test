#include <gtk/gtk.h>
#include <glib/gstdio.h>

// Function for adding new fields (work in progress)
static void new_field(GtkWidget *widget, gpointer data) {
  g_print("New field");
}

static void activate(GtkApplication *app, gpointer user_data) {
  // Create builder to upload window.ui file
  GtkBuilder *builder = gtk_builder_new ();
  GError *error = NULL;

  if (gtk_builder_add_from_file(builder, "window.ui", &error) == 0)
  {
    g_critical("Could not load builder file: %s", error->message);
    g_error_free(error);
    return;
  }

  GObject *window = gtk_builder_get_object (builder, "window");
  gtk_window_set_application (GTK_WINDOW (window), app);

  // Loads CSS file
  GtkCssProvider *css_provider = gtk_css_provider_new ();
  gtk_css_provider_load_from_path (css_provider, "styles.css");
  gtk_style_context_add_provider_for_display (gdk_display_get_default (), GTK_STYLE_PROVIDER (css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  g_object_unref (css_provider);

  // Set window visibility and maximize window 
  gtk_widget_show (GTK_WIDGET (window));
  gtk_window_maximize(GTK_WINDOW (window));

  g_object_unref (builder);
}

int main(int   argc, char *argv[]) {
#ifdef GTK_SRCDIR
  g_chdir (GTK_SRCDIR);
#endif

  GtkApplication *app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);

  int status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
