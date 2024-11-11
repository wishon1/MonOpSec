#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    float user;        // User time
    float nice;        // Nice time
    float system;      // System time
    float idle;        // Idle time
    float iowait;      // I/O wait time
    float irq;         // IRQ time
    float softirq;     // SoftIRQ time
    float total;       // Total time
} CPUUsage;

CPUUsage get_cpu_usage() {
    CPUUsage cpu_usage = {0};
    FILE *fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        perror("Failed to open /proc/stat");
        exit(EXIT_FAILURE);
    }

    char buffer[256];
    fgets(buffer, sizeof(buffer), fp);
    fclose(fp);

    // Parse the CPU line
    sscanf(buffer, "cpu %f %f %f %f %f %f %f",
           &cpu_usage.user,
           &cpu_usage.nice,
           &cpu_usage.system,
           &cpu_usage.idle,
           &cpu_usage.iowait,
           &cpu_usage.irq,
           &cpu_usage.softirq);

    // Calculate total CPU time
    cpu_usage.total = cpu_usage.user + cpu_usage.nice + cpu_usage.system +
                      cpu_usage.idle + cpu_usage.iowait + cpu_usage.irq +
                      cpu_usage.softirq;

    return cpu_usage;
}

void display_cpu_usage(GtkWidget *widget, gpointer data) {
    CPUUsage cpu_usage = get_cpu_usage();
    char buffer[256];
    sprintf(buffer, "CPU Usage:\n"
                    "User: %.2f%%\n"
                    "Nice: %.2f%%\n"
                    "System: %.2f%%\n"
                    "Idle: %.2f%%\n"
                    "I/O Wait: %.2f%%\n"
                    "IRQ: %.2f%%\n"
                    "SoftIRQ: %.2f%%\n",
            (cpu_usage.user / cpu_usage.total) * 100,
            (cpu_usage.nice / cpu_usage.total) * 100,
            (cpu_usage.system / cpu_usage.total) * 100,
            (cpu_usage.idle / cpu_usage.total) * 100,
            (cpu_usage.iowait / cpu_usage.total) * 100,
            (cpu_usage.irq / cpu_usage.total) * 100,
            (cpu_usage.softirq / cpu_usage.total) * 100);

    gtk_label_set_text(GTK_LABEL(data), buffer);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "CPU Usage Monitor");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 300);

    GtkWidget *label = gtk_label_new("Press the button to get CPU usage.");
    GtkWidget *button = gtk_button_new_with_label("Get CPU Usage");

    g_signal_connect(button, "clicked", G_CALLBACK(display_cpu_usage), label);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(vbox), button, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

