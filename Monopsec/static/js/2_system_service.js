// Sidebar toggle logic
$('#toggle-sidebar').click(function() {
    $('#sidebar').toggleClass('collapsed');
});

const services = [
    { name: "SSH Server", description: "OpenSSH server daemon", icon: "fa-server", enabled: true, cpu: "1%", memory: "100MB", status: "running" },
    { name: "Firewall", description: "Uncomplicated Firewall", icon: "fa-shield-alt", enabled: true, cpu: "0%", memory: "50MB", status: "running" },
    { name: "Print Service", description: "CUPS Printing Service", icon: "fa-print", enabled: false, cpu: "0%", memory: "30MB", status: "stopped" },
    { name: "Bluetooth", description: "Bluetooth service", icon: "fa-bluetooth-b", enabled: true, cpu: "2%", memory: "80MB", status: "running" },
    { name: "Apache Web Server", description: "Apache HTTP Server", icon: "fa-globe", enabled: false, cpu: "5%", memory: "200MB", status: "stopped" }
];

const startupApps = [
    { name: "Discord", command: "/usr/bin/discord", enabled: true, cpu: "2%", memory: "300MB", status: "running" },
    { name: "Dropbox", command: "/usr/bin/dropbox", enabled: true, cpu: "1%", memory: "150MB", status: "running" },
    { name: "Slack", command: "/usr/bin/slack", enabled: false, cpu: "0%", memory: "0MB", status: "stopped" }
];

function createServiceItem(service) {
    return `
        <li class="list-group-item">
            <div class="service-info">
                <i class="fas ${service.icon} service-icon"></i>
                <span class="service-name">${service.name}</span>
                <div class="service-description">${service.description}</div>
                <div class="resource-usage">CPU: ${service.cpu}, Memory: ${service.memory}, Status: ${service.status}</div>
            </div>
            <div class="form-check form-switch">
                <input class="form-check-input service-toggle" type="checkbox" ${service.enabled ? 'checked' : ''}>
            </div>
        </li>
    `;
}

function createStartupAppItem(app) {
    return `
        <li class="list-group-item">
            <div class="service-info">
                <span class="service-name">${app.name}</span>
                <div class="service-description">${app.command}</div>
                        <div class="resource-usage">CPU: ${app.cpu}, Memory: ${app.memory}, Status: ${app.status}</div>
                    </div>
                    <div class="form-check form-switch">
                        <input class="form-check-input" type="checkbox" ${app.enabled ? 'checked' : ''}>
                    </div>
                </li>
            `;
        }

        function populateServices() {
            const serviceList = $('#service-list');
            serviceList.empty();
            services.forEach(service => {
                serviceList.append(createServiceItem(service));
            });
        }

        function populateStartupApps() {
            const startupAppList = $('#startup-app-list');
            startupAppList.empty();
            startupApps.forEach(app => {
                startupAppList.append(createStartupAppItem(app));
            });
        }

        // Initial population of services and startup apps
        populateServices();
        populateStartupApps();

