// Sidebar toggle functionality
$('#toggle-sidebar').on('click', function() {
    $('#sidebar').toggleClass('collapsed');
    $('#content').toggleClass('collapsed');
});

// Example service data with CPU and memory usage
const services = [
    { name: 'Apache', status: 'running', cpu: 15, memory: 120 },
    { name: 'MySQL', status: 'stopped', cpu: 0, memory: 0 },
    { name: 'Nginx', status: 'running', cpu: 8, memory: 85 },
    { name: 'MongoDB', status: 'stopped', cpu: 0, memory: 0 }
];

// Render services list dynamically
function renderServices() {
    const serviceList = $('#service-list');
    serviceList.empty();

    services.forEach(service => {
        const statusClass = service.status === 'running' ? 'running' : 'stopped';
        const buttonState = service.status === 'running' ? 
            `<button class="btn btn-danger btn-sm stop-btn">Stop</button>` :
            `<button class="btn btn-success btn-sm start-btn">Start</button>`;

        serviceList.append(`
            <tr>
                <td>${service.name}</td>
                <td class="service-status ${statusClass}">
                    <i class="fas ${service.status === 'running' ? 'fa-check-circle' : 'fa-times-circle'}"></i>
                    <span>${service.status.charAt(0).toUpperCase() + service.status.slice(1)}</span>
                </td>
                <td>${service.cpu}%</td>
                <td>${service.memory} MB</td>
                <td class="control-btns">
                    ${buttonState}
                    <button class="btn btn-warning btn-sm restart-btn">Restart</button>
                </td>
            </tr>
        `);
    });

    // Handle Start/Stop button click events
    $('.start-btn').on('click', function() {
        const serviceName = $(this).closest('tr').find('td:first').text();
        updateServiceStatus(serviceName, 'running');
    });

    $('.stop-btn').on('click', function() {
        const serviceName = $(this).closest('tr').find('td:first').text();
        updateServiceStatus(serviceName, 'stopped');
    });

    $('.restart-btn').on('click', function() {
        const serviceName = $(this).closest('tr').find('td:first').text();
        alert(`${serviceName} is restarting...`);
        setTimeout(() => {
            alert(`${serviceName} restarted successfully!`);
        }, 1500);
    });
}

// Update service status and regenerate CPU/memory usage
function updateServiceStatus(serviceName, status) {
    const service = services.find(s => s.name === serviceName);
    service.status = status;

    // Simulate CPU and memory usage when the service is running
    if (status === 'running') {
        service.cpu = Math.floor(Math.random() * 30) + 1; // Random CPU usage between 1-30%
        service.memory = Math.floor(Math.random() * 200) + 50; // Random memory between 50-250MB
    } else {
        service.cpu = 0;
        service.memory = 0;
    }

    renderServices();
}

// Initial rendering of services
renderServices();

