#include "background_processes.h"

volatile sig_atomic_t timer_flag = 0;
volatile sig_atomic_t keep_running = 1;

// Check if a string contains only numeric characters
int is_numeric(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

// Get the name of a process given its PID
void get_process_name(const char *pid, char *name, size_t name_size) {
    char comm_path[MAX_PATH];
    snprintf(comm_path, sizeof(comm_path), "/proc/%s/comm", pid);
    
    FILE *comm_file = fopen(comm_path, "r");
    if (comm_file) {
        if (fgets(name, name_size, comm_file) == NULL) {
            strncpy(name, "Unknown", name_size);
        } else {
            // Remove newline character if present
            char *newline = strchr(name, '\n');
            if (newline) *newline = '\0';
        }
        fclose(comm_file);
    } else {
        strncpy(name, "Unknown", name_size);
    }
    name[name_size - 1] = '\0';
}

// Calculate CPU usage percentage for a given process
double get_cpu_usage(const char *pid) {
    char stat_path[MAX_PATH];
    snprintf(stat_path, sizeof(stat_path), "/proc/%s/stat", pid);
    
    FILE *stat_file = fopen(stat_path, "r");
    if (!stat_file) return 0.0;

    long long utime, stime;
    fscanf(stat_file, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lld %lld", &utime, &stime);
    fclose(stat_file);

    double total_time = (utime + stime) / sysconf(_SC_CLK_TCK);
    
    // Get process start time
    struct stat st;
    if (stat(stat_path, &st) == 0) {
        double uptime = (double)time(NULL) - st.st_mtime;
        return (total_time / uptime) * 100.0;
    }
    
    return 0.0;
}

// Get memory usage in MB for a given process
long get_memory_usage(const char *pid) {
    char statm_path[MAX_PATH];
    snprintf(statm_path, sizeof(statm_path), "/proc/%s/statm", pid);
    
    FILE *statm_file = fopen(statm_path, "r");
    if (!statm_file) return 0;

    long rss;
    fscanf(statm_file, "%*d %ld", &rss);
    fclose(statm_file);

    return rss * sysconf(_SC_PAGESIZE) / 1024 / 1024; // Convert to MB
}

// Extract and print information about background processes
void extract_background_processes() {
    DIR *proc_dir;
    struct dirent *entry;
    char process_name[MAX_CMDLINE];

    proc_dir = opendir("/proc");
    if (proc_dir == NULL) {
        perror("Error opening /proc directory");
        return;
    }

    printf("\033[2J\033[H"); // Clear screen and move cursor to top-left
    printf("Background Processes (Updated every %d microseconds):\n", UPDATE_INTERVAL_USEC);
    printf("Press Ctrl+C to exit\n\n");

    while ((entry = readdir(proc_dir)) != NULL) {
        if (is_numeric(entry->d_name)) {
            pid_t pid = atoi(entry->d_name);
            
            // Skip the current process
            if (pid == getpid()) {
                continue;
            }
            
            get_process_name(entry->d_name, process_name, sizeof(process_name));
            double cpu_usage = get_cpu_usage(entry->d_name);
            long memory_usage = get_memory_usage(entry->d_name);
            
            printf("%s%.1f%%%.0f MB\n", process_name, cpu_usage, (double)memory_usage);
        }
    }

    closedir(proc_dir);
}

// Handler for the timer signal
void timer_handler(int signum) {
    timer_flag = 1;
}

// Set up the timer for periodic updates
void setup_timer() {
    struct sigaction sa;
    struct itimerval timer;

    // Set up the signal handler for SIGVTALRM
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timer_handler;
    sigaction(SIGVTALRM, &sa, NULL);

    // Configure the timer
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = UPDATE_INTERVAL_USEC;
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = UPDATE_INTERVAL_USEC;

    // Start the timer
    setitimer(ITIMER_VIRTUAL, &timer, NULL);
}

// Handler for the interrupt signal (Ctrl+C)
void signal_handler(int signum) {
    if (signum == SIGINT) {
        printf("\nReceived interrupt signal. Exiting gracefully...\n");
        keep_running = 0;
    }
}

// Set up the signal handler for graceful exit
void setup_signal_handler() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &signal_handler;
    sigaction(SIGINT, &sa, NULL);
}

int main() {
    // Set up the timer and signal handler
    setup_timer();
    setup_signal_handler();

    // Main loop
    while (keep_running) {
        if (timer_flag) {
            extract_background_processes();
            timer_flag = 0;
        }
    }

    printf("Program terminated.\n");
    return 0;
}
