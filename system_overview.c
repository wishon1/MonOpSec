#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    GtkWidget *cpu_label;
    GtkWidget *memory_label;
    GtkWidget *disk_label;
    GtkWidget *network_label;
} AppWidgets;

static void update_system_overview(AppWidgets *app_widgets) {
    // Simulated data
    int cpu_usage = rand() % 101;           // Random CPU usage (0-100%)
    int memory_used = rand() % 17;          // Random memory used (0-16 GB)
    int memory_total = 16;
    int disk_used = rand() % 501;           // Random disk used (0-500 GB)
    int disk_total = 500;
    int network_usage = rand() % 101;       // Random network usage (0-100 Mbps)

    // Update labels with formatted strings
    char cpu_text[20], memory_text[40], disk_text[40], network_text[40];
    snprintf(cpu_text, sizeof(cpu_text), "CPU Usage: %d%%", cpu_usage);
    snprintf(memory_text, sizeof(memory_text), "Memory Usage: %d GB / %d GB", memory_used, memory_total);
    snprintf(disk_text, sizeof(disk_text), "Disk Usage: %d GB / %d GB", disk_used, disk_total);
    snprintf(network_text, sizeof(network_text), "Network Usage: %d Mbps", network_usage);

    gtk_label_set_text(GTK_LABEL(app_widgets->cpu_label), cpu_text);
    gtk_label_set_text(GTK_LABEL(app_widgets->memory_label), memory_text);
    gtk_label_set_text(GTK_LABEL(app_widgets->disk_label), disk_text);
    gtk_label_set_text(GTK_LABEL(app_widgets->network_label), network_text);
}

static gboolean refresh_data(gpointer user_data) {
    AppWidgets *app_widgets = (AppWidgets *)user_data;
    update_system_overview(app_widgets);
    return TRUE; // Continue calling this function
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *sidebar;
    GtkWidget *content_area;
    AppWidgets app_widgets;

    gtk_init(&argc, &argv);
    srand(time(NULL)); // Seed for random data

    // Create main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "OptiMate - System Overview");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    
    // Apply custom CSS
    GtkCssProvider *css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider,
        "window { background-color: #2c3e50; }"
        "label { color: #ecf0f1; font-size: 14px; }"
        "button { background-color: #2980b9; color: white; border: none; padding: 10px; border-radius: 5px; }"
        "button:hover { background-color: #3498db; }"
        "button:active { background-color: #1abc9c; }"
        "box { margin: 10px; }", -1, NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(css_provider);

    // Create a grid layout
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Create sidebar
    sidebar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_widget_set_size_request(sidebar, 250, -1);
    gtk_box_set_homogeneous(GTK_BOX(sidebar), FALSE);

    // Sidebar buttons
    GtkWidget *system_overview_button = gtk_button_new_with_label("System Overview");
    GtkWidget *resource_analytics_button = gtk_button_new_with_label("Resource Analytics");
    GtkWidget *service_controller_button = gtk_button_new_with_label("Service Controller");
    GtkWidget *disk_optimizer_button = gtk_button_new_with_label("Disk Optimizer");
    GtkWidget *security_monitor_button = gtk_button_new_with_label("Security Monitor");
    GtkWidget *performance_booster_button = gtk_button_new_with_label("Performance Booster");

    gtk_box_pack_start(GTK_BOX(sidebar), system_overview_button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(sidebar), resource_analytics_button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(sidebar), service_controller_button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(sidebar), disk_optimizer_button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(sidebar), security_monitor_button, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(sidebar), performance_booster_button, FALSE, FALSE, 5);

    // Add sidebar to grid
    gtk_grid_attach(GTK_GRID(grid), sidebar, 0, 0, 1, 1);

    // Create content area
    content_area = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_grid_attach(GTK_GRID(grid), content_area, 1, 0, 1, 1);

    // Create usage labels with a stylish appearance
    app_widgets.cpu_label = gtk_label_new("CPU Usage: 0%");
    app_widgets.memory_label = gtk_label_new("Memory Usage: 0 GB / 0 GB");
    app_widgets.disk_label = gtk_label_new("Disk Usage: 0 GB / 0 GB");
    app_widgets.network_label = gtk_label_new("Network Usage: 0 Mbps");

    gtk_box_pack_start(GTK_BOX(content_area), app_widgets.cpu_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(content_area), app_widgets.memory_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(content_area), app_widgets.disk_label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(content_area), app_widgets.network_label, FALSE, FALSE, 5);

    // Setup a timer to update data every 5 seconds
    g_timeout_add_seconds(5, refresh_data, &app_widgets);

    // Show all widgets
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
