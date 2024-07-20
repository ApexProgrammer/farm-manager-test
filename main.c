#include <gtk/gtk.h>
#include <glib/gstdio.h>

// Function for adding new fields (work in progress)
static void on_response(GtkDialog *dialog, gint response_id, gpointer data) {
    // Handle the data from the dialog here
    if (response_id == GTK_RESPONSE_OK) {
        // User clicked OK
        g_print("New field\n");
    } else if (response_id == GTK_RESPONSE_CANCEL) {
        // User cancelled
    }

    // Dialog is destroyed when finished
    gtk_window_destroy(GTK_WINDOW(dialog));
}

static void new_field(GtkWidget *widget, gpointer data) {
    GtkBuilder *builder;
    GtkWidget *dialog;
    GError *error = NULL;

    // Create a new GtkBuilder
    builder = gtk_builder_new();

    // Loading the filed_editor.ui file
    if (!gtk_builder_add_from_file(builder, "field_editor.ui", &error)) {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return;
    }

    // Get the dialog from the builder
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "fieldeditordialog"));
    if (!dialog) {
        g_printerr("Error getting object: fieldeditordialog\n");
        return;
    }

    // Set the transient parent
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(data));

    // Connect to the response signal
    g_signal_connect(dialog, "response", G_CALLBACK(on_response), NULL);

    // Show the dialog
    gtk_widget_show(dialog);

    // Show the dialog
    gtk_window_present(GTK_WINDOW(dialog));

    g_object_unref(builder);
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

  // Loads CSS file
  GtkCssProvider *css_provider = gtk_css_provider_new ();
  gtk_css_provider_load_from_path (css_provider, "styles.css");
  gtk_style_context_add_provider_for_display (gdk_display_get_default (), GTK_STYLE_PROVIDER (css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
  g_object_unref (css_provider);

  GObject *window = gtk_builder_get_object (builder, "window");
  gtk_window_set_application (GTK_WINDOW (window), app);

  GObject *button = gtk_builder_get_object (builder, "createfieldbutton1");
  g_signal_connect(button, "clicked", G_CALLBACK(new_field), window);

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
